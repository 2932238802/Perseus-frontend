# Agent Instructions for Perseus

This file contains essential context, build instructions, and coding standards for any AI coding agents (like Cursor, Copilot, or CLI agents) operating within the `Perseus` repository.

## 1. Project Overview

**Perseus** is a C++17 application built using the Qt Framework (Qt 5 or Qt 6 compatible, utilizing `Widgets` and `Network` components). It uses CMake as its build system.

**Directory Structure:**
- `src/view/`: UI components, forms (`.ui`), and view-specific logic.
- `src/core/`: Core business logic and application orchestration.
- `src/models/`: Data representations and structures.
- `src/common/`: Shared utilities, constants, and helper functions.
- `resources/`: Qt resource files (`.qrc`).

Always respect this architectural separation of concerns. Do not put business logic inside UI classes.

---

## 2. Build, Run, and Test Commands

Agents must verify their work by building the project. Do not assume commands; use the following:

### Build & Run
- **Configure CMake:** 
  ```bash
  cmake -B _build -DCMAKE_BUILD_TYPE=Release
  ```
- **Build the Application:** 
  ```bash
  ./build.sh
  ```
  *(Alternatively: `cmake --build _build --config Release`)*
- **Clean and Rebuild:**
  ```bash
  ./re-build.sh
  ```
- **Run the Application:** 
  ```bash
  make run
  ```
  *(Alternatively: `./_build/Perseus`)*

### Testing
*Note: If a testing framework (like GoogleTest or QtTest) is integrated via CTest, use the following commands.*

- **Run all tests:**
  ```bash
  cd _build && ctest --output-on-failure
  ```
- **Run a single test (Crucial for isolated verification):**
  ```bash
  cd _build && ctest -R <ExactTestName> -V
  ```
  *(The `-R` flag uses a regex to match the test name, `-V` enables verbose output to see print statements/errors).*
- **Build a specific test target:**
  ```bash
  cmake --build _build --target <TestTargetName>
  ```

---

## 3. Linting and Formatting

This project enforces strict formatting using `.clang-format`.

- **Style Base:** LLVM
- **Key Rules:** 
  - 120 column limit.
  - 4-space indentation (Tabs are strictly forbidden: `UseTab: Never`).
  - Brace wrapping: `Allman` style (braces go on a new line).
  - Pointer alignment: Right (`Type *ptr`, not `Type* ptr`).

**Agent Action:** Before finalizing any code modification, you **MUST** run the formatter on the changed files:
```bash
clang-format -i path/to/modified_file.cpp path/to/modified_file.h
```

---

## 4. Code Style Guidelines

### Naming Conventions
- **Classes & Structs:** `PascalCase` (e.g., `UserModel`, `NetworkManager`).
- **Functions & Methods:** `camelCase` (e.g., `loadData()`, `parseResponse()`).
- **Variables:** `camelCase` (e.g., `requestPayload`, `itemCount`).
- **Member Variables:** Prefix with `m_` to distinguish from local variables (e.g., `m_apiClient`, `m_retryCount`).
- **Constants & Macros:** `UPPER_SNAKE_CASE` (e.g., `MAX_RETRIES`, `DEFAULT_TIMEOUT`).

### Imports and Includes
- Keep includes sorted alphabetically (enforced by `.clang-format`).
- **Include Order:**
  1. The corresponding header for the source file (e.g., in `Foo.cpp`, `#include "Foo.h"` goes first).
  2. Local project headers (e.g., `#include "models/UserModel.h"`).
  3. Qt Framework headers (e.g., `#include <QString>`, `#include <QNetworkReply>`).
  4. Standard C++ headers (e.g., `#include <memory>`, `#include <vector>`).
- Prefer forward declarations in header files (`class MyWidget;`) instead of `#include`ing the full header, to reduce compilation times. Only `#include` the full header in the `.cpp` file.

### Types and Variables
- Use `auto` when the type is blatantly obvious from the right-hand side (e.g., `auto widget = new QWidget();`, `auto it = map.begin();`). Otherwise, use explicit types for readability.
- Prefer standard fixed-width integer types (`#include <cstdint>`, `int32_t`, `uint64_t`) or Qt's sized types (`qint32`) when data size matters (e.g., network serialization).
- Prefer Qt containers (`QString`, `QList`, `QVector`) when interacting with Qt APIs, but standard C++ containers (`std::vector`, `std::string`) are acceptable in `core/` if Qt dependency is avoided.

### Memory Management
- **Qt Object Tree:** For UI components or any `QObject`-derived classes, pass a `parent` pointer in the constructor to ensure automatic memory cleanup.
  ```cpp
  auto myButton = new QPushButton(this); // 'this' takes ownership
  ```
- **Smart Pointers:** For standard C++ classes or non-QObject data, strictly use smart pointers (`std::unique_ptr`, `std::shared_ptr`). 
- **Raw Pointers:** Avoid raw `new` and `delete` unless you are immediately handing ownership to a Qt parent or a smart pointer.

### Error Handling
- **Early Returns:** Avoid deep nesting. Check for invalid states or null pointers and return early.
  ```cpp
  if (!m_networkManager) {
      qWarning() << "Network manager is not initialized.";
      return;
  }
  ```
- **Qt Networking Errors:** Connect to the `errorOccurred` or `finished` signals of `QNetworkReply`. Never assume network calls will succeed.
- **Exceptions:** Since this is a Qt codebase, prefer returning error codes, boolean success flags, or `std::optional` for non-fatal errors rather than throwing standard C++ exceptions, as exception safety across Qt event loops can be complex.

### Comments & Documentation
- Write code that explains itself through clear naming.
- Use comments to explain **WHY** a piece of logic exists, especially for workarounds, complex math, or obscure business rules.
- Do not leave commented-out dead code. Delete it. Version control will remember it.

### UI and Qt Specifics
- Prefer `.ui` files for static layouts. Load them using `setupUi(this)`.
- Use Qt's **Signals and Slots** mechanism for communicating between decoupled components (e.g., passing data from a `NetworkManager` in `core/` to a `MainWindow` in `view/`). Do not tightly couple the UI to the backend logic.
- Ensure all heavy lifting (network parsing, file I/O) is done asynchronously or in a separate thread so the main GUI thread remains responsive.