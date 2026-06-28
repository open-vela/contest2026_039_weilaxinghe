#!/usr/bin/env python3
"""PC-side mock AI Bridge for the VelaBridge MVP.

The script intentionally uses only the Python standard library. It turns
Chinese demo text into JSON/JSONL messages that can later be sent over a
serial JSON Lines connection to an openvela device.
"""

from __future__ import annotations

import argparse
import json
import sys
import uuid
from datetime import datetime, timezone
from typing import Iterable, List, Optional, Sequence


DANGER_KEYWORDS = ("危险", "让开", "报警", "小心")
HELP_KEYWORDS = ("求助", "帮我联系老师")
SLOW_REPLY_KEYWORDS = ("请说慢一点",)
AFFAIRS_KEYWORDS = ("教务处", "盖章")
BLIND_MENU_KEYWORDS = ("盲人模式", "盲人菜单", "语音菜单")
OCR_KEYWORDS = ("OCR", "ocr", "读字", "门牌", "识别文字")


def _utc_now() -> str:
    return datetime.now(timezone.utc).isoformat(timespec="milliseconds").replace("+00:00", "Z")


def make_message(
    message_type: str,
    text: str,
    *,
    display: Optional[str] = None,
    speak: Optional[str] = None,
    vibration: str = "none",
    mode: str = "NORMAL",
    priority: str = "normal",
    privacy: str = "text_summary_only",
    source: str = "mock_bridge",
    timestamp: Optional[str] = None,
    message_id: Optional[str] = None,
) -> dict:
    """Build a bridge_to_device message following the VelaBridge protocol."""

    return {
        "type": message_type,
        "id": message_id or f"vb-{uuid.uuid4().hex[:12]}",
        "timestamp": timestamp or _utc_now(),
        "text": text,
        "display": display if display is not None else text,
        "speak": speak if speak is not None else "",
        "vibration": vibration,
        "mode": mode,
        "priority": priority,
        "privacy": privacy,
        "source": source,
    }


def _contains_any(text: str, keywords: Iterable[str]) -> bool:
    return any(keyword in text for keyword in keywords)


def build_messages(text: str) -> List[dict]:
    """Convert one user input string into one or more protocol messages."""

    normalized = text.strip()
    if not normalized:
        normalized = "请等待，我正在准备字幕。"

    if _contains_any(normalized, DANGER_KEYWORDS):
        return [
            make_message(
                "danger_alert",
                "检测到紧急或危险关键词，请注意周围环境。",
                display="危险提醒：请暂停并确认周围环境。",
                speak="检测到危险提醒，请注意安全。",
                vibration="urgent",
                mode="CAPTION_MODE",
                priority="high",
            )
        ]

    if _contains_any(normalized, HELP_KEYWORDS):
        return [
            make_message(
                "help",
                "用户请求帮助：请联系老师或现场工作人员。",
                display="已进入求助模式，正在生成求助摘要。",
                speak="已触发求助，请联系老师或工作人员。",
                vibration="long",
                mode="HELP_MODE",
                priority="high",
            )
        ]

    if _contains_any(normalized, SLOW_REPLY_KEYWORDS):
        return [
            make_message(
                "quick_reply",
                "请您说慢一点，我正在看字幕。",
                display="快捷回复：请您说慢一点。",
                speak="请您说慢一点，我正在看字幕。",
                vibration="short",
                mode="CAPTION_MODE",
                priority="normal",
            )
        ]

    if _contains_any(normalized, BLIND_MENU_KEYWORDS):
        return [blind_menu_message()]

    if _contains_any(normalized, OCR_KEYWORDS):
        return [
            make_message(
                "ocr_result",
                "识别结果：教务处服务窗口，请排队办理。",
                display="OCR：教务处服务窗口，请排队办理。",
                speak="识别到教务处服务窗口，请排队办理。",
                vibration="double",
                mode="OCR_MODE",
                priority="normal",
            )
        ]

    if _contains_any(normalized, AFFAIRS_KEYWORDS):
        steps = [
            "第1步：先到教务处服务窗口。",
            "第2步：准备学生证和申请表。",
            "第3步：说明需要盖章，等待老师确认。",
        ]
        return [
            make_message(
                "caption",
                step,
                display=step,
                speak="",
                vibration="short" if index == 0 else "none",
                mode="CAPTION_MODE",
                priority="normal",
            )
            for index, step in enumerate(steps)
        ]

    return [
        make_message(
            "caption",
            normalized,
            display=f"字幕：{normalized}",
            speak="",
            vibration="none",
            mode="CAPTION_MODE",
            priority="normal",
        )
    ]


