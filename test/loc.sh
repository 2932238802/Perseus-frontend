#!/bin/bash

GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
BOLD='\033[1m'
NC='\033[0m'

PROJECT_ROOT="$(cd "$(dirname "$0")/.." && pwd)"
SRC_DIR="$PROJECT_ROOT/src"

total_lines=0
total_blank=0
total_comment=0
total_effective=0
total_files=0

declare -A cat_files cat_effective cat_total

count_file() {
    local file="$1"
    local lines=0 blank=0 comment=0 in_block=0
    while IFS= read -r line || [[ -n "$line" ]]; do
        ((lines++))
        trimmed="${line#"${line%%[![:space:]]*}"}"

        if [[ -z "$trimmed" ]]; then
            ((blank++))
            continue
        fi

        if [[ $in_block -eq 1 ]]; then
            ((comment++))
            [[ "$trimmed" == *"*/" ]] && in_block=0
            continue
        fi
        if [[ "$trimmed" == "/*"* ]]; then
            ((comment++))
            [[ "$trimmed" != *"*/" ]] && in_block=1
            continue
        fi

        if [[ "$trimmed" == "//"* ]]; then
            ((comment++))
            continue
        fi

    done < "$file"

    local effective=$((lines - blank - comment))

    total_lines=$((total_lines + lines))
    total_blank=$((total_blank + blank))
    total_comment=$((total_comment + comment))
    total_effective=$((total_effective + effective))
    total_files=$((total_files + 1))

    local category
    case "$file" in
        */common/*)  category="common"  ;;
        */core/*)    category="core"    ;;
        */models/*)  category="models"  ;;
        */view/*)    category="view"    ;;
        *)           category="root"    ;;
    esac

    cat_files[$category]=$(( ${cat_files[$category]:-0} + 1 ))
    cat_effective[$category]=$(( ${cat_effective[$category]:-0} + effective ))
    cat_total[$category]=$(( ${cat_total[$category]:-0} + lines ))
}

echo -e "${BLUE}========================================================${NC}"
echo -e "${BLUE}       Perseus — 有效代码行数统计                  ${NC}"
echo -e "${BLUE}========================================================${NC}\n"

files=$(find "$SRC_DIR" -type f \( -name "*.cpp" -o -name "*.h" -o -name "*.ui" \) | sort)

for f in $files; do
    count_file "$f"
done

echo -e "${BOLD}  模块明细${NC}"
echo -e "  ─────────────────────────────────────────────"
printf "  ${CYAN}%-12s${NC}  %5s  %8s  %8s\n" "模块" "文件" "有效行" "总行数"
echo -e "  ─────────────────────────────────────────────"

for cat in root common core models view; do
    [[ -z "${cat_files[$cat]}" ]] && continue
    label="$cat"
    case "$cat" in
        root)    label="src/"       ;;
        common)  label="common/"    ;;
        core)    label="core/"      ;;
        models)  label="models/"    ;;
        view)    label="view/"      ;;
    esac
    printf "  ${GREEN}%-12s${NC}  %5d  %8d  %8d\n" \
        "$label" "${cat_files[$cat]}" "${cat_effective[$cat]}" "${cat_total[$cat]}"
done

echo -e "  ─────────────────────────────────────────────"

echo ""
echo -e "${BOLD}  汇总${NC}"
echo -e "  ─────────────────────────────────────────────"
printf "  文件数        ${GREEN}%d${NC}\n" "$total_files"
printf "  总行数        %d\n" "$total_lines"
printf "  空行          %d\n" "$total_blank"
printf "  注释行        %d\n" "$total_comment"
printf "  ${YELLOW}有效代码行    ${BOLD}%d${NC}\n" "$total_effective"
echo -e "  ─────────────────────────────────────────────"
echo ""
