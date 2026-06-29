#!/usr/bin/env bash
set -euo pipefail

echo "该脚本只能安装 Ubuntu 系统依赖，不能解决 SF32LB52 HAL/CMSIS 源码路径问题。"
echo "本脚本不会执行 repo sync，不会修改 openvela.xml，不会烧录固件。"

echo
echo "[1/5] Installing Ubuntu packages..."
sudo apt update
sudo apt install -y git curl wget unzip zip python3 python3-pip python3-venv build-essential cmake ninja-build gperf bison flex libncurses5-dev libncursesw5-dev libssl-dev bc ccache rsync xz-utils file python-is-python git-lfs

echo
echo "[2/5] Initializing Git LFS..."
git lfs install

echo
echo "[3/5] Checking repo tool..."
if command -v repo >/dev/null 2>&1; then
  repo --version | head -n 5
else
  echo "repo not found in PATH."
  echo "Install repo manually, for example:"
  echo "  mkdir -p ~/.bin"
  echo "  curl -sSL https://storage.googleapis.com/git-repo-downloads/repo > ~/.bin/repo"
  echo "  chmod +x ~/.bin/repo"
  echo "  export PATH=\"$HOME/.bin:$PATH\""
fi

echo
echo "[4/5] Checking tool versions..."
if command -v git-lfs >/dev/null 2>&1; then
  git-lfs version
else
  echo "git-lfs not found after apt install; please check apt output."
fi

python3 --version

echo
echo "[5/5] Checking ARM toolchain..."
if command -v arm-none-eabi-gcc >/dev/null 2>&1; then
  command -v arm-none-eabi-gcc
  arm-none-eabi-gcc --version | head -n 1
else
  echo "arm-none-eabi-gcc not found in PATH."
  echo "If the official SF32LB52 build requires it, install the official recommended toolchain before building."
fi

echo
echo "Done. Next step: run scripts/diagnose_sf32lb52_build.sh from the openvela workspace root."
