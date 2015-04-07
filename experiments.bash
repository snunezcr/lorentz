#!/bin/bash

./tlorenz 2 2 2 10 10 1 data-10-1.0.txt
tar -cvzf data-10-1.0.tgz data-10-1.0.txt
rm data-10-1.0.txt

./tlorenz 2 2 2 10 10 0.5 data-10-0.5.txt
tar -cvzf data-10-0.5.tgz data-10-0.5.txt
rm data-10-0.5.txt

./tlorenz 2 2 2 10 10 0.25 data-10-0.25.txt
tar -cvzf data-10-0.25.tgz data-10-0.25.txt
rm data-10-0.25.txt

./tlorenz 2 2 2 25 10 1 data-25-1.0.txt
tar -cvzf data-25-1.0.tgz data-25-1.0.txt
rm data-25-1.0.txt

./tlorenz 2 2 2 25 10 0.5 data-25-0.5.txt
tar -cvzf data-25-0.5.tgz data-25-0.5.txt
rm data-25-0.5.txt

./tlorenz 2 2 2 25 10 0.25 data-25-0.25.txt
tar -cvzf data-25-0.25.tgz data-25-0.25.txt
rm data-25-0.25.txt

./tlorenz 2 2 2 50 10 1 data-50-1.0.txt
tar -cvzf data-50-1.0.tgz data-50-1.0.txt
rm data-50-1.0.txt

./tlorenz 2 2 2 50 10 0.5 data-50-0.5.txt
tar -cvzf data-50-0.5.tgz data-50-0.5.txt
rm data-50-0.5.txt

./tlorenz 2 2 2 50 10 0.25 data-50-0.25.txt
tar -cvzf data-50-0.25.tgz data-50-0.25.txt
rm data-50-0.25.txt

