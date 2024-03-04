#!/usr/bin/env bash
clear
make re -C ./bin/
make clean -C ./bin/
alias minishell="./bin/minishell"
echo "Use command 'minishell' to run"