#!/bin/bash

THIS_DIR="$(realpath "$(dirname "${BASH_SOURCE[0]}")")"
THIS_NAME="$(basename "${BASH_SOURCE[0]}")"

source "${THIS_DIR}/env.sh"

function print_usage()
{
    echo "Usage: $THIS_NAME [OPTIONS]"
    echo "Build Conan package."
    echo ""
    echo "    --host-profile      Conan host profile."
    echo "    --build-profile     Conan build profile."
    echo "    -f, --force         Force rebuild and recreate package."
    echo "    -d, --dry-run       Run the command without execute anything."
    echo "    -h, --help          Show this help."
    echo ""
    echo "Example:"
    echo "    $THIS_NAME --dry-run"
    echo ""
}

function usage()
{
    print_usage
}

function resolve_profile()
{
    local profile="$1"
    if [[ -f "$profile" ]]
    then
        echo "$profile"
    elif [[ -f "${PROJECT_DIR}/conan_profiles/${profile}" ]]
    then
        echo "${PROJECT_DIR}/conan_profiles/${profile}"
    else
        echo "$profile"
    fi
}

DEFAULT_HOST_PROFILE="release/gcc13_x86_64"
DEFAULT_BUILD_PROFILE="release/gcc13_x86_64"
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

if [[ -z "$CONAN_EXEC" ]]
then
    echo "Conan is required"
    exit $E_NG
fi

ENVS=(CONAN_VERSION HOST_PROFILE BUILD_PROFILE)
ENVS+=(FORCE_REBUILD DRY_RUN EXTRA_OPTIONS)
dump_env ${ENVS[@]}

if [[ "$FORCE_REBUILD" == "$YES" ]]
then
    dump_and_run_command "$CONAN_EXEC" remove -c "connix*"
fi

RESOLVED_HOST_PROFILE="$(resolve_profile "$HOST_PROFILE")"
RESOLVED_BUILD_PROFILE="$(resolve_profile "$BUILD_PROFILE")"

CONAN_ARGS=("-pr:h=$RESOLVED_HOST_PROFILE")
CONAN_ARGS+=("-pr:b=$RESOLVED_BUILD_PROFILE")
CONAN_ARGS+=("--build=missing")
if [[ ${#EXTRA_OPTIONS[@]} -gt 0 ]]
then
    CONAN_ARGS+=("${EXTRA_OPTIONS[@]}")
fi

dump_and_run_command "$CONAN_EXEC" create "$PROJECT_DIR" ${CONAN_ARGS[@]}
