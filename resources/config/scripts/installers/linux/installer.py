#!/usr/bin/env python3
# Copyright (c) 2026 LosAngelous (shengjie.lin)


RC_OK                   = 0
RC_TOOL_MISSING         = 2      
RC_NO_PRIV_BACKEND      = 97      # 找不到提权的后端
RC_AUTH_CANCELLED       = 126    
RC_AUTH_FAILED          = 127 

import json, os, shutil, subprocess,sys, getpass

def emit(evt , **kw):
    kw["evt"] = evt
    # flush 可以 直接把结果输出出来
    # dumps 才是 字符串 
    print(json.dumps(kw,ensure_ascii=False,flush=True))
    
def runWithPrivilege(cmd:list):
    if shutil.which("pkexec"):
        emit("auth_request",backend="pkexec")
        rc = subprocess.call(["pkexec",*cmd])
        return rc
    emit("error",code=RC_NO_PRIV_BACKEND,msg="no pkexec on this system")
    return RC_NO_PRIV_BACKEND

def main():
    emit("progress", stage="check", msg="checking npm...")
    if shutil.which("npm") is None:
        emit("error", code=RC_TOOL_MISSING, msg="npm not found")
        sys.exit(RC_TOOL_MISSING)
    emit("progress", stage="install", msg="installing pyright...")
    rc = runWithPrivilege(["npm", "install", "-g", "pyright"])
    if rc == RC_OK:
        emit("done", code=RC_OK, tool="pyright")
    elif rc == RC_AUTH_CANCELLED:
        emit("error", code=rc, msg="authentication cancelled by user")
    elif rc == RC_AUTH_FAILED:
        emit("error", code=rc, msg="authentication failed")
    else:
        emit("error", code=rc, msg=f"install failed (rc={rc})")
    sys.exit(rc)
if __name__ == "__main__":
    main()