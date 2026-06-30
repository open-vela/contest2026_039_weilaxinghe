/****************************************************************************
 * Contest 2026 team 039 - VelaBridge Watch LVGL prototype
 ****************************************************************************/

#include <nuttx/config.h>

#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#if defined(CONFIG_BOARDCTL) && !defined(CONFIG_NSH_ARCHINIT)
#  include <sys/boardctl.h>
#endif

#include "velabridge_watch_ui.h"

#define VB_WATCH_WIDTH   390
#define VB_WATCH_HEIGHT  390

#define VB_COLOR_BG      0x050507
#define VB_COLOR_PANEL   0x16171a
#define VB_COLOR_TEXT    0xf5f5f7
#define VB_COLOR_MUTED   0x8d8f98
#define VB_COLOR_BLUE    0x2a7bff
#define VB_COLOR_GREEN   0x42d77d
#define VB_COLOR_RED     0xff4d4d
#define VB_COLOR_ORANGE  0xffa132
#define VB_COLOR_PURPLE  0x8c6cff

#if defined(CONFIG_GRAPHICS_LVGL) || defined(CONFIG_LVGL)

#include <lvgl.h>

#if defined(CONFIG_BOARDCTL) && !defined(CONFIG_NSH_ARCHINIT)
#  define VB_NEED_BOARDINIT 1
#endif

#if defined(LVGL_VERSION_MAJOR)
#  define VB_LVGL_VERSION_MAJOR LVGL_VERSION_MAJOR
#elif defined(LV_VERSION_MAJOR)
#  define VB_LVGL_VERSION_MAJOR LV_VERSION_MAJOR
#else
#  define VB_LVGL_VERSION_MAJOR 8
#endif

enum vb_watch_screen
{
  VB_SCREEN_BOOT = 0,
  VB_SCREEN_FACE,
  VB_SCREEN_HEART,
  VB_SCREEN_WORKOUT,
  VB_SCREEN_SLEEP,
  VB_SCREEN_SETTINGS,
  VB_SCREEN_COUNT,
};

static lv_obj_t *g_vb_screens[VB_SCREEN_COUNT];
static lv_timer_t *g_vb_demo_timer;
static enum vb_watch_screen g_vb_current_screen = VB_SCREEN_BOOT;

#if LV_USE_NUTTX
static lv_nuttx_result_t g_vb_nuttx_result;
static bool g_vb_nuttx_initialized;
static bool g_vb_lvgl_initialized_by_app;
#endif

static const char *g_vb_screen_names[VB_SCREEN_COUNT] =
{
  "Boot",
  "Watch Face",
  "Heart Rate",
  "Workout",
  "Sleep",
  "Settings",
};

static void *vb_lvgl_default_display(void)
{
#if VB_LVGL_VERSION_MAJOR >= 9
  return lv_display_get_default();
#else
  return lv_disp_get_default();
#endif
}

static void vb_lvgl_set_default_display(void *display)
{
  if (display == NULL)
    {
      return;
    }

#if VB_LVGL_VERSION_MAJOR >= 9
  lv_display_set_default((lv_display_t *)display);
#else
  lv_disp_set_default((lv_disp_t *)display);
#endif
}

static bool vb_device_exists(const char *path)
{
  return path != NULL && access(path, F_OK) == 0;
}

bool velabridge_watch_ui_available(void)
{
  printf("[velabridge][watch_ui] checking default display\n");
  fflush(stdout);

  if (vb_lvgl_default_display() == NULL)
    {
      printf("[velabridge][watch_ui] no default display, fallback\n");
      fflush(stdout);
      return false;
    }

  printf("[velabridge][watch_ui] default display ready\n");
  fflush(stdout);
  return true;
}

