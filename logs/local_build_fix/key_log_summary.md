# SF32LB52 Local Build Diagnosis Key Log Summary

Date: 2026-06-30

This summary records the key results from the local WSL2 build diagnosis. It intentionally excludes the large raw build logs.

## Environment

- Workspace: `/home/czy/openvela_contest`
- Team repo: `/home/czy/openvela_contest/contest2026_039_weilaxinghe`
- Board config: `vendor/sifli/boards/sf32lb52/sf32lb52_lchspi_ulp/configs/nsh`
- `nuttx` branch: `dev-ai-contest-2026`
- `vendor/sifli` branch: `dev-ai-contest-2026`
- Team repo branch: `dev-ai-contest-2026`
- Toolchain used after PATH update: `prebuilts/gcc/linux-x86_64/arm-none-eabi/bin/arm-none-eabi-gcc`, GCC 13.4.0

## Stage Summary

| Stage | Result | Key error |
| --- | --- | --- |
| Clean board build, no workaround | Failed | `ipc_queue/Make.defs: No such file or directory`, then `-mfloat-abi=hard: selected architecture lacks an FPU` |
| Minimal `ipc_queue` workaround | Failed later | FPU hard-float error remained |
| Soft-float workaround | Failed later | `./sifli_ap.c:40:10: fatal error: bf0_hal.h: No such file or directory` |
| Enable VelaBridge app | Not run | Clean board build did not pass |
| Firmware artifact validation | Not passed | Candidate files may exist in workspace, but no confirmed artifact from this run |

## Local Workarounds Used

- `nuttx/arch/arm/src/ipc_queue -> chip/ipc_queue`
- Temporarily changed `vendor/sifli/boards/sf32lb52/sf32lb52_lchspi_ulp/configs/nsh/defconfig` to soft-float.

These are local workarounds only and must not be committed.

## Final Blocker

```text
./sifli_ap.c:40:10: fatal error: bf0_hal.h: No such file or directory
```

## Conclusion

The clean board build fails before enabling VelaBridge app. The current blocker is in the SF32LB52 / SiFli board, HAL, CMSIS, or build config chain, not in VelaBridge app code.
