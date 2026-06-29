# SF32LB52 / openvela 构建调研记录

日期：2026-06-30

队伍名称：维拉星核。项目名称：VelaBridge Watch。比赛编号：039。开发板：黄山派 SF32LB52。

本记录用于整理黄山派 SF32LB52 在 openvela workspace 中的构建卡点。当前目标不是继续手动补 HAL / CMSIS 头文件，而是确认官方推荐的 board config、构建命令、依赖来源和可复现环境。

## 结论摘要

- 当前没有找到黄山派 SF32LB52 在 openvela 比赛仓中的官方直接构建说明。未找到官方直接说明。
- openvela 官方快速入门给出了 Ubuntu 22.04、Git LFS、repo 同步和 `./build.sh <board-config> --cmake -j$(nproc)` 的通用流程，但示例 board 是 goldfish 模拟器，不是 SF32LB52。
- 立创黄山派 Linux 文档给出了 SiFli-SDK / scons 的构建方式，示例 board 为 `sf32lb52-lchspi-ulp`，但这不是 openvela `build.sh` 方式，不能直接当作本仓 openvela 结论。
- 本地 openvela workspace 中确实存在 `lckfb_huangshan_pi`、`sf32lb52_devkit_lcd` 和 `sf32lb52_lchspi_ulp` 三个 SF32LB52 defconfig。
- 多数报错中提到的 HAL / CMSIS 头文件在 `vendor/sifli` 中真实存在，问题更像是 board config / 构建入口 / include path / 工具链链路没有按官方方式打通，而不是文件完全不存在。
- 当前不应继续用批量软链接修复底层头文件；这已经导致过 `chip.h: Too many levels of symbolic links`，会污染 openvela 公共源码。

## 网络与官方资料搜索

### openvela 官方快速入门

来源：

- https://doc.openvela.com/document?id=201&language=cn&version=trunk

确认内容：

- 官方快速入门面向 Ubuntu 22.04。
- 官方快速入门页面写明该通用流程不支持在 WSL 或 Docker 中编译；当前团队实操环境为 WSL2 Ubuntu，因此需要官方确认比赛 SF32LB52 是否允许 / 推荐 WSL2 构建。
- 文档要求配置 Git LFS，否则大文件会以指针文本形式损坏。
- 文档使用 `repo init ... --git-lfs` 和 `repo sync -c -j8` 同步源码。
- 文档示例构建命令为 `./build.sh vendor/openvela/boards/vela/configs/goldfish-arm64-v8a-ap/ --cmake -j$(nproc)`。

未确认内容：

- 未找到该页面对黄山派 SF32LB52、`lckfb_huangshan_pi` 或 `sf32lb52_devkit_lcd` 的直接说明。
- 未找到该页面说明 SF32LB52 是否需要额外 HAL / CMSIS 包、额外 repo 参数或指定 SDK 分支。未找到官方直接说明。

### openvela 比赛仓 README 与官方比赛文档链接

来源：

- 本仓 `README.md`
- https://github.com/open-vela/docs/blob/dev-ai-contest-2026/zh-cn/contest_2026/contest_overview.md
- https://github.com/open-vela/docs/blob/dev-ai-contest-2026/zh-cn/contest_2026/code_submission_guide.md
- https://github.com/open-vela/docs/blob/dev-ai-contest-2026/zh-cn/contest_2026/ai_hardware/ai_hardware_guide_index.md

确认内容：

- 比赛模板要求使用 `dev-ai-contest-2026` 分支。
- 本仓 README 给出的同步方式是 `repo init -u https://github.com/open-vela/contest2026_039_weilaxinghe -b dev-ai-contest-2026 -m contest2026_039_weilaxinghe.xml`，然后 `repo sync -c -j8`。
- 本仓 README 说明 openvela 构建应在 workspace 根目录执行，`build.sh` 接收 board config 路径。
- 本仓 README 说明如需改动 `nuttx` 等公共仓库，应通过公共仓 PR，而不是在比赛仓提交临时修改。

未确认内容：

