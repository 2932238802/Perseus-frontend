#!/usr/bin/env python3
"""
把 C++ 源文件中的所有注释统一转换为:
    /*
     * 内容
     */
支持:
  - /** ... */ 多行 Javadoc 注释
  - /* ... */  已有块注释 (仅去掉多余的 **)
  - // 单独成行的注释   → 转为块注释
  - code; // 行尾注释   → 转为 code; /* ... */
保留:
  - // clang-format off/on   (不处理)
  - #pragma once / #ifndef 等预处理行内注释中的特殊指令
"""

import re
import sys
import os

# ─────────────────────────────────────────────
# 工具函数
# ─────────────────────────────────────────────

def get_indent(line: str) -> str:
    """返回行首的空白字符串。"""
    return line[: len(line) - len(line.lstrip())]


def find_cpp_comment(line: str) -> int:
    """
    找到行中 // 注释的起始位置，忽略字符串字面量中的 //.
    返回位置索引；若不存在则返回 -1。
    """
    in_double = False
    in_single = False
    i = 0
    while i < len(line):
        c = line[i]
        # 转义字符
        if c == '\\' and (in_double or in_single):
            i += 2
            continue
        if c == '"' and not in_single:
            in_double = not in_double
        elif c == "'" and not in_double:
            in_single = not in_single
        elif not in_double and not in_single:
            if c == '/' and i + 1 < len(line) and line[i + 1] == '/':
                return i
        i += 1
    return -1


# ─────────────────────────────────────────────
# 块注释转换
# ─────────────────────────────────────────────

def collect_block(lines: list, start: int):
    """
    从 lines[start] 开始收集一段块注释（/* ... */），
    返回 (block_lines, next_index)。
    """
    block = []
    i = start
    while i < len(lines):
        block.append(lines[i])
        if '*/' in lines[i]:
            break
        i += 1
    return block, i + 1


def convert_block(block_lines: list, indent: str) -> list:
    """
    把一段块注释行列表转换为统一格式。
    若整个注释只有一行（/* text */），则保持单行形式 /* text */。
    """
    if not block_lines:
        return block_lines

    opening = block_lines[0].strip()

    # ── 单行块注释 /* text */  ──────────────────
    if len(block_lines) == 1:
        # 统一去掉 /** ... */ 的多余 *
        new = re.sub(r'^/\*\*+', '/*', opening)
        return [indent + new]

    # ── 多行块注释 ───────────────────────────────
    # 提取开头行携带的文字（/** some text 这种情况）
    first_text = re.sub(r'^/\*+\s*', '', opening).strip()

    # 提取中间内容行
    middle = block_lines[1:-1]

    # 提取结尾行（只关心有没有额外文字，通常是空的 */）
    # 不处理结尾行的额外内容（实际代码里很少见）

    content_texts = []
    if first_text:
        content_texts.append(first_text)

    for ml in middle:
        text = ml.strip()
        # 去掉行首的 * 前缀（如 " * foo" → "foo"）
        text = re.sub(r'^\*+\s?', '', text)
        content_texts.append(text)

    # 构建统一格式
    result = [indent + '/*']
    for t in content_texts:
        if t:
            result.append(indent + ' * ' + t)
        else:
            result.append(indent + ' *')
    result.append(indent + ' */')
    return result


# ─────────────────────────────────────────────
# 主转换函数
# ─────────────────────────────────────────────

# 不处理这些特殊指令
SKIP_PATTERNS = re.compile(
    r'clang-format|NOLINT|NOSONAR|IWYU|NOINSPECTION|pragma\s+once',
    re.IGNORECASE,
)


def convert_content(content: str) -> str:
    lines = content.split('\n')
    result = []
    i = 0

    while i < len(lines):
        line = lines[i]
        rline = line.rstrip()  # 去掉行尾空白，用于判断，但 append 时用 line

        indent = get_indent(rline) if rline.strip() else get_indent(line)
        stripped = rline.strip()

        # ── 块注释开头 /* 或 /** ─────────────────
        if re.match(r'^\s*/\*', rline):
            block, next_i = collect_block(lines, i)

            # 判断是否是单行 /* text */
            if len(block) == 1 and '*/' in block[0]:
                # 单行：只去掉多余的 **
                new_line = re.sub(r'/\*\*+', '/*', block[0].rstrip())
                result.append(new_line)
            else:
                converted = convert_block(block, indent)
                result.extend(converted)

            i = next_i
            continue

        # ── // 注释 ──────────────────────────────
        cpos = find_cpp_comment(rline)
        if cpos >= 0:
            before = rline[:cpos].rstrip()
            comment_text = rline[cpos + 2:].strip()

            # 特殊指令保留原样
            if SKIP_PATTERNS.search(comment_text):
                result.append(line)
                i += 1
                continue

            if not before.strip():
                # 单独成行的 // 注释
                # 尝试把连续的同缩进 // 注释合并为一个块注释
                texts = []
                if comment_text:
                    texts.append(comment_text)
                j = i + 1
                while j < len(lines):
                    nline = lines[j].rstrip()
                    ncpos = find_cpp_comment(nline)
                    if ncpos >= 0 and not nline[:ncpos].rstrip().strip():
                        ntext = nline[ncpos + 2:].strip()
                        # 检查是否特殊指令
                        if SKIP_PATTERNS.search(ntext):
                            break
                        if ntext:
                            texts.append(ntext)
                        else:
                            texts.append('')
                        j += 1
                    else:
                        break

                # 若任何一行注释文字包含 */ 则不能用块注释（会提前关闭），保留 //
                has_close = any('*/' in t for t in texts)
                if has_close:
                    for t in texts:
                        result.append(indent + '// ' + t if t else indent + '//')
                    i = j
                    continue

                result.append(indent + '/*')
                for t in texts:
                    if t:
                        result.append(indent + ' * ' + t)
                    else:
                        result.append(indent + ' *')
                result.append(indent + ' */')
                i = j
            else:
                # 行尾 inline 注释 → /* ... */ 或保留 //（若含 */）
                if comment_text:
                    if '*/' in comment_text:
                        # 含 */ 无法安全放入块注释，保留 //
                        result.append(rline)
                    else:
                        result.append(before + ' /* ' + comment_text + ' */')
                else:
                    result.append(before)
                i += 1

            continue

        # ── 普通代码行，原样保留 ──────────────────
        result.append(line)
        i += 1

    return '\n'.join(result)


# ─────────────────────────────────────────────
# 入口
# ─────────────────────────────────────────────

def process_file(path: str) -> None:
    with open(path, 'r', encoding='utf-8', errors='replace') as f:
        original = f.read()

    converted = convert_content(original)

    if converted != original:
        with open(path, 'w', encoding='utf-8') as f:
            f.write(converted)
        print(f'  converted: {path}')
    else:
        print(f'  no change: {path}')


if __name__ == '__main__':
    targets = sys.argv[1:]
    for t in targets:
        if os.path.isfile(t):
            process_file(t)
        elif os.path.isdir(t):
            for root, dirs, files in os.walk(t):
                # 跳过 build 目录
                dirs[:] = [d for d in dirs if d not in ('_build', 'build', '.git')]
                for fname in files:
                    if fname.endswith(('.cpp', '.h')):
                        process_file(os.path.join(root, fname))
