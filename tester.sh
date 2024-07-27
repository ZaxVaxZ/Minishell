#!/bin/bash
rm -rf outs.txt
rm -rf outs2.txt
while read uwu; do
	# echo -e "\n-------------------------------------\n" >>outs.txt
	echo -e "$uwu" | ./bin/minishell 2>&1 | cat >>outs.txt
	echo -e "$uwu" | bash 2>&1 | cat >>outs2.txt
done < tests.sh