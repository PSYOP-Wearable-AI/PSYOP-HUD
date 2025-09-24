#ifndef LV_CONF_H
#define LV_CONF_H

/* Minimal LVGL configuration */

/* Color depth */
#define LV_COLOR_DEPTH 32

/* Enable/Disable features to keep it light */
#define LV_USE_GPU 0
#define LV_USE_LOG 1

/* Fonts */
#define LV_FONT_MONTSERRAT_14 1

/* Default display resolution fallback (used by some demos) */
#define LV_HOR_RES_MAX 1280
#define LV_VER_RES_MAX 800

/* Tick period (ms) – not used directly here as we drive ticks via SDL timer */
#define LV_TICK_CUSTOM 0

#endif /* LV_CONF_H */
