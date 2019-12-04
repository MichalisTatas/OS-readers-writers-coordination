OBJS = src/main.o src/functions.o

BIN = /bin

ex : $(OBJS)
	gcc $(OBJS) -o ex -lpthread -lrt

clean:
	rm ex $(OBJS)

main1.o : include/functions.h
functions.o : include/functions.h include/struct.h