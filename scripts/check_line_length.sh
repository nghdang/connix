#!/bin/bash

THIS_DIR="$(realpath "$(dirname "${BASH_SOURCE[0]}")")"
THIS_NAME="$(basename "${BASH_SOURCE[0]}")"

source "${THIS_DIR}/env.sh"

function print_usage()
{
    echo "Usage: $THIS_NAME [OPTIONS] [PATHS...]"
    echo "Check files for lines exceeding maximum allowed length."
    echo ""
    echo "    --max-length <N>  Maximum line length allowed (default: 80)."
    echo "    --dry-run         Run the command without executing anything."
    echo "    --help            Show this help."
    echo ""
    echo "Default paths if none specified: connix docs"
    echo ""
    echo "Example:"
    echo "    $THIS_NAME"
    echo "    $THIS_NAME --max-length 80"
    echo "    $THIS_NAME path/to/file.puml"
    echo "    $THIS_NAME --dry-run"
    echo ""
}

DEFAULT_MAX_LENGTH=80
DEFAULT_DRY_RUN="$NO"

MAX_LENGTH="$DEFAULT_MAX_LENGTH"
DRY_RUN="$DEFAULT_DRY_RUN"
TARGET_PATHS=()

while [[ $# -gt 0 ]]
do
    case "$1" in
        --max-length)
            shift
            if [[ -z "$1" ]]
            then
                echo "Error: --max-length requires a positive integer argument"
                exit $E_NG
            fi
            MAX_LENGTH="$1"
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
            echo "Unknown option: $1"
            print_usage
            exit $E_NG
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
ENVS=(MAX_LENGTH TARGET_DIR)
ENVS+=(DRY_RUN)
dump_env ${ENVS[@]}

STATUS_CODE=$E_OK
TOTAL_FILES=0
TOTAL_VIOLATIONS=0

if [[ "$DRY_RUN" == "$YES" ]]
then
    exit $STATUS_CODE
fi

for target in "${TARGET_PATHS[@]}"
do
    if [[ ! -e "$target" ]]
    then
        echo "Warning: Target path does not exist: $target"
        continue
    fi

    file_list=()
    if [[ -f "$target" ]]
    then
        file_list=("$target")
    elif [[ -d "$target" ]]
    then
        while IFS= read -r f
        do
            file_list+=("$f")
        done < <(git -C "$PROJECT_DIR" ls-files --cached --others \
            --exclude-standard "$target")
    fi

    for file_path in "${file_list[@]}"
    do
        if [[ ! -f "$file_path" ]]
        then
            continue
        fi

        if file -b --mime "$file_path" | grep -q "charset=binary"
        then
            continue
        fi

        TOTAL_FILES=$((TOTAL_FILES + 1))

        # Check line length ignoring lines with long URLs
        violations=$(awk -v max_len="$MAX_LENGTH" -v file="$file_path" '
            length($0) > max_len {
                if ($0 !~ /https?:\/\//) {
                    printf "%s:%d: length %d exceeds %d: %s\n",
                        file, NR, length($0), max_len, $0
                }
            }
        ' "$file_path")

        if [[ -n "$violations" ]]
        then
            echo "$violations"
            violation_count=$(echo "$violations" | wc -l)
            TOTAL_VIOLATIONS=$((TOTAL_VIOLATIONS + violation_count))
        fi
    done
done

echo ""
echo "Checked $TOTAL_FILES files: $TOTAL_VIOLATIONS violations found."

if [[ $TOTAL_VIOLATIONS -gt 0 ]]
then
    STATUS_CODE=$E_NG
fi

exit $STATUS_CODE
