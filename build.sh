#!/bin/bash

code="$PWD"
opts=-g
cd build > /dev/null
cc $opts $code/code/main.c -o homebrewJourney
cd $code > /dev/null
