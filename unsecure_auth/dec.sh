#!/bin/bash

IN=${1}
OUT="${IN%.*}"

openssl enc -d -aes-256-cbc -in ${IN} -out "${OUT}" 
if [ $? -eq 0 ]; then
    rm ${IN}
fi

