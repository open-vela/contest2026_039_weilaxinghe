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

当前问题不是 VelaBridge app 业务代码问题，而是黄山派 SF32LB52 / Huangshanpai board config 的底层编译依赖、include 路径和 HAL 宏配置问题。

主要表现为：

- `lckfb_huangshan_pi/configs/nsh` 构建时缺少 `ipc_queue/Make.defs`。
- 修过 `ipc_queue` 后，继续出现 SiFli HAL / CMSIS / board driver 头文件缺失。
- 对照 `sf32lb52_devkit_lcd/configs/nsh` 也曾遇到 `ipc_queue/Make.defs` 缺失，说明问题可能在 SF32LB52 通用编译链路或 HAL / CMSIS include 路径，而不只是 VelaBridge app。
- 当前已经从缺失 `ipc_queue`、FPU、HAL / CMSIS 头文件阶段继续推进到 HAL 宏配置缺失阶段，说明问题不是简单缺头文件，也不是 Ubuntu 系统依赖缺包。
- 当前 blocker 更可能是 `sf32lb52_lchspi_ulp` 在 openvela `build.sh` 链路下缺少正确的 SiFli HAL config / `rtconfig.h` / 编译宏配置，仍需官方确认。

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
error: LCPU2BCPU_MB_CH1_BUF_END_ADDR is not defined
error: USE_HAL_COMP_REGISTER_CALLBACKS is not defined
error: USE_HAL_RNG_REGISTER_CALLBACKS is not defined
error: USE_HAL_HCD_REGISTER_CALLBACKS is not defined
warning: implicit declaration of function '__arm_cx2d'
warning: implicit declaration of function '__arm_mcr2'
warning: implicit declaration of function '__arm_cx2da'
```

## HAL 宏配置缺失阶段结论

新一轮错误已经不再只是 `No such file or directory`。`LCPU2BCPU_MB_CH1_BUF_END_ADDR`、`USE_HAL_COMP_REGISTER_CALLBACKS`、`USE_HAL_RNG_REGISTER_CALLBACKS`、`USE_HAL_HCD_REGISTER_CALLBACKS` 等宏缺失，以及 `__arm_cx2d` / `__arm_mcr2` / `__arm_cx2da` 隐式声明，说明当前构建链路还没有拿到完整的 SiFli HAL config、`rtconfig.h` 或匹配的 ARM intrinsic / 编译宏配置。

当前判断：

- 这不是简单缺头文件问题；HAL / CMSIS 头文件多处已经能在 `vendor/sifli` 中找到。
- 这不是单纯 Ubuntu 依赖问题；继续安装系统包无法自动生成正确的 SiFli HAL 配置宏。
- 更可能是 `sf32lb52_lchspi_ulp` 或其他 SF32LB52 config 在 openvela `build.sh` 下缺少正确的 HAL config / `rtconfig.h` / 编译宏注入方式。
- 不建议继续手动软链接头文件，也不建议手写 `rtconfig.h` 或零散宏定义来“压过”错误；这容易把本地临时状态误当成可复现修复。
- VelaBridge app 已经可以 `Register`，当前 blocker 仍属于 SF32LB52 / SiFli 底层 build config，不是应用层代码问题。

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

## 为什么不能继续靠手动软链接补头文件

手动软链接可以作为一次性排查手段，用来判断下一个编译错误会落在哪里，但不能作为 VelaBridge Watch 的正式构建修复方案。

- 软链接容易污染 `chip.h`、`arch/arm/src/chip` 和 `vendor/sifli/chips/sf32lb52` 等底层路径；本地已经出现过 `chip.h: Too many levels of symbolic links`。
- HAL / CMSIS 头文件在本地 `vendor/sifli` 中多数真实存在，继续手动补文件无法回答真正问题：当前 board config、include path、toolchain flags 和 chip 映射是否符合官方预期。
- 新阶段错误已经进入 HAL 宏配置缺失层面，继续补头文件无法解决 `rtconfig.h`、HAL callback 宏、LCPU/BCPU mailbox 地址或 ARM intrinsic 配置来源问题。
- 当前已经证明 VelaBridge app 可被构建系统识别，日志出现 `Register: velabridge_app`；blocker 更接近 SF32LB52 board config / HAL 路径问题，不是应用层业务代码问题。
- 应优先确认官方推荐的 build config、完整构建命令、额外 HAL/CMSIS 依赖来源、repo sync 参数、分支或 SDK 包。
- 需要官方确认是否应使用 openvela `build.sh`，还是应该使用 SiFli SDK / `scons` 链路，以及 openvela 链路下 `rtconfig.h` / HAL config 应由哪里提供。
- 在官方确认前，底层 workaround 只能保存在本地排查记录中，不提交到 `nuttx/`、`vendor/sifli/`、`apps/` 或 `packages/`。

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
3. 确认 `sf32lb52_lchspi_ulp` 是否适合黄山派，且是否支持 openvela `build.sh`。
4. 确认应使用 openvela `build.sh` 还是 SiFli SDK / `scons`。
5. 确认是否提供 `rtconfig.h` / SiFli HAL config，以及这些配置应从哪里生成或同步。
6. 确认是否需要额外同步 SiFli SDK / HAL / CMSIS 依赖。
7. 在官方确认前，先继续推进 VelaBridge Watch 应用层、AI Bridge mock、协议、演示脚本和测试样例。
8. 本地 workaround 只用于临时定位，不作为长期维护方案。
