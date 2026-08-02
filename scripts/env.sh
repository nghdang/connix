#!/bin/bash

YES="yes"
NO="no"

E_OK=0
E_NG=1

function dump_env() {
    local items=("PROJECT_DIR" "PROJECT_NAME" $@)

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

function dump_command()
{
    args=$@

    printf "Running: %s\n" "${args[*]}"
}

function run_command()
{
    args=$@

    if [[ "$DRY_RUN" == "$NO" ]]
    then
        ${args[@]}
    fi
}

function dump_and_run_command()
{
    args=$@

    dump_command ${args[@]}
    run_command ${args[@]}
}

THIS_NAME="$(basename "${BASH_SOURCE[1]}")"

CXX_COMPILER_EXEC=$(which g++)
CXX_COMPILER_VERSION="$(check_tool_version "g++" "head -n1 | awk '{print \$4}'")"

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

REMOTE_NAME="$(git ls-remote --get-url origin | xargs basename | cut -d'.' -f1)"
