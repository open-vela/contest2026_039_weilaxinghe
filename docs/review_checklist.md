# VelaBridge Bring-up 审查清单

## 基本信息

- 队伍名称：维拉星核
- 项目名称：VelaBridge
- 比赛编号：039
- 仓库名称：`contest2026_039_weilaxinghe`
- 开发板：黄山派 SF32LB52
- 审查日期：2026-06-28

## 已检查文件

- `README.md`
- `AGENTS.md`
- `docs/bringup_log.md`
- `docs/hardware_validation.md`
- `docs/next_hardware_steps.md`
- `docs/task_board.md`
- `docs/assets/bringup/README.md`
- `logs/ai_coding_log.md`
- `ai_bridge/mock_bridge.py`
- `tests/test_mock_bridge.py`
- `protocol/velabridge_protocol.md`
- `app/velabridge_app/`
- `docs/account_and_submission_check.md`

## 已确认通过项

- 开发板已上电。
- Windows PowerShell 已查询到 USB-SERIAL CH340 (COM5)，硬件 ID 为 `USB\VID_1A86&PID_7523`。
- SSCOM V5.13.1 可打开 COM5。
- 串口参数记录为 1000000 baud、8 data bits、1 stop bit、None parity、None flow control。
- SSCOM 已收到开发板日志，接收计数曾达到 R:32236。
- 启动日志中出现 `SiFli Corporation`、`build on May 23 2025, 2.4.0`、`mount /dev success`。
- 开发板进入出厂自动检测 / Factory Test 流程。
- 屏幕显示全屏绿色测试画面，状态为已通过初步验证。
- 触摸屏输出 Touch down 坐标日志，状态为已通过初步验证。
- 证据文件已归档到 `docs/assets/bringup/`，仓库根目录没有残留原始证据文件。
- 当前仓库远端为 `open-vela/contest2026_039_weilaxinghe`。
- 当前分支基于 `dev-ai-contest-2026`。
- 文档账号统一为 `chenzeyuan233` / `13045875890@163.com`。

## 发现的问题

- 部分文档曾将屏幕显示和触摸坐标输出写为“已通过”，不够精确。
- 部分文档曾将喇叭和麦克风写为笼统“待测试”，没有说明出厂自动检测尚未进入音频流程。
- `docs/next_hardware_steps.md` 仍包含已完成的证据上传动作。
- AI Coding 日志缺少早期 MVP scaffold 和队伍名称统一记录。
- 仓库文档缺少独立账号与提交检查记录。

## 已修复的问题

- 将屏幕显示和触摸屏坐标输出统一修正为“已通过初步验证”。
- 将喇叭和麦克风统一修正为“未触发测试 / 待复测”。
- 补充说明当前出厂自动检测停留在屏幕 / 触摸测试循环。
- 补充说明音频 / 喇叭 / 麦克风不能写失败，也不能写通过。
- 更新任务板中的硬件 bring-up 状态和下一步。
- 更新 AI Coding 日志，补齐 MVP scaffold、队伍名称统一、bring-up 证据审查记录。
- 新增 `docs/account_and_submission_check.md`，记录正确仓库、开发分支、比赛账号、比赛邮箱、本地 git config 建议和后续提交要求。
- 更新 README 与 AGENTS 中的比赛账号信息。
- 追加 AI Coding 日志，记录比赛账号统一事件。

## 测试结果

- `python3 ai_bridge/mock_bridge.py --demo --jsonl`：当前 Windows 环境中 `python3` 指向 WindowsApps 占位入口，返回 1 且无输出。
- `python3 -m unittest tests/test_mock_bridge.py`：当前 Windows 环境中 `python3` 指向 WindowsApps 占位入口，返回 1 且无输出。
- `py -X utf8 ai_bridge/mock_bridge.py --demo --jsonl`：通过，输出 7 条 JSON Lines 演示消息。
- `py -X utf8 -m unittest tests/test_mock_bridge.py`：通过，6 个 unittest 用例 OK。

## 账号与提交检查

- 仓库是否正确：已确认，远端为 `open-vela/contest2026_039_weilaxinghe`。
- 分支是否正确：当前修复分支基于 `dev-ai-contest-2026`。
- 文档账号是否统一：已统一为 `chenzeyuan233` / `13045875890@163.com`。
- 证据文件是否在 `docs/assets/bringup/`：已确认，4 个 bring-up 证据文件均在该目录。
- 未测试硬件是否没有被误标通过：已确认，喇叭、麦克风、震动马达仍未标记为已通过。
- README 是否说明当前只是 bring-up 阶段：已确认。
- `logs/ai_coding_log.md` 是否记录账号统一事件：已追加记录。
- 历史 commit author 是否重写：未重写历史 commit author。

## 仍需人工完成的事项

- KEY1 / KEY2 完整串口日志仍需人工测试。
- 音频 / 喇叭 / 麦克风仍需通过单独 demo 或 VelaBridge 固件复测。
- 震动马达仍需确认是否存在板载硬件或可用接口。
- VelaBridge 自定义 openvela 固件尚未编译和烧录。
- 串口 JSON 通信闭环尚未接入开发板验证。
- 设备端字幕显示、按键状态机、震动反馈和日志回传仍需开发板实测。

## 下一步建议

1. 测 KEY1 / KEY2 日志。
2. 不再卡出厂音频测试，音频后续用单独 demo 或 VelaBridge 固件验证。
3. 拉取完整 openvela workspace。
4. 编译官方 hello_app。
5. 确认 `app/velabridge_app` 构建方式。
6. 实现最小闭环：PC AI Bridge JSON → 串口 → 黄山派 → 屏幕 / 日志 / 按键状态机。

## 审查结论

本次审查未发现证据文件缺失。当前文档不声明喇叭、麦克风、震动马达、VelaBridge 固件烧录、设备端字幕显示或 AI Bridge 串口 JSON 闭环已完成。
