#!/bin/bash

# Array to keep track of running and queued programs
declare -a running_programs=()
declare -a queued_programs=()

# Function to compile and run C code
run_c_code() {
    filename="$1"
    binary="${filename%.c}"
    gcc "$filename" -o "$binary"
    if [ $? -eq 0 ]; then
        "./$binary"
        rm "$binary" # Clean up the compiled binary after execution
    else
        echo "Compilation failed."
    fi
}

# Function to handle the dispatcher and queuing
dispatch_and_queue() {
    while true; do
        if [ ${#running_programs[@]} -lt 3 ]; then
            if [ ${#queued_programs[@]} -gt 0 ]; then
                smallest_index=0
                smallest_size=${queued_programs[0]}
                for ((i=0; i<${#queued_programs[@]}; i++)); do
                    if [ "${queued_programs[i]}" -lt "$smallest_size" ]; then
                        smallest_size=${queued_programs[i]}
                        smallest_index=$i
                    fi
                done

                program_to_run="${queued_programs[smallest_index]}"
                unset 'queued_programs[smallest_index]'
                running_programs+=("$program_to_run")
                run_c_code "$program_to_run" &
            fi
        fi
        sleep 1
    done
}

# Start the dispatcher function in the background
dispatch_and_queue &

# Main loop
while true; do
    echo -n "-> "
    read -r input

    # Process the input here
    case "$input" in
        pid)
            # Display the running and queued programs
            echo "Running programs:"
            for program in "${running_programs[@]}"; do
                echo "$program"
            done

            echo "Queued programs:"
            for program in "${queued_programs[@]}"; do
                echo "$program"
            done
            ;;
        *\&*)
            input="${input#& }" # Remove the "&" and space from the input
            if [[ "$input" == *.c ]]; then
                if [ ${#running_programs[@]} -lt 3 ]; then
                    if [ ${#queued_programs[@]} -lt 2 ]; then
                        running_programs+=("$input")
                        queued_programs+=($(stat -c %s "$input")) # Store the physical size of the program
                        run_c_code "$input" &
                    else
                        echo "Queue already full."
                    fi
                else
                    echo "Three processes are already running, putting program $input in the queue."
                    if [ ${#queued_programs[@]} -lt 2 ]; then
                        queued_programs+=($(stat -c %s "$input")) # Store the physical size of the program
                    else
                        echo "Queue already full."
                    fi
                fi
            else
                eval "$input &" # Run other commands in the background
            fi
            ;;
        *.c)
            if [ ${#running_programs[@]} -lt 3 ]; then
                running_programs+=("$input")
                queued_programs+=($(stat -c %s "$input")) # Store the physical size of the program
                run_c_code "$input" &
            else
                echo "Three processes are already running, putting program $input in the queue."
                if [ ${#queued_programs[@]} -lt 2 ]; then
                    queued_programs+=($(stat -c %s "$input")) # Store the physical size of the program
                else
                    echo "Queue already full."
                fi
            fi
            ;;
        exit)
            echo "Goodbye!"
            exit 0
            ;;
        *)
            eval "$input" # Execute other commands
            ;;
    esac
done