- 当前未在已能访问的资料中找到黄山派 SF32LB52 的唯一官方推荐 board config。未找到官方直接说明。
- GitHub raw 文档请求在本地网络中出现连接重置，因此后续仍需人工在浏览器打开官方比赛文档再次确认。

### 立创黄山派 Linux 文档

来源：

- https://wiki.lckfb.com/zh-hans/hspi-sf32lb52/lckfb-hspi-sf32lb52/hspi-linux.html

确认内容：

- 立创文档面向黄山派 SF32LB52 的 Linux 环境与 SiFli-SDK。
- 文档给出的 SDK 获取方式包含 `git clone --recursive -b release/v2.4 https://gitee.com/SiFli/sifli-sdk.git`。
- 文档给出的构建示例使用 `scons --board=sf32lb52-lchspi-ulp -j8`。
- 文档强调需要设置 SiFli-SDK 环境变量，例如 `. ./export.sh`。

未确认内容：

- 该页面不是 openvela `build.sh` 直接构建说明。
- 不能把 `scons --board=sf32lb52-lchspi-ulp` 直接等同于 openvela 的 `vendor/sifli/boards/sf32lb52/.../configs/nsh`。未找到官方直接说明。

### 搜索关键词结果

已搜索关键词：

- `openvela SF32LB52 lckfb_huangshan_pi build`
- `openvela Huangshan Pi SF32LB52 build`
- `openvela ipc_queue Make.defs SF32LB52`
- `SiFli SF32LB52 openvela bf0_hal.h`
- `黄山派 SF32LB52 openvela 编译`
- `lckfb_huangshan_pi configs nsh Make.defs`
- `SF32LB52 HAL CMSIS include path openvela`

搜索结论：

- 未找到 openvela 官方页面直接说明黄山派 SF32LB52 应使用哪个 board config。未找到官方直接说明。
- 未找到官方 issue / discussion 直接说明 `ipc_queue/Make.defs` 缺失应如何修复。未找到官方直接说明。
- 未找到官方页面说明是否需要额外下载 SiFli HAL / CMSIS 依赖包。未找到官方直接说明。

## 本地 openvela workspace 调研

调研工作区：

```text
/home/czy/openvela_contest
```

`build.sh` 位置：

```text
/home/czy/openvela_contest/build.sh -> nuttx/tools/build.sh
```

### SF32LB52 board config

本地存在的 SF32LB52 defconfig：

```text
vendor/sifli/boards/sf32lb52/lckfb_huangshan_pi/configs/nsh/defconfig
vendor/sifli/boards/sf32lb52/sf32lb52_devkit_lcd/configs/nsh/defconfig
vendor/sifli/boards/sf32lb52/sf32lb52_lchspi_ulp/configs/nsh/defconfig
```

目标路径存在性：

```text
FOUND vendor/sifli/boards/sf32lb52/lckfb_huangshan_pi/configs/nsh
FOUND vendor/sifli/boards/sf32lb52/sf32lb52_devkit_lcd/configs/nsh
FOUND vendor/sifli/chips/sf32lb52
FOUND vendor/sifli/chips/drivers/Include
FOUND vendor/sifli/chips/sf32lb52/include
```

当前不能直接判断哪个是黄山派 openvela 官方推荐 config。需要官方确认：

- `vendor/sifli/boards/sf32lb52/lckfb_huangshan_pi/configs/nsh`
- `vendor/sifli/boards/sf32lb52/sf32lb52_devkit_lcd/configs/nsh`
- `vendor/sifli/boards/sf32lb52/sf32lb52_lchspi_ulp/configs/nsh`

### HAL / CMSIS 头文件真实位置

以下文件在本地 workspace 中真实存在：