static int vb_lvgl_display_init(void)
{
#if LV_USE_NUTTX
  const char *touch_path = "/dev/input0";
  bool touch_available = false;
#endif

  if (vb_lvgl_default_display() != NULL)
    {
      printf("[velabridge][watch_ui] default display already exists\n");
      fflush(stdout);
      return 0;
    }

#if LV_USE_NUTTX
#ifdef VB_NEED_BOARDINIT
  int ret;

  printf("[velabridge][watch_ui] board init for display devices\n");
  ret = boardctl(BOARDIOC_INIT, 0);
  if (ret < 0)
    {
      printf("[velabridge][watch_ui] boardctl init failed ret=%d\n", ret);
    }
#endif

  if (!lv_is_initialized())
    {
      printf("[velabridge][watch_ui] lv_init\n");
      lv_init();
      g_vb_lvgl_initialized_by_app = true;
    }
  else
    {
      printf("[velabridge][watch_ui] lvgl already initialized\n");
    }

  if (vb_lvgl_default_display() != NULL)
    {
      printf("[velabridge][watch_ui] default display created by init\n");
      fflush(stdout);
      return 0;
    }

  memset(&g_vb_nuttx_result, 0, sizeof(g_vb_nuttx_result));
  lv_nuttx_dsc_t info;
  lv_nuttx_dsc_init(&info);

#if defined(CONFIG_LV_USE_NUTTX_LCD) || LV_USE_NUTTX_LCD
  info.fb_path = "/dev/lcd0";
  printf("[velabridge][watch_ui] trying NuttX LCD backend %s\n",
         info.fb_path);
#else
  info.fb_path = "/dev/fb0";
  printf("[velabridge][watch_ui] trying NuttX framebuffer backend %s\n",
         info.fb_path);
#endif

#if defined(CONFIG_LV_USE_NUTTX_TOUCHSCREEN) || defined(CONFIG_INPUT_TOUCHSCREEN)
  if (vb_device_exists(touch_path))
    {
      info.input_path = touch_path;
      touch_available = true;
    }
  else
    {
      info.input_path = NULL;
    }
#else
  info.input_path = NULL;
#endif

  lv_nuttx_init(&info, &g_vb_nuttx_result);
  g_vb_nuttx_initialized = true;

  if (g_vb_nuttx_result.disp == NULL)
    {
      printf("[velabridge][watch_ui] lcd open failed ret=%d path=%s\n",
             -ENODEV, info.fb_path ? info.fb_path : "(null)");
      lv_nuttx_deinit(&g_vb_nuttx_result);
      g_vb_nuttx_initialized = false;

      if (g_vb_lvgl_initialized_by_app)
        {
          lv_deinit();
          g_vb_lvgl_initialized_by_app = false;
        }

      fflush(stdout);
      return -ENODEV;
    }

  printf("[velabridge][watch_ui] lcd display ready\n");

#if defined(CONFIG_LV_USE_NUTTX_TOUCHSCREEN) || defined(CONFIG_INPUT_TOUCHSCREEN)
  if (touch_available)
    {
      printf("[velabridge][watch_ui] touchscreen path %s\n", touch_path);
    }
  else
    {
      printf("[velabridge][watch_ui] touchscreen not available, "
             "continue display-only\n");
    }
#endif

  vb_lvgl_set_default_display(g_vb_nuttx_result.disp);

  if (vb_lvgl_default_display() == NULL)
    {
      printf("[velabridge][watch_ui] display created but no default set\n");
      lv_nuttx_deinit(&g_vb_nuttx_result);
      g_vb_nuttx_initialized = false;

      if (g_vb_lvgl_initialized_by_app)
        {
          lv_deinit();
          g_vb_lvgl_initialized_by_app = false;
        }

      fflush(stdout);
      return -ENODEV;
    }

  printf("[velabridge][watch_ui] display initialized path=%s\n",
         info.fb_path ? info.fb_path : "(null)");
  fflush(stdout);
  return 0;
#else
  printf("[velabridge][watch_ui] LVGL NuttX backend disabled\n");
  fflush(stdout);
  return -ENOSYS;
#endif
}

static lv_color_t vb_color(uint32_t hex)
{
  return lv_color_hex(hex);
}

static const lv_font_t *vb_font_big(void)
{
#  if LV_FONT_MONTSERRAT_48
  return &lv_font_montserrat_48;
#  elif LV_FONT_MONTSERRAT_40
  return &lv_font_montserrat_40;
#  elif LV_FONT_MONTSERRAT_36
  return &lv_font_montserrat_36;
#  elif LV_FONT_MONTSERRAT_32
  return &lv_font_montserrat_32;
#  else
  return LV_FONT_DEFAULT;
#  endif
}

