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
    length = int(line.split(":")[1].strip())
    proc.stdout.readline() # \r\n
    body = proc.stdout.read(length).decode('utf-8')
    return body

proc = subprocess.Popen(['./tools/linux/neocmakelsp', 'stdio'], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

send(proc, '{"jsonrpc":"2.0","id":1,"method":"initialize","params":{"processId":null,"rootUri":"file:///tmp","capabilities":{}}}')
read(proc)

send(proc, '{"jsonrpc":"2.0","method":"initialized","params":{}}')
send(proc, '{"jsonrpc":"2.0","method":"textDocument/didOpen","params":{"textDocument":{"uri":"file:///tmp/CMakeLists.txt","languageId":"cmake","version":1,"text":"set(A 1)\n\n\n\nset(B 2)"}}}')
send(proc, '{"jsonrpc":"2.0","id":2,"method":"textDocument/formatting","params":{"textDocument":{"uri":"file:///tmp/CMakeLists.txt"},"options":{"tabSize":4,"insertSpaces":true}}}')

while True:
    res = read(proc)
    if "id" in json.loads(res) and json.loads(res)["id"] == 2:
        print("Final:", res)
        break
    
proc.terminate()
