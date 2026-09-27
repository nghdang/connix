#!/bin/bash

THIS_DIR="$(realpath "$(dirname "${BASH_SOURCE[0]}")")"
THIS_NAME="$(basename "${BASH_SOURCE[0]}")"

source "${THIS_DIR}/env.sh"

function print_usage()
{
    echo "Usage: $THIS_NAME [OPTIONS] [PATHS...]"
    echo "Check spelling using cspell."
    echo ""
    echo "    -d, --dry-run   Run the command without executing anything."
    echo "    -h, --help      Show this help."
    echo ""
    echo "Default paths if none specified: connix docs"
    echo ""
    echo "Example:"
    echo "    $THIS_NAME"
    echo "    $THIS_NAME --dry-run"
    echo "    $THIS_NAME connix/connix-core"
    echo ""
}

DEFAULT_DRY_RUN="$NO"

DRY_RUN="$DEFAULT_DRY_RUN"
EXTRA_OPTIONS=()
TARGET_PATHS=()

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
        -*)
            EXTRA_OPTIONS+=("$1")
            shift
            ;;
        *)
            TARGET_PATHS+=("$1")
            shift
            ;;
    esac
done

if [[ ${#TARGET_PATHS[@]} -eq 0 ]]
then
    TARGET_PATHS=("${PROJECT_DIR}/connix" "${PROJECT_DIR}/docs")
fi

TARGET_DIR="${TARGET_PATHS[*]}"
ENVS=(CSPELL_VERSION TARGET_DIR)
ENVS+=(DRY_RUN EXTRA_OPTIONS)
dump_env ${ENVS[@]}

if [[ -z "$CSPELL_EXEC" ]]
then
    echo "cspell is required but not found in PATH"
    exit $E_NG
fi

CSPELL_ARGS=("--gitignore")
CSPELL_ARGS+=("${EXTRA_OPTIONS[@]}")
CSPELL_ARGS+=("${TARGET_PATHS[@]}")

dump_command "$CSPELL_EXEC" "${CSPELL_ARGS[@]}"

STATUS_CODE=$E_OK
if [[ "$DRY_RUN" == "$NO" ]]
then
    "$CSPELL_EXEC" "${CSPELL_ARGS[@]}"
    STATUS_CODE=$?
fi

exit $STATUS_CODE
