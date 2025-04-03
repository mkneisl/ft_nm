#/bin/bash

testParams=("a" "g" "u" "r" "p")
testFile="$1"

if [ -z "$1" ]; then
        testFile="ft_nm"
fi

testNm(){
    local param=$2

    if [ -n "$param" ]; then
        param="-${param}"
    fi
    ./ft_nm $param $1 > ft_out
    nm $param $1 > gnu_out
    diff ft_out gnu_out > diff_out
    if [ -s "$param" ]; then
        echo "[-] Output not equal:"
        cat diff_out
    else
        echo "[+] Output equal"
    fi
    rm -f ft_out gnu_out diff_out
}

for param in "${testParams[@]}"
do
    testNm "ft_nm" "$param"
done