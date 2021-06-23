CFLAGS = -O

all: libgraph.a

OBJS = dllist.o jval.o jrb.o graph.o pqueue.o test.o

libgraph.a: $(OBJS)
	ar ru libgraph.a $(OBJS)
	ranlib libgraph.a 

clean:
	rm -f core *.o libgraph.a

.SUFFIXES: .c .o
.c.o:
	$(CC) $(CFLAGS) -c $*.c -w
