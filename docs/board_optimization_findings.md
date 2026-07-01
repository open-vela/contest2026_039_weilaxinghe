# SF32LB52 Board Optimization Findings

## 背景

当前 VelaBridge Watch 已在黄山派 SF32LB52 DevKit ULP 上运行 openvela / NuttX + LVGL + LCD + Touchscreen。LCD、触摸、Home、App Wheel Lite 和 AI Bridge 页面可显示，但 MCU 侧 UI 对对象数量、字体渲染和串口日志较敏感。

本文件只记录优化发现，不修改 `nuttx/`、`vendor/sifli/`、`openvela.xml` 或 board defconfig。

## 已在 app 层落地的优化

- 默认进入轻量 Home Grid，不默认进入复杂 App Wheel。
- Home 卡片短标题短状态，长文本移到详情页。
- 页面启动时创建对象，运行中只 hidden/show。
- 串口日志减少到 app start、display ready、screen changed、AI command 和字体缺字一次性提示。
- 中文 label 统一走字体安全层，缺字时显示 ASCII fallback。
- Home 卡片图标从多对象图形降为单个简单圆点。
- 不再使用复杂动画、blur、粒子、大阴影或大面积透明叠层。
- 功能页统一导航栏，减少页面主体误触跳转。

## 未在本次修改的 board / defconfig 项

以下内容可能影响后续流畅度，但需要 board config 或驱动层确认，本次没有修改：

- PSRAM 是否可用于更大的 LVGL draw buffer。
- LVGL draw buffer 大小和 flush 策略是否可调。
- LCD flush 是否使用 DMA / GPU / 2D 加速。
- 是否启用或配置 SiFli GPU / EPIC / V2D 相关路径。
- 触摸输入采样、去抖和 LVGL indev 队列参数。
- LVGL 全局字体配置是否应直接启用 `CONFIG_LV_FONT_SIMSUN_16_CJK`。
- 编译优化等级是否适合 UI demo。

## 当前可能瓶颈

- 中文字体渲染成本：CJK glyph 比 ASCII 更重，小卡片同时显示大量中文会卡。
- LCD flush 带宽：如果每次触摸导致大面积重绘，帧率会下降。
- 对象数量：App Wheel 和复杂图标会增加布局和绘制成本。
- 串口打印：高频 `printf` 会阻塞交互，因此默认关闭详细日志。
- 根 screen reload：已减少重复 `lv_scr_load()`，Home 内部切换优先 hidden/show。

## 后续建议

1. 先用当前 Performance Safe Mode 做比赛展示。
2. 用真机观察点击后 100-200ms 内是否能完成可见反馈。
3. 若仍卡顿，再单独评估 LVGL draw buffer、PSRAM、DMA/GPU flush 和触摸采样参数。
4. 底层配置变更应单独成 PR，并附带 build log、真机日志和回退说明。
