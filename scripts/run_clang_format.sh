#!/bin/bash
# set -e

THIS_DIR="$(realpath "$(dirname "${BASH_SOURCE[0]}")")"

source "${THIS_DIR}/env.sh"

OPTION="$1"

dump_env CLANG_FORMAT_VERSION OPTION | tee "$LOG_PATH"

if [[ "$OPTION" == "fix" ]]
then
    CLANG_FORMAT_ARGS="-i"
else
    CLANG_FORMAT_ARGS="--dry-run"
fi

STATUS_CODE=0
for file_path in $(find "${PROJECT_DIR}/connix" -iname "*.hpp" -o -iname "*.cpp")
do
    echo "Checking '$file_path'" | tee -a "$LOG_PATH"
    result="$("$CLANG_FORMAT_EXEC" $CLANG_FORMAT_ARGS "$file_path" 2>&1)"
    echo "$result" | tee -a "$LOG_PATH"

    if [[ ! -z "$result" ]]
    then
        STATUS_CODE=1
    fi
done

exit $STATUS_CODE
