#!/usr/bin/env bash
clear
make -s re -C ./test/
make -s clean -C ./test/
cat ./test/testcases | ./test/minishell > ./test/results.txt
echo "Testing complete!"