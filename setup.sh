#!/usr/bin/env bash
make re -C ./bin/
make clean -C ./bin/
alias minishell="./bin/minishell"
clear
echo "Use command 'minishell' to run"