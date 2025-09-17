#! /bin/bash
g++ -g *.cpp *.h -o output
./output bank_trans_in.txt > bank_trans_out.txt
./output bank_trans_in.txt 2> bank_trans_err.txt