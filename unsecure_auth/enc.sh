#!/bin/bash

openssl enc -aes-256-cbc -salt -in ${1} -out ${1}.enc

if [ $? -eq 0 ]; then
    rm ${1}
fi

