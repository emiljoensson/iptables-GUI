#!/bin/sh
python example.py
read n
echo $n > "log_test.txt"
if [ ! -z "$n" ];
then
    echo "success"
    echo $n
    exit 0;
else
    echo "failed"
    exit 1;
fi
