#!/bin/bash
# Prompt function
prompt() {
    echo -n "-> "
}


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
                run_c_code "$input" &
            else
                eval "$input &" # Run other commands in the background
            fi
            ;;
        *.c)
            run_c_code "$input"
            ;;
        quit)
            echo "Goodbye!"
            exit 0
            ;;
        *)
            eval "$input" # Execute other commands
            ;;
    esac
done