static const lv_font_t *vb_font_title(void)
{
#  if LV_FONT_MONTSERRAT_28
  return &lv_font_montserrat_28;
#  elif LV_FONT_MONTSERRAT_24
  return &lv_font_montserrat_24;
#  elif LV_FONT_MONTSERRAT_22
  return &lv_font_montserrat_22;
#  else
  return LV_FONT_DEFAULT;
#  endif
}

static const lv_font_t *vb_font_small(void)
{
#  if LV_FONT_MONTSERRAT_14
  return &lv_font_montserrat_14;
#  elif LV_FONT_MONTSERRAT_12
  return &lv_font_montserrat_12;
#  else
  return LV_FONT_DEFAULT;
#  endif
}

static lv_obj_t *vb_label(lv_obj_t *parent, const char *text,
                          uint32_t color, const lv_font_t *font)
{
  lv_obj_t *label = lv_label_create(parent);

  lv_label_set_text(label, text);
  lv_obj_set_style_text_color(label, vb_color(color), 0);
  lv_obj_set_style_text_font(label, font, 0);
  lv_obj_set_style_text_letter_space(label, 0, 0);

  return label;
}

static void vb_anim_y_cb(void *obj, int32_t value)
{
  lv_obj_set_y((lv_obj_t *)obj, value);
}

static void vb_card_float(lv_obj_t *card, int32_t delay)
{
  lv_anim_t anim;

  lv_anim_init(&anim);
  lv_anim_set_var(&anim, card);
  lv_anim_set_exec_cb(&anim, vb_anim_y_cb);
  lv_anim_set_values(&anim, lv_obj_get_y(card) + 8, lv_obj_get_y(card));
  lv_anim_set_time(&anim, 220);
  lv_anim_set_delay(&anim, delay);
  lv_anim_start(&anim);
}

lv_obj_t *vb_create_screen_base(void)
{
  lv_obj_t *screen = lv_obj_create(NULL);

  lv_obj_set_size(screen, VB_WATCH_WIDTH, VB_WATCH_HEIGHT);
  lv_obj_set_style_bg_color(screen, vb_color(VB_COLOR_BG), 0);
  lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, 0);
  lv_obj_set_style_border_width(screen, 0, 0);
  lv_obj_set_style_pad_all(screen, 0, 0);
  lv_obj_clear_flag(screen, LV_OBJ_FLAG_SCROLLABLE);

  return screen;
}

lv_obj_t *vb_create_status_bar(lv_obj_t *parent, const char *middle)
{
  lv_obj_t *bar = lv_obj_create(parent);
  lv_obj_t *left;
  lv_obj_t *center;
  lv_obj_t *right;

  lv_obj_set_size(bar, VB_WATCH_WIDTH - 42, 24);
  lv_obj_align(bar, LV_ALIGN_TOP_MID, 0, 18);
  lv_obj_set_style_bg_opa(bar, LV_OPA_TRANSP, 0);
  lv_obj_set_style_border_width(bar, 0, 0);
  lv_obj_set_style_pad_all(bar, 0, 0);
  lv_obj_clear_flag(bar, LV_OBJ_FLAG_SCROLLABLE);

  left = vb_label(bar, "09:39", VB_COLOR_TEXT, vb_font_small());
  lv_obj_align(left, LV_ALIGN_LEFT_MID, 0, 0);

  center = vb_label(bar, middle, VB_COLOR_MUTED, vb_font_small());
  lv_obj_align(center, LV_ALIGN_CENTER, 0, 0);

  right = vb_label(bar, "86%", VB_COLOR_TEXT, vb_font_small());
  lv_obj_align(right, LV_ALIGN_RIGHT_MID, 0, 0);

  return bar;
}

