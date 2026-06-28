# Contest 2026 team 039 VelaBridge app
if(CONFIG_LVX_USE_DEMO_CONTEST2026_039_VELABRIDGE_APP)
  nuttx_add_application(
    NAME velabridge_app
    SRCS velabridge_app_main.c
    STACKSIZE 4096)
endif()

