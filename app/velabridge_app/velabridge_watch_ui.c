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

#ifndef VB_WATCH_UI_DEBUG
#  define VB_WATCH_UI_DEBUG 0
#endif

#ifndef VB_WATCH_UI_PERF
#  define VB_WATCH_UI_PERF 0
#endif

#define VB_LOG(fmt, ...) \
  do \
    { \
      printf(fmt, ##__VA_ARGS__); \
    } \
  while (0)

#if VB_WATCH_UI_DEBUG
#  define VB_DEBUG_LOG(fmt, ...) VB_LOG(fmt, ##__VA_ARGS__)
#else
#  define VB_DEBUG_LOG(fmt, ...) \
    do \
      { \
      } \
    while (0)
#endif

#if VB_WATCH_UI_PERF
#  define VB_PERF_LOG(fmt, ...) VB_LOG(fmt, ##__VA_ARGS__)
#else
#  define VB_PERF_LOG(fmt, ...) \
    do \
      { \
      } \
    while (0)
#endif

#if defined(CONFIG_GRAPHICS_LVGL) || defined(CONFIG_LVGL)

#include <lvgl.h>

extern const lv_font_t lv_font_simsun_16_cjk;
extern const lv_font_t lv_font_velabridge_cn_18;
extern const lv_font_t lv_font_velabridge_cn_28;

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
  VB_SCREEN_HOME,
  VB_SCREEN_FACE,
  VB_SCREEN_APP_WHEEL,
  VB_SCREEN_HEART,
  VB_SCREEN_WORKOUT,
  VB_SCREEN_SLEEP,
  VB_SCREEN_SETTINGS,
  VB_SCREEN_COUNT,
};

enum vb_home_page
{
  VB_HOME_GRID = 0,
  VB_HOME_CAPTION,
  VB_HOME_OCR,
  VB_HOME_DANGER,
  VB_HOME_REPLY,
  VB_HOME_BLIND,
  VB_HOME_BRIDGE,
  VB_HOME_COUNT,
};

enum vb_app_icon_type
{
  VB_ICON_HEART = 0,
  VB_ICON_SLEEP,
  VB_ICON_WORKOUT,
  VB_ICON_WEATHER,
  VB_ICON_NOTIFICATION,
  VB_ICON_MUSIC,
  VB_ICON_ALARM,
  VB_ICON_SETTINGS,
  VB_ICON_PAY,
  VB_ICON_MAP,
  VB_ICON_BREATH,
  VB_ICON_PHONE,
};

#define VB_APP_COUNT 12
#define VB_WHEEL_VISIBLE_COUNT 7
#define VB_TOUCH_DEBOUNCE_MS 180
#define VB_APP_ART_SIZE 42

struct vb_app_item
{
  const char *name_cn;
  const char *id;
  const char *screen_name;
  enum vb_app_icon_type icon;
  enum vb_watch_screen target;
  uint32_t color;
};

struct vb_wheel_slot
{
  int16_t x;
  int16_t y;
  int16_t size;
  bool show_label;
  lv_opa_t opa;
};

struct vb_home_item
{
  const char *title;
  const char *status;
  const char *id;
  enum vb_home_page page;
  uint32_t color;
};

static lv_obj_t *g_vb_screens[VB_SCREEN_COUNT];
static enum vb_watch_screen g_vb_current_screen = VB_SCREEN_HOME;
static lv_obj_t *g_vb_home_pages[VB_HOME_COUNT];
static enum vb_home_page g_vb_home_page = VB_HOME_GRID;
static uint8_t g_vb_wheel_focus;
static uint32_t g_vb_last_touch_tick;
static lv_obj_t *g_vb_wheel_icons[VB_APP_COUNT];
static lv_obj_t *g_vb_wheel_arts[VB_APP_COUNT];
static lv_obj_t *g_vb_wheel_labels[VB_APP_COUNT];
static lv_obj_t *g_vb_wheel_focus_label;
static bool g_vb_wheel_ready;
#if VB_WATCH_UI_PERF
static uint32_t g_vb_perf_touch_tick;
static uint32_t g_vb_last_loop_tick;
static uint32_t g_vb_loop_interval_ms;
static bool g_vb_perf_touch_active;
#endif

static void vb_set_wheel_focus(uint8_t focus);
static void vb_home_show_page(enum vb_home_page page);
static void vb_open_focused_app(void);
static void vb_wheel_icon_event(lv_event_t *event);
void vb_switch_screen(enum vb_watch_screen next);

#if LV_USE_NUTTX
static lv_nuttx_result_t g_vb_nuttx_result;
static bool g_vb_nuttx_initialized;
static bool g_vb_lvgl_initialized_by_app;
#endif

static const char *g_vb_screen_names[VB_SCREEN_COUNT] =
{
  "Boot",
  "VelaBridge Home",
  "Watch Face",
  "App Wheel",
  "Heart Rate",
  "Workout",
  "Sleep",
  "Settings",
};

static const char *g_vb_home_page_names[VB_HOME_COUNT] =
{
  "VelaBridge Home",
  "Live Caption",
  "OCR Vision",
  "Danger Alert",
  "Quick Reply",
  "Blind Mode",
  "AI Bridge",
};

static const struct vb_home_item g_vb_home_items[] =
{
  { "实时字幕", "正在聆听...", "caption", VB_HOME_CAPTION,
    VB_COLOR_BLUE },
  { "OCR识别", "等待识别结果", "ocr", VB_HOME_OCR,
    VB_COLOR_GREEN },
  { "危险提醒", "前方台阶，请小心", "danger", VB_HOME_DANGER,
    VB_COLOR_RED },
  { "快捷回复", "请说慢一点", "reply", VB_HOME_REPLY,
    VB_COLOR_ORANGE },
  { "盲人模式", "震动/语音提示已开启", "blind", VB_HOME_BLIND,
    VB_COLOR_PURPLE },
  { "AI Bridge", "串口智能体已连接", "bridge", VB_HOME_BRIDGE,
    VB_COLOR_BLUE },
};

#define VB_HOME_ITEM_COUNT \
  (sizeof(g_vb_home_items) / sizeof(g_vb_home_items[0]))

static const struct vb_app_item g_vb_apps[VB_APP_COUNT] =
{
  { "心率", "heart", "Heart Rate", VB_ICON_HEART,
    VB_SCREEN_HEART, VB_COLOR_RED },
  { "睡眠", "sleep", "Sleep", VB_ICON_SLEEP,
    VB_SCREEN_SLEEP, VB_COLOR_PURPLE },
  { "运动", "workout", "Workout", VB_ICON_WORKOUT,
    VB_SCREEN_WORKOUT, VB_COLOR_GREEN },
  { "天气", "weather", "Weather", VB_ICON_WEATHER,
    VB_SCREEN_APP_WHEEL, VB_COLOR_BLUE },
  { "通知", "notification", "Notification", VB_ICON_NOTIFICATION,
    VB_SCREEN_APP_WHEEL, VB_COLOR_BLUE },
  { "音乐", "music", "Music", VB_ICON_MUSIC,
    VB_SCREEN_APP_WHEEL, VB_COLOR_PURPLE },
  { "闹钟", "alarm", "Alarm", VB_ICON_ALARM,
    VB_SCREEN_APP_WHEEL, VB_COLOR_ORANGE },
  { "设置", "settings", "Settings", VB_ICON_SETTINGS,
    VB_SCREEN_SETTINGS, VB_COLOR_BLUE },
  { "支付", "pay", "Pay", VB_ICON_PAY,
    VB_SCREEN_APP_WHEEL, VB_COLOR_BLUE },
  { "地图", "map", "Map", VB_ICON_MAP,
    VB_SCREEN_APP_WHEEL, VB_COLOR_GREEN },
  { "呼吸", "breath", "Breath", VB_ICON_BREATH,
    VB_SCREEN_APP_WHEEL, VB_COLOR_BLUE },
  { "电话", "phone", "Phone", VB_ICON_PHONE,
    VB_SCREEN_APP_WHEEL, VB_COLOR_GREEN },
};

