#!/bin/bash

# путь к исполняемому файлу
EXECUTABLE_PATH=$1

if [ -z "$EXECUTABLE_PATH" ]; then
    echo "Usage: $0 <path_to_executable>"
    exit 1
fi

# код возврата
if ! OUTPUT=$($EXECUTABLE_PATH 2>&1); then
    echo "Test 1: Program execution failed!"
    echo "Got output:"
    echo "$OUTPUT"
    exit 1
fi

# наличие ключевых строк в выводе
CHECKS_PASSED=0
CHECKS_FAILED=0

if echo "$OUTPUT" | grep -q "Standard map contents"; then
    ((CHECKS_PASSED++))
else
    ((CHECKS_FAILED++))
    echo "FAIL: Missing 'Standard map contents'"
fi

if echo "$OUTPUT" | grep -q "Custom allocator map contents"; then
    ((CHECKS_PASSED++))
else
    ((CHECKS_FAILED++))
    echo "FAIL: Missing 'Custom allocator map contents'"
fi

if echo "$OUTPUT" | grep -q "Standard allocator container contents"; then
    ((CHECKS_PASSED++))
else
    ((CHECKS_FAILED++))
    echo "FAIL: Missing 'Standard allocator container contents'"
fi

# наличие вывода для кастомного аллокатора контейнера
if echo "$OUTPUT" | grep -q "Custom allocator container contents"; then
    ((CHECKS_PASSED++))
else
    ((CHECKS_FAILED++))
    echo "FAIL: Missing 'Custom allocator container contents'"
fi

if echo "$OUTPUT" | grep -q "0 1" && \
   echo "$OUTPUT" | grep -q "1 1" && \
   echo "$OUTPUT" | grep -q "2 2" && \
   echo "$OUTPUT" | grep -q "3 6"; then
    ((CHECKS_PASSED++))
else
    ((CHECKS_FAILED++))
    echo "FAIL: Factorial values are incorrect"
fi

if echo "$OUTPUT" | grep -q "0 1 2 3 4 5 6 7 8 9"; then
    ((CHECKS_PASSED++))
else
    ((CHECKS_FAILED++))
    echo "FAIL: Container contents are incorrect"
fi

if [ $CHECKS_FAILED -eq 0 ]; then
    echo "Test 1: All checks passed ($CHECKS_PASSED/6)"
    exit 0
else
    echo "Test 1: Failed ($CHECKS_FAILED/$((CHECKS_PASSED + CHECKS_FAILED)) checks failed)"
    echo "Full output:"
    echo "$OUTPUT"
    exit 1
fi