# 黄山派 SF32LB52 构建问题官方提问稿

各位老师 / openvela 官方同学好：

我们是 2026 首届 openvela AI 硬件开发者大赛 039 队伍，队伍名“维拉星核”。我们当前项目是 VelaBridge Watch，方向是基于 openvela 的多模态无障碍 AI 智能手表，使用开发板为黄山派 SF32LB52。

我们想确认黄山派 SF32LB52 在比赛 openvela workspace 中的官方推荐编译方式，避免继续用手动软链接补底层头文件。

## 已完成准备

- 已安装并初始化 Git LFS。
- openvela workspace `repo sync` 已成功。
- 团队仓库已切回 `dev-ai-contest-2026` 分支。
- VelaBridge app 已经能被构建系统识别，构建日志中多次出现：

```text
Register: velabridge_app
```

## 已尝试 board config

当前主要尝试：

```text
vendor/sifli/boards/sf32lb52/lckfb_huangshan_pi/configs/nsh
```

对照尝试：

```text
vendor/sifli/boards/sf32lb52/sf32lb52_devkit_lcd/configs/nsh
```

clean build 使用 `lckfb_huangshan_pi/configs/nsh` 时，连续遇到底层依赖缺失。对照 `sf32lb52_devkit_lcd/configs/nsh` 时，也遇到过 `ipc_queue/Make.defs` 缺失。当前判断问题不在 VelaBridge app 业务代码，而更像是 SF32LB52 / Huangshanpai board config 的依赖链路、include 路径或工具链配置尚未确认。

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
fatal error: cache.h: No such file or directory
fatal error: mpu_armv8.h: No such file or directory
fatal error: core_mstar.h: No such file or directory
```

## 本地搜索到的线索

- 本地存在 `vendor/sifli/chips/sf32lb52/ipc_queue/Make.defs`。
- 本地存在 `vendor/sifli/chips/drivers/Include/bf0_hal.h`、`vendor/sifli/chips/drivers/cmsis/sf32lb52x/register.h`、`vendor/sifli/chips/external/CMSIS/Include/core_cm33.h` 等 HAL / CMSIS 头文件。
- 本地存在三个 SF32LB52 defconfig：
  - `vendor/sifli/boards/sf32lb52/lckfb_huangshan_pi/configs/nsh/defconfig`
  - `vendor/sifli/boards/sf32lb52/sf32lb52_devkit_lcd/configs/nsh/defconfig`
  - `vendor/sifli/boards/sf32lb52/sf32lb52_lchspi_ulp/configs/nsh/defconfig`
- 因此当前更像是构建方式、include path、chip 映射或工具链配置问题，而不是简单缺少单个文件。

## 想请官方确认的问题

1. 黄山派 SF32LB52 在比赛 openvela workspace 中推荐使用哪个 board config？
2. 推荐的完整 build 命令是什么？是否必须使用 `--cmake`？
3. `lckfb_huangshan_pi/configs/nsh` 是否就是黄山派推荐 config？
4. `sf32lb52_devkit_lcd/configs/nsh` 和 `sf32lb52_lchspi_ulp/configs/nsh` 分别适用于什么硬件或场景？
5. 是否需要额外同步 SiFli HAL / CMSIS 依赖？
6. 是否需要额外 `repo init` / `repo sync` 参数，例如 `--git-lfs`、特定 manifest、特定分支或额外 SDK 包？
7. 比赛 SF32LB52 构建是否支持 WSL2 Ubuntu？如果不支持，推荐的原生 Ubuntu 版本和依赖清单是什么？
8. 是否需要使用指定的 ARM GCC 工具链版本或先执行某个 openvela / SiFli 环境脚本？
9. 遇到 `ipc_queue/Make.defs` 和 HAL / CMSIS 头文件缺失时，官方建议如何排查？
10. 是否明确不建议用手工软链接 workaround 修复这些底层路径？

## 已停止的临时做法

我们曾尝试过批量软链接 workaround，但污染过 `chip.h`，导致：

```text
chip.h: Too many levels of symbolic links
```

后来通过清理 `nuttx` 和 `vendor/sifli` 后恢复。目前已经停止批量软链接，避免把临时 workaround 当成正式修复；也不会把 `nuttx/`、`vendor/sifli/`、`apps/` 或 `packages/` 的临时改动提交到比赛仓库。

希望官方能确认推荐 config、构建命令和依赖同步方式，谢谢！
