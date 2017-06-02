#include "update.h"
#include "bsense.h"
#include "bsenseprobe.h"
#include <QUrl>
#include <QDomDocument>
#include <QMessageBox>
#include <QFileInfo>
#include <QHttp>
#include <QDir>
#include <QMenu>
#include <QMenuBar>
#include <QSettings>
#include <QDateTime>
#include <QDebug>
#include <QTimer>
#include <QDesktopServices>


Update::Update() : QMainWindow() {
    // Self update
    http = new QHttp(this);
    dhttp = new QHttp(this);
    connect(http, SIGNAL(requestFinished(int,bool)), this, SLOT(self_update_parse(int, bool)));
    connect(dhttp, SIGNAL(requestFinished(int,bool)), this, SLOT(self_update_complete(int, bool)));
    qDebug() << "Update object created";
}

void Update::setApplicationDir(QString applicationDir) {
    this->applicationDir = applicationDir;
}

void Update::startTimer() {
#if !defined(Q_OS_WIN32)
    // Remove old files (*.old):
    qDebug() << "Starting to remove *.old files from: "
            << this->applicationDir;
    removeOldFiles(this->applicationDir);
#endif

    // Start network test timer
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(startUpdate()));
    QTimer::singleShot(0, this, SLOT(startUpdate()));
    timer->start(UPDATE_TIMER_TIMEOUT * 60 * 60 * 1000);
    qDebug() << (QString("Update timer started. Timeout: ") + UPDATE_TIMER_TIMEOUT + " hours");
}


void Update::removeOldFiles(QDir qdir) {
    QString curPath = qdir.path();
    qDebug() << "Removing *.old files. Entering " + curPath;
    qdir.setNameFilters(QStringList("*.old"));
    QStringList oldFileList = qdir.entryList();
    for (int i = 0; i < oldFileList.count(); i++) {
        qdir.remove(oldFileList[i]);
        qDebug() << "Removed " + oldFileList[i];
    }
    qdir.setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);
    QStringList dirs = qdir.entryList();
    for (int i = 0; i < dirs.count(); i++) {
        QString newDirPath(curPath + "/" + dirs[i]);
        QDir newDir(newDirPath);
        removeOldFiles(newDir);
    }
}


void Update::startUpdate() {
    // Check for updates if last update more than UPDATE_TIMEOUT_SETTING
    QSettings settings;
    QVariant updateTimeout = settings.value(UPDATE_TIMEOUT_SETTING);
    if (updateTimeout.isNull() ||
        updateTimeout.toDateTime() >= QDateTime::currentDateTime()) {
        self_update();
    }
}


void Update::self_update() {
    QUrl url(QString(UPDATE_URL) + UPDATE_XML);
    http->setHost(url.host());
    xmldata.open(QBuffer::ReadWrite);
    getRequestId = http->get(url.path(), &xmldata);
    qDebug() << "Self Update with URL: " << url.toString();
}


