#!/bin/sh

ls || exit && echo # this should ls and echo
ls -a  && cat Makefile || exit && echo done #not exit
echo exit; echo exit && echo a #exit 
ls -a  && c && exit && echo done #not exit
ls && echo || exit
ls || exit
ls && exit
[ -e test ] && exit 
test -f test && exit 
test exit 
test -d exit 
(echo A && echo B && (echo d && exit )) # only echo d and exit