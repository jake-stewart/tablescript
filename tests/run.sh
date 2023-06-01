echo_error() {
    printf "\x1b[31m"
    echo "$1"
    printf "\x1b[0m"
}

num_tests=0
num_passes=0

for directory in test*; do
    num_tests=$((num_tests + 1))
    ../build/tablescript "$directory/input.tbl" > output
    if cmp --silent "$directory/output" output; then
        num_passes=$((num_passes + 1))
        echo "$directory passed"
    else
        echo_error "$directory failed"
    fi
done

echo
echo "$num_passes out of $num_tests test(s) passed"
rm output
