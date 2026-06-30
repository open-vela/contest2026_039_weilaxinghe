############################################################################
# Contest 2026 team 039 VelaBridge app - Makefile
############################################################################

include $(APPDIR)/Make.defs

PROGNAME  = velabridge_app
PRIORITY  = SCHED_PRIORITY_DEFAULT
STACKSIZE = $(or $(CONFIG_LVX_USE_DEMO_CONTEST2026_039_VELABRIDGE_APP_STACKSIZE),12288)
MODULE    = $(CONFIG_LVX_USE_DEMO_CONTEST2026_039_VELABRIDGE_APP)

CSRCS = velabridge_watch_ui.c
MAINSRC = velabridge_app_main.c

include $(APPDIR)/Application.mk
