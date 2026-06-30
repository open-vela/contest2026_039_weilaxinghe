# contest2026_039_weilaxinghe

| 项目身份 | 内容 |
| --- | --- |
| 队伍名称 | 维拉星核 |
| 项目名称 | VelaBridge |
| 比赛编号 | 039 |
| 仓库 | `contest2026_039_weilaxinghe` |
| 比赛 | 2026 首届 openvela AI 硬件开发者大赛 |
| GitHub 账号 | `chenzeyuan233` |
| 比赛邮箱 | `13045875890@163.com` |
| 项目方向 | AI 硬件 / openvela / 智能手表 / 无障碍沟通 / 多模态交互 |

## VelaBridge 项目简介

维拉星核团队开发的 VelaBridge，是一款基于 openvela 的多模态无障碍 AI 沟通与伴行终端。

VelaBridge 面向听障、视障、语言表达困难者，在校园问路、窗口服务、食堂点餐、读门牌、一键求助等场景中，把外界语音、文字和用户按键事件转换为字幕、语音播报、震动反馈和快捷回复。

当前设备形态按 VelaBridge Watch 推进：基于黄山派 SF32LB52 的多模态无障碍 AI 智能手表。历史文档中的 VelaBridge 表示同一项目的通用名称。

本项目参加 2026 首届 openvela AI 硬件开发者大赛，当前选择 **AI 硬件产品创新** 方向。第一批最小任务先完成电脑端 AI Bridge mock、串口 JSON 协议、MVP 文档、演示脚本、测试样例、AI Coding 日志模板和 GitHub 协作规范。

VelaBridge 不声称替代医疗设备、导盲杖或导盲犬；默认不保存原始音频和图片，只保存脱敏文字摘要、按键事件、状态变化和错误码。

## 当前 MVP 范围

- 串口 JSON Lines 通信：`protocol/velabridge_protocol.md` 定义设备端与 AI Bridge 的消息格式。
- 字幕显示：`caption` 支持普通字幕和“教务处 / 盖章”分步骤简化。
- 按键状态机：设备端预留 `NORMAL`、`CAPTION_MODE`、`BLIND_MODE`、`OCR_MODE`、`HELP_MODE`、`DEMO_MODE`。
- 震动反馈：协议支持 `none`、`short`、`long`、`urgent`、`double`。
- 快捷回复：`quick_reply` 用于“请您说慢一点”等低门槛表达。
- 一键求助：`help` 生成求助摘要并进入 HELP_MODE。
- 演示模式：`python3 ai_bridge/mock_bridge.py --demo --jsonl` 输出固定演示消息。

## 当前硬件进展

| 项目 | 状态 | 说明 |
| --- | --- | --- |
| 开发板 | 已进入出厂自动检测流程 | 黄山派 SF32LB52 开发板已到货并完成初步 bring-up。 |
| USB 供电 | 已通过 | 开发板已上电，板载绿色 LED 已亮。 |
| 串口 | 已通过 | Windows 已识别 USB-SERIAL CH340，COM 口为 COM5，硬件 ID 为 `USB\VID_1A86&PID_7523`。 |
| 串口工具 | 已通过 | SSCOM V5.13.1 已打开 COM5。 |
| 串口参数 | 已通过 | 1000000 8N1，无流控；接收计数已达到 R:32236。 |
| 启动日志 | 已通过 | 串口日志出现 SiFli Corporation、build on May 23 2025、mount /dev success。 |
| 屏幕显示 | 已通过初步验证 | 屏幕已点亮，显示全屏绿色测试画面。 |
| 触摸屏坐标输出 | 已通过初步验证 | 触摸屏已输出 Touch down 坐标日志。 |
| KEY1 | 待测试 | 需要补充按键日志。 |
| KEY2 | 待补充完整流程结果 | 需要补充完整自动检测流程结果。 |
| 喇叭 | 未触发测试 / 待复测 | 出厂自动检测停留在屏幕 / 触摸测试循环，当前没有进入音频测试流程。 |
| 麦克风 | 未触发测试 / 待复测 | 出厂自动检测停留在屏幕 / 触摸测试循环，当前没有进入录音或输入电平测试流程。 |
| 震动 | 待测试 | 暂无人工确认结果。 |
| VelaBridge 自定义固件 | 待做 | 尚未完成 VelaBridge 固件烧录。 |

硬件证据文件已保存到 `docs/assets/bringup/`，包括 CH340 COM5 PowerShell 截图、屏幕绿色出厂测试照片、SSCOM 触摸日志截图和串口原始日志。

