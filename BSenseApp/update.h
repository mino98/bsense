#ifndef UPDATE_H
#define UPDATE_H

#include <QMainWindow>
#include <QBuffer>
#include <QDir>

class QFile;
class QHttp;
class QWidget;

class Update : public QMainWindow
{
    Q_OBJECT

public:
        // Self Update
        Update();
        void startTimer();
        void setApplicationDir(QString);
        QBuffer xmldata;
        int getRequestId;
        int filedownloaded;
        QString latest_version;
        QStringList filelist; // The list of files to be downloaded
        QFile *file;
        QHttp *http;
        QHttp *dhttp;

private:
        void setupMenu();
        void self_update();
        void removeTmpDirTree(QDir dir);
        void removeOldFiles(QDir qdir);
        QString applicationDir;

private slots:
        // Self Update
        void startUpdate();
        void self_update_parse(int requestId, bool error);
        void self_update_download(int i);
        void self_update_complete(int requestId, bool error);

};

#endif // UPDATE_H
