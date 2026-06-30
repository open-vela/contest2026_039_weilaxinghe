# SF32LB52 Local Build Fix Report

Date: 2026-06-30T11:41:16+08:00

- Team: Weilaxinghe
- Project: VelaBridge Watch
- Contest ID: 039
- openvela workspace: /home/czy/openvela_contest
- Team repo: /home/czy/openvela_contest/contest2026_039_weilaxinghe
- Board config: vendor/sifli/boards/sf32lb52/sf32lb52_lchspi_ulp/configs/nsh

## Stage Results

| Stage | Result | Log |
| --- | --- | --- |
| Stage 0/1: environment check | Done. nuttx, vendor/sifli, and team repo are on dev-ai-contest-2026. Initial PATH did not include arm-none-eabi-gcc; subsequent builds used openvela prebuilt GCC 13.4.0. | 00_env_check.log, 00_prepare_clean_build.log |
| Stage 2: official clean board build, no workaround | Failed. distclean hit ipc_queue/Make.defs. build hit hard-float FPU error. | 01_clean_build.log |
| Stage 3: minimal ipc_queue workaround | Done. Created nuttx/arch/arm/src/ipc_queue -> chip/ipc_queue. Rebuild moved to hard-float FPU error. | 02_ipc_fix_analysis.log, 02_after_ipc_fix.log |
| Stage 4: soft-float workaround | Done. Temporarily changed sf32lb52_lchspi_ulp defconfig to soft-float. Rebuild moved to bf0_hal.h include error. | 03_fpu_analysis_before.log, 03_soft_float_edit.log, 03_after_soft_float.log |
| Stage 5: enable VelaBridge app | Not run. Clean board build has not passed yet. | - |
| Stage 6: find firmware artifacts | Board build did not pass. Candidate artifact list was saved only for reference and does not prove this build succeeded. | 05_artifacts.txt |

## Local Changes

All changes below are local workarounds. Do not commit them to the contest repo or openvela public repos.

1. nuttx local workaround:
   - nuttx/arch/arm/src/ipc_queue -> chip/ipc_queue
   - Purpose: work around relative include of ./ipc_queue/Make.defs from chip/Make.defs.

2. vendor/sifli local workaround:
   - Modified vendor/sifli/boards/sf32lb52/sf32lb52_lchspi_ulp/configs/nsh/defconfig
   - Disabled FPU / DPFPU / MVE / hard-float and enabled soft-float.
   - Backup file: vendor/sifli/boards/sf32lb52/sf32lb52_lchspi_ulp/configs/nsh/defconfig.velabridge-local-softfloat.bak.20260630113727

## Does This Prove The Blocker Is Board/HAL Rather Than VelaBridge App?

Yes. VelaBridge app was not enabled. The official clean board config already fails before app-level code is involved.

Failure sequence:

1. ipc_queue/Make.defs relative path issue.
2. FPU hard-float mismatch with current armv7-m / cortex-m3 flags.
3. After soft-float workaround, build reaches SiFli HAL/CMSIS include issue and stops at bf0_hal.h.

Current blocker remains in the SF32LB52 / SiFli board, HAL, CMSIS, or build config chain, not in VelaBridge app code.

## Last Blocker

```text
./sifli_ap.c:40:10: fatal error: bf0_hal.h: No such file or directory
```

See last_errors.txt for extracted key errors.

## Firmware Artifacts

The build did not finish. Do not claim firmware was generated or is flashable.

Artifact scan output was saved to 05_artifacts.txt. Any files listed there are only candidates from the workspace and do not prove this run succeeded.

## Can We Move To Flashing?

No. Clean board build has not passed, and there is no confirmed bin/elf/hex/img artifact from this run.

## Next Suggestions

1. Do not batch-symlink HAL/CMSIS headers. Do not overwrite chip.h.
2. Do not hand-write rtconfig.h or random HAL macros as a formal fix.
3. Report to official maintainers that sf32lb52_lchspi_ulp + openvela build.sh + vendor/sifli dev-ai-contest-2026 still fails on clean board build.
4. Ask which Make.defs / CMakeLists / config is supposed to inject HAL/CMSIS include paths for sf32lb52_lchspi_ulp under openvela build.sh.
5. Ask whether hard-float should be disabled by default or whether different arch/toolchain flags are required.
6. Continue VelaBridge Watch app-layer work such as AI Bridge, UI state machine, protocol, and user research while waiting for the lower-level build fix.

## Warnings

- Do not commit nuttx/ or vendor/sifli/ local workarounds.
- Do not commit build artifacts.
- Do not modify openvela.xml.
- Do not flash the board automatically.
