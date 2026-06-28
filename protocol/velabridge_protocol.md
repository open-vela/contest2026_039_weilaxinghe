# VelaBridge 串口 JSON 协议

队伍名称：维拉星核。项目名称：VelaBridge。比赛编号：039。

维拉星核团队开发的 VelaBridge，是一款基于 openvela 的多模态无障碍 AI 沟通与伴行终端。

VelaBridge 第一阶段 MVP 使用 JSON Lines 作为设备端与电脑端 AI Bridge 的通信格式：每一行是一个完整 JSON 对象，以 `\n` 结尾。这样便于串口、日志、命令行 mock 和 unittest 复用。

## 通信方向

- `bridge_to_device`：电脑端 AI Bridge 发给 openvela 设备端，用于字幕显示、语音播报、震动反馈、模式切换和演示步骤。
- `device_to_bridge`：openvela 设备端发给电脑端 AI Bridge，用于按键事件、模式状态、设备日志、错误码和求助确认。

## bridge_to_device 消息格式

```json
{
  "type": "caption",
  "id": "vb-20260628-0001",
  "timestamp": "2026-06-28T12:00:00.000Z",
  "team": "维拉星核",
  "project": "VelaBridge",
  "contest_id": "039",
  "text": "请先到教务处服务窗口。",
  "display": "第1步：教务处服务窗口",
  "speak": "",
  "vibration": "short",
  "mode": "CAPTION_MODE",
  "priority": "normal",
  "privacy": "text_summary_only",
  "source": "mock_bridge"
}
```

## device_to_bridge 消息格式

```json
{
  "type": "button_event",
  "id": "dev-20260628-0001",
  "timestamp": "2026-06-28T12:00:01.000Z",
  "team": "维拉星核",
  "project": "VelaBridge",
  "contest_id": "039",
  "text": "button A short press",
  "display": "",
  "speak": "",
  "vibration": "none",
  "mode": "NORMAL",
  "priority": "normal",
  "privacy": "event_only",
  "source": "velabridge_device",
  "button": "A",
  "event": "short_press"
}
```

## 字段定义

| 字段 | 类型 | 必填 | 说明 |
| --- | --- | --- | --- |
| `type` | string | 是 | 消息类型，如 `caption`、`quick_reply`、`help`、`button_event`、`error`。 |
| `id` | string | 是 | 消息唯一 ID，建议带来源前缀，便于日志追踪。 |
| `timestamp` | string | 是 | UTC ISO-8601 时间戳，示例 `2026-06-28T12:00:00.000Z`。 |
| `team` | string | 建议 | 队伍名称，固定为 `维拉星核`。 |
| `project` | string | 建议 | 项目名称，固定为 `VelaBridge`。 |
| `contest_id` | string | 建议 | 比赛编号，固定为 `039`。 |
| `text` | string | 是 | 主要文字内容。默认只保存脱敏文字摘要。 |
| `display` | string | 否 | 屏幕展示内容，通常短于 `text`。 |
| `speak` | string | 否 | 需要设备播报的文本。为空表示不播报。 |
| `vibration` | enum | 是 | 震动模式，见震动枚举。 |
| `mode` | enum | 是 | 设备状态机模式，见模式枚举。 |
| `priority` | string | 是 | `low`、`normal`、`high`，危险和求助建议为 `high`。 |
| `privacy` | string | 是 | 隐私策略标签，如 `text_summary_only`、`event_only`。 |
| `source` | string | 是 | 消息来源，如 `mock_bridge`、`velabridge_device`、`serial_test`。 |

## 消息类型

`bridge_to_device` 第一阶段支持：

- `caption`：字幕显示。
- `quick_reply`：快捷回复，通常需要 `speak`。
- `help`：一键求助摘要和提示。
- `blind_menu`：盲人模式语音菜单。
- `ocr_result`：OCR 读字结果。
- `danger_alert`：危险关键词提醒。
- `demo_step`：比赛演示步骤。

`device_to_bridge` 第一阶段支持：

- `button_event`：按键事件。
- `mode_changed`：状态机切换。
- `device_log`：设备日志。
- `ack`：设备收到并处理消息。
- `error`：错误码上报。

## 震动枚举

| 值 | 含义 |
| --- | --- |
| `none` | 不震动。 |
| `short` | 短震一次，普通提示。 |
| `long` | 长震一次，求助或确认。 |
| `urgent` | 急促震动，危险提醒。 |
| `double` | 双短震，菜单切换或 OCR 结果。 |

## 模式枚举

| 值 | 含义 |
| --- | --- |
| `NORMAL` | 默认待机模式。 |
| `CAPTION_MODE` | 字幕模式。 |
| `BLIND_MODE` | 盲人模式 / 语音菜单。 |
| `OCR_MODE` | OCR 读字模式。 |
| `HELP_MODE` | 一键求助模式。 |
| `DEMO_MODE` | 比赛演示模式。 |

## 错误码

