/********************************************************************************
** Form generated from reading UI file 'postcodedialog.ui'
**
** Created: Sun Nov 21 20:42:33 2010
**      by: Qt User Interface Compiler version 4.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_POSTCODEDIALOG_H
#define UI_POSTCODEDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PostCodeDialog
{
public:
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *label_6;
    QSpacerItem *verticalSpacer_4;
    QLabel *label_14;
    QLabel *label_7;
    QLabel *label_8;
    QSpacerItem *verticalSpacer;
    QFrame *line;
    QGridLayout *gridLayout;
    QLabel *label_9;
    QLabel *label;
    QLineEdit *postcodeField;
    QLabel *label_10;
    QLabel *label_2;
    QComboBox *connectionComboBox;
    QLabel *label_11;
    QLabel *label_3;
    QComboBox *ispComboBox;
    QLabel *label_12;
    QLabel *label_4;
    QComboBox *downloadComboBox;
    QLabel *label_13;
    QLabel *label_5;
    QComboBox *uploadComboBox;
    QLabel *label_15;
    QLineEdit *emailField;
    QLabel *label_16;
    QFrame *line_2;
    QSpacerItem *verticalSpacer_3;
    QLabel *label_17;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *PostCodeDialog)
    {
        if (PostCodeDialog->objectName().isEmpty())
            PostCodeDialog->setObjectName(QString::fromUtf8("PostCodeDialog"));
        PostCodeDialog->setWindowModality(Qt::ApplicationModal);
        PostCodeDialog->resize(613, 481);
        PostCodeDialog->setContextMenuPolicy(Qt::DefaultContextMenu);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/bsense-green.svg"), QSize(), QIcon::Normal, QIcon::Off);
        PostCodeDialog->setWindowIcon(icon);
        PostCodeDialog->setAutoFillBackground(true);
        PostCodeDialog->setModal(true);
        layoutWidget = new QWidget(PostCodeDialog);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 10, 594, 461));
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

        label_7 = new QLabel(layoutWidget);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_7->setWordWrap(true);

        verticalLayout->addWidget(label_7);

        label_8 = new QLabel(layoutWidget);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_8->setWordWrap(true);

        verticalLayout->addWidget(label_8);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        line = new QFrame(layoutWidget);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_9 = new QLabel(layoutWidget);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        QFont font1;
        font1.setBold(true);
        font1.setWeight(75);
        label_9->setFont(font1);
        label_9->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_9->setWordWrap(true);

        gridLayout->addWidget(label_9, 0, 0, 1, 1);

        label = new QLabel(layoutWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label->setWordWrap(true);

        gridLayout->addWidget(label, 0, 1, 1, 1);

        postcodeField = new QLineEdit(layoutWidget);
        postcodeField->setObjectName(QString::fromUtf8("postcodeField"));

        gridLayout->addWidget(postcodeField, 0, 2, 1, 1);

        label_10 = new QLabel(layoutWidget);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_10->sizePolicy().hasHeightForWidth());
        label_10->setSizePolicy(sizePolicy);
        label_10->setFont(font1);
        label_10->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_10->setWordWrap(true);

        gridLayout->addWidget(label_10, 1, 0, 1, 1);

        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_2->setWordWrap(true);

        gridLayout->addWidget(label_2, 1, 1, 1, 1);

        connectionComboBox = new QComboBox(layoutWidget);
        connectionComboBox->setObjectName(QString::fromUtf8("connectionComboBox"));

        gridLayout->addWidget(connectionComboBox, 1, 2, 1, 1);

        label_11 = new QLabel(layoutWidget);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setFont(font1);
        label_11->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_11->setWordWrap(true);

        gridLayout->addWidget(label_11, 2, 0, 1, 1);

        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_3->setWordWrap(true);

        gridLayout->addWidget(label_3, 2, 1, 1, 1);

        ispComboBox = new QComboBox(layoutWidget);
        ispComboBox->setObjectName(QString::fromUtf8("ispComboBox"));

        gridLayout->addWidget(ispComboBox, 2, 2, 1, 1);

        label_12 = new QLabel(layoutWidget);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setFont(font1);
        label_12->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_12->setWordWrap(true);

        gridLayout->addWidget(label_12, 3, 0, 1, 1);

        label_4 = new QLabel(layoutWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_4->setWordWrap(true);

        gridLayout->addWidget(label_4, 3, 1, 1, 1);

        downloadComboBox = new QComboBox(layoutWidget);
        downloadComboBox->setObjectName(QString::fromUtf8("downloadComboBox"));

        gridLayout->addWidget(downloadComboBox, 3, 2, 1, 1);

        label_13 = new QLabel(layoutWidget);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setFont(font1);
        label_13->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_13->setWordWrap(true);

        gridLayout->addWidget(label_13, 4, 0, 1, 1);

        label_5 = new QLabel(layoutWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_5->setWordWrap(true);

        gridLayout->addWidget(label_5, 4, 1, 1, 1);

        uploadComboBox = new QComboBox(layoutWidget);
        uploadComboBox->setObjectName(QString::fromUtf8("uploadComboBox"));

        gridLayout->addWidget(uploadComboBox, 4, 2, 1, 1);

        label_15 = new QLabel(layoutWidget);
        label_15->setObjectName(QString::fromUtf8("label_15"));

        gridLayout->addWidget(label_15, 5, 1, 1, 1);

        emailField = new QLineEdit(layoutWidget);
        emailField->setObjectName(QString::fromUtf8("emailField"));

        gridLayout->addWidget(emailField, 5, 2, 1, 1);

        label_16 = new QLabel(layoutWidget);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setFont(font1);
        label_16->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_16->setWordWrap(true);

        gridLayout->addWidget(label_16, 5, 0, 1, 1);

        gridLayout->setColumnStretch(1, 2);

        verticalLayout->addLayout(gridLayout);

        line_2 = new QFrame(layoutWidget);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line_2);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_3);

        label_17 = new QLabel(layoutWidget);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setWordWrap(true);

        verticalLayout->addWidget(label_17);

        buttonBox = new QDialogButtonBox(layoutWidget);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(PostCodeDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), PostCodeDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), PostCodeDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(PostCodeDialog);
    } // setupUi

    void retranslateUi(QDialog *PostCodeDialog)
    {
        PostCodeDialog->setWindowTitle(QApplication::translate("PostCodeDialog", "Welcome to BSense", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("PostCodeDialog", "Thank you for joining the BSense Project!", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("PostCodeDialog", "This software client is for measuring the quality of your broadband connection over time and it only collects measurement results that will be used for broadband mapping in your area.", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("PostCodeDialog", "We would like to ask you a few details about your Internet connection.", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("PostCodeDialog", "All data collected is used only for the purposes of the project. If you have any question, please email us at: info@broadbandforall.net", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("PostCodeDialog", "1", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("PostCodeDialog", "What is your Postcode?", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("PostCodeDialog", "2", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("PostCodeDialog", "How are you connected to the Internet?", 0, QApplication::UnicodeUTF8));
        connectionComboBox->clear();
        connectionComboBox->insertItems(0, QStringList()
         << QApplication::translate("PostCodeDialog", "-- Please select --", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "ADSL", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "3G", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "Satellite", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "Cable", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "Dialup", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "Public wireless", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "ISDN", 0, QApplication::UnicodeUTF8)
        );
        label_11->setText(QApplication::translate("PostCodeDialog", "3", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("PostCodeDialog", "Service Provider (ISP)", 0, QApplication::UnicodeUTF8));
        ispComboBox->clear();
        ispComboBox->insertItems(0, QStringList()
         << QApplication::translate("PostCodeDialog", "-- Please select --", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "Other", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "AOL", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "Aquiss Internet", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "Avanti", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "Beaming", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "Be Broadband", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "BT", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "Bulldog", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "Dark Group Ltd", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "Demon", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "Eclipse Internet", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "Exa Networks", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "Fast.co.uk", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "Firefly Internet", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "Firenet Internet", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "Freedom2Surf", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "Gravity Internet", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "Greenbee Broadband", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "Green ISP Broadband", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "IDNet", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "Karoo", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "Keycom", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "Lumison Limited", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "MacAce.net", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "Madasafish", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "Namesco Ltd", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "Netplan Internet Solutions Ltd", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "NewNet", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "Nildram", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "O2", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "Odyssey Systems Ltd", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "Orange", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "Pipex Internet", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "PlusNet Broadband", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "Polestar Interactive Ltd", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "Post Office Limited", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "Rutland Telecom", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "SAQ - Service Access Quality", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "Scotnet", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "Sky", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "SmallWorld", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "Solutios", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "Stream Networks Ltd (StreamNET)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "Surf Anytime", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "TalkTalk", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "Tesco", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "The Phone Co-op", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "Three", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "Tiscali", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "T-Mobile", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "Toucan", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "Twang", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "UK Online", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "Virgin Media", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "Vivaciti", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "Vodafone", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "Waitrose Broadband", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "YSH", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "Zen Internet", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "Zetnet", 0, QApplication::UnicodeUTF8)
        );
        label_12->setText(QApplication::translate("PostCodeDialog", "4", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("PostCodeDialog", "What is the advertised download speed of your connection (in Mbps)?", 0, QApplication::UnicodeUTF8));
        downloadComboBox->clear();
        downloadComboBox->insertItems(0, QStringList()
         << QApplication::translate("PostCodeDialog", "-- Please select --", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "I don't know", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "0.256", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "0.512", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "2", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "3", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "4", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "5", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "6", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "7", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "8", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "9", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "10", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "12", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "16", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "18", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "20", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "24", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "50", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "100", 0, QApplication::UnicodeUTF8)
        );
        label_13->setText(QApplication::translate("PostCodeDialog", "5", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("PostCodeDialog", "What is the advertised upload speed of your connection (in Mbps)?", 0, QApplication::UnicodeUTF8));
        uploadComboBox->clear();
        uploadComboBox->insertItems(0, QStringList()
         << QApplication::translate("PostCodeDialog", "-- Please select --", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "I don't know", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "0.128", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "0.256", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "0.512", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "1.2", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "1.3", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "1.5", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "2", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "2.5", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "3", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "4", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "5", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "6", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "7", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "8", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "9", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "10", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "12", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "16", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "18", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "20", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "24", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "50", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PostCodeDialog", "100", 0, QApplication::UnicodeUTF8)
        );
        label_15->setText(QApplication::translate("PostCodeDialog", "What is your email address?", 0, QApplication::UnicodeUTF8));
        label_16->setText(QApplication::translate("PostCodeDialog", "6", 0, QApplication::UnicodeUTF8));
        label_17->setText(QApplication::translate("PostCodeDialog", "<i><strong>Privacy note:</strong> we will not provide your address to anyone, and we will not send unsolicited messages.</i>", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class PostCodeDialog: public Ui_PostCodeDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_POSTCODEDIALOG_H
