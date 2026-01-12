/********************************************************************************
** Form generated from reading UI file 'uiLoginWidget.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UILOGINWIDGET_H
#define UI_UILOGINWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoginWidget
{
public:
    QGridLayout *gridLayout_3;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *verticalSpacer;
    QGridLayout *gridLayout;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *userLineEdit;
    QLabel *machneLabel;
    QComboBox *machineComboBox;
    QLineEdit *passLineEdit;
    QGridLayout *gridLayout_2;
    QLabel *loginMessageLabel;
    QSpacerItem *horizontalSpacer;
    QPushButton *loginPushButton;
    QLabel *label_3;
    QFrame *line;

    void setupUi(QWidget *LoginWidget)
    {
        if (LoginWidget->objectName().isEmpty())
            LoginWidget->setObjectName("LoginWidget");
        LoginWidget->resize(725, 405);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(LoginWidget->sizePolicy().hasHeightForWidth());
        LoginWidget->setSizePolicy(sizePolicy);
        gridLayout_3 = new QGridLayout(LoginWidget);
        gridLayout_3->setObjectName("gridLayout_3");
        gridLayout_3->setSizeConstraint(QLayout::SetNoConstraint);
        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout_3->addItem(verticalSpacer_2, 2, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout_3->addItem(verticalSpacer, 3, 0, 1, 1);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");
        label = new QLabel(LoginWidget);
        label->setObjectName("label");
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label, 1, 1, 1, 1);

        label_2 = new QLabel(LoginWidget);
        label_2->setObjectName("label_2");
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(label_2, 0, 1, 1, 1);

        userLineEdit = new QLineEdit(LoginWidget);
        userLineEdit->setObjectName("userLineEdit");
        sizePolicy.setHeightForWidth(userLineEdit->sizePolicy().hasHeightForWidth());
        userLineEdit->setSizePolicy(sizePolicy);

        gridLayout->addWidget(userLineEdit, 0, 2, 1, 1);

        machneLabel = new QLabel(LoginWidget);
        machneLabel->setObjectName("machneLabel");
        sizePolicy.setHeightForWidth(machneLabel->sizePolicy().hasHeightForWidth());
        machneLabel->setSizePolicy(sizePolicy);
        machneLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->addWidget(machneLabel, 2, 1, 1, 1);

        machineComboBox = new QComboBox(LoginWidget);
        machineComboBox->setObjectName("machineComboBox");
        sizePolicy.setHeightForWidth(machineComboBox->sizePolicy().hasHeightForWidth());
        machineComboBox->setSizePolicy(sizePolicy);

        gridLayout->addWidget(machineComboBox, 2, 2, 1, 1);

        passLineEdit = new QLineEdit(LoginWidget);
        passLineEdit->setObjectName("passLineEdit");
        sizePolicy.setHeightForWidth(passLineEdit->sizePolicy().hasHeightForWidth());
        passLineEdit->setSizePolicy(sizePolicy);
        passLineEdit->setEchoMode(QLineEdit::Password);

        gridLayout->addWidget(passLineEdit, 1, 2, 1, 1);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName("gridLayout_2");
        gridLayout_2->setSizeConstraint(QLayout::SetMinimumSize);
        loginMessageLabel = new QLabel(LoginWidget);
        loginMessageLabel->setObjectName("loginMessageLabel");
        loginMessageLabel->setStyleSheet(QString::fromUtf8("color: rgb(192, 28, 40);\n"
"font: 700 11pt \"Ubuntu\";"));

        gridLayout_2->addWidget(loginMessageLabel, 0, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 0, 1, 1, 1);

        loginPushButton = new QPushButton(LoginWidget);
        loginPushButton->setObjectName("loginPushButton");
        sizePolicy.setHeightForWidth(loginPushButton->sizePolicy().hasHeightForWidth());
        loginPushButton->setSizePolicy(sizePolicy);

        gridLayout_2->addWidget(loginPushButton, 0, 2, 1, 1);


        gridLayout->addLayout(gridLayout_2, 3, 1, 1, 2);

        label_3 = new QLabel(LoginWidget);
        label_3->setObjectName("label_3");
        label_3->setMaximumSize(QSize(285, 174));
        label_3->setPixmap(QPixmap(QString::fromUtf8(":/resources/icons/tad_small.png")));
        label_3->setScaledContents(true);

        gridLayout->addWidget(label_3, 0, 0, 4, 1);


        gridLayout_3->addLayout(gridLayout, 1, 0, 1, 1);

        line = new QFrame(LoginWidget);
        line->setObjectName("line");
        line->setLineWidth(3);
        line->setFrameShape(QFrame::Shape::HLine);
        line->setFrameShadow(QFrame::Shadow::Sunken);

        gridLayout_3->addWidget(line, 0, 0, 1, 1);

        QWidget::setTabOrder(userLineEdit, passLineEdit);
        QWidget::setTabOrder(passLineEdit, machineComboBox);
        QWidget::setTabOrder(machineComboBox, loginPushButton);

        retranslateUi(LoginWidget);

        loginPushButton->setDefault(true);


        QMetaObject::connectSlotsByName(LoginWidget);
    } // setupUi

    void retranslateUi(QWidget *LoginWidget)
    {
        LoginWidget->setWindowTitle(QCoreApplication::translate("LoginWidget", "Form", nullptr));
        label->setText(QCoreApplication::translate("LoginWidget", "Password:", nullptr));
        label_2->setText(QCoreApplication::translate("LoginWidget", "Username:", nullptr));
        machneLabel->setText(QCoreApplication::translate("LoginWidget", "Machine:", nullptr));
        loginMessageLabel->setText(QString());
        loginPushButton->setText(QCoreApplication::translate("LoginWidget", "Login", nullptr));
        label_3->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class LoginWidget: public Ui_LoginWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UILOGINWIDGET_H
