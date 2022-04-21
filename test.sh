#!/bin/bash

TEST_DIR=data
CHECKER=./checker

if [[ $# > 0 ]]; then
	CHECKER=$1
fi

ERR=0
TOTAL=0

for f in $TEST_DIR/fail/*.json; do
	cat $f | $CHECKER 1> /dev/null 2> /dev/null
	if [[ $? == 0 ]]; then
		echo -e "\e[31mPASSED\e[0m : \e[1m$f\e[0m : $(cat $f)"
		ERR=$((ERR+1))
	else
		echo -e "\e[32mFAILED\e[0m : \e[1m$f\e[0m"
	fi
	TOTAL=$((TOTAL+1))
done

for f in $TEST_DIR/pass/*.json; do
	cat $f | $CHECKER 1> /dev/null 2> /dev/null
	if [[ $? == 0 ]]; then
		echo -e "\e[32mPASSED\e[0m : \e[1m$f\e[0m"
	else
		echo -e "\e[31mFAILED\e[0m : \e[1m$f\e[0m : $(cat $f)"
		ERR=$((ERR+1))
	fi
	TOTAL=$((TOTAL+1))
done

echo

if [[ $ERR == 0 ]]; then
	echo -e "Total: $TOTAL Passed: \e[1;32m$((TOTAL-ERR))\e[0m Failed: \e[1;32m$ERR\e[0m" 1>&2
else
	echo -e "Total: $TOTAL Passed: \e[1;32m$((TOTAL-ERR))\e[0m Failed: \e[1;31m$ERR\e[0m" 1>&2
fi