def blind_menu_message() -> dict:
    return make_message(
        "blind_menu",
        "盲人模式菜单：短按播放下一项，长按确认，双击退出。",
        display="盲人模式：语音菜单已开启。",
        speak="盲人模式菜单。短按下一项，长按确认，双击退出。",
        vibration="double",
        mode="BLIND_MODE",
        priority="normal",
    )


def demo_messages() -> List[dict]:
    """Return a fixed 7-message demo sequence for serial or JSONL playback."""

    return [
        make_message(
            "demo_step",
            "演示开始：VelaBridge 正在等待 AI Bridge 字幕。",
            display="Demo 1/7：启动并等待字幕",
            speak="演示开始。",
            vibration="short",
            mode="DEMO_MODE",
        ),
        make_message(
            "caption",
            "老师说：请先到教务处服务窗口，再去二楼盖章。",
            display="字幕：请先到教务处服务窗口，再去二楼盖章。",
            mode="CAPTION_MODE",
        ),
        make_message(
            "caption",
            "AI 简化：先去教务处窗口，再到二楼盖章。",
            display="简化：1 教务处窗口；2 二楼盖章。",
            vibration="short",
            mode="CAPTION_MODE",
        ),
        make_message(
            "quick_reply",
            "请您说慢一点，我正在看字幕。",
            display="快捷回复：请您说慢一点。",
            speak="请您说慢一点，我正在看字幕。",
            vibration="short",
            mode="CAPTION_MODE",
        ),
        blind_menu_message(),
        make_message(
            "ocr_result",
            "OCR 识别：食堂一号窗口，今日套餐 15 元。",
            display="OCR：食堂一号窗口，今日套餐 15 元。",
            speak="识别到食堂一号窗口，今日套餐十五元。",
            vibration="double",
            mode="OCR_MODE",
        ),
        make_message(
            "danger_alert",
            "检测到危险关键词：小心。",
            display="危险提醒：有人说小心，请停下确认。",
            speak="危险提醒，请停下确认。",
            vibration="urgent",
            mode="CAPTION_MODE",
            priority="high",
        ),
    ]


def format_messages(messages: Sequence[dict], *, jsonl: bool) -> str:
    if jsonl:
        return "\n".join(json.dumps(message, ensure_ascii=False, separators=(",", ":")) for message in messages)

    if len(messages) == 1:
        return json.dumps(messages[0], ensure_ascii=False, indent=2)
    return json.dumps(list(messages), ensure_ascii=False, indent=2)


def send_serial_jsonl(messages: Sequence[dict], port: str, baudrate: int = 115200) -> bool:
    """Optional serial sender.

    pyserial is deliberately optional for the MVP. When it is not installed,
    the function returns False instead of failing the mock bridge.
    """

    try:
        import serial  # type: ignore
    except ImportError:
        print("TODO: pyserial is not installed; skipped serial sending.", file=sys.stderr)
        return False

    with serial.Serial(port=port, baudrate=baudrate, timeout=1) as serial_port:
        for line in format_messages(messages, jsonl=True).splitlines():
            serial_port.write((line + "\n").encode("utf-8"))
    return True


def parse_args(argv: Sequence[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="VelaBridge PC-side AI Bridge mock")
    parser.add_argument("text", nargs="*", help="Chinese text to convert into VelaBridge JSON")
    parser.add_argument("--demo", action="store_true", help="output a fixed 5-7 step demo sequence")
    parser.add_argument("--jsonl", action="store_true", help="output one JSON object per line")
    parser.add_argument("--serial-port", help="optional serial port path, e.g. COM3 or /dev/ttyUSB0")
    parser.add_argument("--baudrate", type=int, default=115200, help="serial baudrate when --serial-port is used")
    return parser.parse_args(argv)


def main(argv: Optional[Sequence[str]] = None) -> int:
    args = parse_args(argv or sys.argv[1:])

    if args.demo:
        messages = demo_messages()
    else:
        text = " ".join(args.text).strip()
        if not text and not sys.stdin.isatty():
            text = sys.stdin.read().strip()
        if not text:
            text = input("请输入要发送给 VelaBridge 的中文文本：").strip()
        messages = build_messages(text)

    print(format_messages(messages, jsonl=args.jsonl))

    if args.serial_port:
        send_serial_jsonl(messages, args.serial_port, args.baudrate)

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
