
csrc=$(wildcard *.c)

main: $(csrc)
	gcc $^ -o $@ -g -lm

clean:
	rm -f main
