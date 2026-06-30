# Contest 2026 team 039 VelaBridge app
if(CONFIG_LVX_USE_DEMO_CONTEST2026_039_VELABRIDGE_APP)
  if(CONFIG_LVX_USE_DEMO_CONTEST2026_039_VELABRIDGE_APP_STACKSIZE)
    set(VELABRIDGE_APP_STACKSIZE
        ${CONFIG_LVX_USE_DEMO_CONTEST2026_039_VELABRIDGE_APP_STACKSIZE})
  else()
    set(VELABRIDGE_APP_STACKSIZE 16384)
  endif()

  nuttx_add_application(
    NAME velabridge_app
    SRCS
      velabridge_app_main.c
      velabridge_watch_ui.c
    STACKSIZE ${VELABRIDGE_APP_STACKSIZE})
endif()
