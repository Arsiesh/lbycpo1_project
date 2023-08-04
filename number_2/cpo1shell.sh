#!/bin/bash

# Prompt function
prompt() {
    echo -n "-> "
}

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

# Array to keep track of running and queued programs
declare -a running_programs=()
declare -a queued_programs=()

# Background function to handle dispatching and queuing
dispatcher() {
    while true; do
        # Check if there are running programs
        if [ ${#running_programs[@]} -lt 3 ]; then
            # If there are queued programs, start the one with the smallest physical size
            if [ ${#queued_programs[@]} -gt 0 ]; then
                smallest_index=0
                smallest_size=${queued_programs[0]}
                for (( i=0; i<${#queued_programs[@]}; i++ )); do
                    if [ "${queued_programs[i]}" -lt "$smallest_size" ]; then
                        smallest_size=${queued_programs[i]}
                        smallest_index=$i
                    fi
                done
                program_to_run="${queued_programs[smallest_index]}"
                unset 'queued_programs[smallest_index]'
                running_programs+=("$program_to_run")
                run_c_code "$program_to_run" &
            elif [ ${#queued_programs[@]} -eq 3 ]; then
            echo "Queue already full"
            fi
        fi
        sleep 1 # Adjust the sleep interval if needed
    done
}

# Start the dispatcher function in the background
dispatcher &

# Main loop
while true; do
    prompt
    read -r input

    # Process the input here
    case "$input" in
        pid)
             
            ;;
        *\&*)
            input="${input#& }" # Remove the "&" and space from the input
            if [[ "$input" == *.c ]]; then
                if [ ${#running_programs[@]} -lt 3 ]; then
                    # If there are fewer than 3 running programs, start the program immediately
                    running_programs+=("$input")
                    run_c_code "$input" &
                else
                    # Otherwise, queue the program
                    if [ ${#queued_programs[@]} -lt 2 ]; then
                    queued_programs+=($(stat -c %s "$input")) # Store the physical size of the program
                    echo "Program $input queued."
                    else
                    echo "Queue already full"
                    fi
                fi
            else
                eval "$input &" # Run other commands in the background
            fi
            ;;
        *.c)
                run_c_code "$input"
            ;;
        kill*)
            pid="${input#kill}" # Get the argument after "kill"
            kill "$pid"
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

