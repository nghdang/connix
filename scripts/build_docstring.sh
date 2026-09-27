#!/bin/bash

THIS_DIR="$(realpath "$(dirname "${BASH_SOURCE[0]}")")"
THIS_NAME="$(basename "${BASH_SOURCE[0]}")"

source "${THIS_DIR}/env.sh"

function print_usage()
{
    echo "Usage: $THIS_NAME [OPTIONS]"
    echo "Build Doxygen docstrings documentation."
    echo ""
    echo "    --clean       Clean build directory before building."
    echo "    --clean-only  Clean build directory and exit without building."
    echo "    --dry-run     Run the command without executing anything."
    echo "    --help        Show this help."
    echo ""
    echo "Example:"
    echo "    $THIS_NAME"
    echo "    $THIS_NAME --clean"
    echo "    $THIS_NAME --clean-only"
    echo "    $THIS_NAME --dry-run"
    echo ""
}

DEFAULT_SHOULD_CLEAN="$NO"
DEFAULT_CLEAN_ONLY="$NO"
DEFAULT_DRY_RUN="$NO"

SHOULD_CLEAN="$DEFAULT_SHOULD_CLEAN"
CLEAN_ONLY="$DEFAULT_CLEAN_ONLY"
DRY_RUN="$DEFAULT_DRY_RUN"
EXTRA_OPTIONS=()

while [[ $# -gt 0 ]]
do
    case "$1" in
        --clean)
            SHOULD_CLEAN="$YES"
            shift
            ;;
        --clean-only)
            SHOULD_CLEAN="$YES"
            CLEAN_ONLY="$YES"
            shift
            ;;
        --dry-run)
            DRY_RUN="$YES"
            shift
            ;;
        --help)
            print_usage
            exit
            ;;
        -*)
            EXTRA_OPTIONS+=("$1")
            shift
            ;;
        *)
            echo "Unknown argument: $1"
            print_usage
            exit $E_NG
            ;;
    esac
done

DOXYGEN_DIR="${PROJECT_DIR}/docs/doxygen"
DOXYFILE="${DOXYGEN_DIR}/Doxyfile"
OUTPUT_DIR="${DOXYGEN_DIR}/_build"

ENVS=(DOXYGEN_VERSION DOXYFILE OUTPUT_DIR)
ENVS+=(SHOULD_CLEAN CLEAN_ONLY DRY_RUN EXTRA_OPTIONS)
dump_env ${ENVS[@]}

if [[ "$SHOULD_CLEAN" == "$YES" ]]
then
    dump_command rm -rf "$OUTPUT_DIR"
    run_command rm -rf "$OUTPUT_DIR"
fi

if [[ "$CLEAN_ONLY" == "$YES" ]]
then
    exit $E_OK
fi

if [[ -z "$DOXYGEN_EXEC" ]]
then
    echo "doxygen is required but not found in PATH"
    exit $E_NG
fi

if [[ ! -f "$DOXYFILE" ]]
then
    echo "Doxyfile not found at $DOXYFILE"
    exit $E_NG
fi

pushd "$DOXYGEN_DIR" > /dev/null

DOXYGEN_ARGS=("$DOXYFILE")
DOXYGEN_ARGS+=("${EXTRA_OPTIONS[@]}")

dump_command "$DOXYGEN_EXEC" "${DOXYGEN_ARGS[@]}"

STATUS_CODE=$E_OK
if [[ "$DRY_RUN" == "$NO" ]]
then
    "$DOXYGEN_EXEC" "${DOXYGEN_ARGS[@]}"
    STATUS_CODE=$?
fi

popd > /dev/null

exit $STATUS_CODE