void Update::self_update_parse(int requestId, bool error) {
    // If the requests don't match, exit
    if (requestId != getRequestId)  {
        qDebug() << "Request ids don't match: " << requestId << getRequestId;
        return;
    }

    // Read & parse the data
    xmldata.seek(0);
    xmldata.readAll();
    qDebug() << "XML data read";

    // Check file type
    QDomDocument doc("XML");
    if (!doc.setContent(xmldata.buffer(), true)) {
        qDebug() << "Update document not XML";
        return;
    }

    QDomElement root = doc.documentElement();
    if (root.tagName() != "appname") {
        qDebug() << "Root tag not \"appname\"";
        return;
    }

    // Loop over main nodes
    latest_version.clear();
    filelist.clear();
    QDomNode mainnode = root.firstChild();

    // Parse version number
    QDomNodeList versionNodeList = root.elementsByTagName("version");
    if ((versionNodeList.isEmpty()) ||
        (!versionNodeList.item(0).isElement())) {
        qDebug() << "No version number in XML file";
        return;
    }
    latest_version = versionNodeList.item(0).toElement().attribute("value");
    if (latest_version.isNull() || (latest_version.trimmed() == "")) {
        qDebug() << "Wrong version number value in XML file";
        return;
    }

    // Parse files
    QDomNodeList fileNodeList = root.elementsByTagName("file");
    if (fileNodeList.isEmpty()) {
        qDebug() << "No file list in XML file";
        return;
    }
    for (int i = 0; i < fileNodeList.count(); i++) {
        if (!fileNodeList.item(i).isElement()) {
            qDebug() << "File in XML file is not an XML element";
            return;
        }
        QString fileName = fileNodeList.item(i).toElement().attribute("value");
        if (fileName.isNull() || (fileName.trimmed() == "")) {
            qDebug() << "File in XML file doesn't have a proper \"value\" element";
            return;
        }
        filelist.push_back(fileName);
    }

    // If version number is "end" tell user to uninstall
    if (latest_version == "end") {
        // TODO fix messagebox uninstall instructions
        qDebug() << "Current version old: " << VERSION << ", new version: " << latest_version;
#if defined(Q_OS_LINUX)
        QString htmlText =
                "<html>"
                "<b>The BSense broadband measurement experiment has finished.</b><br>"
                "You can now uninstall BSense from your system.<br><br>"
                "Thank you for your partecipation.<br>"
                "For feedback and comments, please write us at info@broadbandforall.net</html>";
#elif defined(Q_OS_WIN32)
        QString  htmlText =
                "<html>"
                "<b>The BSense broadband measurement experiment has finished.</b><br>"
                "You can now uninstall BSense from your system.<br>"
                "To do so, select Start->All Programs->BSense->Uninstall<br><br>"
                "Thank you for your partecipation.<br>"
                "For feedback and comments, please write us at info@broadbandforall.net</html>";
#elif defined(Q_OS_MAC)
        QString  htmlText =
                "<html>"
                "<b>The BSense broadband measurement experiment has finished.</b><br>"
                "You can now uninstall BSense from your system.<br>"
                "To do so, drag the BSense application from the Application folder to the trash bin.<br><br>"
                "Thank you for your partecipation.<br>"
                "For feedback and comments, please write us at info@broadbandforall.net</html>";
#else
        QString  htmlText =
                "<html>"
                "<b>The BSense broadband measurement experiment has finished.</b><br><br>"
                "You can now uninstall BSense from your system"
                "For feedback and comments, please write us at info@broadbandforall.net</html>";
#endif

        // Show message of end of BSense experiment
        QMessageBox::question(this, tr("BSense broadband test finished"),
                              htmlText.trimmed(),
                              QMessageBox::Ok);

        exit(0);
    }

    // Compare current version with the one on the server
    if (VERSION < latest_version) {
        qDebug() << "Current version old: " << VERSION << ", new version: " << latest_version;
#if defined(Q_OS_WIN32)
        QString htmlText =
                "<html>"
                "<b>A new version of BSense is available:</b> " + latest_version + "<br>" +
                "It is very important to run the latest version of the software at all times.<br><br>" +
                "Would you like to download the new version?<br>" +
                "Once the new version has been downloaded, please<br>" +
                "double-click the downloaded file to install.</html>";
#else
        QString htmlText =
                "<html>"
                "<b>A new version of BSense is available:</b> " + latest_version + "<br>" +
                "It is very important to run the latest version of the software at all times.<br><br>" +
                "Would you like to download the new version?</html>";
#endif

        // Check if user wants to download
        if (QMessageBox::question(this, tr("BSense: New Version Available"),
                                  htmlText.trimmed(),
                                  QMessageBox::Yes,
                                  QMessageBox::No)
            == QMessageBox::No) {
            return;
        }

#if defined(Q_OS_WIN32)
        qDebug() << "Update with Windows: redirect to URL";
        QDesktopServices::openUrl(
                QUrl(QString() +
                     "www.broadbandforall.net/dl/installers/bsense-windows.exe"));
        exit(0);
#endif

                // Start Download
        qDebug() << "Start Download";
        filedownloaded = 0;
        self_update_download(0);
    } else {
        qDebug() << "Current version latest: " << latest_version;
    }
}


