# SF32LB52 local build fix report

日期：2026-06-30

项目：VelaBridge Watch
队伍：维拉星核
开发板：黄山派 SF32LB52
openvela workspace：`/home/czy/openvela_contest`
队伍仓库：`/home/czy/openvela_contest/contest2026_039_weilaxinghe`
board config：`vendor/sifli/boards/sf32lb52/sf32lb52_lchspi_ulp/configs/nsh`

## 结论

- clean board build：已通过。
- VelaBridge app build：已通过。
- VelaBridge app 已注册：`Register: velabridge_app`。
- 固件产物已生成：
  - `/home/czy/openvela_contest/nuttx/nuttx`
  - `/home/czy/openvela_contest/nuttx/nuttx.bin`
- 本次没有自动烧录开发板。
- 可以进入人工烧录前检查和手动烧录验证阶段，但当前底层修改均为 local workaround，不能直接提交到 `nuttx/` 或 `vendor/sifli/`。

## 测试命令

clean board build：

```bash
cd /home/czy/openvela_contest
./build.sh vendor/sifli/boards/sf32lb52/sf32lb52_lchspi_ulp/configs/nsh distclean
./build.sh vendor/sifli/boards/sf32lb52/sf32lb52_lchspi_ulp/configs/nsh -j1
```

VelaBridge app build：

```bash
cd /home/czy/openvela_contest
./build.sh vendor/sifli/boards/sf32lb52/sf32lb52_lchspi_ulp/configs/nsh -j1
```

关键日志：

- clean board build 日志：`logs/local_build_fix/04_after_aon_distclean_rebuild.log`
- VelaBridge app build 日志：`logs/local_build_fix/05_velabridge_build.log`
- 产物清单：`logs/local_build_fix/06_artifacts.txt`

## 产物

`logs/local_build_fix/06_artifacts.txt` 记录的相关产物：

```text
/home/czy/openvela_contest/nuttx/nuttx        12944 KB
/home/czy/openvela_contest/nuttx/nuttx.bin    1480 KB
```

`05_velabridge_build.log` 中最终内存摘要：

```text
flash: 1514192 B / 16 MB, 9.03%
sram: 119784 B / 512 KB, 22.85%
psram: 0 B / 8 MB, 0.00%
CP: nuttx.bin
```

## 本地修改记录

以下修改只用于本地诊断和打通构建链路，不能直接提交到底层仓库：

- `nuttx/arch/arm/src/ipc_queue -> chip/ipc_queue`
- `nuttx/arch/arm/src/sifli_hal -> chip/../drivers/hal`
- `nuttx/arch/arm/src/sifli_cmsis -> chip/../drivers/cmsis`
- `vendor/sifli/boards/sf32lb52/sf32lb52_lchspi_ulp/configs/nsh/defconfig`
  - soft-float workaround
  - 启用 `CONFIG_LVX_USE_DEMO_CONTEST2026_039_VELABRIDGE_APP=y`
- `vendor/sifli/boards/sf32lb52/sf32lb52_lchspi_ulp/scripts/Make.defs`
  - 增加 SiFli HAL/CMSIS include path
  - 增加 `SOC_BF0_HCPU`、`SF32LB52X`、`USE_HAL_DRIVER`、`HAL_TICK_PER_SECOND`
  - 临时降低部分 HAL warning 的致命性
- `vendor/sifli/chips/sf32lb52/Make.defs`
  - `ipc_queue/Make.defs` 改为 `chip/ipc_queue/Make.defs`
  - 加入官方 HAL/CMSIS/GPIO/AON/LCPU/SysTick 源文件
- `vendor/sifli/chips/drivers/Include/bf0_hal_conf_hcpu.h`
- `vendor/sifli/chips/drivers/Include/bf0_hal_conf_lcpu.h`
  - 增加 HAL callback 默认宏
  - 保持 PTC、SDADC 等不匹配模块关闭
  - 恢复 AON 模块以链接 HPAON/LPAON 实现
- `vendor/sifli/chips/drivers/Include/bf0_hal_def.h`
  - 增加 `DMA_HandleTypeDef` 前置声明，绕过 HAL DMA 循环 include
- `vendor/sifli/chips/drivers/Include/bf0_hal_lcdc.h`
  - 临时关闭 `HAL_RAMLESS_LCD_ENABLED`，避免 RAMLESS LCD 路径依赖当前未启用的 PTC 常量
- `vendor/sifli/chips/drivers/hal/bf0_hal_mpi_psram.c`
  - include `bf0_hal_aon.h`
- `vendor/sifli/chips/sf32lb52/sf32lb_adc.c`
- `vendor/sifli/chips/sf32lb52/sf32lb_flash.c`
  - include `bf0_hal_dma.h`
- `vendor/sifli/boards/sf32lb52/sf32lb52_lchspi_ulp/src/Makefile`
  - 增加官方板级 BSP、LCD、触摸、CO5300 源文件
- `apps/examples/spi/spi_main.c`
  - 修复一个导致编译失败的异常中文顿号字符

## 阶段结果

- 阶段 1：官方 clean build 初始失败，blocker 是 `ipc_queue/Make.defs` 路径缺失。
- 阶段 2：修复 ipc_queue 路径后进入 FPU hard-float 错误。
- 阶段 3：切到 soft-float 后进入 SiFli HAL/CMSIS include 和宏配置问题。
- 阶段 4：补齐 HAL/CMSIS include、官方源文件、AON/LCPU/SysTick 后 clean board build 通过。
- 阶段 5：启用 VelaBridge app 后 build 通过，日志显示 `Register: velabridge_app`。
- 阶段 6：找到可烧录候选产物 `nuttx.bin`。

## 风险

- 当前成功是本地 workaround 叠加后的结果，不等价于官方推荐配置开箱即用。
- 还没有烧录到黄山派 SF32LB52，也没有验证运行时串口、屏幕、触摸、按键、震动、音频。
- RAMLESS LCD 和 PTC 相关配置被临时关闭，可能影响具体 LCD 刷新路径，需要官方确认正确配置。
- soft-float、HAL 模块裁剪、源文件列表补齐方式都需要官方或维护者确认后再固化。
- `nuttx/`、`vendor/sifli/`、`apps/`、`packages/` 中的对象文件、symlink、底层修改和 build 产物不要提交。

## 下一步建议

1. 人工确认烧录工具、烧录地址和黄山派 SF32LB52 官方烧录流程。
2. 手动烧录 `/home/czy/openvela_contest/nuttx/nuttx.bin`，不要让 Codex 自动烧录。
3. 串口使用 CH340 COM5 / 1000000 波特率观察 boot log。
4. 验证 `velabridge_app` 是否能启动并打印状态机 boot log。
5. 将本地 workaround 整理成最小补丁，优先提交给官方或在 issue 中询问推荐修法，不要直接把底层临时改动推到比赛仓库。