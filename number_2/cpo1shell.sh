#!/bin/bash

# Prompt function
prompt() {
    echo -n "-> "
}

# Custom command: greet
greet() {
    echo "Hello, $1!"
}

# Custom command: run
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
        greet*)
            name="${input#greet}" # Get the argument after "greet"
            greet "$name"
            ;;
        *.c)
            run_c_code "$input"
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
Save the file and make it executable using the following command:

bash
Copy code
chmod +x myShell.sh
Now, you can run your custom shell:

bash
Copy code
./myShell.sh
Your custom shell will provide the following functionality:

The "greet" command: Type greet <name> to get a greeting. For example, greet John will print "Hello, John!"

The "multiply" command: Type multiply <number1> <number2> to multiply two numbers. For example, multiply 5 6 will print "Result: 30".

The "exit" command: Type exit to quit the shell.

Other commands will be executed as regular shell commands.

Feel free to expand this shell with more custom commands or additional features as needed. Remember to carefully validate and sanitize user input when implementing more complex functionality.






