/**
 * BSense   Broadband testing application
 *          Giacomo Bernardi - g.bernardi@sms.ed.ac.uk
 *          WiMo Group - School of Informatics - University of Edinburgh, UK
 */

#include <QtGui>
#include "bsense.h"
#include "bsensegui.h"
#include "bsenseprobe.h"
#include <QtNetwork>
#include <QDebug>
#include "ui_postcodedialog.h"
#include "ui_emaildialog.h"
#include <QNetworkReply>
#include <QSysInfo>

#if defined(Q_OS_LINUX)
#include <sys/utsname.h>
#endif


BSenseProbe::BSenseProbe() {
    running = false;
    sendTimeout = false;
    recvTimeout = false;
    decSendTimeout = false;
    decSendTimeout = false;

    // Connect process signals to slots
    connect(&sendProc, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(finishedItgSend(int, QProcess::ExitStatus)));
    connect(&recvProc, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(finishedItgRecv(int, QProcess::ExitStatus)));
    connect(&sendDecProc, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(finishedItgSendDec(int, QProcess::ExitStatus)));
    connect(&recvDecProc, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(finishedItgRecvDec(int, QProcess::ExitStatus)));
}


void BSenseProbe::setGui(BSenseGui *gui) {
    bSenseGui = gui;
}

void BSenseProbe::setApplicationDir(QString applicationDir) {
    this->applicationDir = applicationDir;
}

void BSenseProbe::testEmailAddress() {
    QSettings settings;
    QString emailAddress;
    bool errors;

    if (!settings.value(EMAIL_SETTING).isNull())
        return;

    do {
        errors = false;

        // Ask for information with dialog
        QDialog dlg;
        Ui::EmailDialog ui;
        ui.setupUi(&dlg);
        dlg.adjustSize();
        if (dlg.exec() == 0)
            exit(0);

        // Get information from the form:
        emailAddress = ui.emailAddress->text();
        emailAddress = emailAddress.trimmed();
        emailAddress = emailAddress.toLower();

        if(emailAddress == "") {
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setWindowTitle("BSense: error");
            msgBox.setText("Please enter a valid email address.");
            msgBox.setStandardButtons(QMessageBox::Retry);
            msgBox.setDefaultButton(QMessageBox::Retry);
            msgBox.exec();
            errors = true;
        }
    } while (errors);

    // Send information to server
    QUrl parameterUrl;
    parameterUrl.setUrl(URL);
    parameterUrl.addQueryItem(ACTION_PARAM, UPDATE_EMAIL_ACTION);
    parameterUrl.addQueryItem(UUID_PARAM, settings.value(UUID_SETTING).toString());
    parameterUrl.addQueryItem("EmailAddress", emailAddress);
    updateEmailReply = qnamPC.get(QNetworkRequest(parameterUrl));
    connect(updateEmailReply, SIGNAL(finished()), this, SLOT(emailAddressFinished()));
    qDebug() << "Setting updateEmail parameters. Query: " << parameterUrl.encodedQuery();
}

void BSenseProbe::emailAddressFinished() {
    qDebug() << "Setting updateEmail parameters session finished. Status: " << updateEmailReply->error();

    // Check if error in network display check network dialog
    if (updateEmailReply->error() != QNetworkReply::NoError) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle("BSense: error");
        msgBox.setText("It seems that you are not currently connected to the Internet. Please check and then restart the application.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
        exit(0);
    }

    // Check if error in emailAddress. Display error dialog
    QString settingValue = updateEmailReply->readLine().trimmed();
    qDebug() << "Setting updateEmail parameters reply: " << settingValue;
    if (settingValue == ERROR_REPLY) {
        QString errorMessage = updateEmailReply->readLine();
        qDebug() << "Error Message: " << errorMessage;
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle("BSense: error");
        msgBox.setText(errorMessage);
        msgBox.setStandardButtons(QMessageBox::Retry);
        msgBox.setDefaultButton(QMessageBox::Retry);
        msgBox.exec();
        testEmailAddress();
    } else {
        // Save emails in the settings:
        QSettings settings;
        settings.setValue(EMAIL_SETTING, true);
    }
}

