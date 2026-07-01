/****************************************************************************
 * Contest 2026 team 039 - VelaBridge Watch LVGL prototype
 ****************************************************************************/

#ifndef VELABRIDGE_WATCH_UI_H
#define VELABRIDGE_WATCH_UI_H

#include <stdbool.h>

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

bool velabridge_watch_ui_available(void);
int velabridge_watch_ui_open(const char *target);
int velabridge_watch_ui_start(void);

#endif /* VELABRIDGE_WATCH_UI_H */
