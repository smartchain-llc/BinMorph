#!/bin/bash
DIR_SRC=/home/orion/Code/scllc/BinMorph
PWD=$(pwd)
function configTemplate() {
    JSON="{
            \"name\": \"${1}\",
            \"type\": \"cppdbg\",
            \"request\": \"launch\",
            \"program\": \"\${workspaceFolder}/build/tests/${1}\",
            \"args\": [],
            \"stopAtEntry\": false,
            \"cwd\": \"\${fileDirname}\",
            \"environment\": [],
            \"externalConsole\": false,
            \"MIMode\": \"gdb\",
            \"setupCommands\": [
                {
                    \"description\": \"Enable pretty-printing for gdb\",
                    \"text\": \"-enable-pretty-printing\",
                    \"ignoreFailures\": true
                },
                {
                    \"description\": \"Set Disassembly Flavor to Intel\",
                    \"text\": \"-gdb-set disassembly-flavor intel\",
                    \"ignoreFailures\": true
                }
            ]
        },"
    echo "${JSON}"
    # cat scripts/launch_template.json | sed -i "s|(CONFIG)|${JSON}(CONFIG)|" 
    # var="replace"
    # cat scripts/launch_template.json | sed "s/(CONFIG)/${JSON}/"
    # echo "${JSON}" | sed "s/(CONFIG)/${var}/" scripts/test
}
[[ "${DIR_SRC}" != "${PWD}" ]] && { 
    cd "${DIR_SRC}"
    for TEST in "${DIR_SRC}"/build/tests/TEST*; do
        # echo "Test file: ${TEST##*/}"

        configTemplate "${TEST##*/}"
    done
}