int BSenseProbe::testPostCode() {
    // Check Unique ID settings
    QSettings settings;

    // Ask for information if no ID
    if (settings.value(UUID_SETTING).isNull()) {
        bool errors;
        QString postcode;
        QString emailAddress;
        QString connectionType;
        QString isp;
        QString advertDownload;
        QString advertUpload;

        do {
            // Ask for information with dialog
            QDialog dlg;
            Ui::PostCodeDialog ui;
            ui.setupUi(&dlg);
            dlg.adjustSize();
            if (dlg.exec() == 0) {
                exit(0);
            }

            // Get information from the form:
            errors = false;
            postcode = ui.postcodeField->text();
            if(!errors && postcode.length() < 6) {
                QMessageBox msgBox;
                msgBox.setIcon(QMessageBox::Critical);
                msgBox.setWindowTitle("BSense: error");
                msgBox.setText("Please enter a valid postcode.");
                msgBox.setStandardButtons(QMessageBox::Retry);
                msgBox.setDefaultButton(QMessageBox::Retry);
                msgBox.exec();
                errors = true;
            }

            emailAddress = ui.emailField->text();
            if(!errors && emailAddress == "") {
                QMessageBox msgBox;
                msgBox.setIcon(QMessageBox::Critical);
                msgBox.setWindowTitle("BSense: error");
                msgBox.setText("Please enter a valid email address.");
                msgBox.setStandardButtons(QMessageBox::Retry);
                msgBox.setDefaultButton(QMessageBox::Retry);
                msgBox.exec();
                errors = true;
            }

            connectionType = ui.connectionComboBox->currentText();
            if(!errors && ui.connectionComboBox->currentIndex() == 0) {
                QMessageBox msgBox;
                msgBox.setIcon(QMessageBox::Critical);
                msgBox.setWindowTitle("BSense: error");
                msgBox.setText("Please select how you are connected to the Internet from the list.");
                msgBox.setStandardButtons(QMessageBox::Retry);
                msgBox.setDefaultButton(QMessageBox::Retry);
                msgBox.exec();
                errors = true;
            }

            isp = ui.ispComboBox->currentText();
            if(!errors && ui.ispComboBox->currentIndex() == 0) {
                QMessageBox msgBox;
                msgBox.setIcon(QMessageBox::Critical);
                msgBox.setWindowTitle("BSense: error");
                msgBox.setText("Please select which is your Internet Service Provider (ISP) from the list.");
                msgBox.setStandardButtons(QMessageBox::Retry);
                msgBox.setDefaultButton(QMessageBox::Retry);
                msgBox.exec();
                errors = true;
            }

            advertDownload = ui.downloadComboBox->currentText();
            if(!errors && ui.downloadComboBox->currentIndex() == 0) {
                QMessageBox msgBox;
                msgBox.setIcon(QMessageBox::Critical);
                msgBox.setWindowTitle("BSense: error");
                msgBox.setText("Please select what is the download speed that was advertised for your connectivity package.");
                msgBox.setStandardButtons(QMessageBox::Retry);
                msgBox.setDefaultButton(QMessageBox::Retry);
                msgBox.exec();
                errors = true;
            } else if(ui.downloadComboBox->currentIndex() == 1) {
                advertDownload = "-1";  // unknown
            }

            advertUpload = ui.uploadComboBox->currentText();
            if(!errors && ui.uploadComboBox->currentIndex() == 0) {
                QMessageBox msgBox;
                msgBox.setIcon(QMessageBox::Critical);
                msgBox.setWindowTitle("BSense: error");
                msgBox.setText("Please select what is the upload speed that was advertised for your connectivity package.");
                msgBox.setStandardButtons(QMessageBox::Retry);
                msgBox.setDefaultButton(QMessageBox::Retry);
                msgBox.exec();
                errors = true;
            } else if(ui.uploadComboBox->currentIndex() == 1) {
                advertUpload = "-1";    // unknown
            }
        } while (errors);

        // Send information to server
        QUrl parameterUrl;
        parameterUrl.setUrl(URL);
        parameterUrl.addQueryItem(ACTION_PARAM, REGISTER_ACTION);
        parameterUrl.addQueryItem("Postcode", postcode);
        parameterUrl.addQueryItem("TechnologyName", connectionType);
        parameterUrl.addQueryItem("OperatorName", isp);
        parameterUrl.addQueryItem("AdvertisedSpeedDown", advertDownload);
        parameterUrl.addQueryItem("AdvertisedSpeedUp", advertUpload);
        parameterUrl.addQueryItem("OS", OS_IDENT);
        parameterUrl.addQueryItem("OSVersion", osVersion());
        parameterUrl.addQueryItem("EmailAddress", emailAddress);
        postCodeReply = qnamPC.get(QNetworkRequest(parameterUrl));
        connect(postCodeReply, SIGNAL(finished()), this, SLOT(postCodeFinished()));
        qDebug() << "Setting postcode parameters. Query: " << parameterUrl.encodedQuery();

        return 2;
    }

    return 1;
}


