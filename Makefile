raycast: main.c
	cc -lraylib -lm main.c -o raycast.out -Wall -std=c11 -g
clean:
	rm *.out -rf
