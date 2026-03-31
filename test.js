const { spawn } = require('child_process');

const lsp = spawn('./tools/linux/neocmakelsp', ['stdio']);

let stage = 0;
lsp.stdout.on('data', (data) => {
    console.log(`STDOUT: ${data.toString()}`);
    if (stage === 0 && data.includes('capabilities')) {
        stage = 1;
        // Send initialized
        let initz = '{"jsonrpc":"2.0","method":"initialized","params":{}}';
        lsp.stdin.write(`Content-Length: ${initz.length}\r\n\r\n${initz}`);
        
        let open = '{"jsonrpc":"2.0","method":"textDocument/didOpen","params":{"textDocument":{"uri":"file:///tmp/CMakeLists.txt","languageId":"cmake","version":1,"text":"proj"}}}';
        lsp.stdin.write(`Content-Length: ${open.length}\r\n\r\n${open}`);

        let comp = '{"jsonrpc":"2.0","id":2,"method":"textDocument/completion","params":{"textDocument":{"uri":"file:///tmp/CMakeLists.txt"},"position":{"line":0,"character":4}}}';
        lsp.stdin.write(`Content-Length: ${comp.length}\r\n\r\n${comp}`);
    } else if (stage === 1) {
        console.log("Got response!");
        process.exit(0);
    }
});

lsp.stderr.on('data', (data) => {
    console.log(`STDERR: ${data.toString()}`);
});

let init = '{"jsonrpc":"2.0","id":1,"method":"initialize","params":{"processId":null,"rootUri":"file:///tmp","capabilities":{}}}';
lsp.stdin.write(`Content-Length: ${init.length}\r\n\r\n${init}`);