```text
vendor/sifli/chips/drivers/Include/bf0_hal.h
vendor/sifli/chips/drivers/cmsis/sf32lb52x/register.h
vendor/sifli/chips/external/CMSIS/Include/core_cm33.h
vendor/sifli/chips/external/CMSIS/Include/cmsis_version.h
vendor/sifli/chips/drivers/cmsis/sf32lb52x/mem_map.h
vendor/sifli/boards/sf32lb52/lckfb_huangshan_pi/include/drv_io.h
vendor/sifli/boards/sf32lb52/sf32lb52_devkit_lcd/include/drv_io.h
vendor/sifli/chips/sf32lb52/include/sf32lb_spi.h
vendor/sifli/chips/sf32lb52/include/sf32lb_rtc.h
vendor/sifli/chips/sf32lb52/include/sf32lb_timer.h
vendor/sifli/chips/drivers/cmsis/Include/system_bf0_ap.h
vendor/sifli/chips/boards/include/config/sf32lb52x/dma_config.h
vendor/sifli/chips/boards/include/config/sf32lb52x/tim_config.h
vendor/sifli/chips/drivers/cmsis/sf32lb52x/bt_mac.h
vendor/sifli/chips/drivers/cmsis/sf32lb52x/cache.h
vendor/sifli/chips/external/CMSIS/Include/mpu_armv8.h
vendor/sifli/chips/external/CMSIS/Include/core_mstar.h
```

判断：

- 这些文件存在，说明当前不能简单结论为 repo 未同步完整。
- 如果编译仍报 `No such file or directory`，更可能是 include path、board config、构建系统选择、工具链或 repo/linkfile 映射方式有问题。
- 这只是本地证据推断，不是官方结论；最终仍需官方确认。

### `ipc_queue/Make.defs` 位置

本地真实路径：

```text
vendor/sifli/chips/sf32lb52/ipc_queue/Make.defs
```

相关 `Make.defs`：

```text
vendor/sifli/boards/sf32lb52/drivers/Make.defs
vendor/sifli/boards/sf32lb52/drivers/input/Make.defs
vendor/sifli/boards/sf32lb52/drivers/lcd/Make.defs
vendor/sifli/boards/sf32lb52/lckfb_huangshan_pi/scripts/Make.defs
vendor/sifli/boards/sf32lb52/sf32lb52_devkit_lcd/scripts/Make.defs
vendor/sifli/boards/sf32lb52/sf32lb52_lchspi_ulp/scripts/Make.defs
vendor/sifli/chips/sf32lb52/Make.defs
vendor/sifli/chips/sf32lb52/ipc_queue/Make.defs
```

`vendor/sifli/chips/sf32lb52/Make.defs` 中存在：

```text
include armv7-m/Make.defs
include ./ipc_queue/Make.defs
```

判断：

- `ipc_queue/Make.defs` 文件本身存在。
- 构建时报 `chip/Make.defs:76: ipc_queue/Make.defs: No such file or directory` 时，重点应检查构建系统解析 `chip/Make.defs` 时的当前目录或 chip 映射路径是否符合官方预期。
- 不应通过手工软链接把 `ipc_queue` 或整个 chip 目录乱链到 `nuttx/arch/arm/src/chip`。

### include path 线索

本地 `CMakeLists.txt` 中有 HAL / CMSIS include 线索：

```text
vendor/sifli/boards/sf32lb52/lckfb_huangshan_pi/src/CMakeLists.txt
vendor/sifli/boards/sf32lb52/sf32lb52_devkit_lcd/src/CMakeLists.txt
vendor/sifli/boards/sf32lb52/sf32lb52_lchspi_ulp/src/CMakeLists.txt
vendor/sifli/boards/sf32lb52/drivers/CMakeLists.txt
vendor/sifli/chips/sf32lb52/CMakeLists.txt
```

其中可见 `drivers/Include` 与 `external/CMSIS/Include` 的 include 目录配置。`vendor/sifli/boards/sf32lb52/drivers/Make.defs` 也有类似 CFLAGS：

```text
CFLAGS += ${INCDIR_PREFIX}$(CHIP_DRIVERS_DIR)$(DELIM)Include
CFLAGS += ${INCDIR_PREFIX}$(CHIP_DRIVERS_DIR)$(DELIM)cmsis$(DELIM)sf32lb52x
CFLAGS += ${INCDIR_PREFIX}$(CHIP_DRIVERS_DIR)$(DELIM)external$(DELIM)CMSIS$(DELIM)Include
CFLAGS += ${INCDIR_PREFIX}$(ARCH_SRCDIR)$(DELIM)chip
CFLAGS += ${INCDIR_PREFIX}$(ARCH_SRCDIR)$(DELIM)chip$(DELIM)include
```