void BSenseProbe::postCodeFinished() {
    qDebug() << "Setting postcode parameters session finished. Status: " << postCodeReply->error();

    // Check if error in network display check network dialog
    if (postCodeReply->error() != QNetworkReply::NoError) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle("BSense: error");
        msgBox.setText("It seems that you are not currently connected to the Internet. Please check and then restart the application.");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
        exit(0);
    }

    // Check if error in postcode. Display postcode error dialog
    QString settingValue = postCodeReply->readLine().trimmed();
    qDebug() << "Setting postcode parameters reply: " << settingValue;
    if (settingValue == ERROR_REPLY) {
        QString errorMessage = postCodeReply->readLine();
        qDebug() << "Error Message: " << errorMessage;
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle("BSense: error");
        msgBox.setText(errorMessage);
        msgBox.setStandardButtons(QMessageBox::Retry);
        msgBox.setDefaultButton(QMessageBox::Retry);
        msgBox.exec();
        testPostCode();
    } else {

        // Add UUID and Email to the settings
        QSettings settings;
        settings.setValue(UUID_SETTING, settingValue);
        settings.setValue(EMAIL_SETTING, true);

        // Start timer thread
        startTimer();
    }
}


void BSenseProbe::testNetwork() {
    if (bSenseGui->isTestEnabled()) {
        deleteTemporaryFiles();
        getTestParameters();
    } else
        qDebug() << "Network test disabled";
}


void BSenseProbe::deleteTemporaryFiles() {
    // Delete the temporary files:
    bool ret = false;

    ret = QFile::remove(ARGUMENT_FILE);
    qDebug() << "Removing file " << ARGUMENT_FILE << ": " << ret;

    ret = QFile::remove(UPSTREAM_BIN_LOG_FILE);
    qDebug() << "Removing file " << UPSTREAM_BIN_LOG_FILE << ": " << ret;

    ret = QFile::remove(DOWNSTREAM_BIN_LOG_FILE);
    qDebug() << "Removing file " << DOWNSTREAM_BIN_LOG_FILE << ": " << ret;

    ret = QFile::remove(UPSTREAM_TXT_LOG_FILE);
    qDebug() << "Removing file " << UPSTREAM_TXT_LOG_FILE << ": " << ret;

    ret = QFile::remove(DOWNSTREAM_TXT_LOG_FILE);
    qDebug() << "Removing file " << DOWNSTREAM_TXT_LOG_FILE << ": " << ret;
}

void BSenseProbe::startTimer() {
    // Start network test timer
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(testNetwork()));
    QTimer::singleShot(0, this, SLOT(testNetwork()));
    timer->start(MEASUREMENTS_INTERVAL);
    qDebug() << "Timer started. Timeout: " << MEASUREMENTS_INTERVAL << "ms";
}


void BSenseProbe::getTestParameters() {
    // Check if already running
    qDebug() << "Trying to lock running mutex";
    if (lock.tryLock()) {
        qDebug() << "Running mutex locked";
        if (isRunning()) {
            qDebug() << "BSense already running. Releasing running lock";
            lock.unlock();
            return;
        } else {
            qDebug() << "Setting BSense as running and releasing running lock";
            // Set test as running
            setRunning(true);
            lock.unlock();
        }
    } else {
        qDebug() << "Couldn't lock running mutex: already locked";
        return;
    }

    // Get UUID setting
    QSettings settings;
    QVariant uuid = settings.value(UUID_SETTING);

    // Check that UUID is set
    if (uuid.isNull()) {
        qDebug() << "Trying to get parameters while UUID not set!";
        exit(0);
    }

    // Retrieve arguments file from http server
    QUrl url;
    url.setUrl(URL);
    url.addQueryItem(ACTION_PARAM, UPSTREAM_PARAMS_ACTION);
    url.addQueryItem(UUID_PARAM, uuid.toString());
    sendReply = qnam.get(QNetworkRequest(url));
    connect(sendReply, SIGNAL(finished()), this, SLOT(argumentsHttpFinished()));
    qDebug() << "Retrieving parameters. Query: " << url.encodedQuery();
}


