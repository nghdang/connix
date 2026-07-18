#!/bin/bash
# set -e

THIS_DIR="$(realpath "$(dirname "${BASH_SOURCE[0]}")")"

source "${THIS_DIR}/env.sh"

BUILD_DIR="${PROJECT_DIR}/"build-release""

dump_env CLANG_TIDY_VERSION BUILD_DIR | tee "$LOG_PATH"

if [[ ! -d "$BUILD_DIR" ]]
then
    mkdir -p "$BUILD_DIR"

    CMAKE_VARIABLES=("-DCMAKE_BUILD_TYPE=Release")

    "$CMAKE_EXEC" ${CMAKE_VARIABLES[@]} -G Ninja -S "$PROJECT_DIR" -B "$BUILD_DIR" 2>&1 | tee -a "$LOG_PATH"
fi


RUN_CLANG_TIDY_EXEC="$(find /usr/bin -type l -name "run-clang-tidy-*.py" | sort -t"-" -k4 -n -r | head -n 1)"

pushd "$BUILD_DIR" > /dev/null
"$RUN_CLANG_TIDY_EXEC" -header-filter="($PROJECT_NAME)" "${PROJECT_DIR}/connix" 2>&1 | tee -a "$LOG_PATH"
popd > /dev/null
