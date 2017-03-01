#!/usr/bin/env bash

set -e
set -o pipefail

CLANG_TIDY=${CLANG_TIDY:-$(scripts/mason.sh PREFIX clang-tidy VERSION 3.9.1)/bin/clang-tidy}
CLANG_FORMAT=${CLANG_FORMAT:-$(scripts/mason.sh PREFIX clang-format VERSION 3.9.1)/bin/clang-format}

command -v ${CLANG_TIDY} >/dev/null 2>&1 || {
    echo "Can't find ${CLANG_TIDY} in PATH."
    if [ -z ${CLANG_TIDY} ]; then
        echo "Alternatively, you can set CLANG_TIDY to point to clang-tidy."
    fi
    exit 1
}

cd $1

CDUP=$(git rev-parse --show-cdup)

function check_tidy() {
    echo "Running clang-tidy on $0..."
    if [[ -n $1 ]] && [[ $1 == "--fix" ]]; then
        OUTPUT=$(${CLANG_TIDY} -p=$PWD -fix -fix-errors ${0} 2>/dev/null)
    else
        OUTPUT=$(${CLANG_TIDY} -p=$PWD ${0} 2>/dev/null)
    fi
    if [[ -n $OUTPUT ]]; then
        echo "Caught clang-tidy warning/error:"
        echo -e "$OUTPUT"
        exit 1
    fi
}

function check_format() {
    echo "Running clang-format on $0..."
    ${CLANG_FORMAT} -i ${CDUP}/$0
}

export CLANG_TIDY CLANG_FORMAT
export -f check_tidy check_format

echo "Running clang checks... (this might take a while)"

if [[ -n $2 ]] && [[ $2 == "--diff" ]]; then
    DIFF_FILES=$(for file in `git diff origin/master --name-only | grep "pp$"`; do echo $file; done)
    if [[ -n $DIFF_FILES ]]; then
        echo "${DIFF_FILES}" | xargs -I{} -P ${JOBS} bash -c 'check_tidy --fix' {}
        # XXX disabled until we run clang-format over the entire codebase.
        #echo "${DIFF_FILES}" | xargs -I{} -P ${JOBS} bash -c 'check_format' {}
        git diff --quiet || {
            echo "Changes were made to source files - please review them before committing."
            exit 1
        }
    fi
    echo "All looks good!"
else
    git ls-files "${CDUP}/src/mbgl/*.cpp" "${CDUP}/platform/*.cpp" "${CDUP}/test/*.cpp" | \
        xargs -I{} -P ${JOBS} bash -c 'check_tidy' {}
fi
