#ifndef BSENSEPROBE_H
#define BSENSEPROBE_H

#include <QNetworkAccessManager>
#include <QThread>
#include <QProcess>
#include <QMutex>
#include "update.h"

class QHttp;
class QNetworkReply;
class QFile;
class BSenseGui;

class BSenseProbe : public QObject
{
Q_OBJECT

public:
    BSenseProbe();
    void setGui(BSenseGui*);
    void setApplicationDir(QString);
    void startTimer();
    int testPostCode();
    void testEmailAddress();
    bool isRunning();

public slots:
    void testNetwork();
    void argumentsHttpFinished();
    void postCodeFinished();
    void emailAddressFinished();
    void httpFinishedResp();
    void recvHttpFinished();
    void finishedWaitingBeforeRecv();
    void finishedSendingFailure();
    void finishedItgSend(int, QProcess::ExitStatus);
    void finishedItgRecv(int, QProcess::ExitStatus);
    void finishedItgSendDec(int, QProcess::ExitStatus);
    void finishedItgRecvDec(int, QProcess::ExitStatus);
    void sendProcTimeout();
    void recvProcTimeout();
    void decSendProcTimeout();
    void decRecvProcTimeout();

private:
    void getTestParameters();
    void sendFailure(int, QString);
    void setRunning(bool);
    void deleteTemporaryFiles();
    QString osVersion();

    QNetworkAccessManager qnam;
    QNetworkAccessManager qnamPC;
    QNetworkAccessManager qnam2;
    QNetworkReply *sendReply;
    QNetworkReply *recvReply;
    QNetworkReply *postCodeReply;
    QNetworkReply *updateEmailReply;
    QNetworkReply *resultReply;
    QNetworkReply *errorReply;
    QString serverAddr;
    QString upId, downId;
    QFile *file;
    QProcess sendProc;
    QProcess recvProc;
    QProcess sendDecProc;
    QProcess recvDecProc;
    QString sendCommand;
    QString recvCommand;
    QString decSendCommand;
    QString decRecvCommand;
    QByteArray sendResults, recvResults;
    BSenseGui *bSenseGui;
    bool running;
    QMutex lock;
    QString applicationDir;
    QByteArray arguments;
    bool sendTimeout;
    bool recvTimeout;
    bool decSendTimeout;
    bool decRecvTimeout;
};

#endif
