#!bin/bash

if [[ -z "${PILOOP_MODE}" ]];then 
  echo "configure first"
fi

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
cd $SCRIPT_DIR && cd ../build
cmake --build .
mv piloop ../