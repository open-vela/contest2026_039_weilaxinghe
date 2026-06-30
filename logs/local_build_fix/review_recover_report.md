# SF32LB52 本地编译恢复报告

日期：2026-06-30

项目：维拉星核 / VelaBridge Watch

工作区：`/home/czy/openvela_contest`

队伍仓库：`/home/czy/openvela_contest/contest2026_039_weilaxinghe`

开发板：黄山派 SF32LB52-DevKit-ULP

使用 board config：`vendor/sifli/boards/sf32lb52/sf32lb52_lchspi_ulp/configs/nsh`

## 结论

- 本次已恢复本地 openvela 编译状态。
- `./build.sh vendor/sifli/boards/sf32lb52/sf32lb52_lchspi_ulp/configs/nsh -j1` 已通过。
- 已生成 `/home/czy/openvela_contest/nuttx/nuttx`。
- 已生成 `/home/czy/openvela_contest/nuttx/nuttx.bin`。
- 已复制到 Windows 烧录路径：`D:\shaolu\nuttx.bin`（WSL 路径：`/mnt/d/shaolu/nuttx.bin`）。
- `nuttx/.config` 中已启用 `CONFIG_LVX_USE_DEMO_CONTEST2026_039_VELABRIDGE_APP=y`。
- ELF 文本中可见 `VelaBridge Watch started` 和 `VelaBridge Watch demo finished`，说明新的可见 demo 输出已进入固件。
- 本次未自动烧录。下一步可使用 sftool GUI 手动烧录 `D:\shaolu\nuttx.bin` 到 `0x12010000`。

## 关键验证

构建日志：

- `logs/local_build_fix/review_recover_build_round40.log`

产物校验：

- `logs/local_build_fix/review_recover_verify.txt`
- `logs/local_build_fix/review_recover_nuttx_bin.sha256`

SHA256：

```text
ca9723046407a06098903e93ac2e2be3a153f8b4c8ad8dd81e294ae930f22b92  nuttx/nuttx.bin
ca9723046407a06098903e93ac2e2be3a153f8b4c8ad8dd81e294ae930f22b92  /mnt/d/shaolu/nuttx.bin
```

产物大小：

```text
/home/czy/openvela_contest/nuttx/nuttx      13M
/home/czy/openvela_contest/nuttx/nuttx.bin 1.5M
/mnt/d/shaolu/nuttx.bin                    1.5M
```

## 本次恢复的本地 workaround

以下改动只用于本地编译恢复，不应直接提交到比赛队伍仓库，也不应提交到 `nuttx/` 或 `vendor/sifli/` 上游：

1. 恢复 `nuttx/arch/arm/src/ipc_queue -> chip/ipc_queue`。
2. 恢复 `nuttx/arch/arm/src/sifli_hal -> chip/../drivers/hal`。
3. 恢复 `nuttx/arch/arm/src/sifli_cmsis -> chip/../drivers/cmsis`。
4. 恢复 `nuttx/arch/arm/src/sifli_external -> chip/../external`。
5. 将 `sf32lb52_lchspi_ulp/configs/nsh/defconfig` 与 `nuttx/.config` 调整为 soft-float，并启用 VelaBridge app。
6. 为 SiFli HAL / CMSIS / board config 增加 include path 与必要宏：`SOC_BF0_HCPU`、`SF32LB52X`、`USE_HAL_DRIVER`、`HAL_TICK_PER_SECOND`。
7. 增加 `openvela_local_compat.h`，为缺失的 HAL callback 宏提供默认值。
8. 在 `vendor/sifli/chips/sf32lb52/Make.defs` 中纳入必要的 SiFli HAL / CMSIS / GPIO / LCD / flash / system 源文件。
9. 在 board `src/Makefile` 中纳入 BSP、LCD、触摸和 CO5300 相关源文件。
10. 在 HAL 配置中临时关闭未支持或当前链路不完整的模块，例如 PTC、SDADC、BUSMON、DSI 等，并恢复 AON。
11. 修正 `bf0_hal_def.h`、`bf0_hal_lcdc.h`、`bf0_hal_mpi_psram.c`、`sf32lb_adc.c`、`sf32lb_flash.c` 的本地编译依赖问题。
12. 修正 `nuttx/arch/arm/src/arm_m/Make.defs` 的 SysTick 判断，让当前配置中的 `CONFIG_ARM_SYSTICK` / `CONFIG_ARMV8M_SYSTICK` 能编译 `arm_systick.c`，解决最终 `systick_initialize` 链接缺失。

