# VelaBridge Bring-up 证据目录

本目录用于保存维拉星核团队 VelaBridge 项目的硬件 bring-up 证据，包括照片、截图、视频和串口日志。

## 证据文件清单

以下文件已由人工实测后上传并归档：

- `windows_ch340_com5_powershell.png`
  - 证明 Windows PowerShell 查询到 USB-SERIAL CH340 (COM5)，硬件 ID 为 `USB\VID_1A86&PID_7523`。
- `screen_green_factory_test.jpg`
  - 证明黄山派 SF32LB52 屏幕进入出厂测试，显示全屏绿色测试画面。
- `sscom_touch_log_COM5.png`
  - 证明 SSCOM 已打开 COM5，波特率 1000000，并接收到 Touch down 坐标日志。
- `sscom_screen_touch_loop_COM5_2026-06-28.txt`
  - 保存 SSCOM 串口日志，用于记录屏幕 / 触摸出厂测试过程。

以下证据仍需要后续人工实测后补充：

- `power_on_green_led.jpg`
- `key1_test_log.png`
- `key2_auto_test_log.png`
- `audio_test_result.mp4`
- `microphone_test_result.mp4`
- `vibration_test_result.mp4`
- `full_auto_test_log.txt`

## 证据要求

- 图片、视频和日志必须由人工实测后上传。
- Codex 不得伪造图片、日志或硬件测试结果。
- 没有实测证据的模块不能标记为已通过。
- 已上传证据应在 `docs/bringup_log.md` 或 `docs/hardware_validation.md` 中注明对应文件名和结论。
