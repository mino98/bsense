#ifndef BSENSE_H
#define BSENSE_H

// Version
#define VERSION                 "0.5.5"

// Setting parameters
#define UUID_SETTING            "UUID"
#define ORG_NAME                "UoE"
#define APP_NAME                "BSense"
#define DOMAIN_NAME             "broadbandforall.net"
#define UPDATE_TIMEOUT_SETTING  "UpdateTimeout"
#define EMAIL_SETTING           "emailAddress"

// Connection parameters
#define URL                     "http://www.broadbandforall.net/ws/"
#define ACTION_PARAM            "a"
#define UUID_PARAM              "UUID"
#define REGISTER_ACTION         "registerUser"
#define UPDATE_EMAIL_ACTION     "updateEmail"
#define UPSTREAM_PARAMS_ACTION  "upstreamParameters"
#define DOWNSTREAM_PARAMS_ACTION "downstreamParameters"
#define RESULT_ACTION           "saveResults"
#define ID_UP_PARAM             "UpstreamExperimentID"
#define ID_DOWN_PARAM           "DownstreamExperimentID"
#define RAW_RESULTS_UP_PARAM    "RawResultsUp"
#define RAW_RESULTS_DOWN_PARAM  "RawResultsDown"
#define ERROR_REPLY             "error"
#define FAILURE_ACTION          "saveFailures"
#define ERROR_CODE_PARAM        "ErrorCode"
#define ERROR_MESSAGE_PARAM     "ErrorMessage"
#define VERSION_PARAM           "BSenseVersion"

// Temporary files for scripts and results
#if defined(Q_OS_LINUX)
#define ARGUMENT_FILE           "/tmp/bsense_tmpUpstreamScript"
#define UPSTREAM_BIN_LOG_FILE   "/tmp/bsense_tmpBinUpstreamLog"
#define DOWNSTREAM_BIN_LOG_FILE "/tmp/bsense_tmpBinDownstreamLog"
#define UPSTREAM_TXT_LOG_FILE   "/tmp/bsense_tmpTxtUpstreamLog"
#define DOWNSTREAM_TXT_LOG_FILE "/tmp/bsense_tmpTxtDownstreamLog"
#elif defined(Q_OS_WIN32)
#define ARGUMENT_FILE           QString(getenv("TEMP")) + "\\tmpUpstreamScript"
#define UPSTREAM_BIN_LOG_FILE   QString(getenv("TEMP")) + "\\tmpBinUpstreamLog"
#define DOWNSTREAM_BIN_LOG_FILE QString(getenv("TEMP")) + "\\tmpBinDownstreamLog"
#define UPSTREAM_TXT_LOG_FILE   QString(getenv("TEMP")) + "\\tmpTxtUpstreamLog"
#define DOWNSTREAM_TXT_LOG_FILE QString(getenv("TEMP")) + "\\tmpTxtDownstreamLog"
#elif defined(Q_OS_MAC)
#define ARGUMENT_FILE           "/tmp/bsense_tmpUpstreamScript"
#define UPSTREAM_BIN_LOG_FILE   "/tmp/bsense_tmpBinUpstreamLog"
#define DOWNSTREAM_BIN_LOG_FILE "/tmp/bsense_tmpBinDownstreamLog"
#define UPSTREAM_TXT_LOG_FILE   "/tmp/bsense_tmpTxtUpstreamLog"
#define DOWNSTREAM_TXT_LOG_FILE "/tmp/bsense_tmpTxtDownstreamLog"
#endif

// ITGDec parameters
#define LOG_SWITCH              "-l"
#define ITG_DEC_COMBINED_SWITCH "-c"
#define ITG_DEC_COMBINED_MS     "100"

// D-ITG command paths
#if defined(Q_OS_LINUX)
#define ITG_SEND_COMMAND        "/bin/ITGSend"
#define ITG_RECV_COMMAND        "/bin/ITGRecv"
#define ITG_DEC_COMMAND         "/bin/ITGDec"
#elif defined(Q_OS_WIN32)
#define ITG_SEND_COMMAND        "\\ITGSend"
#define ITG_RECV_COMMAND        "\\ITGRecv"
#define ITG_DEC_COMMAND         "\\ITGDec"
#elif defined(Q_OS_MAC)
#define ITG_SEND_COMMAND        "/Contents/Resources/bin/ITGSend"
#define ITG_RECV_COMMAND        "/Contents/Resources/bin/ITGRecv"
#define ITG_DEC_COMMAND         "/Contents/Resources/bin/ITGDec"
#endif

// Timeouts (1h)
#define MEASUREMENTS_INTERVAL     15 * 60 * 1000
#define WAIT_BEFORE_RECV          10 * 1000
#define SEND_PROC_TIMEOUT         10 * 60 * 1000
#define RECV_PROC_TIMEOUT         10 * 60 * 1000
#define DEC_SEND_PROC_TIMEOUT     10 * 60 * 1000
#define DEC_RECV_PROC_TIMEOUT     10 * 60 * 1000

// OS identifier strings
#if defined(Q_OS_LINUX)
#define OS_IDENT "Linux"
#elif defined(Q_OS_WIN32)
#define OS_IDENT "Win"
#elif defined(Q_OS_MAC)
#define OS_IDENT "Mac"
#else
#define OS_IDENT "unknown"
#endif

// Update parameters
#define UPDATE_URL           "http://www.broadbandforall.net/dl/"
#define UPDATE_XML           "latest_" + OS_IDENT + ".xml"
#define UPDATE_TIMEOUT_DAYS  1  // Days
#define UPDATE_TIMER_TIMEOUT 4  // Hours
#if defined(Q_OS_LINUX)
#define TMP_DIR              "/tmp"
#elif defined(Q_OS_WIN32)
#define TMP_DIR              "\\tmp"
#elif defined(Q_OS_MAC)
#define TMP_DIR              "/tmp"
#endif

// Error codes for failure report message
#define DITG_SEND_ERROR     1
#define DITG_RECV_ERROR     2
#define DITG_DEC_SEND_ERROR 3
#define DITG_DEC_RECV_ERROR 4
#define SEND_ARGUMENTS_RETRIEVING_ERROR 5
#define SEND_ARGUMENTS_FILE_ERROR       6
#define RECV_ARGUMENTS_RETRIEVING_ERROR 7

#endif // BSENSE_H
