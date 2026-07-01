# Contest 2026 team 039 VelaBridge app
if(CONFIG_LVX_USE_DEMO_CONTEST2026_039_VELABRIDGE_APP)
  if(CONFIG_LVX_USE_DEMO_CONTEST2026_039_VELABRIDGE_APP_STACKSIZE)
    set(VELABRIDGE_APP_STACKSIZE
        ${CONFIG_LVX_USE_DEMO_CONTEST2026_039_VELABRIDGE_APP_STACKSIZE})
  else()
    set(VELABRIDGE_APP_STACKSIZE 16384)
  endif()

  set(VELABRIDGE_APP_SRCS
      velabridge_app_main.c
      velabridge_watch_ui.c)

  if(NOT CONFIG_LV_FONT_SIMSUN_16_CJK)
    add_compile_options(-DLV_FONT_SIMSUN_16_CJK=1)
    list(APPEND VELABRIDGE_APP_SRCS
         ${NUTTX_APPS_DIR}/graphics/lvgl/lvgl/src/font/lv_font_simsun_16_cjk.c)
  endif()

  nuttx_add_application(
    NAME velabridge_app
    SRCS
      ${VELABRIDGE_APP_SRCS}
    STACKSIZE ${VELABRIDGE_APP_STACKSIZE})
endif()
