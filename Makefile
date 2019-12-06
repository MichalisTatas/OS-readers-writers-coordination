OBJS = src/main.o src/functions.o

BIN = /bin

SOURCE = src

ex : $(OBJS)
	gcc $(OBJS) -o ex -lpthread -lrt -lm

clean:
	rm ex
	rm $(SOURCE)/functions.o
	rm $(SOURCE)/main.o

main1.o : include/functions.h
functions.o : include/functions.h include/struct.h