# 下一阶段硬件任务

1. 保存 SSCOM 完整串口日志。
2. 上传 `screen_green_factory_test.jpg`。
3. 上传 `sscom_touch_log_COM5.png`。
4. 测试 KEY1 短按日志。
5. 测试 KEY2 自动检测完整流程。
6. 接入或确认 SPK 喇叭，测试提示音 / 录音回放。
7. 测试麦克风。
8. 确认是否有板载震动马达或马达接口。
9. 准备后续震动马达接线与 PWM/GPIO 控制方案。
10. 拉取完整 openvela workspace。
11. 编译官方 hello_app。
12. 确认 `app/velabridge_app` 的构建方式。
13. 编译 VelaBridge 最小设备端程序。
14. 实现最小闭环：PC AI Bridge 生成 JSON -> 串口发送 -> 黄山派接收 -> 屏幕显示文字 -> 按键切模式 -> 串口输出日志。
15. 所有测试结果继续写入 `docs/bringup_log.md` 和 `logs/ai_coding_log.md`。

