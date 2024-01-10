#!/bin/bash

file_name="$1.cpp"
echo "$file_name"
g++ -std=c++20 "$file_name" -lpthread 

values=(1000 2000 3000 4000 5000)
output_file="values.txt"
temp_file="temp_values.txt" 

for value in "${values[@]}"; do
    > "$temp_file"  
    # Run the C++ executable 10 times
    for i in {1..5}; do
        # Replace 'your_executable' with the name of your C++ program
        ./a.out "$value" >> "$temp_file"
    done

    sum=0
    count=0
    while read -r line; do
        sum=$(echo "$sum + $line" | bc -l)
        count=$((count + 1))
    done < "$temp_file"
    average=$(echo "scale=2; $sum / $count" | bc -l)
    # Write the average to the file
    echo "Average for file: $file_name $value: $average" >> "$output_file"
done

