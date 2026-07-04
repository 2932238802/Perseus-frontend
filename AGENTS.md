# AGENTS.md — Perseus IDE

Perseus is a Qt 6 / C++23 desktop IDE. This file captures only the repo-specific
things an agent will otherwise get wrong. Full conventions live in
[`docs/AGENTS.md`](docs/AGENTS.md); architecture in [`docs/1_architecture/`](docs/1_architecture/).

## Build & run — use the shell scripts, not raw cmake

All build scripts live at repo root, run from repo root, use build dir `_build/`
(leading underscore), and **auto-run `./_build/Perseus` after building**. There
is no separate "just build" script.

- `./re-build.sh` — clean + configure + Release build + run.
  Runs `sudo rm -rf _build`, then `cmake .. -G Ninja` with
  **`CMAKE_C_COMPILER=/usr/bin/clang-18`** and **`clang++-18`** hard-coded.
- `./rbuild.sh` — incremental Release build + run (assumes `_build/` exists).
- `./bbuild.sh` — incremental Debug build + run.
- `./gdb-build.sh` — clean Debug rebuild + run (for GDB).
- `./clean.sh` — remove build artifacts.
- `./env.sh` — one-shot apt install of the whole toolchain on Ubuntu/WSL2.

Do not invent your own `cmake -B build` invocation: the CI workflow uses
`build/` but every local script uses `_build/`. Match the scripts locally.

Configure step is only in `re-build.sh` / `gdb-build.sh`; the incremental
scripts skip configure, so after touching `CMakeLists.txt` you need a full
rebuild.

## Hard prerequisites

- **`git submodule update --init --recursive`** after clone. `third_party/libgit2`
  is a submodule and CMake configure fails without it (see
  `CMakeLists.txt:51` — `add_subdirectory(third_party/libgit2)`).
- **Clang-18** specifically. The scripts hard-code `/usr/bin/clang-18`; gcc or
  other clang versions are not tested and code uses `std::stacktrace` linked
  via `stdc++exp` (`CMakeLists.txt:56`), a libstdc++ experimental symbol.
- **Qt 6** with `Widgets Network Concurrent WebEngineWidgets WebChannel Test
  Core5Compat` — installed by `env.sh`. `Core5Compat` is required (Qt5
  compatibility shim); missing it is a common failure mode.
- Ubuntu 24.04 / WSL2 is the supported dev environment. CI builds
  Windows/MSVC + Qt 6.4.2 (`.github/workflows/build.yml`), and the Windows
  binary is dropped in `build/`, not `_build/`.

## Tests

- One test target: `LosTest` (currently a `hello world` stub in
  `test/LosTest.cpp`). Do not assume Qt Test coverage is meaningful yet.
- Run: `cd _build && ctest --output-on-failure`.
- Focused: `cd _build && ctest -R <name> -V`.
- `test/` is built inside the main tree (`enable_testing()` +
  `add_subdirectory(test)` in root `CMakeLists.txt`), so a full project build
  builds tests too.

## Sources are globbed

`CMakeLists.txt:30` uses `file(GLOB_RECURSE ...)` over `src/{common,core,models,view}`
and `src/*.{h,cpp}`. **Adding a new source file requires re-running the
configure step** (i.e. `./re-build.sh`, not `./rbuild.sh`), or Ninja will not
pick it up. AUTOUIC search path is `src/view/` — put `.ui` files there.

## Formatting

Use `.clang-format` at repo root — do not follow the settings in
`docs/AGENTS.md` verbatim, they are partially stale. The authoritative values:

- `ColumnLimit: 150` (docs claim 120 — wrong)
- `IndentWidth: 4`, `UseTab: Never`, `BreakBeforeBraces: Allman`,
  `PointerAlignment: Right`, `SortIncludes: true`.

Format before committing: `clang-format -i <files>`. Do not reformat
`third_party/` — it is submodule code and intentionally excluded from house
style.

## Naming (differs from typical C++ / Qt)

Perseus uses its own member-variable prefix scheme; do **not** default to `m_`.

- Plain member variables: `L_` prefix (e.g. `L_timer`).
- Service-object / manager members: `LOS_` prefix (e.g. `LOS_runMgr`).
- Classes: `PascalCase`, often `Los*` for core services (`LosRouter`,
  `LosGitManager`, `LosRunManager`, `LosTheme`, `LosNet`, `LosAgent`).
- Singletons are Meyers style: `LosXxx::instance().method()`.

## Architecture rules that constrain edits

- Cross-layer communication goes through the `LosRouter` signal bus. UI code
  in `src/view/` must not `#include` core services directly; wire via signals.
- Long-running work must be async / on a worker thread. The main thread is
  UI-only.
- `src/Perseus.{h,cpp}` is the `QMainWindow` that assembles all modules —
  start here when tracing wiring, not `main.cpp`.

## Instruction files

- [`docs/AGENTS.md`](docs/AGENTS.md) — long-form coding rules (naming, include
  order, error handling, UI/Qt conventions). Follow it, but prefer `.clang-format`
  and this file where they conflict.
- [`docs/1_architecture/`](docs/1_architecture/) — module map, signal bus,
  design patterns.
- [`docs/3_development/setup.md`](docs/3_development/setup.md) — setup detail.
- `.opencode/` contains only an `@opencode-ai/plugin` dependency; there is no
  `opencode.json` and no repo-local plugin config to honor.

## Commit / branch conventions

From [`README.md`](README.md):

- Branches: `dev-<feature>` off `main`; rebase onto `origin/main` before PR.
- Commit prefixes: `[add]`, `[fix]`, `[fit]` (polish), `[modify]` (refactor),
  `[del]`, `[docs]`.
