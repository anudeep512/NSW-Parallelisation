#!/bin/bash

file_name="$1.cpp"
echo "$file_name"
g++ -std=c++20 "$file_name" -lpthread 


# Executable
executable="./a.out"

# Argument for the executable
arg="4000"

# Output file for averages
output_file="values.txt"
second_values=(10 50 100 150 200)

# Values in powers of two (2, 4, 8, ..., 32)
for value in "${second_values[@]}"; do
    # Modify the file
    echo "16 $value 5" > "params.txt"

    sum=0

    # Run the executable 5 times
    for j in {1..5}; do
        # Run executable and capture the output
        output=$($executable $arg)
        echo "Output for $value iteration $j: $output" >> "temp_values.txt"
        sum=$(echo "$sum + $output" | bc)
    done

    # Calculate the average
    average=$(echo "scale=2; $sum / 5" | bc)

    # Write the average to the output file
    echo "Average for file: $file_name $value: $average" >> "$output_file"
done

