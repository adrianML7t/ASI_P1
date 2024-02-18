CC = gcc
CFLAGS = -pthread

SRCS := Ejercicio1_procesos.c Ejercicio2_threads.c client.c
EXES := $(SRCS:.c=)

.PHONY: all clean

all: $(EXES)

$(EXES): %: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	$(RM) $(EXES)
