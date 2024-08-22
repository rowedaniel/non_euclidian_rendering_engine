#! /usr/bin/awk -f

BEGIN {
    failed = 0
    passed = 0
}

{
    split($0, sides, "=", seps)
    if ($0 ~ /^(\.)+$/ || (length(sides) == 2 && sides[1] == sides[2])){
        passed += 1
        printf "."
    } else {
        failed += 1
        printf "x"
    }
}

END {
    printf "\n%d passed, %d failed\n", passed, failed
    }
