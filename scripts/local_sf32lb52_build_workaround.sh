#!/usr/bin/env bash
# Local workaround only. Do not commit generated nuttx/vendor symlinks.
#
# This script is a temporary local diagnostic helper for the SF32LB52
# Huangshanpai build path. It does not modify openvela.xml, does not delete
# official templates, and does not flash the board.

set -euo pipefail

BOARD_CONFIG="vendor/sifli/boards/sf32lb52/lckfb_huangshan_pi/configs/nsh"
APP_CONFIG="CONFIG_LVX_USE_DEMO_CONTEST2026_039_VELABRIDGE_APP"

die() {
  echo "error: $*" >&2
  exit 1
}

info() {
  echo "[velabridge-workaround] $*"
}

require_workspace_root() {
  [[ -f build.sh ]] || die "run this from the openvela workspace root; build.sh was not found"
  [[ -d nuttx ]] || die "run this from the openvela workspace root; nuttx/ was not found"
  [[ -d vendor/sifli ]] || die "run this from the openvela workspace root; vendor/sifli/ was not found"
  [[ -d contest2026_039_weilaxinghe ]] || die "contest2026_039_weilaxinghe/ was not found"
  [[ -d "$BOARD_CONFIG" ]] || die "board config not found: $BOARD_CONFIG"
}

ensure_chip_h_not_symlink() {
  local paths=(
    "nuttx/arch/arm/src/chip.h"
    "nuttx/arch/arm/src/chip/chip.h"
    "vendor/sifli/chips/sf32lb52/chip.h"
  )

  for path in "${paths[@]}"; do
    if [[ -L "$path" ]]; then
      die "$path is a symlink. Restore it before continuing; do not workaround by symlinking chip.h."
    fi
  done
}

create_minimal_ipc_queue_link() {
  local target_parent="nuttx/arch/arm/src/chip"
  local target="$target_parent/ipc_queue"

  [[ -d "$target_parent" ]] || die "$target_parent was not found; configure/build once before applying this workaround"

  if [[ -e "$target" || -L "$target" ]]; then
    info "ipc_queue target already exists: $target"
    return 0
  fi

  local source_make_defs
  source_make_defs="$(find vendor/sifli -path '*/ipc_queue/Make.defs' -type f | head -n 1 || true)"
  [[ -n "$source_make_defs" ]] || die "could not find any vendor/sifli */ipc_queue/Make.defs"

  local source_dir
  source_dir="$(cd "$(dirname "$source_make_defs")" && pwd)"
  info "creating minimal ipc_queue symlink: $target -> $source_dir"
  ln -s "$source_dir" "$target"
}

enable_config() {
  local file="$1"
  local key="$2"

  if grep -qE "^# ${key} is not set$" "$file"; then
    sed -i "s/^# ${key} is not set$/${key}=y/" "$file"
  elif grep -qE "^${key}=" "$file"; then
    sed -i "s/^${key}=.*/${key}=y/" "$file"
  else
    printf '\n%s=y\n' "$key" >> "$file"
  fi
}

disable_config() {
  local file="$1"
  local key="$2"

  if grep -qE "^${key}=" "$file"; then
    sed -i "s/^${key}=.*/# ${key} is not set/" "$file"
  elif ! grep -qE "^# ${key} is not set$" "$file"; then
    printf '\n# %s is not set\n' "$key" >> "$file"
  fi
}

set_soft_float_and_enable_app() {
  local defconfig="$BOARD_CONFIG/defconfig"
  [[ -f "$defconfig" ]] || die "defconfig not found: $defconfig"

  local backup="${defconfig}.velabridge-workaround.bak"
  if [[ ! -f "$backup" ]]; then
    cp "$defconfig" "$backup"
    info "backup created: $backup"
  else
    info "backup already exists: $backup"
  fi

  info "switching local defconfig hints to soft-float"
  disable_config "$defconfig" "CONFIG_ARCH_FPU"
  disable_config "$defconfig" "CONFIG_ARCH_FPU_HARD"
  disable_config "$defconfig" "CONFIG_ARCH_FLOAT_HARD"
  enable_config "$defconfig" "CONFIG_ARCH_FLOAT_SOFT"

  info "enabling VelaBridge app config: $APP_CONFIG=y"
  enable_config "$defconfig" "$APP_CONFIG"
}

main() {
  require_workspace_root
  ensure_chip_h_not_symlink
  create_minimal_ipc_queue_link
  set_soft_float_and_enable_app

  cat <<EOF

[velabridge-workaround] Done.

This script only applied a minimal local workaround for diagnostics:
- board config: $BOARD_CONFIG
- ipc_queue symlink only, no bulk header symlinks
- soft-float defconfig hints
- $APP_CONFIG=y

If the build still reports missing HAL/CMSIS headers, ask the official
openvela / SiFli maintainers for the recommended SF32LB52 build config and
dependency sync path. Do not maintain long-term local symlink workarounds.

Suggested manual build command:
  ./build.sh $BOARD_CONFIG -j8

This script does not flash the board.
EOF
}

main "$@"