static const struct vb_wheel_slot g_vb_wheel_slots[VB_WHEEL_VISIBLE_COUNT] =
{
  { 154, 118, 82, true, LV_OPA_COVER },
  { 250, 110, 48, false, LV_OPA_COVER },
  { 255, 190, 46, false, LV_OPA_COVER },
  { 188, 250, 44, false, LV_OPA_COVER },
  { 88, 228, 44, false, LV_OPA_COVER },
  { 64, 146, 46, false, LV_OPA_COVER },
  { 142, 78, 44, false, LV_OPA_COVER },
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

#if VB_WATCH_UI_PERF
static uint32_t vb_elapsed_ms(uint32_t start, uint32_t end)
{
  return (uint32_t)(end - start);
}

static void vb_perf_touch_begin(void)
{
  uint32_t now = lv_tick_get();

  g_vb_perf_touch_tick = now == 0 ? 1 : now;
  g_vb_perf_touch_active = true;
  VB_PERF_LOG("[perf] touch received t=%lums\n",
              (unsigned long)g_vb_perf_touch_tick);
}

static void vb_perf_focus_done(uint32_t start)
{
  uint32_t now = lv_tick_get();

  VB_PERF_LOG("[perf] focus update cost=%lums\n",
              (unsigned long)vb_elapsed_ms(start, now));
}

static void vb_perf_ui_done(void)
{
  uint32_t now;

  if (!g_vb_perf_touch_active)
    {
      return;
    }

  now = lv_tick_get();
  VB_PERF_LOG("[perf] ui update cost=%lums\n",
              (unsigned long)vb_elapsed_ms(g_vb_perf_touch_tick, now));
  VB_PERF_LOG("[perf] loop interval=%lums\n",
              (unsigned long)g_vb_loop_interval_ms);
  g_vb_perf_touch_active = false;
}
#else
#  define vb_perf_touch_begin() \
    do \
      { \
      } \
    while (0)
#  define vb_perf_focus_done(start) \
    do \
      { \
        (void)(start); \
      } \
    while (0)
#  define vb_perf_ui_done() \
    do \
      { \
      } \
    while (0)
#endif

bool velabridge_watch_ui_available(void)
{
  VB_DEBUG_LOG("[velabridge][watch_ui] checking default display\n");

  if (vb_lvgl_default_display() == NULL)
    {
      VB_DEBUG_LOG("[velabridge][watch_ui] no default display, fallback\n");
      return false;
    }

  VB_DEBUG_LOG("[velabridge][watch_ui] default display ready\n");
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
      VB_DEBUG_LOG("[velabridge][watch_ui] default display already exists\n");
      return 0;
    }

#if LV_USE_NUTTX
#ifdef VB_NEED_BOARDINIT
  int ret;

  VB_DEBUG_LOG("[velabridge][watch_ui] board init for display devices\n");
  ret = boardctl(BOARDIOC_INIT, 0);
  if (ret < 0)
    {
      VB_LOG("[velabridge][watch_ui] boardctl init failed ret=%d\n", ret);
    }
#endif

  if (!lv_is_initialized())
    {
      VB_DEBUG_LOG("[velabridge][watch_ui] lv_init\n");
      lv_init();
      g_vb_lvgl_initialized_by_app = true;
    }
  else
    {
      VB_DEBUG_LOG("[velabridge][watch_ui] lvgl already initialized\n");
    }

  if (vb_lvgl_default_display() != NULL)
    {
      VB_DEBUG_LOG("[velabridge][watch_ui] default display created by init\n");
      return 0;
    }

  memset(&g_vb_nuttx_result, 0, sizeof(g_vb_nuttx_result));
  lv_nuttx_dsc_t info;
  lv_nuttx_dsc_init(&info);

#if defined(CONFIG_LV_USE_NUTTX_LCD) || LV_USE_NUTTX_LCD
  info.fb_path = "/dev/lcd0";
  VB_DEBUG_LOG("[velabridge][watch_ui] trying NuttX LCD backend %s\n",
               info.fb_path);
#else
  info.fb_path = "/dev/fb0";
  VB_DEBUG_LOG("[velabridge][watch_ui] trying NuttX framebuffer backend %s\n",
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
      VB_LOG("[velabridge][watch_ui] lcd open failed ret=%d path=%s\n",
             -ENODEV, info.fb_path ? info.fb_path : "(null)");
      lv_nuttx_deinit(&g_vb_nuttx_result);
      g_vb_nuttx_initialized = false;

      if (g_vb_lvgl_initialized_by_app)
        {
          lv_deinit();
          g_vb_lvgl_initialized_by_app = false;
        }

      return -ENODEV;
    }

  VB_DEBUG_LOG("[velabridge][watch_ui] lcd display ready\n");

#if defined(CONFIG_LV_USE_NUTTX_TOUCHSCREEN) || defined(CONFIG_INPUT_TOUCHSCREEN)
  if (touch_available)
    {
      VB_DEBUG_LOG("[velabridge][watch_ui] touchscreen path %s\n", touch_path);
      if (g_vb_nuttx_result.indev != NULL)
        {
          VB_DEBUG_LOG("[velabridge][watch_ui] touch initialized path=%s\n",
                       touch_path);
        }
      else
        {
          VB_LOG("[velabridge][watch_ui] touchscreen open failed, "
                 "continue display-only\n");
        }
    }
  else
    {
      VB_LOG("[velabridge][watch_ui] touchscreen not available, "
             "continue display-only\n");
    }
#endif

  vb_lvgl_set_default_display(g_vb_nuttx_result.disp);

  if (vb_lvgl_default_display() == NULL)
    {
      VB_LOG("[velabridge][watch_ui] display created but no default set\n");
      lv_nuttx_deinit(&g_vb_nuttx_result);
      g_vb_nuttx_initialized = false;

      if (g_vb_lvgl_initialized_by_app)
        {
          lv_deinit();
          g_vb_lvgl_initialized_by_app = false;
        }

      return -ENODEV;
    }

  VB_DEBUG_LOG("[velabridge][watch_ui] display initialized path=%s\n",
               info.fb_path ? info.fb_path : "(null)");
  return 0;
#else
  VB_LOG("[velabridge][watch_ui] LVGL NuttX backend disabled\n");
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

static const lv_font_t *vb_font_cn(void)
{
  return &lv_font_velabridge_cn_18;
}

static const lv_font_t *vb_font_cn_title(void)
{
  return &lv_font_velabridge_cn_28;
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

static bool vb_text_has_non_ascii(const char *text)
{
  const unsigned char *cursor = (const unsigned char *)text;

  while (cursor != NULL && *cursor != '\0')
    {
      if (*cursor >= 0x80)
        {
          return true;
        }

      cursor++;
    }

  return false;
}

static lv_obj_t *vb_label_cn(lv_obj_t *parent, const char *text,
                             uint32_t color)
{
  return vb_label(parent, text, color, vb_font_cn());
}

static lv_obj_t *vb_label_cn_title(lv_obj_t *parent, const char *text,
                                   uint32_t color)
{
  return vb_label(parent, text, color, vb_font_cn_title());
}

static lv_obj_t *vb_label_auto(lv_obj_t *parent, const char *text,
                               uint32_t color, const lv_font_t *ascii_font)
{
  if (vb_text_has_non_ascii(text))
    {
      return vb_label_cn(parent, text, color);
    }

  return vb_label(parent, text, color, ascii_font);
}

static void vb_card_float(lv_obj_t *card, int32_t delay)
{
  (void)card;
  (void)delay;
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

  center = vb_label_auto(bar, middle, VB_COLOR_MUTED, vb_font_small());
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

  label = vb_label_auto(pill, text, VB_COLOR_TEXT, vb_font_small());
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

  num = vb_label(box, number, color,
                 vb_text_has_non_ascii(number) ? vb_font_cn_title() :
                 vb_font_big());
  lv_obj_align(num, LV_ALIGN_LEFT_MID, 0, 0);

  unit_label = vb_label_auto(box, unit, VB_COLOR_MUTED, vb_font_small());
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

static int16_t vb_pct(int16_t value, int16_t percent)
{
  return (int16_t)((value * percent) / 100);
}

static int16_t vb_at_least(int16_t value, int16_t minimum)
{
  return value < minimum ? minimum : value;
}

static lv_obj_t *vb_shape(lv_obj_t *parent, int16_t x, int16_t y,
                          int16_t w, int16_t h, uint32_t color,
                          lv_opa_t opa, int16_t radius)
{
  lv_obj_t *obj = lv_obj_create(parent);

  lv_obj_set_pos(obj, x, y);
  lv_obj_set_size(obj, w, h);
  lv_obj_set_style_radius(obj, radius, 0);
  lv_obj_set_style_bg_color(obj, vb_color(color), 0);
  lv_obj_set_style_bg_opa(obj, opa, 0);
  lv_obj_set_style_border_width(obj, 0, 0);
  lv_obj_set_style_pad_all(obj, 0, 0);
  lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE);

  return obj;
}

static lv_obj_t *vb_shape_outline(lv_obj_t *parent, int16_t x, int16_t y,
                                  int16_t w, int16_t h, uint32_t color,
                                  lv_opa_t opa, int16_t radius,
                                  int16_t width)
{
  lv_obj_t *obj = vb_shape(parent, x, y, w, h, color, LV_OPA_TRANSP,
                           radius);

  lv_obj_set_style_border_width(obj, width, 0);
  lv_obj_set_style_border_color(obj, vb_color(color), 0);
  lv_obj_set_style_border_opa(obj, opa, 0);

  return obj;
}

static void vb_draw_heart_icon(lv_obj_t *art, int16_t s, uint32_t color)
{
  int16_t thick = vb_at_least(vb_pct(s, 10), 2);

  vb_shape(art, vb_pct(s, 16), vb_pct(s, 50), vb_pct(s, 68), thick,
           color, LV_OPA_COVER, thick / 2);
  vb_shape(art, vb_pct(s, 42), vb_pct(s, 34), thick, vb_pct(s, 32),
           color, LV_OPA_COVER, thick / 2);
  vb_shape(art, vb_pct(s, 56), vb_pct(s, 42), vb_pct(s, 16), thick,
           color, LV_OPA_COVER, thick / 2);
}

static void vb_draw_sleep_icon(lv_obj_t *art, int16_t s, uint32_t color)
{
  vb_shape(art, vb_pct(s, 22), vb_pct(s, 20), vb_pct(s, 52),
           vb_pct(s, 52), color, LV_OPA_COVER, LV_RADIUS_CIRCLE);
  vb_shape(art, vb_pct(s, 40), vb_pct(s, 14), vb_pct(s, 48),
           vb_pct(s, 48), VB_COLOR_PANEL, LV_OPA_COVER, LV_RADIUS_CIRCLE);
}

static void vb_draw_workout_icon(lv_obj_t *art, int16_t s, uint32_t color)
{
  int16_t thick = vb_at_least(vb_pct(s, 12), 3);

  vb_shape(art, vb_pct(s, 44), vb_pct(s, 16), vb_pct(s, 16),
           vb_pct(s, 16), color, LV_OPA_COVER, LV_RADIUS_CIRCLE);
  vb_shape(art, vb_pct(s, 22), vb_pct(s, 46), vb_pct(s, 56), thick,
           color, LV_OPA_COVER, thick / 2);
  vb_shape(art, vb_pct(s, 42), vb_pct(s, 32), thick, vb_pct(s, 38),
           color, LV_OPA_COVER, thick / 2);
}

static void vb_draw_weather_icon(lv_obj_t *art, int16_t s, uint32_t color)
{
  vb_shape(art, vb_pct(s, 56), vb_pct(s, 16), vb_pct(s, 20),
           vb_pct(s, 20), VB_COLOR_ORANGE, LV_OPA_COVER,
           LV_RADIUS_CIRCLE);
  vb_shape(art, vb_pct(s, 18), vb_pct(s, 48), vb_pct(s, 60),
           vb_pct(s, 18), color, LV_OPA_COVER, vb_pct(s, 9));
  vb_shape(art, vb_pct(s, 34), vb_pct(s, 34), vb_pct(s, 34),
           vb_pct(s, 34), color, LV_OPA_COVER, LV_RADIUS_CIRCLE);
}

static void vb_draw_notification_icon(lv_obj_t *art, int16_t s,
                                      uint32_t color)
{
  vb_shape_outline(art, vb_pct(s, 16), vb_pct(s, 22), vb_pct(s, 66),
                   vb_pct(s, 46), color, LV_OPA_COVER, vb_pct(s, 12),
                   vb_at_least(vb_pct(s, 8), 2));
  vb_shape(art, vb_pct(s, 66), vb_pct(s, 18), vb_pct(s, 16),
           vb_pct(s, 16), VB_COLOR_RED, LV_OPA_COVER, LV_RADIUS_CIRCLE);
}

static void vb_draw_music_icon(lv_obj_t *art, int16_t s, uint32_t color)
{
  int16_t thick = vb_at_least(vb_pct(s, 9), 3);

  vb_shape(art, vb_pct(s, 30), vb_pct(s, 62), vb_pct(s, 22),
           vb_pct(s, 18), color, LV_OPA_COVER, LV_RADIUS_CIRCLE);
  vb_shape(art, vb_pct(s, 54), vb_pct(s, 20), thick, vb_pct(s, 48),
           color, LV_OPA_COVER, thick / 2);
  vb_shape(art, vb_pct(s, 54), vb_pct(s, 20), vb_pct(s, 26), thick,
           color, LV_OPA_COVER, thick / 2);
}

static void vb_draw_alarm_icon(lv_obj_t *art, int16_t s, uint32_t color)
{
  int16_t thick = vb_at_least(vb_pct(s, 8), 2);

  vb_shape_outline(art, vb_pct(s, 22), vb_pct(s, 24), vb_pct(s, 56),
                   vb_pct(s, 56), color, LV_OPA_COVER, LV_RADIUS_CIRCLE,
                   thick);
  vb_shape(art, vb_pct(s, 49), vb_pct(s, 38), thick, vb_pct(s, 22),
           color, LV_OPA_COVER, thick / 2);
  vb_shape(art, vb_pct(s, 49), vb_pct(s, 54), vb_pct(s, 18), thick,
           color, LV_OPA_COVER, thick / 2);
}

static void vb_draw_settings_icon(lv_obj_t *art, int16_t s, uint32_t color)
{
  vb_shape_outline(art, vb_pct(s, 24), vb_pct(s, 24), vb_pct(s, 52),
                   vb_pct(s, 52), color, LV_OPA_COVER, LV_RADIUS_CIRCLE,
                   vb_at_least(vb_pct(s, 9), 2));
  vb_shape_outline(art, vb_pct(s, 40), vb_pct(s, 40), vb_pct(s, 20),
                   vb_pct(s, 20), color, LV_OPA_COVER, LV_RADIUS_CIRCLE,
                   vb_at_least(vb_pct(s, 7), 2));
}

static void vb_draw_pay_icon(lv_obj_t *art, int16_t s, uint32_t color)
{
  vb_shape_outline(art, vb_pct(s, 14), vb_pct(s, 28), vb_pct(s, 72),
                   vb_pct(s, 48), color, LV_OPA_COVER, vb_pct(s, 10),
                   vb_at_least(vb_pct(s, 7), 2));
  vb_shape(art, vb_pct(s, 20), vb_pct(s, 42), vb_pct(s, 60),
           vb_at_least(vb_pct(s, 9), 3), color, LV_OPA_COVER,
           vb_pct(s, 3));
}

static void vb_draw_map_icon(lv_obj_t *art, int16_t s, uint32_t color)
{
  vb_shape(art, vb_pct(s, 35), vb_pct(s, 14), vb_pct(s, 30),
           vb_pct(s, 30), color, LV_OPA_COVER, LV_RADIUS_CIRCLE);
  vb_shape(art, vb_pct(s, 43), vb_pct(s, 22), vb_pct(s, 14),
           vb_pct(s, 14), VB_COLOR_PANEL, LV_OPA_COVER,
           LV_RADIUS_CIRCLE);
  vb_shape(art, vb_pct(s, 47), vb_pct(s, 42), vb_at_least(vb_pct(s, 9), 3),
           vb_pct(s, 34), color, LV_OPA_COVER, vb_pct(s, 5));
}

static void vb_draw_breath_icon(lv_obj_t *art, int16_t s, uint32_t color)
{
  vb_shape_outline(art, vb_pct(s, 10), vb_pct(s, 10), vb_pct(s, 80),
                   vb_pct(s, 80), color, LV_OPA_COVER, LV_RADIUS_CIRCLE, 2);
  vb_shape(art, vb_pct(s, 43), vb_pct(s, 43), vb_pct(s, 14),
           vb_pct(s, 14), color, LV_OPA_COVER, LV_RADIUS_CIRCLE);
}

static void vb_draw_phone_icon(lv_obj_t *art, int16_t s, uint32_t color)
{
  int16_t thick = vb_at_least(vb_pct(s, 13), 4);

  vb_shape(art, vb_pct(s, 25), vb_pct(s, 30), thick, vb_pct(s, 40),
           color, LV_OPA_COVER, thick / 2);
  vb_shape(art, vb_pct(s, 32), vb_pct(s, 64), vb_pct(s, 36), thick,
           color, LV_OPA_COVER, thick / 2);
  vb_shape(art, vb_pct(s, 60), vb_pct(s, 52), thick, vb_pct(s, 20),
           color, LV_OPA_COVER, thick / 2);
}

static void vb_draw_app_art(lv_obj_t *art, enum vb_app_icon_type type,
                            int16_t size, uint32_t color)
{
  switch (type)
    {
      case VB_ICON_HEART:
        vb_draw_heart_icon(art, size, color);
        break;
      case VB_ICON_SLEEP:
        vb_draw_sleep_icon(art, size, color);
        break;
      case VB_ICON_WORKOUT:
        vb_draw_workout_icon(art, size, color);
        break;
      case VB_ICON_WEATHER:
        vb_draw_weather_icon(art, size, color);
        break;
      case VB_ICON_NOTIFICATION:
        vb_draw_notification_icon(art, size, color);
        break;
      case VB_ICON_MUSIC:
        vb_draw_music_icon(art, size, color);
        break;
      case VB_ICON_ALARM:
        vb_draw_alarm_icon(art, size, color);
        break;
      case VB_ICON_SETTINGS:
        vb_draw_settings_icon(art, size, color);
        break;
      case VB_ICON_PAY:
        vb_draw_pay_icon(art, size, color);
        break;
      case VB_ICON_MAP:
        vb_draw_map_icon(art, size, color);
        break;
      case VB_ICON_BREATH:
        vb_draw_breath_icon(art, size, color);
        break;
      case VB_ICON_PHONE:
        vb_draw_phone_icon(art, size, color);
        break;
      default:
        break;
    }
}

static uint8_t vb_wheel_slot_for_item(uint8_t item_index)
{
  return (uint8_t)((item_index + VB_APP_COUNT - g_vb_wheel_focus) %
                   VB_APP_COUNT);
}

static void vb_update_wheel_icon(uint8_t item_index, bool animate)
{
  uint8_t slot = vb_wheel_slot_for_item(item_index);
  const struct vb_wheel_slot *pos;
  const struct vb_app_item *item = &g_vb_apps[item_index];
  lv_obj_t *icon = g_vb_wheel_icons[item_index];
  lv_obj_t *art = g_vb_wheel_arts[item_index];
  lv_obj_t *label = g_vb_wheel_labels[item_index];
  bool focused;
  int16_t art_size;
  int16_t art_zoom;
  int16_t label_x;
  int16_t label_y;
  uint32_t bg_color;
  uint32_t border_color;

  (void)animate;
  if (icon == NULL || art == NULL || label == NULL)
    {
      return;
    }

  if (slot >= VB_WHEEL_VISIBLE_COUNT)
    {
      lv_obj_add_flag(icon, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(label, LV_OBJ_FLAG_HIDDEN);
      return;
    }

  pos = &g_vb_wheel_slots[slot];
  focused = slot == 0;
  art_size = focused ? 42 : 24;
  bg_color = focused ? 0x0d1421 : VB_COLOR_PANEL;
  border_color = focused ? item->color : 0x2a2c33;

  if (art_size < 18)
    {
      art_size = 18;
    }

  lv_obj_clear_flag(icon, LV_OBJ_FLAG_HIDDEN);
  lv_obj_set_size(icon, pos->size, pos->size);
  lv_obj_set_pos(icon, pos->x, pos->y);
  lv_obj_set_style_opa(icon, pos->opa, 0);

  lv_obj_set_style_radius(icon, focused ? 26 : pos->size / 3, 0);
  lv_obj_set_style_bg_color(icon, vb_color(bg_color), 0);
  lv_obj_set_style_bg_opa(icon, LV_OPA_COVER, 0);
  lv_obj_set_style_border_width(icon, focused ? 3 : 1, 0);
  lv_obj_set_style_border_color(icon, vb_color(border_color), 0);
  lv_obj_set_style_shadow_width(icon, 0, 0);
  lv_obj_set_style_shadow_opa(icon, LV_OPA_TRANSP, 0);
  lv_obj_set_style_pad_all(icon, 0, 0);

  art_zoom = (int16_t)((art_size * 256) / VB_APP_ART_SIZE);
  lv_obj_set_size(art, VB_APP_ART_SIZE, VB_APP_ART_SIZE);
  lv_obj_set_style_bg_opa(art, LV_OPA_TRANSP, 0);
  lv_obj_set_style_border_width(art, 0, 0);
  lv_obj_set_style_pad_all(art, 0, 0);
  lv_obj_clear_flag(art, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_clear_flag(art, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_set_style_transform_zoom(art, art_zoom, 0);
  lv_obj_align(art, LV_ALIGN_CENTER, 0,
               focused ? -8 : 0);

  if (focused)
    {
      lv_obj_clear_flag(label, LV_OBJ_FLAG_HIDDEN);
      lv_label_set_text(label, item->name_cn);
      lv_obj_set_style_text_color(label, vb_color(VB_COLOR_TEXT), 0);
      lv_obj_set_style_text_font(label, vb_font_cn(), 0);
      lv_obj_set_width(label, 96);
      lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);
      label_x = (int16_t)(pos->x + pos->size / 2 - 48);
      label_y = (int16_t)(pos->y + pos->size + 3);
      lv_obj_set_pos(label, label_x, label_y);
      lv_obj_set_style_opa(label, LV_OPA_COVER, 0);
    }
  else
    {
      lv_obj_add_flag(label, LV_OBJ_FLAG_HIDDEN);
    }

  if (focused)
    {
      lv_obj_move_foreground(icon);
      lv_obj_move_foreground(label);
    }
}

static lv_obj_t *vb_create_app_icon(lv_obj_t *parent, uint8_t app_index)
{
  const struct vb_app_item *item = &g_vb_apps[app_index];
  lv_obj_t *icon = lv_obj_create(parent);
  lv_obj_t *art;
  lv_obj_t *label;

  lv_obj_clear_flag(icon, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_add_flag(icon, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_add_event_cb(icon, vb_wheel_icon_event, LV_EVENT_ALL,
                      (void *)(uintptr_t)(app_index + 1));
  lv_obj_set_style_bg_opa(icon, LV_OPA_COVER, 0);
  lv_obj_set_style_transform_zoom(icon, 256, 0);

  art = lv_obj_create(icon);
  lv_obj_set_size(art, VB_APP_ART_SIZE, VB_APP_ART_SIZE);
  lv_obj_set_style_bg_opa(art, LV_OPA_TRANSP, 0);
  lv_obj_set_style_border_width(art, 0, 0);
  lv_obj_set_style_pad_all(art, 0, 0);
  lv_obj_clear_flag(art, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_clear_flag(art, LV_OBJ_FLAG_CLICKABLE);
  vb_draw_app_art(art, item->icon, VB_APP_ART_SIZE, item->color);

  label = vb_label_cn(parent, "", VB_COLOR_MUTED);
  lv_obj_add_flag(label, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_add_event_cb(label, vb_wheel_icon_event, LV_EVENT_ALL,
                      (void *)(uintptr_t)(app_index + 1));

  g_vb_wheel_icons[app_index] = icon;
  g_vb_wheel_arts[app_index] = art;
  g_vb_wheel_labels[app_index] = label;

  vb_update_wheel_icon(app_index, false);
  return icon;
}

static lv_obj_t *vb_create_wheel_icon(lv_obj_t *parent, uint8_t app_index)
{
  return vb_create_app_icon(parent, app_index);
}

static void vb_set_wheel_focus(uint8_t focus)
{
  uint8_t item_index;
  uint8_t next_focus = (uint8_t)(focus % VB_APP_COUNT);
  bool focus_changed = next_focus != g_vb_wheel_focus;
  char focus_text[48];
  bool animate = g_vb_wheel_ready;

  if (g_vb_wheel_ready && !focus_changed)
    {
      return;
    }

  g_vb_wheel_focus = next_focus;

  for (item_index = 0; item_index < VB_APP_COUNT; item_index++)
    {
      vb_update_wheel_icon(item_index, animate);
    }

  if (g_vb_wheel_focus_label != NULL)
    {
      snprintf(focus_text, sizeof(focus_text), "聚焦 · %s",
               g_vb_apps[g_vb_wheel_focus].name_cn);
      lv_label_set_text(g_vb_wheel_focus_label, focus_text);
    }

  if (g_vb_wheel_ready && focus_changed)
    {
      VB_LOG("[velabridge][watch_ui] wheel focus=%s\n",
             g_vb_apps[g_vb_wheel_focus].id);
    }
}

static bool vb_accept_touch_action(const char *action)
{
  uint32_t now = lv_tick_get();

  if (g_vb_last_touch_tick != 0 &&
      (uint32_t)(now - g_vb_last_touch_tick) < VB_TOUCH_DEBOUNCE_MS)
    {
      VB_DEBUG_LOG("[velabridge][watch_ui] touch debounce ignored\n");
      return false;
    }

  g_vb_last_touch_tick = now == 0 ? 1 : now;
  VB_LOG("[velabridge][watch_ui] %s\n", action);
  vb_perf_touch_begin();
  return true;
}

static bool vb_accept_touch_click(void)
{
  return vb_accept_touch_action("touch click");
}

static void vb_open_focused_app(void)
{
  const struct vb_app_item *item = &g_vb_apps[g_vb_wheel_focus];

  VB_DEBUG_LOG("[velabridge][watch_ui] open app=%s\n", item->id);

  if (item->target != VB_SCREEN_APP_WHEEL)
    {
      vb_switch_screen(item->target);
    }
  else
    {
      vb_perf_ui_done();
    }
}

static void vb_wheel_icon_event(lv_event_t *event)
{
  lv_event_code_t code = lv_event_get_code(event);
  uintptr_t encoded = (uintptr_t)lv_event_get_user_data(event);
  uint8_t item_index;

  if (encoded == 0)
    {
      return;
    }

  item_index = (uint8_t)(encoded - 1);
  if (item_index >= VB_APP_COUNT)
    {
      return;
    }

  if (code != LV_EVENT_CLICKED)
    {
      return;
    }

  if (!vb_accept_touch_click())
    {
      return;
    }

  if (item_index == g_vb_wheel_focus)
    {
      vb_open_focused_app();
      return;
    }

  {
#if VB_WATCH_UI_PERF
    uint32_t focus_start = lv_tick_get();
#endif

    vb_set_wheel_focus(item_index);
#if VB_WATCH_UI_PERF
    vb_perf_focus_done(focus_start);
#endif
    vb_perf_ui_done();
  }
}

void vb_switch_screen(enum vb_watch_screen next)
{
  if (next >= VB_SCREEN_COUNT || g_vb_screens[next] == NULL)
    {
      return;
    }

  g_vb_current_screen = next;
  if (next != VB_SCREEN_HOME)
    {
      VB_LOG("[velabridge][watch_ui] screen=%s\n", g_vb_screen_names[next]);
    }

  lv_scr_load(g_vb_screens[next]);

  if (next == VB_SCREEN_HOME)
    {
      vb_home_show_page(VB_HOME_GRID);
    }
  else if (next == VB_SCREEN_APP_WHEEL)
    {
      vb_set_wheel_focus(g_vb_wheel_focus);
      vb_perf_ui_done();
    }
  else
    {
      vb_perf_ui_done();
    }
}

static void vb_card_clicked(lv_event_t *event)
{
  uintptr_t next = (uintptr_t)lv_event_get_user_data(event);
  enum vb_watch_screen target = (enum vb_watch_screen)next;

  if (!vb_accept_touch_click())
    {
      return;
    }

  if (g_vb_current_screen != VB_SCREEN_BOOT &&
      g_vb_current_screen != VB_SCREEN_HOME &&
      g_vb_current_screen != VB_SCREEN_FACE &&
      g_vb_current_screen != VB_SCREEN_APP_WHEEL)
    {
      target = VB_SCREEN_HOME;
    }

  vb_switch_screen(target);
}

static void vb_screen_clicked(lv_event_t *event)
{
  enum vb_watch_screen next;

  (void)event;
  if (!vb_accept_touch_click())
    {
      return;
    }

  if (g_vb_current_screen == VB_SCREEN_APP_WHEEL)
    {
      return;
    }

  if (g_vb_current_screen == VB_SCREEN_BOOT)
    {
      next = VB_SCREEN_HOME;
    }
  else if (g_vb_current_screen == VB_SCREEN_HOME)
    {
      next = VB_SCREEN_APP_WHEEL;
    }
  else
    {
      next = VB_SCREEN_HOME;
    }

  vb_switch_screen(next);
}

static void vb_bind_screen_next(lv_obj_t *obj)
{
  lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_add_event_cb(obj, vb_screen_clicked, LV_EVENT_CLICKED, NULL);
}

static void vb_bind_next(lv_obj_t *obj, enum vb_watch_screen next)
{
  lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_add_event_cb(obj, vb_card_clicked, LV_EVENT_CLICKED,
                      (void *)(uintptr_t)next);
}

static void vb_home_show_page(enum vb_home_page page)
{
  uint8_t i;

  if (page >= VB_HOME_COUNT)
    {
      return;
    }

  if (g_vb_home_page == page && page != VB_HOME_GRID)
    {
      return;
    }

  for (i = 0; i < VB_HOME_COUNT; i++)
    {
      if (g_vb_home_pages[i] == NULL)
        {
          continue;
        }

      if (i == page)
        {
          lv_obj_clear_flag(g_vb_home_pages[i], LV_OBJ_FLAG_HIDDEN);
        }
      else
        {
          lv_obj_add_flag(g_vb_home_pages[i], LV_OBJ_FLAG_HIDDEN);
        }
    }

  g_vb_home_page = page;
  VB_LOG("[velabridge][watch_ui] screen=%s\n",
         g_vb_home_page_names[page]);
  vb_perf_ui_done();
}

static void vb_home_card_clicked(lv_event_t *event)
{
  uintptr_t encoded = (uintptr_t)lv_event_get_user_data(event);
  enum vb_home_page page = (enum vb_home_page)encoded;

  if (lv_event_get_code(event) != LV_EVENT_CLICKED)
    {
      return;
    }

  if (!vb_accept_touch_click())
    {
      return;
    }

  vb_home_show_page(page);
}

static void vb_home_back_clicked(lv_event_t *event)
{
  if (lv_event_get_code(event) != LV_EVENT_CLICKED)
    {
      return;
    }

  if (!vb_accept_touch_click())
    {
      return;
    }

  vb_home_show_page(VB_HOME_GRID);
}

static void vb_home_open_wheel(lv_event_t *event)
{
  lv_event_code_t code = lv_event_get_code(event);

  if (code != LV_EVENT_CLICKED && code != LV_EVENT_LONG_PRESSED)
    {
      return;
    }

  if (!vb_accept_touch_click())
    {
      return;
    }

  vb_switch_screen(VB_SCREEN_APP_WHEEL);
}

static lv_obj_t *vb_create_metric_card(lv_obj_t *parent, int16_t x,
                                       int16_t y, int16_t w, int16_t h,
                                       const char *label,
                                       const char *value, uint32_t color)
{
  lv_obj_t *card = vb_create_card(parent, x, y, w, h);
  lv_obj_t *title = vb_label_auto(card, label, VB_COLOR_MUTED,
                                  vb_font_small());
  lv_obj_t *number = vb_label_auto(card, value, color, vb_font_title());

  lv_obj_align(title, LV_ALIGN_TOP_LEFT, 0, 0);
  lv_obj_align(number, LV_ALIGN_BOTTOM_LEFT, 0, 0);
  vb_card_float(card, x / 2);

  return card;
}

static lv_obj_t *vb_create_home_page(lv_obj_t *screen)
{
  lv_obj_t *page = lv_obj_create(screen);

  lv_obj_set_size(page, VB_WATCH_WIDTH, VB_WATCH_HEIGHT);
  lv_obj_set_pos(page, 0, 0);
  lv_obj_set_style_bg_color(page, vb_color(VB_COLOR_BG), 0);
  lv_obj_set_style_bg_opa(page, LV_OPA_COVER, 0);
  lv_obj_set_style_border_width(page, 0, 0);
  lv_obj_set_style_pad_all(page, 0, 0);
  lv_obj_clear_flag(page, LV_OBJ_FLAG_SCROLLABLE);

  return page;
}

static void vb_create_home_icon(lv_obj_t *parent, uint32_t color)
{
  vb_shape(parent, 0, 1, 20, 20, color, LV_OPA_COVER, LV_RADIUS_CIRCLE);
  vb_shape(parent, 25, 7, 24, 6, color, LV_OPA_COVER, 3);
  vb_shape(parent, 25, 19, 14, 4, color, LV_OPA_70, 2);
}

static lv_obj_t *vb_create_home_card(lv_obj_t *parent, int16_t x,
                                     int16_t y,
                                     const struct vb_home_item *item)
{
  lv_obj_t *card = vb_create_card(parent, x, y, 154, 72);
  lv_obj_t *title;
  lv_obj_t *status;

  lv_obj_set_style_radius(card, 18, 0);
  lv_obj_set_style_pad_all(card, 10, 0);
  lv_obj_set_style_bg_color(card, vb_color(0x101216), 0);
  lv_obj_set_style_bg_opa(card, LV_OPA_COVER, 0);
  lv_obj_set_style_border_color(card, vb_color(0x272b33), 0);
  lv_obj_set_style_border_width(card, 1, 0);
  lv_obj_add_flag(card, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_add_event_cb(card, vb_home_card_clicked, LV_EVENT_CLICKED,
                      (void *)(uintptr_t)item->page);

  vb_create_home_icon(card, item->color);

  title = vb_label_cn(card, item->title, VB_COLOR_TEXT);
  lv_obj_set_width(title, 112);
  lv_obj_align(title, LV_ALIGN_TOP_LEFT, 0, 28);

  status = vb_label_cn(card, item->status, VB_COLOR_MUTED);
  lv_obj_set_width(status, 132);
  lv_obj_align(status, LV_ALIGN_BOTTOM_LEFT, 0, 0);

  return card;
}

static void vb_build_home_grid_page(lv_obj_t *screen)
{
  static const int16_t xs[2] = { 28, 208 };
  static const int16_t ys[3] = { 94, 178, 262 };

  lv_obj_t *page = vb_create_home_page(screen);
  lv_obj_t *title;
  lv_obj_t *subtitle;
  lv_obj_t *wheel_button;
  size_t i;

  vb_create_status_bar(page, "首页");

  title = vb_label(page, "VelaBridge", VB_COLOR_TEXT, vb_font_title());
  lv_obj_align(title, LV_ALIGN_TOP_LEFT, 30, 56);

  subtitle = vb_label_cn(page, "无障碍沟通终端", VB_COLOR_MUTED);
  lv_obj_align(subtitle, LV_ALIGN_TOP_LEFT, 30, 82);

  wheel_button = vb_create_pill(page, "应用", VB_COLOR_BLUE);
  lv_obj_align(wheel_button, LV_ALIGN_TOP_RIGHT, -28, 58);
  lv_obj_add_flag(wheel_button, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_add_event_cb(wheel_button, vb_home_open_wheel, LV_EVENT_CLICKED,
                      NULL);

  lv_obj_add_flag(page, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_add_event_cb(page, vb_home_open_wheel, LV_EVENT_LONG_PRESSED,
                      NULL);

  for (i = 0; i < VB_HOME_ITEM_COUNT; i++)
    {
      vb_create_home_card(page, xs[i % 2], ys[i / 2],
                          &g_vb_home_items[i]);
    }

  g_vb_home_pages[VB_HOME_GRID] = page;
}

static void vb_build_home_detail_page(lv_obj_t *screen,
                                      const struct vb_home_item *item)
{
  lv_obj_t *page = vb_create_home_page(screen);
  lv_obj_t *card;
  lv_obj_t *title;
  lv_obj_t *status;
  lv_obj_t *hint;

  vb_create_status_bar(page, item->title);

  card = vb_create_card(page, 28, 72, 334, 248);
  lv_obj_set_style_bg_color(card, vb_color(0x101216), 0);
  lv_obj_set_style_border_color(card, vb_color(item->color), 0);
  lv_obj_set_style_radius(card, 24, 0);

  vb_create_home_icon(card, item->color);

  title = vb_label_cn_title(card, item->title, VB_COLOR_TEXT);
  lv_obj_align(title, LV_ALIGN_TOP_LEFT, 0, 50);

  status = vb_label_cn(card, item->status, item->color);
  lv_obj_set_width(status, 280);
  lv_obj_align(status, LV_ALIGN_TOP_LEFT, 0, 104);

  hint = vb_label_cn(card, "点击返回首页", VB_COLOR_MUTED);
  lv_obj_align(hint, LV_ALIGN_BOTTOM_LEFT, 0, 0);

  lv_obj_add_flag(page, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_add_event_cb(page, vb_home_back_clicked, LV_EVENT_CLICKED, NULL);
  lv_obj_add_flag(card, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_add_event_cb(card, vb_home_back_clicked, LV_EVENT_CLICKED, NULL);

  lv_obj_add_flag(page, LV_OBJ_FLAG_HIDDEN);
  g_vb_home_pages[item->page] = page;
}

static void vb_build_home(void)
{
  lv_obj_t *screen = vb_create_screen_base();
  size_t i;

  memset(g_vb_home_pages, 0, sizeof(g_vb_home_pages));
  g_vb_home_page = VB_HOME_GRID;

  vb_build_home_grid_page(screen);
  for (i = 0; i < VB_HOME_ITEM_COUNT; i++)
    {
      vb_build_home_detail_page(screen, &g_vb_home_items[i]);
    }

  g_vb_screens[VB_SCREEN_HOME] = screen;
}

static void vb_build_boot(void)
{
  lv_obj_t *screen = vb_create_screen_base();
  lv_obj_t *mark = vb_create_card(screen, 132, 72, 126, 126);
  lv_obj_t *x = vb_label_cn_title(mark, "星", VB_COLOR_TEXT);
  lv_obj_t *star = vb_label(mark, "+", VB_COLOR_BLUE, vb_font_title());
  lv_obj_t *brand = vb_label_cn_title(screen, "星阈", VB_COLOR_TEXT);
  lv_obj_t *product = vb_label(screen, "VelaBridge Watch",
                               VB_COLOR_MUTED, vb_font_small());
  lv_obj_t *pill = vb_create_pill(screen, "openvela 就绪", VB_COLOR_BLUE);

  lv_obj_set_style_bg_color(mark, vb_color(0x0b1425), 0);
  lv_obj_set_style_border_color(mark, vb_color(VB_COLOR_BLUE), 0);
  lv_obj_center(x);
  lv_obj_align(star, LV_ALIGN_TOP_MID, 0, 12);
  lv_obj_align(brand, LV_ALIGN_TOP_MID, 0, 218);
  lv_obj_align(product, LV_ALIGN_TOP_MID, 0, 255);
  lv_obj_align(pill, LV_ALIGN_TOP_MID, 0, 286);
  vb_bind_screen_next(screen);
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
  lv_obj_t *heart;
  lv_obj_t *move;
  lv_obj_t *sleep;

  vb_create_status_bar(screen, "表盘");
  time = vb_label(screen, "09:39", VB_COLOR_TEXT, vb_font_big());
  lv_obj_align(time, LV_ALIGN_TOP_LEFT, 34, 62);

  brand = vb_label(screen, "VelaBridge", VB_COLOR_TEXT, vb_font_title());
  lv_obj_align(brand, LV_ALIGN_TOP_LEFT, 38, 136);

  pill = vb_create_pill(screen, "AI 就绪", VB_COLOR_BLUE);
  lv_obj_align(pill, LV_ALIGN_TOP_LEFT, 38, 172);

  heart = vb_create_metric_card(screen, 28, 222, 104, 88, "心率", "72",
                                VB_COLOR_RED);
  move = vb_create_metric_card(screen, 143, 222, 104, 88, "活动", "58%",
                               VB_COLOR_GREEN);
  sleep = vb_create_metric_card(screen, 258, 222, 104, 88, "睡眠", "7小时",
                                VB_COLOR_PURPLE);
  vb_bind_next(heart, VB_SCREEN_HEART);
  vb_bind_next(move, VB_SCREEN_WORKOUT);
  vb_bind_next(sleep, VB_SCREEN_SLEEP);

  battery = vb_label_cn(screen, "电量 86%", VB_COLOR_MUTED);
  lv_obj_align(battery, LV_ALIGN_BOTTOM_MID, 0, -28);

  vb_bind_screen_next(screen);
  g_vb_screens[VB_SCREEN_FACE] = screen;
}

static void vb_build_app_wheel(void)
{
  lv_obj_t *screen = vb_create_screen_base();
  lv_obj_t *title;
  lv_obj_t *line;
  uint8_t i;

  vb_create_status_bar(screen, "应用");
  g_vb_wheel_ready = false;

  title = vb_label_cn_title(screen, "应用", VB_COLOR_TEXT);
  lv_obj_align(title, LV_ALIGN_TOP_LEFT, 32, 58);

  for (i = 0; i < VB_APP_COUNT; i++)
    {
      vb_create_wheel_icon(screen, i);
    }

  line = lv_obj_create(screen);
  lv_obj_set_size(line, 210, 2);
  lv_obj_align(line, LV_ALIGN_BOTTOM_MID, 0, -58);
  lv_obj_set_style_bg_color(line, vb_color(VB_COLOR_BLUE), 0);
  lv_obj_set_style_bg_opa(line, LV_OPA_70, 0);
  lv_obj_set_style_border_width(line, 0, 0);
  lv_obj_clear_flag(line, LV_OBJ_FLAG_SCROLLABLE);

  g_vb_wheel_focus_label =
    vb_label_cn(screen, "聚焦 · 心率", VB_COLOR_TEXT);
  lv_obj_align(g_vb_wheel_focus_label, LV_ALIGN_BOTTOM_MID, 0, -30);

  vb_set_wheel_focus(g_vb_wheel_focus);
  g_vb_wheel_ready = true;

  g_vb_screens[VB_SCREEN_APP_WHEEL] = screen;
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

  vb_create_status_bar(screen, "健康");
  card = vb_create_card(screen, 28, 58, 334, 286);
  title = vb_label_cn_title(card, "心率", VB_COLOR_TEXT);
  lv_obj_align(title, LV_ALIGN_TOP_LEFT, 0, 0);

  number = vb_create_big_number(card, "72", "次/分", VB_COLOR_RED);
  lv_obj_align(number, LV_ALIGN_TOP_LEFT, 0, 58);

  ring = vb_create_progress_ring(card, 72, VB_COLOR_RED);
  lv_obj_align(ring, LV_ALIGN_RIGHT_MID, -6, -8);

  resting = vb_create_pill(card, "静息 61", VB_COLOR_RED);
  lv_obj_align(resting, LV_ALIGN_BOTTOM_LEFT, 0, 0);

  peak = vb_create_pill(card, "峰值 128", VB_COLOR_ORANGE);
  lv_obj_align(peak, LV_ALIGN_BOTTOM_RIGHT, 0, 0);

  vb_card_float(card, 0);
  vb_bind_screen_next(screen);
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

  vb_create_status_bar(screen, "运动");
  card = vb_create_card(screen, 28, 58, 334, 286);
  title = vb_label_cn_title(card, "户外跑步", VB_COLOR_TEXT);
  lv_obj_align(title, LV_ALIGN_TOP_LEFT, 0, 0);

  distance = vb_create_big_number(card, "3.42", "公里", VB_COLOR_GREEN);
  lv_obj_align(distance, LV_ALIGN_TOP_LEFT, 0, 58);

  ring = vb_create_progress_ring(card, 58, VB_COLOR_GREEN);
  lv_obj_align(ring, LV_ALIGN_RIGHT_MID, -6, -8);

  pace = vb_create_pill(card, "配速 5'28\"", VB_COLOR_GREEN);
  lv_obj_align(pace, LV_ALIGN_BOTTOM_LEFT, 0, 0);

  calories = vb_create_pill(card, "消耗 284 千卡", VB_COLOR_ORANGE);
  lv_obj_align(calories, LV_ALIGN_BOTTOM_RIGHT, 0, 0);

  vb_card_float(card, 0);
  vb_bind_screen_next(screen);
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

  text = vb_label_cn(parent, label, VB_COLOR_MUTED);
  lv_obj_align_to(text, seg, LV_ALIGN_OUT_BOTTOM_MID, 0, 8);
}

static void vb_build_sleep(void)
{
  lv_obj_t *screen = vb_create_screen_base();
  lv_obj_t *card;
  lv_obj_t *title;
  lv_obj_t *time;

  vb_create_status_bar(screen, "睡眠");
  card = vb_create_card(screen, 28, 58, 334, 286);
  title = vb_label_cn_title(card, "睡眠", VB_COLOR_TEXT);
  lv_obj_align(title, LV_ALIGN_TOP_LEFT, 0, 0);

  time = vb_create_big_number(card, "7小时", "42分", VB_COLOR_PURPLE);
  lv_obj_align(time, LV_ALIGN_TOP_LEFT, 0, 58);

  vb_segment(card, 0, 78, VB_COLOR_PURPLE, "深睡");
  vb_segment(card, 86, 140, VB_COLOR_BLUE, "核心");
  vb_segment(card, 234, 64, VB_COLOR_ORANGE, "快眼");

  vb_card_float(card, 0);
  vb_bind_screen_next(screen);
  vb_bind_next(card, VB_SCREEN_SETTINGS);
  g_vb_screens[VB_SCREEN_SLEEP] = screen;
}

static void vb_build_settings(void)
{
  static const char *items[] =
  {
    "显示",
    "健康",
    "触感",
    "系统",
  };

  lv_obj_t *screen = vb_create_screen_base();
  lv_obj_t *title;
  int i;

  vb_create_status_bar(screen, "设置");
  title = vb_label_cn_title(screen, "设置", VB_COLOR_TEXT);
  lv_obj_align(title, LV_ALIGN_TOP_LEFT, 32, 64);

  for (i = 0; i < 4; i++)
    {
      lv_obj_t *row = vb_create_card(screen, 32, 116 + i * 54, 326, 44);
      lv_obj_t *text = vb_label_cn(row, items[i], VB_COLOR_TEXT);
      lv_obj_t *arrow = vb_label(row, ">", VB_COLOR_BLUE, vb_font_small());

      lv_obj_set_style_radius(row, 16, 0);
      lv_obj_align(text, LV_ALIGN_LEFT_MID, 0, 0);
      lv_obj_align(arrow, LV_ALIGN_RIGHT_MID, 0, 0);
      vb_card_float(row, i * 40);
      vb_bind_next(row, VB_SCREEN_FACE);
    }

  vb_bind_screen_next(screen);
  g_vb_screens[VB_SCREEN_SETTINGS] = screen;
}

static void vb_build_all_screens(void)
{
  vb_build_boot();
  vb_build_home();
  vb_build_face();
  vb_build_app_wheel();
  vb_build_heart();
  vb_build_workout();
  vb_build_sleep();
  vb_build_settings();
}

int velabridge_watch_ui_start(void)
{
  int ret;

  VB_LOG("[velabridge][watch_ui] LVGL enabled\n");
  VB_DEBUG_LOG("[velabridge][watch_ui] checking default display\n");

  ret = vb_lvgl_display_init();
  if (ret < 0)
    {
      VB_LOG("[velabridge][watch_ui] display init failed ret=%d, fallback\n",
             ret);
      return ret;
    }

  VB_LOG("[velabridge][watch_ui] display ready\n");
  VB_LOG("[velabridge][watch_ui] performance mode enabled\n");
  VB_LOG("[velabridge][watch_ui] wheel objects persistent\n");
  VB_DEBUG_LOG("[velabridge][watch_ui] design=%dx%d\n",
               VB_WATCH_WIDTH, VB_WATCH_HEIGHT);

  vb_build_all_screens();
  g_vb_current_screen = VB_SCREEN_HOME;
  lv_scr_load(g_vb_screens[VB_SCREEN_HOME]);
  vb_home_show_page(VB_HOME_GRID);

  VB_DEBUG_LOG("[velabridge][watch_ui] ui loop start\n");

  while (1)
    {
      uint32_t idle = lv_timer_handler();

#if VB_WATCH_UI_PERF
      uint32_t now = lv_tick_get();

      if (g_vb_last_loop_tick != 0)
        {
          g_vb_loop_interval_ms = vb_elapsed_ms(g_vb_last_loop_tick, now);
        }

      g_vb_last_loop_tick = now == 0 ? 1 : now;
#endif

      if (idle == 0 || idle > 5)
        {
          idle = 5;
        }

      usleep(idle * 1000);
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
  VB_LOG("[velabridge][watch_ui] LVGL prototype disabled\n");
  VB_LOG("[velabridge][watch_ui] enable CONFIG_GRAPHICS_LVGL "
         "to render watch UI\n");

  return -ENOSYS;
}

#endif
