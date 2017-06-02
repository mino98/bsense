#ifndef BSENSEGUI_H
#define BSENSEGUI_H

#include <QSystemTrayIcon>
#include <QDialog>

QT_BEGIN_NAMESPACE
class QAction;
class QCheckBox;
class QComboBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QMenu;
class QPushButton;
class QSpinBox;
class QTextEdit;
class BSenseProbe;
QT_END_NAMESPACE

class BSenseGui : public QDialog {
    Q_OBJECT

    public:
        BSenseGui();
        void setIcon();
        bool isTestEnabled();
        void setProbe(BSenseProbe*);
        QString applicationDir;

    protected:

    private slots:
        void showMessage(QString, QSystemTrayIcon::MessageIcon);
        void about();
        void stats();
        void problem();
        void enableTriggered();

    private:
        void createTrayIcon();
        void setIcon(int);
        enum {green, yellow, red};
        QAction *enableTestsAction;
        QAction *quitAction;
        QAction *helpAction;
        QAction *statsAction;
        QAction *problemAction;
        BSenseProbe *bSenseProbe;
        QSystemTrayIcon *trayIcon;
        QMenu *trayIconMenu;

};

#endif
