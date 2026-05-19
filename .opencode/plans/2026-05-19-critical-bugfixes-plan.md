# Perseus Critical Bug Fix Plan

## Overview
Fix 16 issues across the Perseus codebase, organized in 2 phases.

## Phase 1: Critical Bugs (7 items)

### Fix 1: `LosFileSystem::createFile()` — Uninitialized QFileInfo
- **File:** `src/core/LosFileSystem/LosFileSystem.cpp`
- **Current:**
  ```cpp
  QFileInfo fileInfo;
  QString dirPath = fileInfo.absolutePath();
  ```
- **Fix:**
  ```cpp
  QFileInfo fileInfo(file_path);
  QString dirPath = fileInfo.absolutePath();
  ```

### Fix 2: `LosToolChainManager::parseToolNode()` — LOS_language Never Set
- **File:** `src/core/LosToolChainManager/LosToolChainManager.cpp`
- Need to determine how to get language info. Read the file first.
- **Approach:** Either pass language from caller or infer from tool config JSON.

### Fix 3: `LosLspClient::didClose()` — Inverted Condition
- **File:** `src/core/LosLsp/LosLspClient/LosLspClient.cpp`
- **Current:** `if (!file_path.isEmpty()) return;`
- **Fix:** `if (file_path.isEmpty()) return;`

### Fix 4: `LosLspPython` Constructor — Missing Base Class Call
- **File:** `src/core/LosLsp/LosLspPython/LosLspPython.cpp`
- **Current:** No initializer list calling `LosLspClient(parent)`
- **Fix:** `LosLspPython::LosLspPython(QObject *parent) : LosLspClient(parent)`

### Fix 5: `LosScriptRunner` — Signal/Slot Parameter Order Mismatch
- **File:** `src/core/LosRunner/LosScriptRunner/LosScriptRunner.cpp`
- **Signal:** `_cmd_runScript(working_dir, script_path, args)`
- **Current lambda:** `(script_path, working_dir, args)`
- **Fix:** Reorder to `(working_dir, script_path, args)`

### Fix 6: `LosIssuesUi` — insertRow Inside setRowCount Loop
- **File:** `src/view/LosIssuesUi/LosIssuesUi.cpp`
- **Fix:** Remove `L_table->insertRow(i);` from the loop

### Fix 7: `LosPluginDetailUi` — Missing Comma in unzip Arguments
- **File:** `src/view/LosPluginDetailUi/LosPluginDetailUi.cpp`
- **Current:** `"UTF-8""-o"`
- **Fix:** `"UTF-8", "-o"`

## Phase 2: High-Severity Issues (9 items)

### Fix 8: LosFileSystem — Signal Before Write
- **File:** `src/core/LosFileSystem/LosFileSystem.cpp`
- Move `emit _cmd_fileSystemChanged()` after `file.write()` / `file.close()`

### Fix 9: LosRunManager — Missing break
- **File:** `src/core/LosRunner/LosRunManager/LosRunManager.cpp`
- Add `break;` before `default:` in PYTHON case

### Fix 10: Missing errorOccurred on QProcess
- **Files:** LosCmakeRunner.cpp, LosPythonRunner.cpp, LosRustcRunner.cpp, LosScriptRunner.cpp
- Add `connect(process, &QProcess::errorOccurred, ...)` with logging

### Fix 11: No State Check Before start()
- **Files:** LosSingleCppRunner.cpp, LosRustcRunner.cpp, LosCmakeRunner.cpp
- Guard with `if (state() == QProcess::NotRunning)`

### Fix 12: stop() Without waitForFinished()
- **Files:** LosSingleCppRunner.cpp, LosRustcRunner.cpp, LosScriptRunner.cpp
- Add `waitForFinished(1000)` after `kill()`

### Fix 13: LosConfigCMake — Multiple Processes
- **File:** `src/core/LosConfig/LosConfigCMake/LosConfigCMake.cpp`
- Kill previous process before creating new one

### Fix 14: LosConfigManager — Leaks Old Config
- **File:** `src/core/LosConfig/LosConfigManager/LosConfigManager.cpp`
- `delete LOS_config;` before reassignment

### Fix 15: LosTerminalUi — initConnect Never Called
- **File:** `src/view/LosTerminalUi/LosTerminalUi.cpp`
- Add `initConnect();` in constructor

### Fix 16: LosRouter — Orphan Signal
- **File:** `src/view/LosToolMissUi/LosToolMissUi.cpp` / signal routing
- Connect `_cmd_manuallySet` to appropriate handler
