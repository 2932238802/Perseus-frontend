PROJECT_NAME="Perseus"
BUILD_NAME="_build"
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'


cd ${BUILD_NAME}
cmake --build . --config Release
"./${PROJECT_NAME}"