void BSenseProbe::argumentsHttpFinished() {
    qDebug() << "Retrieving parameters session finished. Status: " << sendReply->error();

    // Check if parameters OK
    if (sendReply->error() != QNetworkReply::NoError) {
        sendFailure(SEND_ARGUMENTS_RETRIEVING_ERROR, QString()
                    + "Error retrieving ITGSend arguments. \n"
                    + "Error code: " + sendReply->error());
        setRunning(false);
        return;
    }

    upId = sendReply->readLine().trimmed();
    if (upId.startsWith("error")) {
        QString errorMessage = sendReply->readAll();
        sendFailure(SEND_ARGUMENTS_RETRIEVING_ERROR, QString()
                    + "Error retrieving ITGSend arguments.\n"
                    + "Error: " + errorMessage);
        qDebug() << "Error retrieving parameters: " << errorMessage;
        setRunning(false);
        return;
    }

    arguments = sendReply->readAll();

    qDebug() << "Send parameters: " + arguments;

    // Write arguments in argument file
    // TODO it can probably be done better by redirecting output
    QFile argFile(ARGUMENT_FILE);

    if (!argFile.open(QIODevice::WriteOnly)) {
        sendFailure(SEND_ARGUMENTS_FILE_ERROR, QString()
                    + "Error opening ITGSend argument file.\n"
                    + "Experiment ID: " + upId + "\n"
                    + "File: " + ARGUMENT_FILE + "\n"
                    + "Arguments: " + arguments);
        setRunning(false);
        return;
    }
    argFile.write(arguments + "\n");
    argFile.close();

    // Run ITG send command
    sendCommand = "\"" + this->applicationDir + ITG_SEND_COMMAND + "\" " +
                  "\"" + ARGUMENT_FILE + "\" " +
                  LOG_SWITCH + " " +
                  "\"" + UPSTREAM_BIN_LOG_FILE + "\"";
    qDebug() << "Send command: " << sendCommand;
    sendProc.start(sendCommand);
    if (!(sendProc.waitForStarted())) {
        QString errorMsg = sendCommand + "\n"
                           + "ITGSend start error.\n"
                           + "Exit code: " + QString::number(sendProc.exitCode()) + "\n"
                           + "Experiment ID: " + upId + "\n"
                           + "Output:\n"
                           + sendProc.readAll() + "\n\n"
                           + "Arguments:\n"
                           + arguments;
        qDebug() << errorMsg;
        sendFailure(DITG_SEND_ERROR, errorMsg);
        setRunning(false);
        return;
    }

    // Start timeout for D-ITG process
    QTimer::singleShot(SEND_PROC_TIMEOUT, this, SLOT(sendProcTimeout()));

    qDebug() << "ITG send started OK";
}


void BSenseProbe::sendProcTimeout() {
    // Kills the process if still running
    if (sendProc.state() == QProcess::Running) {
        sendProc.kill();
        sendTimeout = true;
        qDebug() << "D-ITG Send process killed";
    }
}


