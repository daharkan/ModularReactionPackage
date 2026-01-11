#include "uiExperimentListWidget.h"
#include "qlabel.h"
#include "qlineedit.h"
#include "qpushbutton.h"
#include "ui_uiExperimentListWidget.h"
#include "RedisDBManager.h"
#include <QDateTime>
#include <QHeaderView>

namespace {
QString formatExperimentDate(unsigned long createdAt) {
    if (createdAt == 0) {
        return "--";
    }
    return QDateTime::fromMSecsSinceEpoch(createdAt).toString("yyyy-MM-dd HH:mm");
}
} // namespace



ExperimentListWidget::ExperimentListWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ExperimentListWidget)
{
    ui->setupUi(this);

    ui->experimentsTableWidget->setWindowTitle("Experiments");
    ui->experimentsTableWidget->setColumnCount(3);
    ui->experimentsTableWidget->setHorizontalHeaderLabels(QStringList() << tr("name") << tr("Created By") << tr("Created At"));
    ui->experimentsTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->experimentsTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->experimentsTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(ui->experimentsTableWidget, &QTableWidget::itemSelectionChanged, this, &ExperimentListWidget::sgn_selectionChanged);
    connect(ui->experimentsTableWidget, &QTableWidget::itemDoubleClicked, this, &ExperimentListWidget::handleItemDoubleClicked);
    QGridLayout *layout = new QGridLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setHorizontalSpacing(4);
    layout->setVerticalSpacing(4);

    QPushButton *allButton = new QPushButton(tr("All"), this);
    layout->addWidget(allButton, 0, 0);
    connect(allButton, &QPushButton::clicked, this, &ExperimentListWidget::listAllData);

    int row = 0;
    int col = 1;
    int columnCount = 9;
    for (char letter = 'A'; letter <= 'Z'; ++letter) {
        QPushButton *button = new QPushButton(QString(letter), this);
        layout->addWidget(button, row, col);
        connect(button, &QPushButton::clicked, this, &ExperimentListWidget::filterTableByLetter);

        col++;
        if (col >= columnCount) {
            col = 0;
            row++;
        }
    }


    ui->horizontalLayout->addLayout(layout);

    QHBoxLayout *searchLayout = new QHBoxLayout();
    QLabel *searchLabel = new QLabel(tr("Search: "), this);
    searchLayout->addWidget(searchLabel);
    QLineEdit* searchLineEdit = new QLineEdit(this);
    searchLayout->addWidget(searchLineEdit);
    ui->horizontalLayout->addLayout(searchLayout);
    // Connect search box signal
    connect(searchLineEdit, &QLineEdit::textChanged, this, &ExperimentListWidget::filterTableBySearch);


    reloadExperiments();

}

void ExperimentListWidget::handleItemDoubleClicked(QTableWidgetItem *item)
{
    if (!item) {
        return;
    }
    emit sgn_experimentActivated();
}

ExperimentListWidget::~ExperimentListWidget()
{
    delete ui;
}


void ExperimentListWidget::listAllData() {
    for (int row = 0; row < ui->experimentsTableWidget->rowCount(); ++row) {
        ui->experimentsTableWidget->setRowHidden(row, false);
    }
}

void ExperimentListWidget::filterTableByLetter()
{
    QPushButton *senderButton = qobject_cast<QPushButton*>(sender());
    if (senderButton) {
        QString filterLetter = senderButton->text();
        // Filter table according to clicked letter
        for (int row = 0; row < ui->experimentsTableWidget->rowCount(); ++row) {
            bool hideRow = true;
            for (int col = 0; col < ui->experimentsTableWidget->columnCount(); ++col) {
                QTableWidgetItem *item = ui->experimentsTableWidget->item(row, col);
                if (item && item->text().startsWith(filterLetter, Qt::CaseInsensitive)) {
                    hideRow = false;
                    break;
                }
            }
            ui->experimentsTableWidget->setRowHidden(row, hideRow);
        }
    }
}

void ExperimentListWidget::filterTable(const QString &filterText)
{
    for (int row = 0; row < ui->experimentsTableWidget->rowCount(); ++row) {
        bool hideRow = true;
        for (int col = 0; col < ui->experimentsTableWidget->columnCount(); ++col) {
            QTableWidgetItem *item = ui->experimentsTableWidget->item(row, col);
            if (item && item->text().contains(filterText, Qt::CaseInsensitive)) {
                hideRow = false;
                break;
            }
        }
        ui->experimentsTableWidget->setRowHidden(row, hideRow);
    }
}

void ExperimentListWidget::filterTableBySearch(const QString &text)
{
    QString searchText = text.trimmed();
    if (searchText.length() >= 3) {
        filterTable(searchText);
    }else{
        listAllData();
    }

}

void ExperimentListWidget::reloadExperiments()
{
    if (!RedisDBManager::getInstance()->isConnected()) {
        RedisDBManager::getInstance()->connectToDB("127.0.0.1", 6379);
    }

    m_experiments = RedisDBManager::getInstance()->getExperiments();
    ui->experimentsTableWidget->setRowCount(static_cast<int>(m_experiments.size()));

    for (int row = 0; row < static_cast<int>(m_experiments.size()); ++row) {
        const Experiment &experiment = m_experiments.at(row);
        QString expName = QString::fromStdString(experiment.name());
        if (expName.isEmpty()) {
            expName = tr("(Unnamed)");
        }
        QTableWidgetItem *nameItem = new QTableWidgetItem(expName);
        nameItem->setData(Qt::UserRole, QString::fromStdString(experiment.experimentId()));
        nameItem->setData(Qt::UserRole + 1, experiment.experimentType());
        ui->experimentsTableWidget->setItem(row, 0, nameItem);

        QString createdBy = QString::fromStdString(experiment.owner().username());
        if (createdBy.isEmpty()) {
            createdBy = "--";
        }
        ui->experimentsTableWidget->setItem(row, 1, new QTableWidgetItem(createdBy));

        ui->experimentsTableWidget->setItem(row, 2, new QTableWidgetItem(formatExperimentDate(experiment.createdAtMSecs())));
    }

    listAllData();
}

QString ExperimentListWidget::selectedExperimentId() const
{
    QList<QTableWidgetItem*> items = ui->experimentsTableWidget->selectedItems();
    if (items.isEmpty()) {
        return QString();
    }
    QTableWidgetItem *item = ui->experimentsTableWidget->item(items.first()->row(), 0);
    if (!item) {
        return QString();
    }
    return item->data(Qt::UserRole).toString();
}

Experiment ExperimentListWidget::selectedExperiment() const
{
    QList<QTableWidgetItem*> items = ui->experimentsTableWidget->selectedItems();
    if (items.isEmpty()) {
        return Experiment();
    }
    int row = items.first()->row();
    if (row < 0 || row >= static_cast<int>(m_experiments.size())) {
        return Experiment();
    }
    return m_experiments.at(row);
}

bool ExperimentListWidget::hasSelection() const
{
    return !ui->experimentsTableWidget->selectedItems().isEmpty();
}
