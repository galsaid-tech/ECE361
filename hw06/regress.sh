
#!/bin/sh

set -eu

for name in test1 test2
do
    echo "Testing $name..."

    ./replay "scenarios/$name.txt" > actual.txt

    if diff -u "expected/$name.txt" actual.txt; then
        echo "PASS: $name"
    else
        echo "FAIL: $name"
        rm -f actual.txt
        exit 1
    fi
done

rm -f actual.txt
echo "All regression tests passed."