lv_obj_t *vb_create_card(lv_obj_t *parent, int16_t x, int16_t y,
                         int16_t w, int16_t h)
{
  lv_obj_t *card = lv_obj_create(parent);

  lv_obj_set_size(card, w, h);
  lv_obj_align(card, LV_ALIGN_TOP_LEFT, x, y);
  lv_obj_set_style_radius(card, 24, 0);
  lv_obj_set_style_bg_color(card, vb_color(VB_COLOR_PANEL), 0);
  lv_obj_set_style_bg_opa(card, LV_OPA_COVER, 0);
  lv_obj_set_style_border_width(card, 1, 0);
  lv_obj_set_style_border_color(card, vb_color(0x2a2c33), 0);
  lv_obj_set_style_pad_all(card, 18, 0);
  lv_obj_clear_flag(card, LV_OBJ_FLAG_SCROLLABLE);

  return card;
}

lv_obj_t *vb_create_pill(lv_obj_t *parent, const char *text, uint32_t color)
{
  lv_obj_t *pill = lv_obj_create(parent);
  lv_obj_t *label;

  lv_obj_set_size(pill, LV_SIZE_CONTENT, 30);
  lv_obj_set_style_radius(pill, 15, 0);
  lv_obj_set_style_bg_color(pill, vb_color(color), 0);
  lv_obj_set_style_bg_opa(pill, LV_OPA_20, 0);
  lv_obj_set_style_border_width(pill, 1, 0);
  lv_obj_set_style_border_color(pill, vb_color(color), 0);
  lv_obj_set_style_pad_left(pill, 12, 0);
  lv_obj_set_style_pad_right(pill, 12, 0);
  lv_obj_set_style_pad_top(pill, 4, 0);
  lv_obj_set_style_pad_bottom(pill, 4, 0);
  lv_obj_clear_flag(pill, LV_OBJ_FLAG_SCROLLABLE);

  label = vb_label(pill, text, VB_COLOR_TEXT, vb_font_small());
  lv_obj_center(label);

  return pill;
}

