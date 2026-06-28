# VelaBridge 第一周任务板

| Issue 标题 | 负责人角色 | 目录 | 验收标准 | 是否需要开发板 | 优先级 |
| --- | --- | --- | --- | --- | --- |
| openvela 开发环境跑通 | 固件工程师 | `README.md` / 外层 openvela 工作区 | 能按官方命令 `repo sync`，并找到 `build.sh` 与目标 board config。 | 是 | P0 |
| app/velabridge_app 工程骨架 | 固件工程师 | `app/velabridge_app/` | Kconfig、Makefile、CMakeLists、入口 C 文件存在，启动打印三行 boot log。 | 否 | P0 |
| 串口 JSON 接收 | 固件工程师 | `app/velabridge_app/` | 能按行接收 JSON Lines，并对非法 JSON 返回 `INVALID_JSON`。 | 是 | P0 |
| 屏幕字幕显示 | 固件工程师 | `app/velabridge_app/` | 收到 `caption.display` 后屏幕显示短字幕，不遮挡状态提示。 | 是 | P0 |
| 按键状态机 | 固件工程师 | `app/velabridge_app/` | 支持短按、长按、双击，能切换 `NORMAL` / `CAPTION_MODE` / `HELP_MODE`。 | 是 | P0 |
| 震动反馈 | 硬件工程师 | `app/velabridge_app/` | `short`、`long`、`urgent`、`double` 至少 4 种反馈可区分。 | 是 | P0 |
| 快捷回复 | 产品 / 固件工程师 | `app/velabridge_app/` / `ai_bridge/` | 按键或 mock 输入触发 `quick_reply`，设备端显示并预留播报。 | 是 | P1 |
| 一键求助 | 产品 / 固件工程师 | `app/velabridge_app/` / `protocol/` | 长按触发 HELP_MODE，串口返回求助日志，设备端显示求助提示。 | 是 | P0 |
| ai_bridge/mock_bridge.py | AI 工程师 | `ai_bridge/mock_bridge.py` | 支持中文输入、`--demo`、`--jsonl`，无外部依赖，unittest 通过。 | 否 | P0 |
| protocol/velabridge_protocol.md | 协议 / 文档负责人 | `protocol/` | 写清 JSON Lines、字段、枚举、错误码和 10 个完整示例。 | 否 | P0 |
| 演示脚本 | 产品 / 视频负责人 | `docs/demo_script.md` | 5 分钟脚本覆盖痛点、硬件、字幕、快捷回复、盲人模式、求助和日志。 | 否 | P1 |
| AI Coding 日志整理 | 项目负责人 | `logs/` | 每次开发有日期、工具、Prompt、输出摘要、测试结果、风险和 TODO。 | 否 | P0 |
| OCR 读字 mock | AI 工程师 | `ai_bridge/` / `protocol/` | 输入“读字”或“门牌”输出 `ocr_result`，设备端显示 / 播报字段明确。 | 否 | P1 |
| 危险关键词提醒 | AI / 固件工程师 | `ai_bridge/` / `app/velabridge_app/` | “危险”“让开”“报警”“小心”触发 `danger_alert` 和 `urgent` 震动。 | 是 | P1 |
| GitHub 协作规范 | 项目负责人 | `AGENTS.md` / `README.md` | 写清小步提交、测试、PR 描述、AI Coding 日志和禁止事项。 | 否 | P1 |

> 当前环境 `gh` CLI 已安装但未登录，暂不自动创建 GitHub Issues。可使用 `docs/github_issues_seed.md` 复制创建。

