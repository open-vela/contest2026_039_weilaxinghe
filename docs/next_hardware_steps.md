# 下一阶段硬件任务

1. 继续保存后续 SSCOM 完整串口日志，已归档的屏幕 / 触摸日志见 `docs/assets/bringup/`。
2. 测试 KEY1 短按 / 长按日志。
3. 测试 KEY2 自动检测完整流程。
4. 不再卡出厂音频测试，喇叭后续用单独 demo 或 VelaBridge 固件验证。
5. 麦克风后续用单独 demo 或 VelaBridge 固件验证。
6. 音频 / 喇叭 / 麦克风当前只能标记为“未触发测试 / 待复测”，不能写失败或通过。
7. 保存所有新增照片、截图和串口日志到 `docs/assets/bringup/`。
8. 确认是否有板载震动马达或马达接口。
9. 准备后续震动马达接线与 PWM/GPIO 控制方案。
10. 拉取完整 openvela workspace。
11. 编译官方 hello_app。
12. 确认 `app/velabridge_app` 的构建方式。
13. 编译 VelaBridge 最小设备端程序。
14. 实现最小闭环：PC AI Bridge 生成 JSON -> 串口发送 -> 黄山派接收 -> 屏幕显示文字 -> 按键切模式 -> 串口输出日志。
15. 所有测试结果继续写入 `docs/bringup_log.md` 和 `logs/ai_coding_log.md`。
