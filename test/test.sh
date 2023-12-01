#!/bin/bash

cp ../build/md_editor ./md_editor
rm -f *.md
rm -f *.out

program="./md_editor"

for i in {1..5}; do
    input_file="${i}.in"
    output_file="${i}.out"
    answer_file="${i}.ans"
    $program < $input_file > $output_file
    if [ $i -eq 5 ]; then
        cp "$output_file" "${output_file}.copy"
        cp "$answer_file" "${answer_file}.copy"
        sed -i '' '10,11d' "$output_file"
        sed -i '' '10,11d' "$answer_file"
    fi
    diff "$output_file" "$answer_file" > /dev/null
    if [ $? -eq 0 ]; then
        echo "Group $i: Output matches the answer."
    else
        echo "Group $i: Output does not match the answer."
    fi
    if [ $i -eq 5 ]; then
        mv "${output_file}.copy" "$output_file"
        mv "${answer_file}.copy" "$answer_file"
    fi
done
