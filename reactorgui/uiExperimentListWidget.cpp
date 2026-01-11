#include "uiExperimentListWidget.h"
#include "qlabel.h"
#include "qlineedit.h"
#include "qpushbutton.h"
#include "ui_uiExperimentListWidget.h"


std::string generateRandomWord(int length) {
    static const char alphabet[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    std::string word;
    for (int i = 0; i < length; ++i) {
        word += alphabet[rand() % (sizeof(alphabet) - 1)];
    }
    return word;
}



ExperimentListWidget::ExperimentListWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ExperimentListWidget)
{
    ui->setupUi(this);

    ui->experimentsTableWidget->setWindowTitle("Alphabet Table");
    ui->experimentsTableWidget->setRowCount(26); // 26 rows for each letter of the alphabet
    ui->experimentsTableWidget->setColumnCount(1); // One column for names
    ui->experimentsTableWidget->setHorizontalHeaderLabels(QStringList() << "Name");
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


    ui->experimentsTableWidget->setRowCount(45); // Example row count
    ui->experimentsTableWidget->setColumnCount(3); // Example column count
    ui->experimentsTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    char Aletter = 'A';
    // Set initial data for the table (example)
    for (int row = 0; row < 45; ++row) {
        QTableWidgetItem *itemExp = new QTableWidgetItem(QString("%1 Experiment").arg( QString::fromStdString(generateRandomWord(5))) );
        ui->experimentsTableWidget->setItem(row, 0, itemExp);

        for (int col = 1; col < 3; ++col) {
            QTableWidgetItem *item = new QTableWidgetItem(QString("%1 Col %2").arg(rand()%555).arg(col + 1));
            ui->experimentsTableWidget->setItem(row, col, item);
        }
    }

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
