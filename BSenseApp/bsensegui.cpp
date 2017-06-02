/**
 * BSense   Broadband testing application
 *          Giacomo Bernardi - g.bernardi@sms.ed.ac.uk
 *          WiMo Group - School of Informatics - University of Edinburgh, UK
 */

#include <QtGui>
#include "bsense.h"
#include "bsensegui.h"
#include "bsenseprobe.h"

BSenseGui::BSenseGui() {
    // Create the contextual menu and the tray icon:
    createTrayIcon();
}


void BSenseGui::setProbe(BSenseProbe *probe) {
    bSenseProbe = probe;
}


/**
 * Shows the message bubble near the systray:
 */
void BSenseGui::showMessage(QString message, QSystemTrayIcon::MessageIcon messageType) {
    QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(messageType);
    trayIcon->showMessage(tr("BSense"), message, icon, 10000);
}

/**
 * Select one of the three available icons, the index can be:
 * - 0: normal green icon
 * - 1: yellow icon, test in progress
 * - 2: red icon, the last test failed
 */
void BSenseGui::setIcon(int index) {
    switch(index) {
        case green:
            trayIcon->setIcon(QIcon(":/images/bsense-green.svg"));
            trayIcon->setToolTip(tr("No broadband measurement is currently running."));
            break;

        case yellow:
            trayIcon->setIcon(QIcon(":/images/bsense-yellow.svg"));
            trayIcon->setToolTip(tr("A broadband measurement is in progress..."));
            break;

        case red:
            trayIcon->setIcon(QIcon(":/images/bsense-red.svg"));
            trayIcon->setToolTip(tr("Broaband measurements are currently disabled."));
            break;
    }
}

/**
 * Creates the tray icon and the associated contextual menu:
 */
void BSenseGui::createTrayIcon() {
    // Create the menu items
    enableTestsAction = new QAction(tr("&Enable broadband measurements"), this);
    enableTestsAction->setCheckable(true);
    enableTestsAction->setChecked(true);
    connect(enableTestsAction, SIGNAL(triggered(bool)), this, SLOT(enableTriggered()));

    statsAction = new QAction("&Show my broadband statistics...", this);
    connect(statsAction, SIGNAL(triggered()), this, SLOT(stats()));
    problemAction = new QAction("&Report a problem with this software...", this);
    connect(problemAction, SIGNAL(triggered()), this, SLOT(problem()));

    helpAction = new QAction("&About BSense", this);
    connect(helpAction, SIGNAL(triggered()), this, SLOT(about()));

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    // Assemble everything:
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(enableTestsAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(statsAction);
    trayIconMenu->addAction(problemAction);
    trayIconMenu->addAction(helpAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);

    // Add the default icon:
    BSenseGui::setIcon(0);

    // Show the icon:
    trayIcon->show();
}


bool BSenseGui::isTestEnabled() {
    return enableTestsAction->isChecked();
}


void BSenseGui::about() {
    QString  htmlText =
            QString() + "<html>"
            "<b>BSense broadband monitoring system</b><br />"
            "Version " + VERSION + " for " + OS_IDENT + "<br /><br />"

            "A research project by the School of Informatics, University of Edinburgh.<br /><br />"

            "This software is licensed under the term of the <a href='http://www.gnu.org/licenses/gpl-3.0.txt'>GPL v3 License</a>, and "
            "includes parts of the <a href='http://www.grid.unina.it/software/ITG/'>D-ITG</a> project, which are distributed under the "
            "same terms.<br /><br />"

            "For feedback and comments, please write us at <a href='mailto:info@broadbandforall.net'>info@broadbandforall.net</a></html>";

    QMessageBox::information(this, tr("About BSense"),
                            htmlText.trimmed());

}

void BSenseGui::stats() {
    QSettings settings;
    QVariant uuidVar = settings.value(UUID_SETTING);
    if (!uuidVar.isNull()) {
       QDesktopServices::openUrl(QUrl("http://www.broadbandforall.net/stats/?UUID=" +
                                      uuidVar.toString()));
    }
}

void BSenseGui::problem() {
    QSettings settings;
    QVariant uuidVar = settings.value(UUID_SETTING);
    if (!uuidVar.isNull()) {
        QDesktopServices::openUrl(QUrl(QString() +
                                       "http://www.broadbandforall.net/support/?UUID=" +
                                       uuidVar.toString() +
                                       "&version=" + VERSION +
                                       "&OS=" + OS_IDENT));
    }
}


void BSenseGui::enableTriggered() {
    setIcon();
}


void BSenseGui::setIcon() {
    if (isTestEnabled()) {
        if (bSenseProbe->isRunning())
            setIcon(yellow);
        else
            setIcon(green);
    } else
        setIcon(red);
}