判断：

- 本地源码中存在 include path 配置线索。
- 如果实际构建时仍缺少 HAL / CMSIS 头文件，可能是 board drivers `Make.defs` 没有被当前构建路径纳入，或 `ARCH_SRCDIR/chip` 映射不是官方预期形态。
- 仍需官方确认应使用 `--cmake`、哪一个 config、是否需要执行额外环境脚本。

### FPU / MVE / SOC 配置线索

本地 defconfig 线索：

```text
vendor/sifli/boards/sf32lb52/lckfb_huangshan_pi/configs/nsh/defconfig:CONFIG_ARCH_FPU=y
vendor/sifli/boards/sf32lb52/sf32lb52_devkit_lcd/configs/nsh/defconfig:CONFIG_ARCH_FPU=y
vendor/sifli/boards/sf32lb52/sf32lb52_lchspi_ulp/configs/nsh/defconfig:CONFIG_ARCH_FPU=y
```

本地搜索未在这几个 defconfig 中看到 `CONFIG_ARM_MVE`。

`SOC_BF0_HCPU` 出现在 board/chip CMake 与 Make 配置中：

```text
vendor/sifli/boards/sf32lb52/drivers/Make.defs
vendor/sifli/boards/sf32lb52/lckfb_huangshan_pi/src/CMakeLists.txt
vendor/sifli/boards/sf32lb52/sf32lb52_devkit_lcd/src/CMakeLists.txt
vendor/sifli/chips/sf32lb52/CMakeLists.txt
```

判断：

- `-mfloat-abi=hard` 报错不能只靠手动把 FPU 关掉长期规避。
- 需要官方确认 SF32LB52 HCPU 的正确架构/FPU/toolchain flags，以及应使用的 `arm-none-eabi-gcc` 版本或工具链安装方式。

### 本地工具链状态

在当前 WSL shell 中检查到：

```text
git-lfs/3.4.1
repo: command not found
arm-none-eabi-gcc: command not found
Python 3.12.3
```

同时找到了 repo 候选文件：

```text
/home/czy/.bin/repo
/home/czy/openvela_contest/.repo/repo/repo
```

判断：

- `repo sync` 已经由人工确认完成，但当前 shell 的 `PATH` 中没有 `repo`。
- `git-lfs` 已安装。
- 当前 shell 未找到 `arm-none-eabi-gcc`，后续应先用 `scripts/setup_openvela_wsl_deps.sh` 和 `scripts/diagnose_sf32lb52_build.sh` 做环境确认。

## 是否需要额外 init / sync / LFS

已确认：

- openvela 官方快速入门要求使用 Git LFS。
- 本地 `git-lfs` 已安装。
- 本地多数 HAL / CMSIS 头文件已存在。

未确认：

- 是否需要针对比赛 `dev-ai-contest-2026` 使用额外 `repo init` 参数。未找到官方直接说明。
- 是否需要额外拉取 SiFli SDK 包或指定 SDK 分支。未找到官方直接说明。
- 是否需要把 LCKFB 的 `release/v2.4` SiFli-SDK 与 openvela workspace 混用。未找到官方直接说明。

## 当前建议

1. 暂停手动软链接底层头文件。
2. 在 openvela workspace 根目录运行 `bash contest2026_039_weilaxinghe/scripts/diagnose_sf32lb52_build.sh` 收集只读诊断输出。
3. 使用 `docs/official_question_sf32lb52_build.md` 向官方确认推荐 config、构建命令、HAL/CMSIS 依赖、repo 参数、分支或 SDK 包。
4. 在官方确认前，不声明 VelaBridge Watch 固件已经编译通过。
5. 并行推进 VelaBridge Watch 应用层 UI、AI Bridge mock、协议、演示脚本和用户调研。
