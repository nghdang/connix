#!/bin/bash

THIS_DIR="$(realpath "$(dirname "${BASH_SOURCE[0]}")")"
THIS_NAME="$(basename "${BASH_SOURCE[0]}")"

source "${THIS_DIR}/env.sh"

function usage()
{
    echo "Usage: $THIS_NAME [OPTIONS]"
    echo "Run clang-tidy."
    echo ""
    echo "    -d, --dry-run   Run the command without execute anything."
    echo "    -h, --help      Show this help."
    echo ""
    echo "Example:"
    echo "    $THIS_NAME --dry-run"
    echo ""
}

TARGET_DIR="${PROJECT_DIR}/${PROJECT_NAME}"
BUILD_DIR="${PROJECT_DIR}/build-release"

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

TARGET_DIR="${PROJECT_DIR}/${PROJECT_NAME}"
ENVS=(CLANG_TIDY_VERSION BUILD_DIR TARGET_DIR)
ENVS+=(SHOULD_FIX DRY_RUN EXTRA_OPTIONS)
dump_env ${ENVS[@]}

if [[ ! -d "$BUILD_DIR" ]]
then
    echo "Build release profile is required"
    exit $E_NG
fi

RUN_CLANG_TIDY_EXEC="$(find /usr/bin -type l -name "run-clang-tidy-*.py" \
    | sort -t"-" -k4 -n -r | head -n 1)"

pushd "$BUILD_DIR" > /dev/null
dump_and_run_command "$RUN_CLANG_TIDY_EXEC" -header-filter="($PROJECT_NAME)" \
    "$TARGET_DIR"
popd > /dev/null