## 修改过的底层文件

本地修改涉及：

- `nuttx/arch/arm/src/arm_m/Make.defs`
- `nuttx/arch/arm/src/ipc_queue`
- `nuttx/arch/arm/src/sifli_hal`
- `nuttx/arch/arm/src/sifli_cmsis`
- `nuttx/arch/arm/src/sifli_external`
- `vendor/sifli/boards/sf32lb52/sf32lb52_lchspi_ulp/configs/nsh/defconfig`
- `vendor/sifli/boards/sf32lb52/sf32lb52_lchspi_ulp/scripts/Make.defs`
- `vendor/sifli/boards/sf32lb52/sf32lb52_lchspi_ulp/src/Makefile`
- `vendor/sifli/chips/sf32lb52/Make.defs`
- `vendor/sifli/chips/sf32lb52/openvela_local_compat.h`
- `vendor/sifli/chips/drivers/Include/bf0_hal_conf_hcpu.h`
- `vendor/sifli/chips/drivers/Include/bf0_hal_conf_lcpu.h`
- `vendor/sifli/chips/drivers/Include/bf0_hal_def.h`
- `vendor/sifli/chips/drivers/Include/bf0_hal_lcdc.h`
- `vendor/sifli/chips/drivers/hal/bf0_hal_mpi_psram.c`
- `vendor/sifli/chips/sf32lb52/sf32lb_adc.c`
- `vendor/sifli/chips/sf32lb52/sf32lb_flash.c`

## 队伍仓库应用代码

队伍仓库内的 `app/velabridge_app/velabridge_app_main.c` 已修改为运行 `velabridge_app` 时打印清晰 demo 日志：

```text
VelaBridge Watch started
Board: SF32LB52 DevKit ULP
Mode: normal
Blind mode: available
AI bridge: mock ready

[caption] 老师说：请先到教务处服务窗口
[ocr] 食堂一号窗口，今日套餐 15 元
[danger_alert] 检测到前方台阶，请小心
[quick_reply] 请您说慢一点，我正在看字幕

VelaBridge Watch demo finished
```

该文本已在本次生成的 ELF 中通过 `strings nuttx/nuttx` 查到。

## 不要提交的内容

不要提交以下内容：

- `nuttx/` 的本地 workaround。
- `vendor/sifli/` 的本地 workaround。
- `apps/`、`packages/` 的构建产物。
- `openvela.xml`。
- `nuttx/nuttx`、`nuttx/nuttx.bin` 或任何 `.o` / `.a` / `.dep` / `.built` 构建产物。
- `logs/local_build_fix/` 下过大的完整 build log，除非后续只整理摘要。

## 下一步

1. 使用 sftool GUI 手动选择 `D:\shaolu\nuttx.bin`。
2. 目标地址仍使用 `0x12010000`。
3. 烧录后进入 NuttShell，运行：

```text
velabridge_app
```

4. 预期串口控制台打印 VelaBridge Watch demo 日志。
5. 若运行后仍没有输出，优先确认串口终端是否在同一 NSH 控制台、是否有行缓冲显示、是否烧录的是最新 `D:\shaolu\nuttx.bin`。

## 风险说明

当前能编译通过并生成固件，但底层修复仍是本地 workaround，不是上游正式方案。后续应把这些 workaround 整理成最小补丁或向官方确认推荐的 SF32LB52 HAL / CMSIS / board config 配置方式，再决定是否提交正式修复。