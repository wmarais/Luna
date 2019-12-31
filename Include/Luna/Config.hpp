#pragma once

/** Specify the data type to use for colour component, i.e. Red, Green, Blue and
 * Alpha. This can be changed to uint16_t in future. */
#define LUNA_CONF_COLOUR_COMP_TYPE    uint8_t

/** Specify the data type to use for the Rect components, i.e. x and y. This
 * can be reduced or increased to suit the display environment. */
#define LUNA_CONF_RECT_COMP_TYPE      int64_t

/** Define the path relative to the user's home directory where the socket
 * will be located. */
#define LUNA_CONF_RUN_PATH            "/run/luna"
