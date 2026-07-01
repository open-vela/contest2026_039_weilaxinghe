/****************************************************************************
 * Contest 2026 team 039 - VelaBridge Watch LVGL animation helpers
 ****************************************************************************/

#ifndef VELABRIDGE_WATCH_ANIM_H
#define VELABRIDGE_WATCH_ANIM_H

#include <nuttx/config.h>

#if defined(CONFIG_GRAPHICS_LVGL) || defined(CONFIG_LVGL)

#include <lvgl.h>
#include <stdint.h>

void vb_anim_set_x(lv_obj_t *obj, int32_t x, uint32_t duration_ms);
void vb_anim_set_y(lv_obj_t *obj, int32_t y, uint32_t duration_ms);
void vb_anim_set_opa(lv_obj_t *obj, lv_opa_t opa, uint32_t duration_ms);
void vb_anim_set_size(lv_obj_t *obj, int32_t w, int32_t h,
                      uint32_t duration_ms);
void vb_anim_focus_icon(lv_obj_t *obj, int32_t x, int32_t y,
                        int32_t size, lv_opa_t opa,
                        uint32_t duration_ms);
void vb_anim_press_feedback(lv_obj_t *obj);
void vb_anim_page_fade_in(lv_obj_t *screen, uint32_t duration_ms);

#endif

#endif /* VELABRIDGE_WATCH_ANIM_H */
