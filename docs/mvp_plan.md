# VelaBridge MVP 计划

队伍名称：维拉星核。项目名称：VelaBridge。比赛编号：039。

## VelaBridge 最终要做什么

维拉星核团队开发的 VelaBridge，是一款基于 openvela 的多模态无障碍 AI 沟通与伴行终端。

它面向听障、视障、语言表达困难者，把外界语音、文字和用户按键事件转换为字幕、语音播报、震动反馈、快捷回复和求助摘要，让校园问路、窗口服务、食堂点餐、读门牌等高频场景更容易沟通。

VelaBridge 不是医疗设备，不替代导盲杖或导盲犬，也不承诺完整安全监护。第一阶段只验证沟通闭环和 openvela 端硬件交互。

## 第一阶段 MVP 闭环

电脑 AI Bridge 发送 JSON -> 黄山派接收 -> 屏幕显示 -> 按键切换 -> 震动 / 提示音 -> 串口返回日志。

第一阶段所有 AI 能力先 mock：ASR、OCR、LLM 简化、危险关键词检测、快捷回复推荐和求助摘要都由 `ai_bridge/mock_bridge.py` 生成结构化 JSON，保证演示可复现。

## 设备端负责什么

- 屏幕：显示字幕、简化步骤、OCR 结果、求助状态和演示步骤。
- 按键：处理短按、长按、双击，切换字幕 / 盲人 / OCR / 求助 / 演示模式。
- 震动：根据协议中的 `vibration` 枚举触发不同提示。
- 音频：后续接入提示音和 TTS 播报，第一阶段可先用 log stub。
- 状态机：维护 `NORMAL`、`CAPTION_MODE`、`BLIND_MODE`、`OCR_MODE`、`HELP_MODE`、`DEMO_MODE`。
- 日志：串口返回按键事件、模式变化、错误码和脱敏文字摘要。
- 串口通信：逐行解析 JSON Lines，校验字段和错误码。

## AI Bridge 负责什么

- ASR mock：把命令行输入文本当作语音识别结果。
- OCR mock：把门牌、窗口、菜单文字转换为 `ocr_result`。
- LLM mock：把长句拆成短字幕和步骤。
- 字幕简化：对“教务处”“盖章”等高频场景输出分步骤字幕。
- 快捷回复：生成“请您说慢一点”等固定回复。
- 求助摘要：把“求助”“帮我联系老师”等触发词转换为 `help` 消息。
- 危险提醒：对“危险”“让开”“报警”“小心”等关键词输出 `danger_alert`。

## 第一周验收标准

1. `python3 ai_bridge/mock_bridge.py --demo --jsonl` 能输出 5-7 条 JSON Lines。
2. `python3 -m unittest tests/test_mock_bridge.py` 全部通过。
3. `protocol/velabridge_protocol.md` 包含字段、枚举、错误码和至少 10 个 JSON 示例。
4. `app/velabridge_app` 有参考 `hello_app` 的最小 openvela 应用骨架。
5. 设备端 stub 启动时能打印 `VelaBridge boot`、`mode=NORMAL`、`waiting for JSON messages`。
6. README 能让评委快速理解项目范围、运行 mock、运行测试和查看日志。
7. `docs/demo_script.md` 能支撑 5 分钟演示视频拍摄。
8. `logs/ai_coding_log.md` 能作为后续 AI Coding 记录模板。

## 第一周不要做什么

- 不做复杂外壳和量产结构设计。
- 不做复杂室内导航、地图定位或避障。
- 不做完整离线大模型。
- 不接真实 OpenAI API，不提交任何 API Key。
- 不默认保存原始音频、图片、定位和联系人信息。
- 不写未经开发板验证的屏幕、按键、震动、音频驱动调用。
- 不把项目扩展成万能导盲机器人。
