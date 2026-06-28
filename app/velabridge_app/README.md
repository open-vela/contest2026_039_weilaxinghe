# velabridge_app

VelaBridge device-side state machine skeleton for the openvela contest repo.

This directory follows the same minimal app layout as `app/hello_app`:

- `Kconfig` defines the app switch.
- `Make.defs` adds the app to `CONFIGURED_APPS` when enabled.
- `Makefile` builds the NuttX application entry point.
- `CMakeLists.txt` registers the app for CMake-based builds.
- `velabridge_app_main.c` contains a small state machine and log stubs.

## Current Scope

The current implementation is intentionally a skeleton:

- no GPIO driver code
- no real screen driver code
- no real vibrator driver code
- no real audio or TTS driver code
- no full JSON parser yet

The code uses `printf` to simulate display, prompt, vibration, logging, and JSON handling.

## States

- `NORMAL`
- `CAPTION_MODE`
- `BLIND_MODE`
- `OCR_MODE`
- `HELP_MODE`
- `DEMO_MODE`

## Events

- `EVT_BOOT`
- `EVT_KEY_SHORT`
- `EVT_KEY_LONG`
- `EVT_JSON_CAPTION`
- `EVT_JSON_DANGER`
- `EVT_JSON_HELP`
- `EVT_TIMEOUT`
- `EVT_ERROR`

## Follow-up Integration Points

- Screen: replace `velabridge_display_text()` with the board display / LVGL / UI API after the target display stack is confirmed.
- Buttons: call `velabridge_dispatch_event(EVT_KEY_SHORT, ...)` and `EVT_KEY_LONG` from the confirmed key input callback.
- Vibration: replace `velabridge_vibrate()` with the confirmed vibrator framework or board driver.
- Audio: replace `velabridge_play_prompt()` with the confirmed prompt / TTS / audio playback API.
- Serial: feed each received JSON Lines frame into `velabridge_handle_json_line()`.

## Build Notes

This app is linked into the full openvela workspace by `contest2026_039_weilaxinghe.xml`:

```xml
<linkfile src="app/velabridge_app" dest="packages/demos/contest2026_039_velabridge_app"/>
```

The exact board config and full build command still need manual confirmation in a synced openvela workspace. The contest README shows the generic entry point:

```bash
cd ..
./build.sh <board-config-path> [menuconfig|distclean] [-j8]
```

