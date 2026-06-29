# VelaBridge 第一周任务板

队伍名称：维拉星核。项目名称：VelaBridge。比赛编号：039。

维拉星核团队开发的 VelaBridge，是一款基于 openvela 的多模态无障碍 AI 沟通与伴行终端。

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
| 黄山派 SF32LB52 屏幕出厂检测记录 | 硬件 / 固件工程师 | `docs/bringup_log.md` / `docs/hardware_validation.md` | 记录出厂自动检测、CH340 COM5、SSCOM、1000000 波特率；屏幕点亮 / 循环显示标记为已通过初步验证。 | 是 | P0 |
| 触摸日志确认 | 硬件 / 固件工程师 | `docs/hardware_validation.md` | 使用 SSCOM 连接 COM5，波特率 1000000，保存触摸日志并确认是否通过。 | 是 | P0 |
| 喇叭 / 麦克风 / 震动验证 | 硬件 / 固件工程师 | `docs/hardware_validation.md` | 分别完成测试并补充人工确认结果；当前喇叭 / 麦克风保持未触发测试 / 待复测，震动保持待测试。 | 是 | P1 |
| 记录 SF32LB52 构建问题 | 固件 / 文档负责人 | `docs/build_notes.md` | 写清 repo sync、board config、VelaBridge app Register、底层缺失头文件和 workaround 风险。 | 否 | P0 |
| 询问官方推荐 build config | 项目负责人 / 固件工程师 | `docs/official_question_sf32lb52_build.md` | 形成可发给官方群、GitHub issue 或指导老师的问题说明。 | 否 | P0 |
| 查询官方 SF32LB52 推荐编译方式 | 项目负责人 / 固件工程师 | `docs/sf32lb52_build_research.md` | 整理 openvela 官方文档、比赛 README、SiFli / 立创资料和本地证据；找不到直接说明时写明“未找到官方直接说明”。 | 否 | P0 |
| 运行 diagnose_sf32lb52_build.sh | 固件工程师 | `scripts/diagnose_sf32lb52_build.sh` | 在 openvela workspace 根目录运行只读诊断脚本，保存 board config、HAL/CMSIS 路径、git-lfs、repo、toolchain 输出。 | 否 | P0 |
| 安装/确认 WSL 编译依赖 | 固件工程师 | `scripts/setup_openvela_wsl_deps.sh` | 安装或确认 Git、CMake、Ninja、Git LFS、Python、repo 和 ARM 工具链状态；不执行 repo sync 或烧录。 | 否 | P0 |
| 等官方回复后修复 build | 固件工程师 | 外层 openvela workspace | 官方确认推荐 config、构建命令和 HAL/CMSIS 依赖后，再做可复现修复并记录。 | 是 | P1 |
| 暂停手工软链接底层头文件 | 固件工程师 | `docs/build_notes.md` | 停止提交或扩大 `nuttx/`、`vendor/sifli/` 手工软链接 workaround；保留风险说明。 | 否 | P1 |
| 并行推进 VelaBridge Watch 应用层 UI/AI Bridge/用户调研 | 产品 / AI / 固件工程师 | `ai_bridge/` / `app/velabridge_app/` / `docs/` | 在底层 build blocker 等官方确认期间，继续推进 UI 流程、AI Bridge mock、协议、演示脚本和用户调研。 | 否 | P1 |
| 保持 VelaBridge Watch 应用层开发推进 | AI / 固件工程师 | `ai_bridge/` / `app/velabridge_app/` / `docs/` | 在底层 build blocker 未解决前，继续推进 mock、协议、状态机和演示脚本。 | 否 | P0 |
| 等官方确认后修复底层 build | 固件工程师 | 外层 openvela workspace | 官方确认推荐 config 和 HAL / CMSIS 依赖后，再修复本地构建链路。 | 是 | P1 |
| 编译 VelaBridge app | 固件工程师 | `app/velabridge_app/` | `Register: velabridge_app` 后继续完成完整编译，不把未通过写成已完成。 | 是 | P1 |
| 烧录黄山派验证 | 硬件 / 固件工程师 | 外层 openvela workspace / 开发板 | 固件完整编译通过后再烧录黄山派，验证启动日志和应用入口。 | 是 | P1 |

> 当前环境 `gh` CLI 已安装但未登录，暂不自动创建 GitHub Issues。可使用 `docs/github_issues_seed.md` 复制创建。

## 硬件 Bring-up 当前进度

| 任务 | 优先级 | 状态 | 进展 / 证据 | 下一步 / 依赖 |
| --- | --- | --- | --- | --- |
| 硬件 bring-up 记录 | P0 | 进行中 | USB 串口、COM5、启动日志、屏幕、触摸已通过初步验证。 | 按键、音频、震动测试。 |
| 屏幕基础验证 | P0 | 已通过初步验证 | 全屏绿色测试画面；证据：`docs/assets/bringup/screen_green_factory_test.jpg`。 | 后续可补充更多色块照片。 |
| 触摸屏验证 | P0 | 已通过初步验证 | Touch down 坐标日志；证据：`docs/assets/bringup/sscom_touch_log_COM5.png`。 | 后续验证应用层触摸事件。 |
| 串口基础验证 | P0 | 已通过 | USB-SERIAL CH340 COM5，SSCOM 1000000，R:32236；证据：`docs/assets/bringup/windows_ch340_com5_powershell.png` 和串口日志。 | 后续继续保存完整测试日志。 |
| 按键状态机 | P0 | 待做 | 待补充人工结果。 | 依赖 KEY1 / KEY2 日志确认。 |
| 喇叭提示音 | P0 | 未触发测试 / 待复测 | 出厂自动检测停留在屏幕 / 触摸循环，尚无音频测试结果。 | 后续用单独 demo 或 VelaBridge 固件验证。 |
| 麦克风输入 | P0 | 未触发测试 / 待复测 | 出厂自动检测停留在屏幕 / 触摸循环，尚无录音或输入电平结果。 | 后续用单独 demo 或 VelaBridge 固件验证。 |
| 震动反馈 | P0 | 待测试 | 待补充人工结果。 | 确认板载震动马达或马达接口。 |
| 串口 JSON 接收 | P0 | 待做 | 待补充人工结果。 | 依赖 VelaBridge 自定义固件接入。 |
| openvela repo sync | P0 | 已完成 | WSL2 Ubuntu 中 `repo sync` 已成功。 | 继续确认官方推荐 build config。 |
| VelaBridge app Register | P0 | 已确认 | 构建日志多次出现 `Register: velabridge_app`。 | 继续排查底层 HAL / CMSIS include 链路。 |
| SF32LB52 构建 blocker | P0 | 阻塞中 | `ipc_queue/Make.defs` 与 HAL / CMSIS 头文件连续缺失。 | 询问官方 / 老师推荐 config 和依赖同步方式。 |
| openvela workspace 编译 | P0 | 阻塞中 | 当前 blocker 属于 SF32LB52 / Huangshanpai board config 底层依赖链路。 | 官方确认前不写固件已编译通过。 |
| VelaBridge 设备端最小程序 | P0 | 待做 | 构建系统已 Register，但完整编译和烧录尚未通过。 | 等底层 build blocker 解决后编译。 |
