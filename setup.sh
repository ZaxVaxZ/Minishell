#!/usr/bin/env bash
clear
make -s re -C ./bin/
make -s clean -C ./bin/
alias minishell="./bin/minishell"
echo "Setup complete! Use command 'minishell' to run"
valgrind --leak-check=full --show-leak-kinds=all --suppressions=readline.supp --track-fds=yes --trace-children=yes ./bin/minishell