void BSenseProbe::finishedItgSend(int signal, QProcess::ExitStatus status) {
    if ((signal != 0) || (status != QProcess::NormalExit)) {
#if defined(Q_OS_LINUX) || defined(Q_OS_MAC)
        QString pid = QString::number(sendProc.pid());
#else
        QString pid = "n/a";
#endif
        QString errorMsg = sendCommand + "\n"
                           + "ITGSend finish error.\n"
                           + (sendTimeout? "Timeout reached\n" : "") +
                           + "PID: " + pid + "\n"
                           + "Exit code: " + QString::number(sendProc.exitCode()) + "\n"
                           + "Signal: " + QString::number(signal) + "\n"
                           + "Exit status: " + QString::number(status) + "\n"
                           + "Experiment ID: " + upId  + "\n"
                           + "Output:\n"
                           + sendProc.readAll() + "\n\n"
                           + "Arguments:\n"
                           + arguments;
        sendTimeout = false;
        qDebug() << errorMsg;
        sendFailure(DITG_SEND_ERROR, errorMsg);
        setRunning(false);
        return;
    }
    qDebug() << "ITG send OK. Exit code: " << signal;

    // Start server for ITG recv by HTTP request
    QSettings settings;
    QUrl url;
    url.setUrl(URL);
    url.addQueryItem(ACTION_PARAM, DOWNSTREAM_PARAMS_ACTION);
    url.addQueryItem(UUID_PARAM, settings.value(UUID_SETTING).toString());
    recvReply = qnam.get(QNetworkRequest(url));
    connect(recvReply, SIGNAL(finished()), this, SLOT(recvHttpFinished()));
    qDebug() << "Starting request for send in server. Query: " << url.encodedQuery();
}


void BSenseProbe::recvHttpFinished() {
    qDebug() << "Request to start send in server finished. Status: " << recvReply->error();

    // Check reply OK
    if (recvReply->error() != QNetworkReply::NoError) {
        sendFailure(RECV_ARGUMENTS_RETRIEVING_ERROR, QString()
                    + "Error retrieving ITGRecv arguments.\n"
                    + "Error code: " + recvReply->error());
        setRunning(false);
        return;
    }

    // Experiment ID retrieval
    char buf[1024];
    qint64 lineLength = recvReply->readLine(buf, sizeof(buf));
    if (lineLength < 0) {
        sendFailure(RECV_ARGUMENTS_RETRIEVING_ERROR, QString()
                    + "Error retrieving ITGRecv experiment ID.\n"
                    + "Line length < 0");
        setRunning(false);
        return;
    } else if (lineLength == 0) {
        sendFailure(RECV_ARGUMENTS_RETRIEVING_ERROR, QString()
                    + "Error retrieving ITGRecv experiment ID.\n"
                    + "Line length == 0");
        setRunning(false);
        return;
    }

    if(QString(buf).trimmed() == "error") {
        QString errorMessage = recvReply->readAll();
        sendFailure(SEND_ARGUMENTS_RETRIEVING_ERROR, QString()
                    + "Error retrieving ITGRecv parameters.\n"
                    + "Error: " + errorMessage);
        qDebug() << "Error retrieving ITGRecv parameters: " << errorMessage;
        setRunning(false);
        return;
    }
    downId = QString(buf).trimmed(); // recvReply->readLine().trimmed();

    // Recv arguments retrieval
    lineLength = recvReply->readLine(buf, sizeof(buf));
    if (lineLength < 0) {
        sendFailure(RECV_ARGUMENTS_RETRIEVING_ERROR, QString()
                    + "Error retrieving ITGRecv arguments.\n"
                    + "Line length < 0.\n"
                    + "Experiment ID: " + downId);
        setRunning(false);
        return;
    } else if (lineLength == 0) {
        sendFailure(RECV_ARGUMENTS_RETRIEVING_ERROR, QString()
                    + "Error retrieving ITGRecv arguments.\n"
                    + "Line length == 0.\n"
                    + "Experiment ID: " + downId);
        setRunning(false);
        return;
    }
    serverAddr = QString(buf).trimmed();

    qDebug() << "Recv parameters: " + serverAddr;

    // Wait for WAIT_BEFORE_RECV ms
    QTimer::singleShot(WAIT_BEFORE_RECV, this, SLOT(finishedWaitingBeforeRecv()));

}