lv_obj_t *vb_create_big_number(lv_obj_t *parent, const char *number,
                               const char *unit, uint32_t color)
{
  lv_obj_t *box = lv_obj_create(parent);
  lv_obj_t *num;
  lv_obj_t *unit_label;

  lv_obj_set_size(box, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_obj_set_style_bg_opa(box, LV_OPA_TRANSP, 0);
  lv_obj_set_style_border_width(box, 0, 0);
  lv_obj_set_style_pad_all(box, 0, 0);
  lv_obj_clear_flag(box, LV_OBJ_FLAG_SCROLLABLE);

  num = vb_label(box, number, color, vb_font_big());
  lv_obj_align(num, LV_ALIGN_LEFT_MID, 0, 0);

  unit_label = vb_label(box, unit, VB_COLOR_MUTED, vb_font_small());
  lv_obj_align_to(unit_label, num, LV_ALIGN_OUT_RIGHT_BOTTOM, 8, -5);

  return box;
}

lv_obj_t *vb_create_progress_ring(lv_obj_t *parent, int value,
                                  uint32_t color)
{
  lv_obj_t *arc = lv_arc_create(parent);

  lv_obj_set_size(arc, 104, 104);
  lv_arc_set_range(arc, 0, 100);
  lv_arc_set_value(arc, value);
  lv_obj_remove_style(arc, NULL, LV_PART_KNOB);
  lv_obj_clear_flag(arc, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_set_style_arc_color(arc, vb_color(0x2a2c33), LV_PART_MAIN);
  lv_obj_set_style_arc_width(arc, 9, LV_PART_MAIN);
  lv_obj_set_style_arc_color(arc, vb_color(color), LV_PART_INDICATOR);
  lv_obj_set_style_arc_width(arc, 9, LV_PART_INDICATOR);

  return arc;
}

void vb_switch_screen(enum vb_watch_screen next)
{
  if (next >= VB_SCREEN_COUNT || g_vb_screens[next] == NULL)
    {
      return;
    }

  g_vb_current_screen = next;
  printf("[velabridge][watch_ui] screen=%s\n", g_vb_screen_names[next]);
  fflush(stdout);

  lv_scr_load_anim(g_vb_screens[next], LV_SCR_LOAD_ANIM_FADE_IN,
                   180, 0, false);
}

static void vb_card_clicked(lv_event_t *event)
{
  uintptr_t next = (uintptr_t)lv_event_get_user_data(event);

  vb_switch_screen((enum vb_watch_screen)next);
}

static void vb_demo_timer_cb(lv_timer_t *timer)
{
  enum vb_watch_screen next = (g_vb_current_screen + 1) % VB_SCREEN_COUNT;

  (void)timer;
  vb_switch_screen(next);
}

static void vb_bind_next(lv_obj_t *obj, enum vb_watch_screen next)
{
  lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_add_event_cb(obj, vb_card_clicked, LV_EVENT_CLICKED,
                      (void *)(uintptr_t)next);
}

static void vb_create_metric_card(lv_obj_t *parent, int16_t x, int16_t y,
                                  int16_t w, int16_t h,
                                  const char *label, const char *value,
                                  uint32_t color)
{
  lv_obj_t *card = vb_create_card(parent, x, y, w, h);
  lv_obj_t *title = vb_label(card, label, VB_COLOR_MUTED, vb_font_small());
  lv_obj_t *number = vb_label(card, value, color, vb_font_title());

  lv_obj_align(title, LV_ALIGN_TOP_LEFT, 0, 0);
  lv_obj_align(number, LV_ALIGN_BOTTOM_LEFT, 0, 0);
  vb_card_float(card, x / 2);
}

static void vb_build_boot(void)
{
  lv_obj_t *screen = vb_create_screen_base();
  lv_obj_t *mark = vb_create_card(screen, 132, 72, 126, 126);
  lv_obj_t *x = vb_label(mark, "X", VB_COLOR_TEXT, vb_font_big());
  lv_obj_t *star = vb_label(mark, "+", VB_COLOR_BLUE, vb_font_title());
  lv_obj_t *brand = vb_label(screen, "XINGYU", VB_COLOR_TEXT, vb_font_title());
  lv_obj_t *product = vb_label(screen, "VelaBridge Watch",
                               VB_COLOR_MUTED, vb_font_small());
  lv_obj_t *pill = vb_create_pill(screen, "openvela ready", VB_COLOR_BLUE);

  lv_obj_set_style_bg_color(mark, vb_color(0x0b1425), 0);
  lv_obj_set_style_border_color(mark, vb_color(VB_COLOR_BLUE), 0);
  lv_obj_center(x);
  lv_obj_align(star, LV_ALIGN_TOP_MID, 0, 12);
  lv_obj_align(brand, LV_ALIGN_TOP_MID, 0, 218);
  lv_obj_align(product, LV_ALIGN_TOP_MID, 0, 255);
  lv_obj_align(pill, LV_ALIGN_TOP_MID, 0, 286);
  vb_bind_next(mark, VB_SCREEN_FACE);

  g_vb_screens[VB_SCREEN_BOOT] = screen;
}

static void vb_build_face(void)
{
  lv_obj_t *screen = vb_create_screen_base();
  lv_obj_t *time;
  lv_obj_t *brand;
  lv_obj_t *pill;
  lv_obj_t *battery;

  vb_create_status_bar(screen, "Watch");
  time = vb_label(screen, "09:39", VB_COLOR_TEXT, vb_font_big());
  lv_obj_align(time, LV_ALIGN_TOP_LEFT, 34, 62);

  brand = vb_label(screen, "VelaBridge", VB_COLOR_TEXT, vb_font_title());
  lv_obj_align(brand, LV_ALIGN_TOP_LEFT, 38, 136);

  pill = vb_create_pill(screen, "AI Ready", VB_COLOR_BLUE);
  lv_obj_align(pill, LV_ALIGN_TOP_LEFT, 38, 172);

  vb_create_metric_card(screen, 28, 222, 104, 88, "Heart", "72", VB_COLOR_RED);
  vb_create_metric_card(screen, 143, 222, 104, 88, "Move", "58%", VB_COLOR_GREEN);
  vb_create_metric_card(screen, 258, 222, 104, 88, "Sleep", "7h", VB_COLOR_PURPLE);

  battery = vb_label(screen, "Battery 86%", VB_COLOR_MUTED, vb_font_small());
  lv_obj_align(battery, LV_ALIGN_BOTTOM_MID, 0, -28);

  vb_bind_next(screen, VB_SCREEN_HEART);
  g_vb_screens[VB_SCREEN_FACE] = screen;
}

static void vb_build_heart(void)
{
  lv_obj_t *screen = vb_create_screen_base();
  lv_obj_t *card;
  lv_obj_t *title;
  lv_obj_t *number;
  lv_obj_t *resting;
  lv_obj_t *peak;
  lv_obj_t *ring;

  vb_create_status_bar(screen, "Health");
  card = vb_create_card(screen, 28, 58, 334, 286);
  title = vb_label(card, "Heart Rate", VB_COLOR_TEXT, vb_font_title());
  lv_obj_align(title, LV_ALIGN_TOP_LEFT, 0, 0);

  number = vb_create_big_number(card, "72", "BPM", VB_COLOR_RED);
  lv_obj_align(number, LV_ALIGN_TOP_LEFT, 0, 58);

  ring = vb_create_progress_ring(card, 72, VB_COLOR_RED);
  lv_obj_align(ring, LV_ALIGN_RIGHT_MID, -6, -8);

  resting = vb_create_pill(card, "Resting 61", VB_COLOR_RED);
  lv_obj_align(resting, LV_ALIGN_BOTTOM_LEFT, 0, 0);

  peak = vb_create_pill(card, "Peak 128", VB_COLOR_ORANGE);
  lv_obj_align(peak, LV_ALIGN_BOTTOM_RIGHT, 0, 0);

  vb_card_float(card, 0);
  vb_bind_next(card, VB_SCREEN_WORKOUT);
  g_vb_screens[VB_SCREEN_HEART] = screen;
}

static void vb_build_workout(void)
{
  lv_obj_t *screen = vb_create_screen_base();
  lv_obj_t *card;
  lv_obj_t *title;
  lv_obj_t *distance;
  lv_obj_t *pace;
  lv_obj_t *calories;
  lv_obj_t *ring;

  vb_create_status_bar(screen, "Workout");
  card = vb_create_card(screen, 28, 58, 334, 286);
  title = vb_label(card, "Outdoor Run", VB_COLOR_TEXT, vb_font_title());
  lv_obj_align(title, LV_ALIGN_TOP_LEFT, 0, 0);

  distance = vb_create_big_number(card, "3.42", "km", VB_COLOR_GREEN);
  lv_obj_align(distance, LV_ALIGN_TOP_LEFT, 0, 58);

  ring = vb_create_progress_ring(card, 58, VB_COLOR_GREEN);
  lv_obj_align(ring, LV_ALIGN_RIGHT_MID, -6, -8);

  pace = vb_create_pill(card, "Pace 5'28\"", VB_COLOR_GREEN);
  lv_obj_align(pace, LV_ALIGN_BOTTOM_LEFT, 0, 0);

  calories = vb_create_pill(card, "Calories 284", VB_COLOR_ORANGE);
  lv_obj_align(calories, LV_ALIGN_BOTTOM_RIGHT, 0, 0);

  vb_card_float(card, 0);
  vb_bind_next(card, VB_SCREEN_SLEEP);
  g_vb_screens[VB_SCREEN_WORKOUT] = screen;
}

static void vb_segment(lv_obj_t *parent, int16_t x, int16_t w,
                       uint32_t color, const char *label)
{
  lv_obj_t *seg = lv_obj_create(parent);
  lv_obj_t *text;

  lv_obj_set_size(seg, w, 22);
  lv_obj_align(seg, LV_ALIGN_TOP_LEFT, x, 146);
  lv_obj_set_style_radius(seg, 11, 0);
  lv_obj_set_style_bg_color(seg, vb_color(color), 0);
  lv_obj_set_style_bg_opa(seg, LV_OPA_COVER, 0);
  lv_obj_set_style_border_width(seg, 0, 0);
  lv_obj_clear_flag(seg, LV_OBJ_FLAG_SCROLLABLE);

  text = vb_label(parent, label, VB_COLOR_MUTED, vb_font_small());
  lv_obj_align_to(text, seg, LV_ALIGN_OUT_BOTTOM_MID, 0, 8);
}

static void vb_build_sleep(void)
{
  lv_obj_t *screen = vb_create_screen_base();
  lv_obj_t *card;
  lv_obj_t *title;
  lv_obj_t *time;

  vb_create_status_bar(screen, "Sleep");
  card = vb_create_card(screen, 28, 58, 334, 286);
  title = vb_label(card, "Sleep", VB_COLOR_TEXT, vb_font_title());
  lv_obj_align(title, LV_ALIGN_TOP_LEFT, 0, 0);

  time = vb_create_big_number(card, "7h", "42m", VB_COLOR_PURPLE);
  lv_obj_align(time, LV_ALIGN_TOP_LEFT, 0, 58);

  vb_segment(card, 0, 78, VB_COLOR_PURPLE, "Deep");
  vb_segment(card, 86, 140, VB_COLOR_BLUE, "Core");
  vb_segment(card, 234, 64, VB_COLOR_ORANGE, "REM");

  vb_card_float(card, 0);
  vb_bind_next(card, VB_SCREEN_SETTINGS);
  g_vb_screens[VB_SCREEN_SLEEP] = screen;
}

static void vb_build_settings(void)
{
  static const char *items[] =
  {
    "Display",
    "Health",
    "Haptics",
    "System",
  };

  lv_obj_t *screen = vb_create_screen_base();
  lv_obj_t *title;
  int i;

  vb_create_status_bar(screen, "Settings");
  title = vb_label(screen, "Settings", VB_COLOR_TEXT, vb_font_title());
  lv_obj_align(title, LV_ALIGN_TOP_LEFT, 32, 64);

  for (i = 0; i < 4; i++)
    {
      lv_obj_t *row = vb_create_card(screen, 32, 116 + i * 54, 326, 44);
      lv_obj_t *text = vb_label(row, items[i], VB_COLOR_TEXT, vb_font_small());
      lv_obj_t *arrow = vb_label(row, ">", VB_COLOR_BLUE, vb_font_small());

      lv_obj_set_style_radius(row, 16, 0);
      lv_obj_align(text, LV_ALIGN_LEFT_MID, 0, 0);
      lv_obj_align(arrow, LV_ALIGN_RIGHT_MID, 0, 0);
      vb_card_float(row, i * 40);
      vb_bind_next(row, VB_SCREEN_BOOT);
    }

  g_vb_screens[VB_SCREEN_SETTINGS] = screen;
}

static void vb_build_all_screens(void)
{
  vb_build_boot();
  vb_build_face();
  vb_build_heart();
  vb_build_workout();
  vb_build_sleep();
  vb_build_settings();
}

int velabridge_watch_ui_start(void)
{
  int ret;

  printf("[velabridge][watch_ui] LVGL enabled\n");
  printf("[velabridge][watch_ui] checking default display\n");

  ret = vb_lvgl_display_init();
  if (ret < 0)
    {
      printf("[velabridge][watch_ui] display init failed ret=%d, fallback\n",
             ret);
      fflush(stdout);
      return ret;
    }

  printf("[velabridge][watch_ui] display ready\n");
  printf("[velabridge][watch_ui] design=%dx%d\n",
         VB_WATCH_WIDTH, VB_WATCH_HEIGHT);

  vb_build_all_screens();
  vb_switch_screen(VB_SCREEN_BOOT);

  if (g_vb_demo_timer == NULL)
    {
      g_vb_demo_timer = lv_timer_create(vb_demo_timer_cb, 2000, NULL);
    }

  printf("[velabridge][watch_ui] auto demo timer=2000ms\n");
  fflush(stdout);

  while (1)
    {
      lv_timer_handler();
      usleep(20 * 1000);
    }

#if LV_USE_NUTTX
  if (g_vb_nuttx_initialized)
    {
      lv_nuttx_deinit(&g_vb_nuttx_result);
      g_vb_nuttx_initialized = false;
    }

  if (g_vb_lvgl_initialized_by_app)
    {
      lv_deinit();
      g_vb_lvgl_initialized_by_app = false;
    }
#endif

  return 0;
}

#else

bool velabridge_watch_ui_available(void)
{
  return false;
}

int velabridge_watch_ui_start(void)
{
  printf("[velabridge][watch_ui] LVGL prototype disabled\n");
  printf("[velabridge][watch_ui] enable CONFIG_GRAPHICS_LVGL "
         "to render watch UI\n");
  fflush(stdout);

  return -ENOSYS;
}

#endif
