
default: main

.PHONY: clean

clean:
	-rm main

main: code.rs Makefile
	rustc $< -o main

