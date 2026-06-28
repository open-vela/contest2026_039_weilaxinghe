# VelaBridge 硬件验证清单

队伍名称：维拉星核。项目名称：VelaBridge。比赛编号：039。

维拉星核团队开发的 VelaBridge，是一款基于 openvela 的多模态无障碍 AI 沟通与伴行终端。

## 当前硬件与串口配置

- 开发板：黄山派 SF32LB52
- 串口设备：USB-SERIAL CH340
- 硬件 ID：`USB\VID_1A86&PID_7523`
- COM 口：COM5
- 串口工具：SSCOM V5.13.1
- 串口参数：1000000 8N1，无流控
- SSCOM 接收计数：R:32236

## 验证矩阵

| 模块 | 验证方法 | 当前状态 | 通过标准 | 当前证据 | 后续动作 |
| --- | --- | --- | --- | --- | --- |
| 电源 / USB 供电 | USB 连接电脑后观察开发板上电状态。 | 已通过 | 开发板上电，板载绿色 LED 亮。 | 板载绿色 LED 已亮。 | 保存上电照片 `power_on_green_led.jpg`。 |
| CH340 USB 串口 | 在 Windows PowerShell 检查串口设备。 | 已通过 | 识别到 USB-SERIAL CH340 和硬件 ID。 | USB-SERIAL CH340，`USB\VID_1A86&PID_7523`；证据：`docs/assets/bringup/windows_ch340_com5_powershell.png`。 | 如需复核，可再次运行 PowerShell 串口查询命令。 |
| COM5 串口通信 | 使用 SSCOM 打开 COM5。 | 已通过 | COM5 可打开并持续接收数据。 | SSCOM COM5 已打开，R:32236；证据：`docs/assets/bringup/sscom_touch_log_COM5.png`、`docs/assets/bringup/sscom_screen_touch_loop_COM5_2026-06-28.txt`。 | 后续继续保存完整测试日志。 |
| 启动日志 | 观察串口启动输出。 | 已通过 | 出现厂商、构建版本和挂载成功日志。 | SiFli Corporation；build on May 23 2025, 2.4.0；mount /dev success。 | 保存完整自动检测日志。 |
| 屏幕显示 | 观察出厂自动检测屏幕画面。 | 已通过初步验证 | 屏幕点亮并显示测试画面。 | 屏幕显示全屏绿色测试画面；证据：`docs/assets/bringup/screen_green_factory_test.jpg`。 | 后续可补充更多测试色块照片。 |
| 触摸屏坐标输出 | 触摸屏幕并观察 SSCOM 输出。 | 已通过初步验证 | 触摸时输出连续 Touch down 坐标。 | Touch down [223,280] 等坐标日志；证据：`docs/assets/bringup/sscom_touch_log_COM5.png`、`docs/assets/bringup/sscom_screen_touch_loop_COM5_2026-06-28.txt`。 | 后续继续验证 KEY / 应用层触摸事件。 |
| KEY1 | 按下 KEY1 并观察串口日志。 | 待测试 | 串口输出 KEY1 相关日志或状态变化。 | 待补充人工结果。 | 执行 KEY1 短按 / 长按测试。 |
| KEY2 | 执行 KEY2 自动检测流程并保存日志。 | 待补充完整流程结果 | 有完整自动检测流程结果。 | 待补充人工结果。 | 执行并保存 KEY2 自动检测日志。 |
| 喇叭 | 播放测试音或提示音。 | 未触发测试 / 待复测 | 可听到测试音，日志无错误。 | 出厂自动检测停留在屏幕 / 触摸测试循环，尚未进入音频 / 喇叭测试流程。 | 后续用单独 demo 或 VelaBridge 固件验证提示音。 |
| 麦克风 | 执行录音或输入电平测试。 | 未触发测试 / 待复测 | 可采集声音或看到输入电平变化。 | 出厂自动检测停留在屏幕 / 触摸测试循环，尚未进入麦克风录音或输入电平测试流程。 | 后续用单独 demo 或 VelaBridge 固件验证输入。 |
| 震动马达 | 触发震动测试或确认马达接口。 | 待测试 | 可触发 `short` / `long` / `urgent` 等震动模式。 | 待补充人工结果。 | 确认是否有板载震动马达或马达接口。 |
| VelaBridge 自定义固件 | 拉取完整 openvela workspace 后编译 / 烧录。 | 待做 | `app/velabridge_app` 可编译、烧录并输出启动日志。 | 待补充人工结果。 | 拉取 workspace，确认构建入口和 board config。 |
| 串口 JSON 通信 | PC AI Bridge 通过串口发送 JSON Lines。 | 待 VelaBridge 固件接入 | 串口 JSON -> 屏幕显示 -> 按键状态机 -> 日志输出闭环跑通。 | 待补充人工结果。 | 接入 VelaBridge 固件串口接收逻辑。 |

## 约束

- 没有实测证据的模块必须保持“待测试 / 待复测 / 待补充人工结果”。
- 音频 / 喇叭 / 麦克风当前未触发测试，不能写失败，也不能写通过。
- 当前不得声明 KEY1、KEY2、喇叭、麦克风、震动马达、VelaBridge 自定义固件或串口 JSON 闭环已通过。
- Codex 不得伪造图片、视频、日志或硬件测试结果。