当前结果只代表硬件 bring-up 阶段，不代表 VelaBridge 应用功能已经完成；还没有完成设备端字幕显示、AI Bridge 串口 JSON 闭环或 VelaBridge 自定义固件烧录。下一步是测试 KEY1 / KEY2 日志；音频不再卡出厂测试流程，后续用单独 demo 或 VelaBridge 固件验证；继续测试震动马达，并开始 openvela workspace 编译、官方 hello_app 编译和 VelaBridge 最小设备端适配。

详细记录见 `docs/bringup_log.md`、`docs/hardware_validation.md`、`docs/assets/bringup/README.md` 和 `docs/next_hardware_steps.md`。没有实测证据的模块不能标记为已通过。

## 当前构建进展

| 项目 | 状态 | 说明 |
| --- | --- | --- |
| openvela repo sync | 已完成 | WSL2 Ubuntu 中完整 workspace 已同步成功。 |
| 团队仓库分支 | 已完成 | 团队仓库已切回 `dev-ai-contest-2026`。 |
| AI Bridge mock | 已通过 | `ai_bridge/mock_bridge.py --demo --jsonl` 可运行。 |
| Python unittest | 已通过 | `python3 -m unittest tests/test_mock_bridge.py` 已通过，6 tests OK。 |
| VelaBridge app Register | 已确认 | 构建日志多次出现 `Register: velabridge_app`。 |
| 当前 build blocker | 阻塞中 | 已从 `ipc_queue`、FPU、HAL / CMSIS 头文件缺失推进到 HAL 宏配置 / `rtconfig.h` / ARM intrinsic 配置缺失阶段。 |
| 官方确认 | 待确认 | 正在等待官方 / 老师确认推荐 board config、是否使用 openvela `build.sh` 或 SiFli SDK `scons`、HAL config 来源和依赖同步方式。 |

当前不能写“固件已编译通过”，也不能写“已经可以烧录”。构建问题记录见 `docs/build_notes.md`，官方 / 老师提问稿见 `docs/official_question_sf32lb52_build.md`。

## 仓库目录说明

```text
ai_bridge/                 # 电脑端 AI Bridge mock，不接真实 API Key
protocol/                  # 串口 JSON Lines 协议文档
docs/                      # MVP 计划、任务板、演示脚本
scripts/                   # 本地临时排查脚本，不提交生成的 openvela 公共源码改动
tests/                     # Python 标准库 unittest 测试
logs/                      # AI Coding 日志模板和官方日志示例目录
app/hello_app/             # 官方应用形态样例，当前批次不修改
quickapp/hello_quickapp/   # 官方快应用形态样例，当前批次不修改
board/contest_board/       # 官方板级适配样例，当前批次不修改
```

当前批次不修改 `openvela.xml`，不修改官方 `app/hello_app`，也不写真实硬件驱动。

## 运行 AI Bridge Mock

```bash
python3 ai_bridge/mock_bridge.py "我要去教务处盖章，请问流程是什么" --jsonl
python3 ai_bridge/mock_bridge.py "小心，前面危险" --jsonl
python3 ai_bridge/mock_bridge.py --demo --jsonl
```

如果需要后续串口发送，可在安装 `pyserial` 后使用 `--serial-port`；未安装 `pyserial` 时脚本不会因为缺少串口依赖而退出。

## 运行测试

```bash
python3 -m unittest tests/test_mock_bridge.py
```

测试覆盖普通字幕、教务处长句简化、危险关键词 urgent 震动、求助、盲人模式菜单和 demo 输出数量。

> Windows 环境如果 `python3` 指向 WindowsApps 占位入口，可使用 `py` 替代，例如 `py -m unittest tests/test_mock_bridge.py`。

## 第一周开发目标

1. 用 `ai_bridge/mock_bridge.py` 跑通 demo JSON Lines 输出。
2. 用 `tests/test_mock_bridge.py` 固化普通字幕、简化字幕、危险提醒、求助、盲人菜单和 demo 测试。
3. 根据 `protocol/velabridge_protocol.md` 评审设备端与 AI Bridge 的消息字段和错误码。
4. 使用 `docs/demo_script.md` 准备 5 分钟演示视频。
5. 根据 `docs/task_board.md` 拆分后续设备端 app、串口、屏幕、按键和震动任务。
6. 按 `logs/ai_coding_log.md` 记录 AI Coding 过程，并按官方 `logs/README.md` 归集真实 JSONL 日志。

## 代码与 AI Coding 日志提交要求

- 小步提交，PR 描述写清修改文件、测试命令、开发板人工验证项和风险。
- 不提交密钥、API Key、原始音频、原始图片或未授权个人信息。
- 保留官方比赛模板文件，不删除 `hello_app`、`hello_quickapp`、`contest_board` 和 `logs/README.md`。
- 每次较大变更同步更新 README、协议文档、任务板或 AI Coding 日志模板。

