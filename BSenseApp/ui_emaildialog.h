/********************************************************************************
** Form generated from reading UI file 'emaildialog.ui'
**
** Created: Wed Aug 25 14:47:10 2010
**      by: Qt User Interface Compiler version 4.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EMAILDIALOG_H
#define UI_EMAILDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EmailDialog
{
public:
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *label_6;
    QSpacerItem *verticalSpacer_4;
    QLabel *label_14;
    QLineEdit *emailAddress;
    QSpacerItem *verticalSpacer_2;
    QFrame *line;
    QLabel *label;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *EmailDialog)
    {
        if (EmailDialog->objectName().isEmpty())
            EmailDialog->setObjectName(QString::fromUtf8("EmailDialog"));
        EmailDialog->setWindowModality(Qt::ApplicationModal);
        EmailDialog->resize(529, 274);
        EmailDialog->setContextMenuPolicy(Qt::DefaultContextMenu);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/bsense-green.svg"), QSize(), QIcon::Normal, QIcon::Off);
        EmailDialog->setWindowIcon(icon);
        EmailDialog->setAutoFillBackground(true);
        EmailDialog->setModal(true);
        layoutWidget = new QWidget(EmailDialog);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 10, 557, 251));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        label_6 = new QLabel(layoutWidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        QFont font;
        font.setPointSize(20);
        label_6->setFont(font);
        label_6->setAlignment(Qt::AlignBottom|Qt::AlignLeading|Qt::AlignLeft);

        verticalLayout->addWidget(label_6);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_4);

        label_14 = new QLabel(layoutWidget);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setWordWrap(true);

        verticalLayout->addWidget(label_14);

        emailAddress = new QLineEdit(layoutWidget);
        emailAddress->setObjectName(QString::fromUtf8("emailAddress"));

        verticalLayout->addWidget(emailAddress);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        line = new QFrame(layoutWidget);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line);

        label = new QLabel(layoutWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setWordWrap(true);

        verticalLayout->addWidget(label);

        buttonBox = new QDialogButtonBox(layoutWidget);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(EmailDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), EmailDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), EmailDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(EmailDialog);
    } // setupUi

    void retranslateUi(QDialog *EmailDialog)
    {
        EmailDialog->setWindowTitle(QApplication::translate("EmailDialog", "BSense: update your details", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("EmailDialog", "Update your details:", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("EmailDialog", "At this stage, we are asking our existing volunteers to provide an email address for updates regarding the availability of results and to get in touch in case of problems with their measurements.\n"
"\n"
"Please enter your email address:", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("EmailDialog", "<i><strong>Privacy note:</strong> we will not provide your address to anyone, and we will not send unsolicited messages.</i>", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class EmailDialog: public Ui_EmailDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EMAILDIALOG_H
