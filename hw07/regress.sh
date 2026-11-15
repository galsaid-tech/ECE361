#!/bin/sh
# Compare both printed output and exit status for each input.
set -u

fail=0
total=0
for input in inputs/*.txt; do
    name=$(basename "$input" .txt)
    total=$((total + 1))
    actual_output=$(mktemp)
    if ./balance < "$input" > "$actual_output"; then
        status=0
    else
        status=$?
    fi
    expected_status=$(cat "expected/$name.status")
    if [ "$status" -eq "$expected_status" ] &&
       diff -u "expected/$name.out" "$actual_output"; then
        echo "PASS: $name"
    else
        echo "FAIL: $name (expected status $expected_status, got $status)"
        fail=$((fail + 1))
    fi
    rm -f "$actual_output"
done

echo "Regression summary: $((total - fail)) passed, $fail failed"
[ "$fail" -eq 0 ]
