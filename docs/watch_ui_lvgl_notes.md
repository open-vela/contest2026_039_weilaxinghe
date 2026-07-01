# VelaBridge Watch LVGL UI 记录

## 当前状态

VelaBridge Watch 的 LVGL 手表 UI 已经在黄山派 SF32LB52 DevKit ULP 上进入真机验证阶段。

- LCD backend 已通过 `/dev/lcd0` 初始化。
- Touch backend 已通过 `/dev/input0` 初始化。
- `velabridge_app` 已保留串口 fallback；如果显示初始化失败，仍会回到 ASCII 串口 demo。
- 当前页面包括 Boot、表盘、应用轮盘、心率、运动、睡眠和设置。
- 当前 UI loop 持续调用 `lv_timer_handler()`，应用不会在显示 ready 后立即退出。

## 简化版应用轮盘

本阶段新增的是 LVGL 简化版应用轮盘，不实现复杂拖拽、惯性滚动或表冠物理滚动。

轮盘包含 12 个中文应用入口：

- 心率
- 睡眠
- 运动
- 天气
- 通知
- 音乐
- 闹钟
- 设置
- 支付
- 地图
- 呼吸
- 电话

当前交互：

- 点击应用轮盘页面任意位置，切换到下一个焦点应用。
- 串口输出当前焦点，例如 `[velabridge][watch_ui] wheel focus=心率`。
- 自动 demo timer 会在应用轮盘页停留并切换几次焦点，然后继续进入后续页面。

## 中文显示说明

队伍 app 的 Kconfig 会选择 `LV_FONT_SIMSUN_16_CJK`，用于显示简体中文界面文案。若当前工作区复用了旧 `.config`，Makefile 会把 LVGL 自带的 `lv_font_simsun_16_cjk.c` 作为 app 源文件兜底编译，避免中文标签变成方块字。

若真机仍出现方块字或缺字，请先确认 `nuttx/.config` 中存在：

```text
CONFIG_LV_FONT_SIMSUN_16_CJK=y
```

用户可见 UI 文案已尽量中文化；技术日志仍保留英文 screen 名称，便于串口调试和后续定位。

## 暂不实现

- 不做复杂 blur、粒子、3D 或大面积透明叠层。
- 不做拖拽惯性、真实表冠滚动和复杂物理动画。
- 不频繁删除和重建 screen，优先使用静态对象和稳定更新。

## 后续计划

- 加入 AI 无障碍核心页面，例如字幕、OCR、危险提醒和快捷回复。
- 完善图标样式，逐步接近 Figma 中的精修版轮盘。
- 增加轻量动效，但以真机稳定性为优先。
- 优化页面过渡和触摸反馈。
