#!/bin/bash

cd "$(dirname $0)"

emo='../emo'

# running without any arguments should fail and print usage
printf "Testing \"emo\" without any arguments..."
"$emo" >/dev/null 2>&1
if test $? -eq 0; then
    printf "FAIL\n"
else
    printf "SUCCESS\n"
fi

# run tests on valid .emo files
for f in ./emo-files/valid/*; do
    printf "Testing \"emo $f\"..."
    "$emo" "$f" >/dev/null 2>&1
    if test $? -ne 0; then
        printf "FAIL\n"
    else
        printf "SUCCESS\n"
    fi
done

# run tests on invalid .emo files
for f in ./emo-files/invalid/*; do
    printf "Testing \"emo $f\"..."
    "$emo" "$f" >/dev/null 2>&1
    if test $? -eq 0; then
        printf "FAIL\n"
    else
        printf "SUCCESS\n"
    fi
done
