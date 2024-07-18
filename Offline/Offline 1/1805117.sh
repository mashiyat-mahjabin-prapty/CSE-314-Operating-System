#!/bin/bash
marks=100
range=5
rm output.csv
if [ $1 ]; then
    marks=$1
fi
if [ $2 ]; then
    range=$2
fi
echo "student_id,score" >> output.csv
for((i=0;i<range;i++))
do
    roll=$((1805121+i))
    #echo $roll
    if [ -d "Submissions/${roll}" ] && [ -f "Submissions/${roll}/${roll}.sh" ]; then
        #echo "exists"
        bash "Submissions/${roll}/${roll}.sh" >> temp.txt
        diff -w temp.txt AcceptedOutput.txt >> difference.txt
        points=$(grep -c '[><]' difference.txt)
        points=$((points*5))
        points=$((marks-points))
        if [ ${points} -lt 0 ]; then
            points=0
        fi
        for((j=0;j<range;j++))
        do 
            roll2=$((1805121+j))
            if [ -d "Submissions/${roll2}" ] && [ -f "Submissions/${roll2}/${roll2}.sh" ] && [ $i -ne $j ]; then
                diff -Z -B "Submissions/${roll}/${roll}.sh" "Submissions/${roll2}/${roll2}.sh" >> difference2.txt
                points2=$(grep -c '[><]' difference2.txt)
                rm difference2.txt
                if [ ${points2} -eq 0 ] && [ ${points} -ge 0 ]; then
                    points=$((-points))
                fi
            fi 
        done
        echo "${roll},${points}" >> output.csv 
        rm difference.txt temp.txt
    else 
    echo "${roll},0" >> output.csv
    fi
done