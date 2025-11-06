#!/bin/bash

BASE_FILE="base.rt"
OUTPUT_PREFIX="cyl"
STEPS=6

spherical_to_cartesian() {
    local phi=$1
    local theta=$2
    local dx dy dz

    phi_rad=$(echo "scale=10; $phi * 3.141592653589793 / 180" | bc -l)
    theta_rad=$(echo "scale=10; $theta * 3.141592653589793 / 180" | bc -l)

    dx=$(echo "scale=10; s($phi_rad) * c($theta_rad)" | bc -l)
    dy=$(echo "scale=10; s($phi_rad) * s($theta_rad)" | bc -l)
    dz=$(echo "scale=10; c($phi_rad)" | bc -l)

    printf "%.3f,%.3f,%.3f" "$dx" "$dy" "$dz"
}

# Avoid poles: start phi just above 0 and end just below 180
phi_start=10
phi_end=170
phi_step=$(echo "scale=10; ($phi_end - $phi_start) / ($STEPS - 1)" | bc -l)

theta_step=$(echo "scale=10; 360 / $STEPS" | bc -l)

i=1
for ((p = 0; p < STEPS; p++)); do
    phi=$(echo "scale=10; $phi_start + $p * $phi_step" | bc -l)
    for ((t = 0; t < STEPS; t++)); do
        theta=$(echo "scale=10; $t * $theta_step" | bc -l)
        direction=$(spherical_to_cartesian "$phi" "$theta")
        output_file="${OUTPUT_PREFIX}_$(printf "%02d" $i).rt"

        cp "$BASE_FILE" "$output_file"
        echo "cy 0,0,20 $direction 5 24 255,0,0" >> "$output_file"

        echo "Generated $output_file with direction $direction"
        ((i++))
    done
done

echo "All files generated."
