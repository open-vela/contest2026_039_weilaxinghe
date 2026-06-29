# 黄山派 SF32LB52 构建问题官方提问稿

各位老师 / openvela 官方同学好：

我们是 2026 首届 openvela AI 硬件开发者大赛 039 队伍，队伍名“维拉星核”。我们当前项目是 VelaBridge Watch，方向是基于 openvela 的多模态无障碍 AI 智能手表，使用开发板为黄山派 SF32LB52。

目前我们已经完成以下准备：

- openvela workspace `repo sync` 已成功。
- 团队仓库已切回 `dev-ai-contest-2026` 分支。
- VelaBridge app 已经能被构建系统识别，构建日志中多次出现：

```text
Register: velabridge_app
```

当前使用的 board config 是：

```text
vendor/sifli/boards/sf32lb52/lckfb_huangshan_pi/configs/nsh
```

我们也尝试过对照：

```text
vendor/sifli/boards/sf32lb52/sf32lb52_devkit_lcd/configs/nsh
```

但在底层编译过程中连续遇到 `ipc_queue/Make.defs` 和一系列 SiFli HAL / CMSIS 头文件缺失问题。当前判断问题不在 VelaBridge app 业务代码，而更像是 SF32LB52 / Huangshanpai board config 的依赖链路或 include 路径尚未确认。

## 错误摘要

```text
chip/Make.defs:76: ipc_queue/Make.defs: No such file or directory
cc1: error: '-mfloat-abi=hard': selected architecture lacks an FPU
fatal error: bf0_hal.h: No such file or directory
fatal error: register.h: No such file or directory
fatal error: core_cm33.h: No such file or directory
fatal error: cmsis_version.h: No such file or directory
fatal error: mem_map.h: No such file or directory
fatal error: drv_io.h: No such file or directory
fatal error: sf32lb_spi.h: No such file or directory
fatal error: sf32lb_rtc.h: No such file or directory
fatal error: sf32lb_timer.h: No such file or directory
fatal error: system_bf0_ap.h: No such file or directory
fatal error: dma_config.h: No such file or directory
fatal error: tim_config.h: No such file or directory
fatal error: bt_mac.h: No such file or directory
```

我们想请教几个问题：

1. 黄山派 SF32LB52 推荐的官方 build config 是哪个？
2. 是否应该使用 `vendor/sifli/boards/sf32lb52/lckfb_huangshan_pi/configs/nsh`？
3. 是否需要额外同步 SiFli HAL / CMSIS 依赖？
4. 是否有官方推荐的完整编译命令？
5. 这类 `ipc_queue/Make.defs` 和 HAL / CMSIS 头文件缺失，是否应避免手动软链接 workaround？

另外，我们曾经尝试过批量软链接 workaround，但污染过 `chip.h`，导致：

```text
chip.h: Too many levels of symbolic links
```

后来通过清理 `nuttx` 和 `vendor/sifli` 后恢复。我们目前已经停止批量软链接，避免把临时 workaround 当成正式修复。希望官方能确认推荐 config 和依赖同步方式，谢谢！
