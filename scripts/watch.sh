#!/bin/bash

[[ -z $1 ]] && {
    echo "watch.sh requires a directory argument"
    exit 1
}

DIR="$1"
[[ ! -e "${DIR}" ]] && {
    echo "Directory '${DIR}' does not exist"
    exit 1
}

EXT_TYPE=${2:-""}
[[ -z "${EXT_TYPE}" ]] && {
    echo "File extension type required (cxx, h)"
    exit 1
}

EXE_TARGET=${3:-""}
[[ -z "${EXE_TARGET}" ]] && {
    echo "A valid CMAKE target executable is required to watch"
    exit 1
}

EXE=$(find . -iregex ".*${EXE_TARGET}")
[[ -z "${EXE}" ]] && {
    echo "Could not find CMAKE target ${EXE_TARGET}"
    exit 1
}

echo ""
echo "Run Command:"
echo "find ${DIR} -iregex \".*[${EXT_TYPE}]\" | entr -s 'cmake -S. -Bbuild && cmake --build build -t "${EXE_TARGET}" -- && clear && "${EXE}"'"
# find ${DIR} -iregex ".*[${EXT_TYPE}]" | entr -s 'cmake -S. -Bbuild && cmake --build $(pwd)/build -t "${EXE_TARGET}" -- && "${EXE}"' 
