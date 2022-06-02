#!/usr/bin/env bash

set -o pipefail

function print_header() {
    echo -e "\n***** ${1} *****"
}

function check_log() {
    LOG=$( { ${1}; } 2>&1 )
    STATUS=$?
    echo "$LOG"
    if echo "$LOG" | grep -q -E "${2}"
    then
        exit 1
    fi

    if [ $STATUS -ne 0 ]
    then
        exit $STATUS
    fi
}


print_header "RUN cppcheck"
check_log "cppcheck graphs --enable=all --inconclusive --error-exitcode=1 -I graphs/include --suppress=missingIncludeSystem \
        --suppress=noExplicitConstructor --suppress=unusedFunction --suppress=useStlAlgorithm" "\(information\)"

print_header "RUN clang-tidy"
check_log "clang-tidy graphs/src/* graphs/tests/* -warnings-as-errors=* -- -x c++ -Igraphs/include -std=c++17" "Error (?:reading|while processing)"

print_header "RUN cpplint"
check_log "cpplint --extensions=cpp graphs/include/* graphs/src/* graphs/tests/*" "Can't open for reading"

print_header "SUCCESS"
