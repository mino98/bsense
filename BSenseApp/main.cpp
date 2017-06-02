/**
 * BSense   Broadband testing application
 *          Giacomo Bernardi - g.bernardi@sms.ed.ac.uk
 *          WiMo Group - School of Informatics - University of Edinburgh, UK
 */

#include <QtGui>
#include "bsense.h"
#include "bsensegui.h"
#include "bsenseprobe.h"
#include "update.h"
#include <QSettings>


// The following imports the qsvg plugin, when compiling statically on Mac and Linux:
#if !defined(QT_SHARED) && (defined(Q_OS_LINUX) || defined(Q_OS_MAC))
    #include <QtPlugin>
    Q_IMPORT_PLUGIN(qsvg)
#endif

int main(int argc, char *argv[]) {
    // Initialize our own resources:
    Q_INIT_RESOURCE(bsense);
    QApplication app(argc, argv);

    // Set application details
    QCoreApplication::setOrganizationName(ORG_NAME);
    QCoreApplication::setOrganizationDomain(DOMAIN_NAME);
    QCoreApplication::setApplicationName(APP_NAME);

    // Set autostart
#if defined(Q_OS_LINUX)
#elif defined(Q_OS_WIN32)
#elif defined(Q_OS_MAC)
    QSettings settings(QFileInfo(QSettings().fileName()).absoluteDir().absolutePath() +
                       "/loginwindow.plist",
                       QSettings::NativeFormat);
    QList<QVariant> autoStartAppList = settings.value("AutoLaunchedApplicationDictionary").toList();
    bool bsenseFound = false;
    for (int i = 0; i < autoStartAppList.size(); i++) {
        if (autoStartAppList[i].toMap().value("Path").toString().contains(QString("bsense"))) {
            bsenseFound = true;
            break;
        }
    }
    if (!bsenseFound) {
        QProcess uperfProc;
        QString command = QString() + "bash -c \"defaults write loginwindow AutoLaunchedApplicationDictionary -array-add \'{Hide=0;Path=" +
                          app.applicationDirPath() + "/../../../bsense.app;}\'\"";
        uperfProc.start(command);
        uperfProc.waitForStarted();
        uperfProc.waitForFinished();
        qDebug() << "Autostart property set: " << command + "\n" << uperfProc.readAll();
    }
#else
#endif

    // Find and set the application directory:
    QString applicationDir = QApplication::applicationDirPath();
    applicationDir = QDir::toNativeSeparators(applicationDir);

#if defined(Q_OS_MAC)
    // Only on Mac, we need to remove all that follows '/Contents/MacOS' at the end
    // (e.g.: from /path/bsense.app/Contents/MacOS to /path/bsense.app)
    applicationDir = applicationDir.left(applicationDir.lastIndexOf(QString("/Contents/MacOS"), -1, Qt::CaseInsensitive));
#endif
    qDebug() << (QString("Application dir: ") + applicationDir);

    // Let's try to create the systray icon:
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(0,
                              QObject::tr("BSense"),
                              QObject::tr("I couldn't detect any system tray on this system.")
                              );
        return 1;
    }

    // Keep the application running even when there are no open windows:
    QApplication::setQuitOnLastWindowClosed(false);

    // Instantiate the GUI
    BSenseGui bSenseGui;

    // Instantiate the network test class
    BSenseProbe bSenseProbe;
    bSenseProbe.setGui(&bSenseGui);
    bSenseProbe.setApplicationDir(applicationDir);
    bSenseGui.setProbe(&bSenseProbe);

    // Test postcode setting
    int postcodeTestStatus = bSenseProbe.testPostCode();
    if(postcodeTestStatus == 1) {
        // settings are valid, let's check if there is a valid email address:
        bSenseProbe.testEmailAddress();

        // finally, start the timer for the network measurements:
        bSenseProbe.startTimer();
    } else {
        qDebug() << "default";
    }

    // Instantiate update class
    Update update;
    update.setApplicationDir(applicationDir);
    update.startTimer();

    // Run the app:
    return app.exec();
}

