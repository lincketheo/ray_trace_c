
csrc=$(wildcard *.c)

main: $(csrc)
	gcc $^ -o $@ -g -lm -O3 -ffast-math -fassociative-math -fopenmp

clean:
	rm -f main
