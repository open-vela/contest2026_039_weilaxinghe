import unittest

from ai_bridge import mock_bridge


class MockBridgeTest(unittest.TestCase):
    def test_normal_caption(self):
        messages = mock_bridge.build_messages("你好，请问食堂怎么走")

        self.assertEqual(len(messages), 1)
        self.assertEqual(messages[0]["type"], "caption")
        self.assertIn("食堂", messages[0]["text"])
        self.assertEqual(messages[0]["mode"], "CAPTION_MODE")

    def test_affairs_sentence_is_simplified_into_steps(self):
        messages = mock_bridge.build_messages("我要去教务处盖章，请问流程是什么")

        self.assertGreaterEqual(len(messages), 3)
        self.assertTrue(all(message["type"] == "caption" for message in messages))
        self.assertIn("第1步", messages[0]["display"])
        self.assertIn("盖章", messages[-1]["display"])

    def test_danger_keywords_trigger_urgent_vibration(self):
        messages = mock_bridge.build_messages("小心，前面危险，请让开")

        self.assertEqual(len(messages), 1)
        self.assertEqual(messages[0]["type"], "danger_alert")
        self.assertEqual(messages[0]["vibration"], "urgent")
        self.assertEqual(messages[0]["priority"], "high")

    def test_help_triggers_help_message(self):
        messages = mock_bridge.build_messages("我要求助，帮我联系老师")

        self.assertEqual(len(messages), 1)
        self.assertEqual(messages[0]["type"], "help")
        self.assertEqual(messages[0]["mode"], "HELP_MODE")

    def test_blind_menu(self):
        messages = mock_bridge.build_messages("打开盲人模式菜单")

        self.assertEqual(len(messages), 1)
        self.assertEqual(messages[0]["type"], "blind_menu")
        self.assertEqual(messages[0]["mode"], "BLIND_MODE")
        self.assertIn("短按", messages[0]["speak"])

    def test_demo_outputs_at_least_five_messages(self):
        messages = mock_bridge.demo_messages()

        self.assertGreaterEqual(len(messages), 5)
        self.assertLessEqual(len(messages), 7)
        self.assertIn("demo_step", {message["type"] for message in messages})
        self.assertIn("ocr_result", {message["type"] for message in messages})


if __name__ == "__main__":
    unittest.main()
