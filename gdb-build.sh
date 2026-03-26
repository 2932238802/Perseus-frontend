PROJECT_NAME="Perseus"
BUILD_NAME="_build"
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

echo -e " ${GREEN} 开始编译 ... ${NC}"
sudo rm -rf _build
mkdir _build && cd _build && cmake .. -G Ninja -DCMAKE_C_COMPILER:FILEPATH=/usr/bin/clang-18 -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/clang++-18
cmake --build .
"./${PROJECT_NAME}"