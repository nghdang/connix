#!/bin/bash

THIS_DIR="$(realpath "$(dirname "${BASH_SOURCE[0]}")")"
THIS_NAME="$(basename "${BASH_SOURCE[0]}")"

source "${THIS_DIR}/env.sh"

function print_usage()
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
ENVS+=(DRY_RUN EXTRA_OPTIONS)
dump_env ${ENVS[@]}

if [[ ! -d "$BUILD_DIR" ]]
then
    echo "Build release profile is required"
    exit $E_NG
fi

RUN_CLANG_TIDY_EXEC="$(find /usr/bin -type l -name "run-clang-tidy-*.py" \
    | sort -t"-" -k4 -n -r | head -n 1)"

pushd "$BUILD_DIR" > /dev/null

CLANG_TIDY_ARGS=("-quiet")
CLANG_TIDY_ARGS+=("-warnings-as-errors=*")
CLANG_TIDY_ARGS+=("-header-filter=($PROJECT_NAME)")
CLANG_TIDY_ARGS+=("${EXTRA_OPTIONS[@]}")
CLANG_TIDY_ARGS+=("$TARGET_DIR")

dump_command "$RUN_CLANG_TIDY_EXEC" "${CLANG_TIDY_ARGS[@]}"

STATUS_CODE=$E_OK
if [[ "$DRY_RUN" == "$NO" ]]
then
    "$RUN_CLANG_TIDY_EXEC" "${CLANG_TIDY_ARGS[@]}" 2>&1 \
        | grep -v -E "(^[0-9]+ (warning|error)s? generated\.|^clang-tidy|Suppressed [0-9]+ warnings?|Use -header-filter=)"
    STATUS_CODE=${PIPESTATUS[0]}
fi

popd > /dev/null

exit $STATUS_CODE
