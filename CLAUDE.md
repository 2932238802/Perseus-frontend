# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

**Perseus** is a C++20 desktop IDE built with Qt 6 (Widgets, Network, Concurrent, WebEngineWidgets, WebChannel). It supports editing, running, and LSP-powered analysis for C++, Rust, Python, and CMake projects. The build system uses CMake + Ninja with Clang-18.

---

## Commands

### Environment Setup (first time)
```bash
./env.sh
```
Installs all system dependencies (Qt6, Clang-18, CMake, Ninja, clangd, etc.) via apt.

### Build & Run
```bash
./re-build.sh   # Full clean rebuild (deletes _build, reconfigures, builds, runs)
./build.sh      # Incremental build from existing _build, then runs
./gdb-build.sh  # Debug build (no Release flag)
./clean.sh      # Delete _build only
./_build/Perseus  # Run without rebuilding
```

`re-build.sh` is the canonical first-build command. It configures with:
```
cmake .. -G Ninja -DCMAKE_C_COMPILER=/usr/bin/clang-18 -DCMAKE_CXX_COMPILER=/usr/bin/clang++-18
```

### Testing
```bash
cd _build && ctest --output-on-failure       # Run all tests
cd _build && ctest -R <TestName> -V          # Run a single test (regex match, verbose)
cmake --build _build --target LosTest        # Build only the test target
```
Tests live in `test/LosTest.cpp` and link against the `PerseusCore` static library.

### Formatting
Run after every code change — CI enforces `.clang-format`:
```bash
clang-format -i path/to/file.cpp path/to/file.h
```

### Code Stats
```bash
./test/loc.sh   # Count effective lines of code by module
```

---

## Architecture

### Layer Separation

| Directory | Namespace | Role |
|-----------|-----------|------|
| `src/view/` | `LosView` | Qt UI widgets and `.ui` forms only — no business logic |
| `src/core/` | `LosCore` | All business logic, managers, runners, LSP clients |
| `src/models/` | `LosModel` | Data structures (`LosFilePath`, `LosFileNode`, `LosFileTreeModel`, `LosFileContext`) |
| `src/common/` | `LosCommon` | Constants, enums, structs, utility functions |

`Perseus` (root `QMainWindow`) owns and wires all core managers. Never put business logic in `LosView` classes.

### Key Singletons

- **`LosRouter`** — central event bus. All cross-component communication goes through its Qt signals. Never call a core service directly from a view; emit a `LosRouter` signal instead.
- **`LosState`** — thread-safe key-value store (`QHash<SG_STR, QVariant>`) for global state such as the current project path (`SG_STR::PROJECT_DIR`).
- **`LosLog`** — logging singleton with convenience macros: `INF(msg, cmp)`, `WAR(msg, cmp)`, `ERR(msg, cmp)`, `SUC(msg, cmp)`.
- **`LosNet`** — Qt network singleton for plugin listing and download.

### Core Subsystems

- **`LosRunManager`** — dispatches file execution to the correct runner (`LosSingleCppRunner`, `LosRustcRunner`, `LosPythonRunner`, `LosCmakeRunner`), each extending `LosAbstractRunner`.
- **`LosLspManager`** — owns one `LosLspClient` per language (Clangd, rust-analyzer, neocmakelsp). Clients communicate via `QProcess` using the LSP JSON-RPC protocol; pending requests are queued in `L_pendings` until the server is ready.
- **`LosToolChainManager`** — validates and locates toolchain executables from a JSON config; emits `_cmd_toolChainReady` / `_cmd_toolChainMissing` signals via `LosRouter`.
- **`LosConfigManager`** — detects the project type (e.g., CMake) via `LosConfigFactory` and runs analysis; triggers LSP workspace setup.
- **`LosSession`** — persists and restores open files, active tab, and project directory across launches.

### UI Subsystem

- **`LosEditorTabUi`** — manages the tab widget; maps file paths to `LosEditorUi` instances.
- **`LosEditorUi`** — `QPlainTextEdit` subclass with: line numbers (`LosLineNumberUi`), syntax highlighting (`LosHighlighter`), LSP completions (`LosCompleterUi`), hover tooltips, diagnostic underlines, and format-on-demand.

---

## Code Conventions

### Naming

| Entity | Convention | Example |
|--------|-----------|---------|
| Classes / Structs | `PascalCase` | `LosRunManager` |
| Methods / Functions | `camelCase` | `initConnect()` |
| Plain member variables | `L_` prefix | `L_timer`, `L_dirty` |
| Owned service-object members | `LOS_` prefix | `LOS_runMgr`, `LOS_tabUi` |
| Constants / Macros | `UPPER_SNAKE_CASE` | `MAX_RETRIES` |

> Note: The codebase uses `L_` / `LOS_` prefixes, **not** `m_` as described in `AGENTS.md`.

### Formatting Rules (`.clang-format`)
- LLVM base style, 120-column limit
- 4-space indentation, tabs forbidden
- Allman brace style (opening brace on its own line)
- Pointer alignment: right (`Type *ptr`)
- Includes sorted alphabetically

### Include Order
1. Corresponding header (`"Foo.h"` first in `Foo.cpp`)
2. Local project headers (`"core/..."`, `"models/..."`)
3. Qt headers (`<QString>`)
4. Standard C++ headers (`<memory>`)

Prefer forward declarations in `.h` files; include the full header only in `.cpp`.

### Memory Management
- `QObject`-derived types: pass `this` as parent — Qt object tree handles cleanup.
- Non-`QObject` types: use `std::unique_ptr` / `std::shared_ptr`.
- Avoid raw `new`/`delete` unless immediately handing ownership to a Qt parent or smart pointer.

### Error Handling
- Use early returns to avoid nesting; guard with `qWarning()` / `ERR()` before returning.
- Prefer `bool` / `std::optional` return values over exceptions (Qt event loops and exceptions don't mix well).
- Never assume network calls succeed; connect to `errorOccurred` or `finished` signals.
