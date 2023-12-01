#!/bin/bash

cp ../build/md_editor ./md_editor
rm -f *.md
rm -f *.out

program="./md_editor"

for i in {1..5}; do
    input_file="${i}.in"
    output_file="${i}.out"
    answer_file="${i}.ans"
    create_file="test${i}.md"
    curr_file="test${i}.ans"

    $program < $input_file > $output_file

    # 时间戳特殊处理
    if [ $i -eq 5 ]; then
        cp "$output_file" "${output_file}.copy"
        cp "$answer_file" "${answer_file}.copy"
        sed -i '' '10,11d' "$output_file"
        sed -i '' '10,11d' "$answer_file"
    fi

    diff "$output_file" "$answer_file" > /dev/null
    if [ $? -eq 0 ]; then
        echo "Test $i: Output Right."
    else
        echo "Test $i: Output Wrong!"
    fi

    if [ $i -eq 5 ]; then
        mv "${output_file}.copy" "$output_file"
        mv "${answer_file}.copy" "$answer_file"
    fi
done

for i in {1..5}; do
    diff "$create_file" "$curr_file" > /dev/null
    if [ $? -eq 0 ]; then
        echo "Test $i: Create .md Right."
    else
        echo "Test $i: Create .md Wrong!"
    fi
done


    