# VelaBridge 黄山派 SF32LB52 Bring-up 记录

## 基本信息

| 项目 | 内容 |
| --- | --- |
| 队伍 | 维拉星核 |
| 项目 | VelaBridge |
| 比赛编号 | 039 |
| 仓库 | `contest2026_039_weilaxinghe` |
| 比赛 | 2026 首届 openvela AI 硬件开发者大赛 |
| 开发板 | 黄山派 SF32LB52 |
| 项目方向 | 基于 openvela 的多模态无障碍 AI 沟通与伴行终端 |
| 作品形态 | AI 无障碍沟通胸牌 / 挂件终端 |
| 操作日期 | 2026-06-28 |

维拉星核团队开发的 VelaBridge，是一款基于 openvela 的多模态无障碍 AI 沟通与伴行终端。

## 证据文件

- `docs/assets/bringup/windows_ch340_com5_powershell.png`：CH340 COM5 识别证据来自 PowerShell 截图，显示 USB-SERIAL CH340 (COM5)，硬件 ID 为 `USB\VID_1A86&PID_7523`。
- `docs/assets/bringup/screen_green_factory_test.jpg`：屏幕测试证据来自 `screen_green_factory_test.jpg`，显示黄山派 SF32LB52 进入出厂测试并显示全屏绿色测试画面。
- `docs/assets/bringup/sscom_touch_log_COM5.png`：触摸测试证据来自 `sscom_touch_log_COM5.png`，显示 SSCOM 打开 COM5、波特率 1000000，并接收到 Touch down 坐标日志。
- `docs/assets/bringup/sscom_screen_touch_loop_COM5_2026-06-28.txt`：串口原始日志来自 `sscom_screen_touch_loop_COM5_2026-06-28.txt`，用于记录屏幕 / 触摸出厂测试过程。

## USB 串口识别结果

| 项目 | 内容 |
| --- | --- |
| 设备名称 | USB-SERIAL CH340 |
| COM 口 | COM5 |
| 硬件 ID | `USB\VID_1A86&PID_7523` |
| 状态 | 已通过 |

## SSCOM 串口配置

| 项目 | 内容 |
| --- | --- |
| 工具 | SSCOM V5.13.1 |
| Port | COM5 |
| Baudrate | 1000000 |
| Data bits | 8 |
| Stop bits | 1 |
| Parity | None |
| Flow control | None |
| SSCOM 状态 | COM5 已打开 |
| 接收计数 | R:32236 |

## 屏幕测试

| 项目 | 内容 |
| --- | --- |
| 当前现象 | 屏幕已点亮，显示全屏绿色测试画面 |
| 状态 | 已通过 |
| 证据 | `docs/assets/bringup/screen_green_factory_test.jpg` |

## 触摸测试

| 项目 | 内容 |
| --- | --- |
| 当前现象 | 触摸屏幕时，SSCOM 输出 Touch down 坐标 |
| 状态 | 已通过 |
| 证据 | `docs/assets/bringup/sscom_touch_log_COM5.png`、`docs/assets/bringup/sscom_screen_touch_loop_COM5_2026-06-28.txt` |

### Touch down 坐标日志示例

```text
Touch down [223,280]
Touch down [228,285]
Touch down [233,289]
Touch down [237,291]
Touch down [243,293]
Touch down [249,291]
Touch down [255,289]
Touch down [259,286]
Touch down [261,283]
Touch down [263,279]
Touch down [264,275]
Touch down [264,268]
Touch down [265,260]
Touch down [264,253]
Touch down [264,246]
Touch down [263,240]
Touch down [262,235]
Touch down [261,230]
Touch down [260,226]
Touch down [259,222]
Touch down [258,219]
Touch down [257,217]
Touch down [256,215]
Touch down [255,213]
Touch down [255,212]
Touch down [255,210]
Touch down [254,210]
Touch down [254,209]
Touch down [253,208]
```

## 启动日志摘要

```text
SiFli Corporation
build on May 23 2025, 2.4.0
mount /dev success
```

## 硬件模块状态

| 硬件模块 | 当前状态 | 证据 / 说明 |
| --- | --- | --- |
| USB 供电 | 已通过 | 开发板已上电，板载绿色 LED 已亮。 |
| CH340 串口识别 | 已通过 | Windows 已识别 USB-SERIAL CH340，硬件 ID 为 `USB\VID_1A86&PID_7523`；证据：`docs/assets/bringup/windows_ch340_com5_powershell.png`。 |
| COM5 串口连接 | 已通过 | SSCOM V5.13.1 已打开 COM5；证据：`docs/assets/bringup/sscom_touch_log_COM5.png`、`docs/assets/bringup/sscom_screen_touch_loop_COM5_2026-06-28.txt`。 |
| 启动日志输出 | 已通过 | 串口日志包含 SiFli Corporation、build on May 23 2025, 2.4.0、mount /dev success。 |
| 屏幕点亮 | 已通过 | 屏幕已点亮，显示全屏绿色测试画面；证据：`docs/assets/bringup/screen_green_factory_test.jpg`。 |
| 触摸坐标输出 | 已通过 | 触摸时 SSCOM 连续输出 Touch down 坐标日志；证据：`docs/assets/bringup/sscom_touch_log_COM5.png`、`docs/assets/bringup/sscom_screen_touch_loop_COM5_2026-06-28.txt`。 |
| KEY1 | 待测试 | 需要补充按键日志。 |
| KEY2 | 待补充完整结果 | 需要补充完整自动检测流程结果。 |
| 喇叭 | 待测试 | 不得标记为已通过。 |
| 麦克风 | 待测试 | 不得标记为已通过。 |
| 震动马达 | 待测试 | 需要确认是否有板载震动马达或马达接口。 |
| VelaBridge 固件烧录 | 待做 | 需要拉取完整 openvela workspace 后编译 / 烧录。 |

## 当前结论

黄山派 SF32LB52 已完成基础上电、USB 串口枚举、COM5 串口通信、启动日志输出、屏幕点亮和触摸坐标输出验证，具备进入 openvela 应用适配和 VelaBridge MVP 设备端开发的条件。

## 下一步

- 测试 KEY1 / KEY2。
- 测试喇叭和麦克风。
- 确认震动马达或马达接口。
- 拉取完整 openvela workspace。
- 编译官方 hello_app。
- 编译 / 适配 `app/velabridge_app`。
- 跑通串口 JSON -> 屏幕显示 -> 按键状态机 -> 日志输出。