| 错误码 | 说明 |
| --- | --- |
| `INVALID_JSON` | 收到的串口行不是合法 JSON。 |
| `UNSUPPORTED_TYPE` | `type` 不在当前固件支持范围内。 |
| `TEXT_TOO_LONG` | `text` 或 `display` 超过设备显示 / 缓冲区限制。 |
| `DEVICE_BUSY` | 设备正在播报、震动或处理高优先级事件。 |
| `AI_TIMEOUT` | AI Bridge 等待 ASR/OCR/LLM mock 超时。 |

## 完整 JSON 示例

### 1. 普通字幕

```json
{"type":"caption","id":"vb-0001","timestamp":"2026-06-28T12:00:00.000Z","text":"请问食堂怎么走？","display":"字幕：请问食堂怎么走？","speak":"","vibration":"none","mode":"CAPTION_MODE","priority":"normal","privacy":"text_summary_only","source":"mock_bridge"}
```

### 2. 教务处分步骤字幕

```json
{"type":"caption","id":"vb-0002","timestamp":"2026-06-28T12:00:01.000Z","text":"第1步：先到教务处服务窗口。","display":"第1步：先到教务处服务窗口。","speak":"","vibration":"short","mode":"CAPTION_MODE","priority":"normal","privacy":"text_summary_only","source":"mock_bridge"}
```

### 3. 快捷回复

```json
{"type":"quick_reply","id":"vb-0003","timestamp":"2026-06-28T12:00:02.000Z","text":"请您说慢一点，我正在看字幕。","display":"快捷回复：请您说慢一点。","speak":"请您说慢一点，我正在看字幕。","vibration":"short","mode":"CAPTION_MODE","priority":"normal","privacy":"text_summary_only","source":"mock_bridge"}
```

### 4. 一键求助

```json
{"type":"help","id":"vb-0004","timestamp":"2026-06-28T12:00:03.000Z","text":"用户请求帮助：请联系老师或现场工作人员。","display":"已进入求助模式，正在生成求助摘要。","speak":"已触发求助，请联系老师或工作人员。","vibration":"long","mode":"HELP_MODE","priority":"high","privacy":"text_summary_only","source":"mock_bridge"}
```

### 5. 盲人模式菜单

```json
{"type":"blind_menu","id":"vb-0005","timestamp":"2026-06-28T12:00:04.000Z","text":"盲人模式菜单：短按播放下一项，长按确认，双击退出。","display":"盲人模式：语音菜单已开启。","speak":"盲人模式菜单。短按下一项，长按确认，双击退出。","vibration":"double","mode":"BLIND_MODE","priority":"normal","privacy":"text_summary_only","source":"mock_bridge"}
```

### 6. OCR 读字结果

```json
{"type":"ocr_result","id":"vb-0006","timestamp":"2026-06-28T12:00:05.000Z","text":"识别结果：教务处服务窗口，请排队办理。","display":"OCR：教务处服务窗口，请排队办理。","speak":"识别到教务处服务窗口，请排队办理。","vibration":"double","mode":"OCR_MODE","priority":"normal","privacy":"text_summary_only","source":"mock_bridge"}
```

### 7. 危险关键词提醒

```json
{"type":"danger_alert","id":"vb-0007","timestamp":"2026-06-28T12:00:06.000Z","text":"检测到紧急或危险关键词，请注意周围环境。","display":"危险提醒：请暂停并确认周围环境。","speak":"检测到危险提醒，请注意安全。","vibration":"urgent","mode":"CAPTION_MODE","priority":"high","privacy":"text_summary_only","source":"mock_bridge"}
```

### 8. 演示步骤

```json
{"type":"demo_step","id":"vb-0008","timestamp":"2026-06-28T12:00:07.000Z","text":"演示开始：VelaBridge 正在等待 AI Bridge 字幕。","display":"Demo 1/7：启动并等待字幕","speak":"演示开始。","vibration":"short","mode":"DEMO_MODE","priority":"normal","privacy":"text_summary_only","source":"mock_bridge"}
```

### 9. 设备按键事件

```json
{"type":"button_event","id":"dev-0001","timestamp":"2026-06-28T12:00:08.000Z","text":"button A long press","display":"","speak":"","vibration":"none","mode":"NORMAL","priority":"normal","privacy":"event_only","source":"velabridge_device","button":"A","event":"long_press"}
```

### 10. 设备错误上报

```json
{"type":"error","id":"dev-0002","timestamp":"2026-06-28T12:00:09.000Z","text":"INVALID_JSON","display":"协议错误：INVALID_JSON","speak":"","vibration":"double","mode":"NORMAL","priority":"high","privacy":"event_only","source":"velabridge_device","code":"INVALID_JSON","detail":"serial line is not valid JSON"}
```

## 隐私策略

- 默认不保存原始音频和原始图片。
- 默认只保存脱敏后的文字摘要、模式变化、按键事件、错误码和演示日志。
- 求助场景只记录用户主动触发后的摘要，不自动上传联系人、定位或身份信息。
- 后续如需保存音频、图片、定位或联系人信息，必须增加明确授权流程和可关闭选项。
