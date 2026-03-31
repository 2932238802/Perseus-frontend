import subprocess
import json
import time

def send(proc, data):
    msg = f"Content-Length: {len(data)}\r\n\r\n{data}"
    proc.stdin.write(msg.encode('utf-8'))
    proc.stdin.flush()

def read(proc):
    line = proc.stdout.readline().decode('utf-8')
    if not line: return None
    print("GOT HEADER:", line.strip())
    length = int(line.split(":")[1].strip())
    proc.stdout.readline() # \r\n
    body = proc.stdout.read(length).decode('utf-8')
    print("GOT BODY:", body)
    return body

proc = subprocess.Popen(['./tools/linux/neocmakelsp', 'stdio'], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

init = '{"jsonrpc":"2.0","id":1,"method":"initialize","params":{"processId":null,"rootUri":"file:///tmp","capabilities":{}}}'
send(proc, init)
read(proc)

initz = '{"jsonrpc":"2.0","method":"initialized","params":{}}'
send(proc, initz)
open_doc = '{"jsonrpc":"2.0","method":"textDocument/didOpen","params":{"textDocument":{"uri":"file:///tmp/CMakeLists.txt","languageId":"cmake","version":1,"text":"project  (  test )"}}}'
send(proc, open_doc)
fmt = '{"jsonrpc":"2.0","id":2,"method":"textDocument/formatting","params":{"textDocument":{"uri":"file:///tmp/CMakeLists.txt"},"options":{"tabSize":4,"insertSpaces":true}}}'
send(proc, fmt)

while True:
    res = read(proc)
    if "id" in json.loads(res) and json.loads(res)["id"] == 2:
        break
    
proc.terminate()
