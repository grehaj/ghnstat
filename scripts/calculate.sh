#!/usr/bin/bash

katalog=$1
src_count='"src_count":'
port_count='"port_count":'
time_count='"time_count":'
stat_count='"stat_count":'
digit='[0-9]\+'

function calculate_statistics {
    pattern="$1"
    local num=0
    local statistics=0
    while read line
    do
        ((num++))
        statistics=$(($statistics + $line))
    done <<< "$(grep -iRh "$pattern" "$katalog"/stat_*.json | grep -oh "$digit")"

    if [ $num -eq 0 ]
    then
        echo 0
    else
        echo $(echo "$statistics / $num" | bc)
    fi  
    echo $num $statistics
}

echo "Average src_count:    " $(calculate_statistics "$src_count")
echo "Average port_count:   " $(calculate_statistics "$port_count")
echo "Average time_count:   " $(calculate_statistics "$time_count")
echo "Average stat_count:   " $(calculate_statistics "$stat_count")