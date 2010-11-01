.phony all:
all: rsi inf args

rsi: rsi.c rsi.h
	gcc rsi.c -lreadline -lhistory -ltermcap -g -Wall -o rsi
inf: inf.c
	gcc inf.c -o inf
args: args.c 
	gcc args.c -o args

.PHONY clean:
clean:
	rm -rf *.o *.exe rsi
