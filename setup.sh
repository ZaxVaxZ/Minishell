#!/usr/bin/env bash
clear
make -s re -C ./bin/
make -s clean -C ./bin/
alias minishell="./bin/minishell"
echo "Setup complete! Use command 'minishell' to run"