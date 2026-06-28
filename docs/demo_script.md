# VelaBridge 5 分钟演示视频脚本

## 0:00-0:30 痛点

画面：校园服务窗口、食堂点餐、走廊门牌、嘈杂问路场景切换。

旁白：听障同学在窗口服务和校园问路时常常看不到完整语音信息；视障同学读门牌、找窗口时需要更明确的语音和触觉反馈；语言表达困难者也需要低门槛的快捷回复。VelaBridge 先做一个可落地 MVP：把外界文字和语音 mock 转成字幕、播报、震动和求助提示。

## 0:30-1:00 硬件

画面：展示黄山派开发板、屏幕、按键、震动模块或占位提示、电脑端串口窗口。

旁白：第一阶段不做复杂外壳和昂贵硬件，只验证 openvela 端应用、按键状态机、震动反馈、屏幕显示和电脑端 AI Bridge 的串口 JSON 通信。

## 1:00-1:50 听障字幕 + AI 简化

画面：电脑运行：

```bash
python3 ai_bridge/mock_bridge.py "我要去教务处盖章，请问流程是什么" --jsonl
```

设备端显示分步骤字幕：

```text
第1步：先到教务处服务窗口。
第2步：准备学生证和申请表。
第3步：说明需要盖章，等待老师确认。
```

旁白：VelaBridge 把长句简化成短字幕，优先照顾小屏幕可读性和现场沟通效率。

## 1:50-2:40 快捷回复

画面：输入或按键触发：

```bash
python3 ai_bridge/mock_bridge.py "请说慢一点" --jsonl
```

设备端显示“快捷回复：请您说慢一点。”，预留语音播报字段。

旁白：当用户听不清或来不及表达时，可以用固定快捷回复降低沟通压力。后续接入真实 TTS 后，设备可以直接播报给对方。

## 2:40-3:30 盲人模式 + OCR 读字 mock

画面：输入：

```bash
python3 ai_bridge/mock_bridge.py "打开盲人模式菜单" --jsonl
python3 ai_bridge/mock_bridge.py "读字 门牌" --jsonl
```

设备端进入 `BLIND_MODE`，震动双短提示；随后展示 OCR 结果并预留播报。

旁白：盲人模式先验证语音菜单和触觉反馈，不承诺替代导盲杖。OCR 读字也先做 mock，后续再接真实摄像头和 OCR。

## 3:30-4:10 一键求助

画面：长按按键或输入：

```bash
python3 ai_bridge/mock_bridge.py "求助，帮我联系老师" --jsonl
```

设备端进入 `HELP_MODE`，长震，屏幕显示求助摘要。

旁白：一键求助只生成用户主动触发后的文字摘要，默认不保存原始音频和图片，也不自动上传敏感信息。

## 4:10-4:45 openvela 日志、AI Bridge、协议、Codex 开发记录

画面：展示 `app/velabridge_app/`、`ai_bridge/mock_bridge.py`、`protocol/velabridge_protocol.md`、`logs/ai_coding_log.md`、测试命令。

旁白：设备端使用 openvela 应用骨架，电脑端 AI Bridge 输出 JSON Lines，协议文档定义消息字段、震动枚举、模式枚举和错误码。开发过程通过 AI Coding 日志记录 Prompt、输出摘要、人工审核、测试结果和风险 TODO。

## 4:45-5:00 总结

画面：固定演示命令：

```bash
python3 ai_bridge/mock_bridge.py --demo --jsonl
```

旁白：VelaBridge 第一阶段目标很明确：先把字幕、按键、震动、求助和日志闭环跑起来。下一步接入真实开发板串口、屏幕、震动和音频，再逐步替换 mock AI 能力。

