CFLAGS=-Wall -O3 -march=native

LDLIBS=-lmpfr -lgmp -lm

LDFLAGS=-O3 -march=native

#ALL: $(OBJS)

SIN_OBJS=comparisons.o sin.o
sin: $(SIN_OBJS)

SQRT_OBJS=comparisons.o sqrt.o
sqrt: $(SQRT_OBJS) 

clean:
	rm *.o
	rm sin sqrt

