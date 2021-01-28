CFLAGS=-Wall

.PHONY:all
all: tool

clean:
	rm -f tool *.o

tool: main.o route.o
	gcc $(CFLAGS) $^ -o $@

.c.o:
	gcc $(CFLAGS) -c $< -o $@
