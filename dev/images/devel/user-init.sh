#!/bin/bash
USER=dev
DIR_USER_HOME=/home/$USER

[[ ! -e "${DIR_USER_HOME}" ]] && { mkdir -p ${DIR_USER_HOME}/BinMorph/{core,tests,thirdparty,examples} ; } ;

[[ $(cat /etc/passwd | grep dev) != 0 ]] && {
    echo "Creating user: ${USER}"
    echo "${USER}:x:1000:1000:${USER}:${DIR_USER_HOME}:/bin/bash" >> /etc/passwd
}
