#!/bin/bash
# set -e

THIS_DIR="$(realpath "$(dirname "${BASH_SOURCE[0]}")")"

source "${THIS_DIR}/env.sh"

BUILD_DIR="${PROJECT_DIR}/build-native"

OPTION="$1"

dump_env GPP_VERSION CMAKE_VERSION NINJA_BUILD_VERSION BUILD_DIR OPTION | tee "$LOG_PATH"

if [[ "$OPTION" == "dump" ]]
then
    exit 0
fi

if [[ "$OPTION" == "-fn" || "$OPTION" == "-f" ]]
then
    rm -rf "$BUILD_DIR"
    mkdir -p "$BUILD_DIR"
fi

CMAKE_VARIABLES=("-DCMAKE_BUILD_TYPE=Debug")

"$CMAKE_EXEC" ${CMAKE_VARIABLES[@]} -G Ninja -S "$PROJECT_DIR" -B "$BUILD_DIR" 2>&1 | tee -a "$LOG_PATH"

if [[ "$OPTION" != "-fn" && "$OPTION" != "-n" ]]
then
    "$CMAKE_EXEC" --build "$BUILD_DIR" --target "all" -- -j$(nproc) 2>&1 | tee -a "$LOG_PATH"
fi
