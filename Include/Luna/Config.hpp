#pragma once

/** Specify the data type to use for colour component, i.e. Red, Green, Blue and
 * Alpha. This can be changed to uint16_t in future. */
#define LUNA_CONF_COLOUR_COMP_TYPE uint8_t

/** Specify the data type to use for the Rect components, i.e. x and y. This
 * can be reduced or increased to suit the display environment. */
#define LUNA_CONF_RECT_COMP_TYPE int64_t

/** Define the path relative to the user's home directory where the socket
 * will be located. */
#define LUNA_CONF_RUN_PATH "/run/luna"

#define LUNA_OS_LINUX

/** The maximum number of log messages that can be queue before the Log will
 * block. This is intentionally limited to throttle rogue / poor / debug enabled
 * applications anbd hopefully will protect the system from being accidentally
 * or intentionally DDoSed.  */
#define LUNA_CONF_MAX_LOG_QUEUE_LEN 10000

/** Specify the maximum length of log messages. This is intentionally limited to
 * throttle rogue / poor / debug enabled applications anbd hopefully will
 * protect the system from being accidentally or intentionally DDoSed. If Log
 * messages appear to be truncated (you will see messages suffixed with "...[MSG
 * TRUNCATED]"), increase the this size. */
#define LUNA_CONF_MAX_LOG_MSG_LEN 1024

#define LUNA_CONF_ENABLE_LOG_EMERGENCY
#define LUNA_CONF_ENABLE_LOG_ALERT
#define LUNA_CONF_ENABLE_LOG_CRITICAL
#define LUNA_CONF_ENABLE_LOG_ERROR
#define LUNA_CONF_ENABLE_LOG_WARNING
#define LUNA_CONF_ENABLE_LOG_NOTICE
#define LUNA_CONF_ENABLE_LOG_INFO
#define LUNA_CONF_ENABLE_LOG_DEBUG

#define LUNA_CONF_DEBUG_BUILD
