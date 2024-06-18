.PHONY: clean
.PHONY: simple-launcher

simple-launcher: simple-launcher.c
	$(CC) -o '$@' '$<' -lSDL2 -lSDL2_image -lSDL2_ttf

clean:
	-rm simple-launcher