/****************************************************************************
 * Contest 2026 team 039 - VelaBridge Watch LVGL animation helpers
 ****************************************************************************/

#include "velabridge_watch_anim.h"

#if defined(CONFIG_GRAPHICS_LVGL) || defined(CONFIG_LVGL)

#define VB_ANIM_PRESS_DOWN_ZOOM  240
#define VB_ANIM_PRESS_UP_ZOOM    256

static void vb_anim_exec_x(void *obj, int32_t value)
{
  lv_obj_set_x((lv_obj_t *)obj, value);
}

static void vb_anim_exec_y(void *obj, int32_t value)
{
  lv_obj_set_y((lv_obj_t *)obj, value);
}

static void vb_anim_exec_w(void *obj, int32_t value)
{
  lv_obj_set_width((lv_obj_t *)obj, value);
}

static void vb_anim_exec_h(void *obj, int32_t value)
{
  lv_obj_set_height((lv_obj_t *)obj, value);
}

static void vb_anim_exec_opa(void *obj, int32_t value)
{
  lv_obj_set_style_opa((lv_obj_t *)obj, (lv_opa_t)value, 0);
}

static void vb_anim_exec_zoom(void *obj, int32_t value)
{
  lv_obj_set_style_transform_zoom((lv_obj_t *)obj, value, 0);
}

static void vb_anim_start(lv_obj_t *obj, lv_anim_exec_xcb_t exec,
                          int32_t from, int32_t to, uint32_t duration_ms)
{
  lv_anim_t anim;

  if (obj == NULL)
    {
      return;
    }

  lv_anim_del(obj, exec);
  lv_anim_init(&anim);
  lv_anim_set_var(&anim, obj);
  lv_anim_set_exec_cb(&anim, exec);
  lv_anim_set_values(&anim, from, to);
  lv_anim_set_time(&anim, duration_ms);
  lv_anim_set_path_cb(&anim, lv_anim_path_ease_out);
  lv_anim_start(&anim);
}

void vb_anim_set_x(lv_obj_t *obj, int32_t x, uint32_t duration_ms)
{
  vb_anim_start(obj, vb_anim_exec_x, lv_obj_get_x(obj), x, duration_ms);
}

void vb_anim_set_y(lv_obj_t *obj, int32_t y, uint32_t duration_ms)
{
  vb_anim_start(obj, vb_anim_exec_y, lv_obj_get_y(obj), y, duration_ms);
}

void vb_anim_set_opa(lv_obj_t *obj, lv_opa_t opa, uint32_t duration_ms)
{
  vb_anim_start(obj, vb_anim_exec_opa, lv_obj_get_style_opa(obj, 0), opa,
                duration_ms);
}

void vb_anim_set_size(lv_obj_t *obj, int32_t w, int32_t h,
                      uint32_t duration_ms)
{
  if (obj == NULL)
    {
      return;
    }

  vb_anim_start(obj, vb_anim_exec_w, lv_obj_get_width(obj), w, duration_ms);
  vb_anim_start(obj, vb_anim_exec_h, lv_obj_get_height(obj), h,
                duration_ms);
}

void vb_anim_focus_icon(lv_obj_t *obj, int32_t x, int32_t y,
                        int32_t size, lv_opa_t opa,
                        uint32_t duration_ms)
{
  vb_anim_set_x(obj, x, duration_ms);
  vb_anim_set_y(obj, y, duration_ms);
  vb_anim_set_size(obj, size, size, duration_ms);
  vb_anim_set_opa(obj, opa, duration_ms);
}

void vb_anim_press_feedback(lv_obj_t *obj)
{
  if (obj == NULL)
    {
      return;
    }

  vb_anim_start(obj, vb_anim_exec_zoom, VB_ANIM_PRESS_DOWN_ZOOM,
                VB_ANIM_PRESS_UP_ZOOM, 100);
}

void vb_anim_page_fade_in(lv_obj_t *screen, uint32_t duration_ms)
{
  if (screen == NULL)
    {
      return;
    }

  lv_obj_set_style_opa(screen, LV_OPA_TRANSP, 0);
  vb_anim_set_opa(screen, LV_OPA_COVER, duration_ms);
}

#endif
