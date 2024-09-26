.PHONY: clean run
.SILENT:

AsciiVisualizer3D: main.c
	gcc -o AsciiVisualizer3D main.c -lm

run: AsciiVisualizer3D
	./AsciiVisualizer3D

clean:
	rm -rf *.o AsciiVisualizer3D

