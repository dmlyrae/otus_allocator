#!/bin/bash

# путь к исполняемому файлу
EXECUTABLE_PATH=$1

# ожидаемая вер
EXPECTED_VERSION=$2

if [ -z "$EXECUTABLE_PATH" ] || [ -z "$EXPECTED_VERSION" ]; then
    echo "Usage: $0 <path_to_executable> <expected_version>"
    exit 1
fi

OUTPUT=$($EXECUTABLE_PATH)

# проверяем, содержит ли вывод вер
if echo "$OUTPUT" | grep -q "Version: $EXPECTED_VERSION"; then
    echo "Test 1: Version check passed"
    exit 0
else
    echo "Test 1: Version check failed!"
    echo "Expected version: $EXPECTED_VERSION"
    echo "Got output:"
    echo "$OUTPUT"
    exit 1
fi