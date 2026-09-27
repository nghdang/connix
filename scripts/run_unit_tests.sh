#!/bin/bash

THIS_DIR="$(realpath "$(dirname "${BASH_SOURCE[0]}")")"
THIS_NAME="$(basename "${BASH_SOURCE[0]}")"

source "${THIS_DIR}/env.sh"

function usage()
{
    echo "Usage: $THIS_NAME [OPTIONS]"
    echo "Run unit tests."
    echo ""
    echo "    -d, --dry-run   Run the command without execute anything."
    echo "    -h, --help      Show this help."
    echo ""
    echo "Example:"
    echo "    $THIS_NAME --dry-run"
    echo ""
}

BUILD_DIR="${PROJECT_DIR}/build-native"

DEFAULT_DRY_RUN="$NO"

DRY_RUN="$DEFAULT_DRY_RUN"
EXTRA_OPTIONS=()

while [[ $# -gt 0 ]]
do
    case "$1" in
        -d|--dry-run)
            DRY_RUN="$YES"
            shift
            ;;
        -h|--help)
            print_usage
            exit
            ;;
        *)
            EXTRA_OPTIONS+=("$1")
            shift
            ;;
    esac
done

ENVS=(CTEST_VERSION LCOV_VERSION BUILD_DIR)
ENVS+=(DRY_RUN EXTRA_OPTIONS)
dump_env ${ENVS[@]}

if [[ ! -d "$BUILD_DIR" ]]
then
    echo "Build native profile is required"
    exit $E_NG
fi

dump_and_run_command "$CMAKE_EXEC" --build "$BUILD_DIR" \
    --target "connix-core-unit-tests-run" -- -j$(nproc)

dump_and_run_command "$CMAKE_EXEC" --build "$BUILD_DIR" \
    --target "gen-code-coverage" -- -j$(nproc)