---

👋 欢迎参加 **2026 首届 openvela AI 硬件开发者大赛**！

这是组委会为你的队伍创建的**专属参赛仓库**（本仓为样例/模板，队伍编号 `039`；你看到的将是你自己的 `contest2026_<编号>_<队伍名>`）。比赛期间，你的全部参赛代码、打包产物与 AI Coding 日志都提交到这里。

> 本仓既是「代码仓」，又内置了一键拉取整套 openvela 工程的 `repo` 清单（manifest）。你只需跟它打交道，**自始至终只动一个文件夹**。

---

## 一、先读这些官方文档

**通用（所有赛道必读）：**

| 文档                                                                                                                                     | 用途                                           |
| ---------------------------------------------------------------------------------------------------------------------------------------- | ---------------------------------------------- |
| [《大赛总览》](https://github.com/open-vela/docs/blob/dev-ai-contest-2026/zh-cn/contest_2026/contest_overview.md)                        | 赛道、流程、评分、资源，建议先通读             |
| [《参赛代码提交指南》](https://github.com/open-vela/docs/blob/dev-ai-contest-2026/zh-cn/contest_2026/code_submission_guide.md)           | 仓库获取、提交流程、时间与权限（**以此为准**） |
| [《AI Coding 日志归集与提交手册》](https://github.com/open-vela/docs/blob/dev-ai-contest-2026/zh-cn/contest_2026/ai_coding_log_guide.md) | 如何导出 AI 对话日志并提交到 `logs/`           |

**按你的赛道选读（三选一）：**

| 赛道                  | 教程导航                                                                                                                                                 |
| --------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------- |
| 快应用 / 手表应用创新 | [快应用教程导航](https://github.com/open-vela/docs/blob/dev-ai-contest-2026/zh-cn/contest_2026/quickapp/quickapp_guide_index.md)                         |
| AI 硬件产品创新       | [AI 硬件赛道教程导航](https://github.com/open-vela/docs/blob/dev-ai-contest-2026/zh-cn/contest_2026/ai_hardware/ai_hardware_guide_index.md)              |
| 新硬件适配            | [新硬件适配赛道教程导航](https://github.com/open-vela/docs/blob/dev-ai-contest-2026/zh-cn/contest_2026/hardware_porting/hardware_porting_guide_index.md) |

---

## 二、第一步：拉取完整工程

用组委会提供的命令一键拉取「openvela 全量源码 + 你的专属仓」：

```bash
repo init -u https://github.com/open-vela/contest2026_039_weilaxinghe \
  -b dev-ai-contest-2026 -m contest2026_039_weilaxinghe.xml
repo sync -c -j8
```

同步后，你的整个仓库位于工作区的 `contest2026_039_weilaxinghe/`，openvela 全量源码在外层（`nuttx/`、`apps/`、`packages/`、`vendor/` 等）。

---

## 三、第二步：在哪里写代码

**只在自己的仓目录 `contest2026_039_weilaxinghe/` 里开发。** 不同作品形态放在对应子目录，manifest 会通过 `<linkfile>` 把它们**软链**到 openvela 编译树该在的位置——你不用手动 copy：

| 作品形态 | 你的代码放这里             | 系统自动映射到                                 |
| -------- | -------------------------- | ---------------------------------------------- |
| 应用     | `app/hello_app/`           | `packages/demos/contest2026_039_hello_app`     |
| 快应用   | `quickapp/hello_quickapp/` | `packages/apps/contest2026_039_hello_quickapp` |
| 板级适配 | `board/contest_board/`     | `vendor/openvela/boards/contest2026_039_board` |

> 用不到的形态目录可以删掉；新增作品时按同样规则加子目录，并在 `contest2026_039_weilaxinghe.xml` 里补一条 `<linkfile>` 映射即可。**生产仓库（packages/nuttx/vendor 等）零改动。**

建议仓库目录约定（便于评委定位）：

```text
app/ | quickapp/ | board/   # 你的作品代码
logs/                       # AI Coding 日志（主动导出后提交，格式见 logs/README.md）
README.md                   # 作品说明（提交前请改成你自己的，见第六节）
```

> 仓内附带了一个 `.gitignore.example`，给出了**编译产物**等不需要进仓的文件示例。如需启用，`cp .gitignore.example .gitignore` 后按需增删即可。**注意 `logs/` 下最终导出的 AI Coding 日志必须提交，不要忽略。**
>
> `logs/` 的目录结构与提交格式见 [logs/README.md](logs/README.md)。

---

## 四、第三步：编译与运行

编译/运行步骤随作品形态不同而不同，请参考你所在赛道的教程导航：

- 快应用 / 手表应用：[快应用教程导航](https://github.com/open-vela/docs/blob/dev-ai-contest-2026/zh-cn/contest_2026/quickapp/quickapp_guide_index.md)（含模拟器与开发板部署）。
- AI 硬件产品创新：[AI 硬件赛道教程导航](https://github.com/open-vela/docs/blob/dev-ai-contest-2026/zh-cn/contest_2026/ai_hardware/ai_hardware_guide_index.md)（环境搭建、编译烧录、Skill 开发）。
- 新硬件适配：[新硬件适配赛道教程导航](https://github.com/open-vela/docs/blob/dev-ai-contest-2026/zh-cn/contest_2026/hardware_porting/hardware_porting_guide_index.md)（BSP 移植、最小 NSH 基线）。

子目录已通过 manifest 中的 `<linkfile>` 软链进 openvela 编译树，因此构建在 openvela 工作区**根目录**（即你这个仓的上一级）进行。openvela 使用 `build.sh` 作为统一入口，接收一个 **board config 路径**作为参数：

```bash
# 进入 openvela 工作区根目录（你的仓的上一级）
cd ..

# 通用语法：第一个参数是 board config 路径，第二个参数可以是 menuconfig / distclean 等
./build.sh <board-config-path> [menuconfig|distclean] [-j8]
```

> 具体的 board config 路径、目标产物、模拟器/真机部署方式请以你所在赛道的教程导航为准。本仓 `app/` `quickapp/` `board/` 三个示例骨架对应的 Kconfig 选项可通过 `menuconfig` 启用。

---

## 五、第四步：提交作品

1. **fork** 你的专属仓 → 开发 → `git commit` 并推送 → 向专属仓发起 **Pull Request**，可**自行 review 并合入**（无需等组委会）。
2. **AI Coding 日志**：与 AI 工具的对话会自动记录到本机 staging（不会自动上传），需你**主动导出/打包**选定会话到仓内 `logs/` 目录后一并提交。详见[《AI Coding 日志归集与提交手册》](https://github.com/open-vela/docs/blob/dev-ai-contest-2026/zh-cn/contest_2026/ai_coding_log_guide.md)。
3. 若需改动 **nuttx 等公共仓库**，不在本仓改，而是 fork 对应公共仓、以 PR 提交到 `dev-ai-contest-2026` 分支，由组委会 review 后合入。

> ⏰ **提交作品截止：9 月 20 日**。截止后统一收回 push 权限，仍可查看 / clone。
>
> 获奖后再按要求将作品 PR 至 openvela 上游对应仓库（走标准 PR + CI 流程）。

### 关于 PR 与 CLA

- 本仓所有改动通过 **Pull Request** 合入（分支保护强制，可自行合入自己的 PR）。
- 首次贡献需在[**官网签署 CLA**](https://openvela.com/#/community/cla)；PR 上会自动跑 `cla/signature` 检查，在官网签署成功后，在 PR 评论 `/check-cla` 复检即可通过。

---

## 六、提交前：把本 README 改成你的作品说明

本文件目前是组委会给的**使用说明书**。**作品提交前，请把它替换成你自己作品的说明**，方便评委快速了解你做了什么、怎么跑起来。建议至少包含以下内容：

```markdown
# <你的作品名>

## 一、作品简介
<一句话/一段话说明这个作品是什么、解决什么问题、亮点在哪>

## 二、选题方向
<快应用 / 手表应用创新 ｜ AI 硬件产品创新 ｜ 新硬件适配 ｜ 自定方向，并简述理由>

## 三、目录结构
<列出你这个仓里各目录/文件的作用，例如：>
- `app/xxx/`        — <说明>
- `board/xxx/`      — <说明>
- `quickapp/xxx/`   — <说明>
- `logs/`           — AI Coding 日志
- `docs/` 或其他    — <说明>

## 四、运行方式
<拉取工程后，如何编译、烧录/部署、运行的完整步骤；最好能让评委照着一步步复现>

## 五、AI Coding 使用说明
<说明本作品如何借助 AI 辅助开发：
- 在需求拆解 / 方案设计 / 编码 / 调试 / 文档等环节如何与 AI 协作；
- AI 对开发效率或质量带来的实际帮助。
完整对话日志见 logs/ 目录>
```

> 提示：将会根据「作品本身 + 你的 README 说明 + `logs/` 里的 AI Coding 日志」来理解和评估你的作品，README 写清楚很重要。

---

## 附：仓库命名规范

`contest2026_<编号>_<队伍名>` — 编号三位零填充；队名 slug（全小写、英文/拼音、连字符）。例：`contest2026_039_weilaxinghe`。
（仓库由组委会统一创建，**每队仅一个仓**，无需自行命名。）