void BSenseProbe::finishedWaitingBeforeRecv() {
    qDebug() << "Finished waiting for server to start";

    // Run ITGRecv
    recvCommand = "\"" + this->applicationDir + ITG_RECV_COMMAND + "\" " +
                  serverAddr.trimmed() + " " +
                  LOG_SWITCH + " " +
                  "\"" + DOWNSTREAM_BIN_LOG_FILE + "\"";
    qDebug() << "Recv command: " + recvCommand;
    recvProc.start(recvCommand);
    if (!recvProc.waitForStarted()) {
        QString errorMsg = recvCommand + "\n"
                           + "ITGRecv start error.\n"
                           + "Exit code: " + QString::number(recvProc.exitCode()) + "\n"
                           + "Experiment ID: " + downId + "\n"
                           + recvProc.readAll();
        qDebug() << errorMsg;
        sendFailure(DITG_RECV_ERROR, errorMsg);
        setRunning(false);
        return;
    }

    // Recv timeout for D-ITG process
    QTimer::singleShot(RECV_PROC_TIMEOUT, this, SLOT(recvProcTimeout()));
}


void BSenseProbe::recvProcTimeout() {
    // Kills the ITG recv process if still running
    if (recvProc.state() == QProcess::Running) {
        recvProc.kill();
        recvTimeout = true;
        qDebug() << "D-ITG Recv process killed";
    }
}


void BSenseProbe::finishedItgRecv(int signal, QProcess::ExitStatus status) {
    if ((signal != 0) || (status != QProcess::NormalExit)) {
#if defined(Q_OS_LINUX) || defined(Q_OS_MAC)
        QString pid = QString::number(sendProc.pid());
#else
        QString pid = "n/a";
#endif
        QString errorMsg = recvCommand + "\n"
                           + "ITGRecv finish error.\n"
                           + (recvTimeout? "Timeout reached\n" : "")
                           + "PID: " + pid + "\n"
                           + "Exit code: " + QString::number(recvProc.exitCode()) + "\n"
                           + "Signal: " + QString::number(signal) + "\n"
                           + "Exit status: " + QString::number(status) + "\n"
                           + "Experiment ID: " + downId + "\n" +
                           recvProc.readAll();
        recvTimeout = false;
        qDebug() << errorMsg;
        sendFailure(DITG_RECV_ERROR, errorMsg);
        setRunning(false);
        return;
    }
    qDebug() << "ITG recv OK. Exit code: " << QString::number(recvProc.exitCode()) << recvProc.readAll();

    // Run ITGDec for send
    decSendCommand = "\"" + this->applicationDir + ITG_DEC_COMMAND + "\" " +
                     "\"" + UPSTREAM_BIN_LOG_FILE + "\" " +
                     ITG_DEC_COMBINED_SWITCH + " " +
                     ITG_DEC_COMBINED_MS + " " +
                     "\"" + UPSTREAM_TXT_LOG_FILE + "\"";
    qDebug() << "ITGDec command: " + decSendCommand;
    sendDecProc.start(decSendCommand);
    if (!sendDecProc.waitForStarted()) {
        QString errorMsg = decSendCommand + "\n"
                           + "ITGDec for send start error.\n"
                           + "Exit code: " + QString::number(sendDecProc.exitCode()) + "\n"
                           + "Experiment ID: " + upId + "\n"
                           + sendDecProc.readAll();
        qDebug() << errorMsg;
        sendFailure(DITG_DEC_SEND_ERROR, errorMsg);
        setRunning(false);
        return;
    }

    // Start timeout for D-ITG Dec process for send
    QTimer::singleShot(DEC_SEND_PROC_TIMEOUT, this, SLOT(decSendProcTimeout()));
}


void BSenseProbe::decSendProcTimeout() {
    // Kills the ITG Dec for send process if still running
    if (sendDecProc.state() == QProcess::Running) {
        sendDecProc.kill();
        decSendTimeout = true;
        qDebug() << "D-ITG Send Decoding process killed";
    }
}


