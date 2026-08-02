#!/bin/bash

THIS_DIR="$(realpath "$(dirname "${BASH_SOURCE[0]}")")"
THIS_NAME="$(basename "${BASH_SOURCE[0]}")"

source "${THIS_DIR}/env.sh"

function print_usage()
{
    echo "Usage: $THIS_NAME [OPTIONS]"
    echo "Build application with native profile."
    echo ""
    echo "    --host-profile      Conan host profile."
    echo "    --build-profile     Conan build profile."
    echo "    -f, --force         Force rebuild entire project."
    echo "    -d, --dry-run       Run the command without execute anything."
    echo "    -h, --help          Show this help."
    echo ""
    echo "Example:"
    echo "    $THIS_NAME --dry-run"
    echo ""
}

BUILD_DIR="${PROJECT_DIR}/build-native"

DEFAULT_HOST_PROFILE="debug/gcc13_x86_64"
DEFAULT_BUILD_PROFILE="debug/gcc13_x86_64"
DEFAULT_FORCE_REBUILD="$NO"
DEFAULT_DRY_RUN="$NO"

HOST_PROFILE="$DEFAULT_HOST_PROFILE"
BUILD_PROFILE="$DEFAULT_BUILD_PROFILE"
FORCE_REBUILD="$DEFAULT_FORCE_REBUILD"
DRY_RUN="$DEFAULT_DRY_RUN"
EXTRA_OPTIONS=()

while [[ $# -gt 0 ]]
do
    case "$1" in
        --host-profile)
            HOST_PROFILE="$2"
            shift 2
            ;;
        --build-profile)
            BUILD_PROFILE="$2"
            shift 2
            ;;
        -f|--force)
            FORCE_REBUILD="$YES"
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

ENVS=(CXX_COMPILER_VERSION CMAKE_VERSION NINJA_BUILD_VERSION BUILD_DIR)
ENVS+=(FORCE_REBUILD DRY_RUN EXTRA_OPTIONS)
dump_env ${ENVS[@]}

if [[ "$FORCE_REBUILD" == "$YES" ]]
then
    dump_and_run_command rm -rf "$BUILD_DIR"
    dump_and_run_command mkdir -p "$BUILD_DIR"
fi

CMAKE_VARIABLES=("-DCMAKE_BUILD_TYPE=Debug")
CMAKE_VARIABLES+=("-DBUILD_MOCKS=ON -DBUILD_TESTS=ON")
CMAKE_VARIABLES+=("-DCONAN_HOST_PROFILE=$HOST_PROFILE")
CMAKE_VARIABLES+=("-DCONAN_BUILD_PROFILE=$BUILD_PROFILE")

dump_and_run_command "$CMAKE_EXEC" ${CMAKE_VARIABLES[@]} \
    -G Ninja -S "$PROJECT_DIR" -B "$BUILD_DIR"

dump_and_run_command "$CMAKE_EXEC" --build "$BUILD_DIR" \
    --target "all" -- -j$(nproc)
