#!/bin/bash

THIS_DIR="$(realpath "$(dirname "${BASH_SOURCE[0]}")")"
THIS_NAME="$(basename "${BASH_SOURCE[0]}")"

source "${THIS_DIR}/env.sh"

function usage()
{
    echo "Usage: $THIS_NAME [OPTIONS]"
    echo "Check whether code coverage satisfy the threshold."
    echo ""
    echo "    -m, --min       Minimum percentage to archive."
    echo "    -d, --dry-run   Run the command without execute anything."
    echo "    -h, --help      Show this help."
    echo ""
    echo "Example:"
    echo "    $THIS_NAME --dry-run"
    echo ""
}

COVERAGE_INFO_PATH="${PROJECT_DIR}/build-native/coverage.info"

DEFAULT_MIN_COVERAGE=100
DEFAULT_DRY_RUN="$NO"

MIN_COVERAGE="$DEFAULT_MIN_COVERAGE"
DRY_RUN="$DEFAULT_DRY_RUN"
EXTRA_OPTIONS=()

while [[ $# -gt 0 ]]
do
    case "$1" in
        -m|--min)
            MIN_COVERAGE="$2"
            shift 2
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

ENVS=(LCOV_VERSION COVERAGE_INFO_PATH)
ENVS+=(MIN_COVERAGE DRY_RUN EXTRA_OPTIONS)
dump_env ${ENVS[@]}

if [[ ! -f "$COVERAGE_INFO_PATH" ]]
then
    echo "Unit test coverage result is required"
    exit $E_NG
fi

LINE_COVERAGE=$("$LCOV_EXEC" --summary "$COVERAGE_INFO_PATH" | grep "lines" \
    | sed -E 's/.*: ([0-9.]+)%.*/\1/')

RESULT_MESSAGE="Line coverage: ${LINE_COVERAGE}% (required: ${MIN_COVERAGE}%)"

if awk "BEGIN {exit !($LINE_COVERAGE >= $MIN_COVERAGE)}"; then
    echo "$RESULT_MESSAGE -> Passed"
    exit $E_OK
else
    echo "$RESULT_MESSAGE -> Failed"
    exit $E_NG
fi