void BSenseProbe::finishedItgSendDec(int signal, QProcess::ExitStatus status) {
    if ((signal != 0) || (status != QProcess::NormalExit)) {
        QString errorMsg = decSendCommand + "\n"
                           + "ITGDec for send finish error."
                           + (decSendTimeout? "Timeout reached\n" : "")
                           + "Exit code: " + QString::number(sendDecProc.exitCode()) + "\n"
                           + "Signal: " + QString::number(signal) + "\n"
                           + "Exit status: " + QString::number(status) + "\n"
                           + "Experiment ID: " + upId + "\n"
                           + sendDecProc.readAll();
        decSendTimeout = false;
        qDebug() << errorMsg;
        sendFailure(DITG_DEC_SEND_ERROR, errorMsg);
        setRunning(false);
        return;
    }
    qDebug() << "ITGDec for send OK. Exit code: " << QString::number(signal) << sendDecProc.readAll();

    // Open and read output file
    QFile sendLogFile(UPSTREAM_TXT_LOG_FILE);
    if (!sendLogFile.open(QIODevice::ReadOnly)) {
        setRunning(false);
        return;
    }
    sendResults.clear();
    sendResults.append(sendLogFile.readAll());
    sendLogFile.close();
    qDebug() << "Send results: " << sendResults;

    // Run ITGDec for recv
    QString decRecvCommand = "\"" + this->applicationDir + ITG_DEC_COMMAND + "\" " +
                     "\"" + DOWNSTREAM_BIN_LOG_FILE + "\" " +
                     ITG_DEC_COMBINED_SWITCH + " " +
                     ITG_DEC_COMBINED_MS + " " +
                     "\"" + DOWNSTREAM_TXT_LOG_FILE + "\"";
    qDebug() << "ITGDec command: " + decRecvCommand;
    recvDecProc.start(decRecvCommand);
    if (!recvDecProc.waitForStarted()) {
        QString errorMsg = decRecvCommand + "\n"
                           + "ITGDec for recv start error.\n"
                           + "Exit code: " + QString::number(recvDecProc.exitCode()) + "\n"
                           + "Experiment ID: " + downId + "\n" +
                           recvDecProc.readAll();
        qDebug() << errorMsg;
        sendFailure(DITG_DEC_RECV_ERROR, errorMsg);
        setRunning(false);
        return;
    }

    // Start timeout for D-ITG Dec process for recv
    QTimer::singleShot(DEC_RECV_PROC_TIMEOUT, this, SLOT(decRecvProcTimeout()));
}


void BSenseProbe::decRecvProcTimeout() {
    // Kills the process if still running
    if (recvDecProc.state() == QProcess::Running) {
        recvDecProc.kill();
        decRecvTimeout = true;
        qDebug() << "D-ITG Recv Decoding process killed";
    }
}


void BSenseProbe::finishedItgRecvDec(int signal, QProcess::ExitStatus status) {
    if ((signal != 0) || (status != QProcess::NormalExit)) {
        QString errorMsg = decRecvCommand + "\n"
                           + "ITGDec for recv finish error.\n"
                           + (decRecvTimeout? "Timeout reached\n" : "")
                           + "Exit code: " + QString::number(recvDecProc.exitCode()) + "\n"
                           + "Signal: " + QString::number(signal) + "\n"
                           + "Exit status: " + QString::number(status) + "\n"
                           + "Experiment ID: " + downId + "\n"
                           + recvDecProc.readAll();
        decRecvTimeout = false;
        qDebug() << errorMsg;
        sendFailure(signal, errorMsg);
        setRunning(false);
        return;
    }
    qDebug() << "ITGDec for recv OK. Exit code: " << QString::number(signal) << recvDecProc.readAll();

    // Open and read output file
    QFile recvLogFile(DOWNSTREAM_TXT_LOG_FILE);
    if (!recvLogFile.open(QIODevice::ReadOnly)) {
        setRunning(false);
        return;
    }
    recvResults.clear();
    recvResults.append(recvLogFile.readAll());
    recvLogFile.close();
    qDebug() << "Recv results: " << recvResults;

    // Check Send result back
    QSettings settings;
    QUrl responseUrl, params;
    QByteArray resBA;
    responseUrl.setUrl(URL);
    params.addQueryItem(ACTION_PARAM, RESULT_ACTION);
    params.addQueryItem(UUID_PARAM, settings.value(UUID_SETTING).toString());
    params.addQueryItem(ID_UP_PARAM, upId);
    params.addQueryItem(ID_DOWN_PARAM, downId);
    params.addQueryItem(RAW_RESULTS_UP_PARAM, sendResults);
    params.addQueryItem(RAW_RESULTS_DOWN_PARAM, recvResults);
    params.addQueryItem(VERSION_PARAM, VERSION);
    resBA.append(params.toEncoded());
    resBA.remove(0,1);
    resultReply = qnam2.post(QNetworkRequest(responseUrl), resBA);
    connect(resultReply, SIGNAL(finished()), this, SLOT(httpFinishedResp()));

    qDebug() << "Result sent back. Query: " + responseUrl.toString()
            + "\nParams: " + params.toString();

    // deleting temporary files:
    deleteTemporaryFiles();
}


