#!/bin/bash
# set -e

function dump_env() {
    local items=("THIS_DIR" "PROJECT_DIR" "PROJECT_NAME" "CMAKE_PROJECT_NAME" "LOG_PATH" $@)

    local max_len=0
    for item in ${items[@]}
    do
        if [[ ${#item} -gt $max_len ]]
        then
            max_len=${#item}
        fi
    done

    max_len=$((max_len + 4))

    local SEPARATOR="#$(printf "%0.s-" $(seq 1 79))"

    echo "$SEPARATOR"
    for item in ${items[@]}
    do
        echo "# $(printf "%-*s" $max_len $item): ${!item}"
        echo "$SEPARATOR"
    done
}

function check_tool_version()
{
    local tool_name="$1"
    local tool_version_cmd="$2"
    local tool_exec=$(which "$tool_name")

    if [[ -z "$tool_exec" ]]
    then
        echo "Not found"
    else
        if [[ -z "$tool_version_cmd" ]]
        then
            tool_version="$("$tool_exec" --version)"
        else
            tool_version="$("$tool_exec" --version | eval "$tool_version_cmd")"
        fi
        echo "$tool_exec ($tool_version)"
    fi
}

THIS_NAME="$(basename "${BASH_SOURCE[1]}")"

GPP_EXEC=$(which g++)
GPP_VERSION="$(check_tool_version "g++" "head -n1 | awk '{print \$4}'")"

CMAKE_EXEC=$(which cmake)
CMAKE_VERSION="$(check_tool_version "cmake" "head -n1 | awk '{print \$3}'")"

CTEST_EXEC=$(which ctest)
CTEST_VERSION="$(check_tool_version "ctest" "head -n1 | awk '{print \$3}'")"

NINJA_BUILD_EXEC=$(which ninja)
NINJA_BUILD_VERSION="$(check_tool_version "ninja")"

CLANG_FORMAT_EXEC=$(which clang-format)
CLANG_FORMAT_VERSION="$(check_tool_version "clang-format" "awk '{print \$4}'")"

CLANG_TIDY_EXEC=$(which clang-tidy)
CLANG_TIDY_VERSION="$(check_tool_version "clang-tidy" "head -n1 | awk '{print \$4}'")"

LCOV_EXEC=$(which lcov)
LCOV_VERSION="$(check_tool_version "lcov" "awk '{print \$4}'")"

DOXYGEN_EXEC=$(which doxygen)
DOXYGEN_VERSION="$(check_tool_version "doxygen")"

PYTHON_EXEC=$(which python)
PYTHON_VERSION="$(check_tool_version "python")"

CONAN_EXEC=$(which conan)
CONAN_VERSION="$(check_tool_version "conan" "awk '{print \$3}'")"

PROJECT_DIR=$(git rev-parse --show-toplevel 2>/dev/null)
if [[ -z "$PROJECT_DIR" ]]
then
    PROJECT_DIR="$(dirname "$THIS_DIR")"
fi
PROJECT_NAME="$(basename "$PROJECT_DIR")"

LOG_NAME=$(echo "$THIS_NAME" | awk -F. '{print $1}')
LOG_PATH="${THIS_DIR}/${LOG_NAME}.log"

CMAKE_PROJECT_NAME="$(grep "project(" "${PROJECT_DIR}/CMakeLists.txt" | awk -F"(" '{split($2, arr, " "); print arr[1]}')"
