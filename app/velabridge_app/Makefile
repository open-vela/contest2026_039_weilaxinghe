############################################################################
# Contest 2026 team 039 VelaBridge app - Makefile
############################################################################

include $(APPDIR)/Make.defs

PROGNAME  = velabridge_app
PRIORITY  = SCHED_PRIORITY_DEFAULT
STACKSIZE = 16384
MODULE    = $(CONFIG_LVX_USE_DEMO_CONTEST2026_039_VELABRIDGE_APP)

CSRCS = velabridge_watch_ui.c
MAINSRC = velabridge_app_main.c

ifneq ($(CONFIG_LV_FONT_SIMSUN_16_CJK),y)
  CFLAGS += -DLV_FONT_SIMSUN_16_CJK=1
  VPATH += $(APPDIR)/graphics/lvgl/lvgl/src/font
  CSRCS += lv_font_simsun_16_cjk.c
endif

include $(APPDIR)/Application.mk
