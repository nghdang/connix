#!/bin/bash

THIS_DIR="$(realpath "$(dirname "${BASH_SOURCE[0]}")")"
THIS_NAME="$(basename "${BASH_SOURCE[0]}")"

source "${THIS_DIR}/env.sh"

function usage()
{
    echo "Usage: $THIS_NAME [OPTIONS]"
    echo "Run clang-format."
    echo ""
    echo "    -f, --fix       Fix clang-format issues."
    echo "    -s, --show      Show clang-format issues."
    echo "    -d, --dry-run   Run the command without execute anything."
    echo "    -h, --help      Show this help."
    echo ""
    echo "Example:"
    echo "    $THIS_NAME --dry-run"
    echo ""
}

TARGET_DIR="${PROJECT_DIR}/${PROJECT_NAME}"

DEFAULT_SHOULD_FIX="$NO"
DEFAULT_DRY_RUN="$NO"

SHOULD_FIX="$DEFAULT_SHOULD_FIX"
DRY_RUN="$DEFAULT_DRY_RUN"
EXTRA_OPTIONS=()

while [[ $# -gt 0 ]]
do
    case "$1" in
        -f|--fix)
            SHOULD_FIX="$YES"
            shift
            ;;
        -s|--show)
            SHOULD_FIX="$NO"
            shift
            ;;
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

dump_env CLANG_FORMAT_VERSION TARGET_DIR SHOULD_FIX DRY_RUN EXTRA_OPTIONS

if [[ "$SHOULD_FIX" == "$YES" ]]
then
    CLANG_FORMAT_ARGS="-i"
else
    CLANG_FORMAT_ARGS="--dry-run"
fi

STATUS_CODE=$E_OK
for FILE_PATH in $(find "$TARGET_DIR" -iname "*.hpp" -o -iname "*.cpp")
do
    dump_command "$CLANG_FORMAT_EXEC" $CLANG_FORMAT_ARGS "$FILE_PATH"
    RESULT="$(run_command "$CLANG_FORMAT_EXEC" $CLANG_FORMAT_ARGS \
        "$FILE_PATH" 2>&1)"

    if [[ ! -z "$RESULT" ]]
    then
        STATUS_CODE=$E_NG
        echo "$RESULT"
    fi
done

exit $STATUS_CODE
