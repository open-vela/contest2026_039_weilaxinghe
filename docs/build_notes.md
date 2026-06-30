# VelaBridge Watch SF32LB52 构建问题记录

## 基本信息

- 队伍名称：维拉星核
- 项目名称：VelaBridge Watch
- 比赛编号：039
- 仓库：`contest2026_039_weilaxinghe`
- 项目方向：基于 openvela 的多模态无障碍 AI 智能手表
- 开发板：黄山派 SF32LB52
- 当前环境：WSL2 Ubuntu

## 当前构建环境

- openvela `repo sync` 已成功。
- 团队仓库已切回 `dev-ai-contest-2026` 分支。
- 当前使用的 board config：
  - `vendor/sifli/boards/sf32lb52/lckfb_huangshan_pi/configs/nsh`
- 对照测试 board config：
  - `vendor/sifli/boards/sf32lb52/sf32lb52_devkit_lcd/configs/nsh`
- VelaBridge app 已经能被构建系统识别，构建日志多次出现：

```text
Register: velabridge_app
```

## 已通过阶段

- openvela workspace `repo sync` 已完成。
- 团队仓库分支已切回 `dev-ai-contest-2026`。
- `ai_bridge/mock_bridge.py --demo --jsonl` 可运行。
- `python3 -m unittest tests/test_mock_bridge.py` 已通过，6 tests OK。
- `app/velabridge_app` 的 Kconfig / 构建入口已能被构建系统识别。
- VelaBridge app 已出现 `Register: velabridge_app`。
- 黄山派 SF32LB52 硬件 bring-up 已完成部分验证：
  - CH340 COM5 串口识别。
  - SSCOM 1000000 波特率可接收日志。
  - 屏幕点亮。
  - 触摸坐标输出。

## 当前卡点

当前问题不是 VelaBridge app 业务代码问题，而是黄山派 SF32LB52 / Huangshanpai board config 的底层编译依赖和 include 路径问题。

主要表现为：

- `lckfb_huangshan_pi/configs/nsh` 构建时缺少 `ipc_queue/Make.defs`。
- 修过 `ipc_queue` 后，继续出现 SiFli HAL / CMSIS / board driver 头文件缺失。
- 对照 `sf32lb52_devkit_lcd/configs/nsh` 也曾遇到 `ipc_queue/Make.defs` 缺失，说明问题可能在 SF32LB52 通用编译链路或 HAL / CMSIS include 路径，而不只是 VelaBridge app。

## 已出现错误列表

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

## 本地 workaround 尝试记录

- 本仓提供 `scripts/local_sf32lb52_build_workaround.sh` 作为本地临时验证脚本。该脚本应在 openvela workspace 根目录运行，例如从外层 workspace 执行 `bash contest2026_039_weilaxinghe/scripts/local_sf32lb52_build_workaround.sh`。
- 曾尝试为 `ipc_queue/Make.defs` 做本地最小软链接，以继续观察后续编译错误。
- 曾尝试将 board defconfig 调整为 soft-float，以绕过 `-mfloat-abi=hard` 与当前架构/FPU 不匹配的问题。
- 曾尝试补 HAL / CMSIS include 路径，但后续缺失头文件较多，说明需要确认官方推荐依赖链路。
- 曾经使用批量软链接 workaround 时污染过 `chip.h`，导致：

```text
chip.h: Too many levels of symbolic links
```

- 后来通过清理 `nuttx` 和 `vendor/sifli` 后恢复。

重要结论：

- 不要把 `nuttx/arch/arm/src/chip.h` 或 `vendor/sifli/chips/sf32lb52/chip.h` 改成软链接。
- 不要批量乱链接整个 `sf32lb52` 目录。
- 不要把 `nuttx/` 或 `vendor/sifli/` 的临时修改提交到比赛仓库。

## 风险说明

- 当前尚未证明 VelaBridge Watch 固件已完整编译通过。
- 当前尚未完成烧录验证。
- 当前构建 blocker 属于 SF32LB52 / Huangshanpai board config 的底层依赖链路，不能写成 VelaBridge app 业务代码错误。
- 不应修改 `openvela.xml`。
- 不应提交对 `nuttx/`、`vendor/sifli/`、`apps/`、`packages/` 或 openvela 公共源码的临时 workaround。
- 不应把本地 workaround 写成官方修复。

## 下一步建议

1. 询问官方 / 指导老师：黄山派 SF32LB52 推荐的官方 build config 是哪个。
2. 确认是否应使用 `lckfb_huangshan_pi/configs/nsh`，还是应改用其他 SF32LB52 config。
3. 确认是否需要额外同步 SiFli HAL / CMSIS 依赖。
4. 确认是否有官方推荐的编译命令。
5. 在官方确认前，先继续推进 VelaBridge Watch 应用层、AI Bridge mock、协议、演示脚本和测试样例。
6. 本地 workaround 只用于临时定位，不作为长期维护方案。

## 2026-06-30 本地自动化诊断结果

本次在 WSL2 本地 openvela workspace `/home/czy/openvela_contest` 中实际执行了分阶段构建诊断，使用官方推荐 board config：

```text
vendor/sifli/boards/sf32lb52/sf32lb52_lchspi_ulp/configs/nsh
```

诊断日志与报告保存在：

```text
logs/local_build_fix/build_fix_report.md
logs/local_build_fix/key_log_summary.md
```

阶段结果：

- clean board build 不启用 VelaBridge app 时已经失败，先出现 `ipc_queue/Make.defs: No such file or directory`。
- 创建本地最小 workaround `nuttx/arch/arm/src/ipc_queue -> chip/ipc_queue` 后，错误推进到 `-mfloat-abi=hard: selected architecture lacks an FPU`。
- 临时把 `sf32lb52_lchspi_ulp/configs/nsh/defconfig` 改为 soft-float 后，错误继续推进到 `./sifli_ap.c:40:10: fatal error: bf0_hal.h: No such file or directory`。
- 因 clean board build 尚未通过，未启用 VelaBridge app，也未进入烧录阶段。

本次只整理诊断报告和摘要，不提交以下本地 workaround：

- `nuttx/arch/arm/src/ipc_queue` 临时软链接。
- `vendor/sifli/boards/sf32lb52/sf32lb52_lchspi_ulp/configs/nsh/defconfig` soft-float 临时修改。
- defconfig 备份文件。
- build 产物或大体积原始日志。

结论：当前仍然证明 blocker 在 SF32LB52 / SiFli board、HAL、CMSIS 或 build config 链路，不在 VelaBridge app 应用层代码。当前不能写“固件已编译通过”，也不能进入烧录阶段。
