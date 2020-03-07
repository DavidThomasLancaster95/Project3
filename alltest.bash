#!/bin/bash
rm play.exe
g++ -Wall -Werror -std=c++17 -g *.cpp -o play

./play.exe tests/in30.txt > actual.txt
diff -w actual.txt tests/out30.txt

./play.exe tests/in33.txt > actual.txt
diff -w actual.txt tests/out33.txt

./play.exe tests/in35.txt > actual.txt
diff -w actual.txt tests/out35.txt

./play.exe tests/in36.txt > actual.txt
diff -w actual.txt tests/out36.txt

./play.exe tests/in37.txt > actual.txt
diff -w actual.txt tests/out37.txt

./play.exe tests/in41.txt > actual.txt
diff -w actual.txt tests/out41.txt

./play.exe tests/in42.txt > actual.txt
diff -w actual.txt tests/out42.txt

./play.exe tests/in43.txt > actual.txt
diff -w actual.txt tests/out43.txt

./play.exe tests/in44.txt > actual.txt
diff -w actual.txt tests/out44.txt

./play.exe tests/in61.txt > actual.txt
diff -w actual.txt tests/out61.txt

./play.exe tests/in62.txt > actual.txt
diff -w actual.txt tests/out62.txt
