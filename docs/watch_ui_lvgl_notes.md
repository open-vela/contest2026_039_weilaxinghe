# VelaBridge Watch LVGL UI 记录

## 当前真机 UI 架构

VelaBridge Watch 当前在黄山派 SF32LB52 DevKit ULP 上使用 openvela / NuttX + LVGL + NuttX LCD backend 运行。

- `velabridge_app` 从 NuttShell 启动。
- LCD 使用 `/dev/lcd0` 初始化。
- Touch 使用 `/dev/input0` 初始化。
- 若显示初始化失败，仍保留 ASCII 串口 demo fallback。
- 页面容器在启动时创建，运行中以 hidden/show 切换，避免点击时频繁创建或删除 LVGL 对象。

## Home Grid

默认首页改为高性能 2x3 Home Grid，而不是复杂应用轮盘。

Home 卡片只显示短标题和短状态：

- 字幕 / Ready
- OCR / Scan
- 危险 / Alert
- 回复 / Reply
- 盲人 / Safe
- Bridge / Agent

长中文内容只放在详情页中，避免小卡片文字重叠。每个 label 均设置固定宽度，并使用 `LV_LABEL_LONG_CLIP` 或 wrap 模式。

## App Wheel Lite

App Wheel Lite 保留为备用页，不作为默认展示重点。

- 入口：Home 右上角“应用”按钮，或串口命令 `VB_OPEN wheel`。
- 不做自动轮播。
- 不做复杂动画、粒子、blur 或大面积阴影。
- 焦点变化只更新已有对象的位置、大小、边框和可见性。

## 统一导航栏

功能页新增统一导航栏：

- 左侧：返回
- 中间：当前页面标题
- 右侧：首页
- 右上角：电量 `86%`

页面切换统一通过业务状态机：

- `VB_SCREEN_HOME`
- `VB_SCREEN_WHEEL`
- `VB_SCREEN_CAPTION`
- `VB_SCREEN_OCR`
- `VB_SCREEN_DANGER`
- `VB_SCREEN_REPLY`
- `VB_SCREEN_BLIND`
- `VB_SCREEN_SCENE`
- `VB_SCREEN_BRIDGE`
- `VB_SCREEN_PROOF`
- `VB_SCREEN_SETTINGS`

点击返回回到上一个业务页面，点击首页直接回到 Home。页面主体默认不再乱跳，只有明确按钮和卡片触发切换。

## 中文字体策略

app 层统一使用 `lv_font_simsun_16_cjk` 作为中文显示字体。当前 `.config` 未必启用 `CONFIG_LV_FONT_SIMSUN_16_CJK`，因此 `app/velabridge_app/Makefile` 会在未启用全局字体时把 LVGL 自带 `lv_font_simsun_16_cjk.c` 作为 app 源文件编译进来。

新增字体安全层：

- `vb_ui_get_cn_font()`
- `vb_create_label(...)`
- `vb_set_label_text_safe(...)`
- `vb_font_has_text(...)`

启动时只打印一次：

```text
[velabridge][font] cjk font ready
```

如果某个中文字符串缺字，只打印一次：

```text
[velabridge][font] missing glyph in text id=caption_text
```

缺字时屏幕改用 ASCII fallback，例如 `Tap Home`、`Agent Ready`、`Caption ready`，避免显示方块。

## AI Bridge 命令

当前支持以下串口命令：

```text
VB_OPEN home
VB_OPEN caption
VB_OPEN ocr
VB_OPEN danger
VB_OPEN reply
VB_OPEN blind
VB_OPEN scene
VB_OPEN bridge
VB_OPEN proof
VB_OPEN wheel
VB_CAPTION <text>
VB_OCR <text>
VB_ALERT <text>
VB_REPLY <text>
VB_SCENE <text>
VB_RISK low/medium/high
VB_ADVICE <text>
```

串口日志保持英文 ASCII，只打印命令类型和 screen changed，不输出中文 payload。

## 性能优化策略

本次只做 MCU 友好的 Performance Safe Mode：

- 默认进入 Home Grid。
- 初始化时创建页面容器，点击时 hidden/show。
- 禁止 auto demo。
- 禁止复杂 `lv_anim`。
- 禁止 blur、粒子、3D 和复杂发光。
- Home 图标降级为单个简单圆点。
- 减少小卡片中文 label，避免中文字体频繁重绘。
- `lv_timer_handler()` 仍以短间隔循环运行，loop 中不打印日志。

## 不修改底层的原因

本次只修 app 层 UI。没有修改：

- `nuttx/`
- `vendor/sifli/`
- `openvela.xml`
- `nuttx.bin`

PSRAM、GPU、LVGL 全局字体配置、flush buffer 等属于 board / defconfig / 驱动层议题，需要后续单独验证，不能在比赛 app PR 中盲改。