void Update::self_update_download(int i) {
    // Create a temporary folder
    if(!QFile::exists(this->applicationDir + TMP_DIR)) {
        QDir dir;
        dir.mkpath(this->applicationDir + TMP_DIR);
        qDebug() << "Temp dir created: " + this->applicationDir + TMP_DIR;
    }

    // More files to be downloaded
    if(i < filelist.size()) {
        QString urlPathName = latest_version + "_" + OS_IDENT + "/" + filelist[i];
        QUrl url(UPDATE_URL + urlPathName);
        QString tmpFilePathName = this->applicationDir + QString(TMP_DIR) + "/" + filelist[i];
        qDebug() << "More files to be downloaded.\nURL: " << url.path() << "\nFILE: " << tmpFilePathName;

        if (QFile::exists(tmpFilePathName)) {
            if (QMessageBox::question(this, tr("BSense"),
                                      tr("There already exists a file called %1 in "
                                         "the current directory. Overwrite?").arg(tmpFilePathName),
                                      QMessageBox::Ok|QMessageBox::Cancel, QMessageBox::Cancel)
                == QMessageBox::Cancel)
                return;
            QFile::remove(tmpFilePathName);
            qDebug() << "Existing file removed";
        }

        // Create subdirectories
        QFileInfo tmpFileInfo(tmpFilePathName);
        QDir dir;
        dir.mkpath(tmpFileInfo.path());
        qDebug() << "Path created: " + tmpFileInfo.path();

        file = new QFile(tmpFilePathName);
        if (!file->open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("BSense"),
                                     tr("Unable to save the file %1: %2.")
                                     .arg(tmpFilePathName).arg(file->errorString()));
            delete file;
            file = 0;
            return;
        }

        QHttp::ConnectionMode mode = url.scheme().toLower() == "https" ? QHttp::ConnectionModeHttps : QHttp::ConnectionModeHttp;
        dhttp->setHost(url.host(), mode, url.port() == -1 ? 0 : url.port());

        QByteArray path = QUrl::toPercentEncoding(url.path(), "!$&'()*+,;=:@/");
        if (path.isEmpty())
            path = "/";
        getRequestId = dhttp->get(path, file);
        qDebug() << "File request sent";
    }

    // Last file downloaded
    else if(filedownloaded == filelist.size() && filedownloaded != 0){
        qDebug() << "No more files to be downloaded";

        QDir dir(this->applicationDir);

        // TODO maybe we should delete all old files (not only the ones that have the
        // same name as the new ones

        // Backup old files and rename them
        for(int i=0; i<filelist.size(); i++){
            dir.rename(this->applicationDir + "/" + filelist[i],
                       this->applicationDir + "/" + filelist[i] + ".old");
            qDebug() << "Renaming file: "
                    << this->applicationDir + "/" + filelist[i]
                    << " to: "
                    << this->applicationDir + "/" + filelist[i] + ".old";
        }

        // Copy new file from tmp dir to app dir
        for (int i = 0; i < filelist.size(); i++) {
            QFileInfo fileInfo(filelist[i]);

            dir.mkpath(this->applicationDir + "/" + fileInfo.path());
            qDebug() << "Dir created: "
                    << this->applicationDir + "/" + fileInfo.path();

            bool ok = QFile::copy(this->applicationDir + TMP_DIR + "/" + filelist[i], this->applicationDir + "/" + filelist[i]);
            qDebug() << "Copied file: "
                    << this->applicationDir + TMP_DIR + "/" + filelist[i]
                    << " to: "
                    << this->applicationDir + "/" + filelist[i]
                    << ok;

            // Set permissions
            QFile oldFile(this->applicationDir + "/" + filelist[i] + ".old");
            if (oldFile.exists()) {
                QFile::setPermissions(this->applicationDir + "/" + filelist[i], oldFile.permissions());
                qDebug() << "Set the permissions of: "
                        << this->applicationDir + "/" + filelist[i]
                        << " as those of: "
                        << this->applicationDir + "/" + filelist[i] + ".old";
            } else {
                QFile::setPermissions(this->applicationDir + "/" + filelist[i], QFile::ExeOwner | QFile::ExeUser | QFile::ExeGroup |
                                      QFile::ExeOther | QFile::WriteOwner | QFile::ReadOwner |
                                      QFile::ReadUser | QFile::ReadGroup | QFile::ReadOther);
                qDebug() << "Set the permissions of: "
                        << this->applicationDir + "/" + filelist[i];
            }

            ok = QFile::remove(this->applicationDir + TMP_DIR + "/" + filelist[i]);
            qDebug() << "Removed file: "
                    << this->applicationDir + TMP_DIR + "/" + filelist[i]
                    << ok;
        }

        // Remove tmp dir tree
        qDebug() << "Starting to remove *.old files from: "
                 << this->applicationDir + TMP_DIR;
        QDir tmpDir(this->applicationDir + TMP_DIR);
        removeTmpDirTree(tmpDir);

        QDir().rmdir(this->applicationDir + TMP_DIR);
        qDebug() << "Tmp dir "
                 << this->applicationDir + TMP_DIR
                 << " removed";

        // Set new update date setting
        QSettings settings;
        QDateTime nextUpdateDate = QDateTime::currentDateTime().addDays(UPDATE_TIMEOUT_DAYS);
        settings.setValue(UPDATE_TIMEOUT_SETTING, nextUpdateDate);
        qDebug() << "New update time setting: " << nextUpdateDate;

        QMessageBox::information(this, tr("Update Sucess"), tr("Please restart BSense for the updates to take effect."));
        exit(0);
    }
}


void Update::removeTmpDirTree(QDir qdir) {
    QString curPath = qdir.path();
    qDebug() << "Removing tmp path. Entering " + curPath;
    qdir.setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);
    QStringList dirs = qdir.entryList();
    for (int i = 0; i < dirs.count(); i++) {
        QString newDirPath(curPath + "/" + dirs[i]);
        QDir newDir(newDirPath);
        removeTmpDirTree(newDir);
        bool ok = qdir.rmdir(dirs[i]);
        qDebug() << "Removed " + newDirPath + ": " << ok;
    }
}


void Update::self_update_complete(int requestId, bool error) {

    if(requestId != getRequestId)  {
        qDebug() << "Ids don't match: " << requestId << ", " << getRequestId;
        return;
    }

    file->close();

    if (error) {
        file->remove();
        qDebug() << "Error downloading file: " << dhttp->errorString();
        QMessageBox::information(this, tr("BSense"),
                                 tr("Download failed: %1.")
                                 .arg(dhttp->errorString()));
    } else {
        filedownloaded++;
        qDebug() << "File downloaded correctly";
    }

    delete file;
    file = 0;
    self_update_download(filedownloaded);
}
