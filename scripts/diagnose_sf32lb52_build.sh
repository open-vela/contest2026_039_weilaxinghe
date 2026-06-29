#!/usr/bin/env bash
set -u

pass() { printf '[OK] %s\n' "$1"; }
warn() { printf '[WARN] %s\n' "$1"; }
fail() { printf '[MISSING] %s\n' "$1"; }

check_path() {
  local path="$1"
  if [ -e "$path" ]; then
    pass "$path"
  else
    fail "$path"
  fi
}

echo "SF32LB52 openvela build diagnosis"
echo "This script is read-only: no symlink, no source edit, no compile, no flash."
echo

echo "## Workspace"
pwd
if [ -e build.sh ] && [ -d nuttx ] && [ -d vendor/sifli ]; then
  pass "looks like openvela workspace root"
else
  warn "current directory may not be the openvela workspace root"
  warn "run from the directory that contains build.sh, nuttx/, and vendor/sifli/"
fi

echo
echo "## Required paths"
check_path "vendor/sifli/boards/sf32lb52/lckfb_huangshan_pi/configs/nsh"
check_path "vendor/sifli/boards/sf32lb52/sf32lb52_devkit_lcd/configs/nsh"
check_path "vendor/sifli/chips/sf32lb52"
check_path "vendor/sifli/chips/drivers/Include/bf0_hal.h"
check_path "vendor/sifli/chips/sf32lb52/ipc_queue/Make.defs"

echo
echo "## ipc_queue/Make.defs real path"
if [ -e vendor/sifli/chips/sf32lb52/ipc_queue/Make.defs ]; then
  realpath vendor/sifli/chips/sf32lb52/ipc_queue/Make.defs
else
  find vendor/sifli -path '*/ipc_queue/Make.defs' -print 2>/dev/null | head -20
fi

echo
echo "## Available SF32LB52 defconfigs"
find vendor/sifli/boards/sf32lb52 -path '*/configs/*/defconfig' -print 2>/dev/null | sort

echo
echo "## Header spot check"
for path in \
  vendor/sifli/chips/drivers/cmsis/sf32lb52x/register.h \
  vendor/sifli/chips/external/CMSIS/Include/core_cm33.h \
  vendor/sifli/chips/external/CMSIS/Include/cmsis_version.h \
  vendor/sifli/chips/drivers/cmsis/Include/system_bf0_ap.h \
  vendor/sifli/chips/boards/include/config/sf32lb52x/dma_config.h \
  vendor/sifli/chips/boards/include/config/sf32lb52x/tim_config.h \
  vendor/sifli/chips/sf32lb52/include/sf32lb_spi.h \
  vendor/sifli/chips/sf32lb52/include/sf32lb_rtc.h \
  vendor/sifli/chips/sf32lb52/include/sf32lb_timer.h
do
  check_path "$path"
done

echo
echo "## Build metadata spot check"
grep -RInE 'CONFIG_ARCH_FPU|CONFIG_ARCH_FPU_ABI|CONFIG_ARM_MVE' vendor/sifli/boards/sf32lb52/*/configs/nsh/defconfig 2>/dev/null | head -80 || true
grep -RIn 'SOC_BF0_HCPU' vendor/sifli/boards/sf32lb52 vendor/sifli/chips/sf32lb52 2>/dev/null | head -40 || true
grep -RInE 'drivers/Include|external/CMSIS/Include|sf32lb52/include' vendor/sifli/boards/sf32lb52 vendor/sifli/chips/sf32lb52 2>/dev/null | head -80 || true

echo
echo "## Tool checks"
if command -v git-lfs >/dev/null 2>&1; then
  git-lfs version
else
  warn "git-lfs not found"
fi

if command -v repo >/dev/null 2>&1; then
  repo --version | head -n 5
else
  warn "repo not found in PATH"
fi

if command -v arm-none-eabi-gcc >/dev/null 2>&1; then
  command -v arm-none-eabi-gcc
  arm-none-eabi-gcc --version | head -n 1
else
  warn "arm-none-eabi-gcc not found in PATH"
fi

echo
echo "## Diagnosis"
echo "- If HAL/CMSIS headers above exist but build says 'No such file or directory', suspect include path or board config wiring rather than missing files only."
echo "- If vendor/sifli/chips/sf32lb52/ipc_queue/Make.defs exists but build says './ipc_queue/Make.defs' is missing, suspect chip Make.defs current-directory or arch chip mapping."
echo "- Do not create symlinks or edit nuttx/vendor files from this script."
echo "- Ask official maintainers to confirm recommended SF32LB52 board config, build command, toolchain, and HAL/CMSIS dependency source."
