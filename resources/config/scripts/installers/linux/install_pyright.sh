#!/bin/bash
# Copyright (c) 2026 LosAngelous (shengjie.lin)

echo "==== Starting Pyright Installation ===="
if ! command -v npm &> /dev/null; then
    echo "Error: npm is not installed. Please install Node.js first."
    exit 1
fi
echo "Installing pyright globally via npm..."
sudo npm install -g pyright
echo "==== Installation Finished ===="