void BSenseProbe::httpFinishedResp() {
    // TODO show error msg if error?
    qDebug() << "Result sending session finished. Status: " << resultReply->error();
    setRunning(false);
}


/**
Sends failure message back to server
@arg errorCode error code (see bsense.h)
@arg errorMsg the complete error message
*/
void BSenseProbe::sendFailure(int errorCode, QString errorMsg) {
    QSettings settings;
    QUrl responseUrl, params;
    QByteArray resBA;
    responseUrl.setUrl(URL);
    params.addQueryItem(ACTION_PARAM, FAILURE_ACTION);
    params.addQueryItem(UUID_PARAM, settings.value(UUID_SETTING).toString());
    params.addQueryItem(ERROR_CODE_PARAM, QString::number(errorCode));
    params.addQueryItem(VERSION_PARAM, VERSION);
    params.addQueryItem(ERROR_MESSAGE_PARAM, errorMsg);
    resBA.append(params.toEncoded());
    resBA.remove(0,1);
    errorReply = qnam2.post(QNetworkRequest(responseUrl), resBA);
    connect(errorReply, SIGNAL(finished()), this, SLOT(finishedSendingFailure()));

    qDebug() << "Error sent back. Query: " << responseUrl.encodedQuery() << ", Params: " << params.toEncoded();

    // deleting temporary files:
    deleteTemporaryFiles();
}


void BSenseProbe::finishedSendingFailure() {
    qDebug() << "Error sending session finished. Status: " << errorReply->error();
}


void BSenseProbe::setRunning(bool running) {
    this->running = running;
    bSenseGui->setIcon();
}


bool BSenseProbe::isRunning() {
    return running;
}

QString BSenseProbe::osVersion() {

#if defined(Q_OS_LINUX)
        utsname buf;
        QString osVersion;
        if(uname(&buf) != -1) {
                osVersion.append(buf.release).append(QString(' '));
                osVersion.append(buf.sysname).append(QString(' '));
                osVersion.append(buf.machine).append(QString(' '));
                osVersion.append(QString(" (")).append(buf.machine).append(QString(')'));
        } else {
                osVersion = QString("(unknown)");
        }
        return osVersion;
#elif defined(Q_OS_MAC)
        switch(QSysInfo::MacintoshVersion) {
                case QSysInfo::MV_9         : return "OS 9 (unsupported)";
                case QSysInfo::MV_10_0      : return "OS X 10.0 (unsupported)";
                case QSysInfo::MV_10_1      : return "OS X 10.1 (unsupported)";
                case QSysInfo::MV_10_2      : return "OS X 10.2 (unsupported)";
                case QSysInfo::MV_10_3      : return "OS X 10.3";
                case QSysInfo::MV_10_4      : return "OS X 10.4";
                case QSysInfo::MV_10_5      : return "OS X 10.5";
                case QSysInfo::MV_10_6      : return "OS X 10.6";
                case QSysInfo::MV_Unknown   : return "OS X (unknown)";
                default                     : return "OS X (unknown)";
        }
#elif defined(Q_OS_WIN32)
        switch(QSysInfo::WindowsVersion) {
                case QSysInfo::WV_32s       : return "3.1 with Win32s";
                case QSysInfo::WV_95        : return "95";
                case QSysInfo::WV_98        : return "98";
                case QSysInfo::WV_Me        : return "Me";
                case QSysInfo::WV_NT        : return "NT";
                case QSysInfo::WV_2000      : return "2000";
                case QSysInfo::WV_XP        : return "XP";
                case QSysInfo::WV_2003      : return "Server 2003, 2003 R2, Home Server, or XP Pro x64";
                case QSysInfo::WV_VISTA     : return "Vista, or Server 2008";
                case QSysInfo::WV_WINDOWS7  : return "7, or Server 2008 R2";
                case QSysInfo::WV_CE        : return "CE";
                case QSysInfo::WV_CENET     : return "CE .NET";
                default                     : return "(unknown)";
        }
#else
        return "(unknown)";
#endif
}

