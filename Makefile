
WARNING = -Wall -Wshadow --pedantic
ERROR = -Wvla -Werror
GCC = gcc -std=c99 -g $(WARNING) $(ERROR) 

SRCS = a5.c
OBJS = $(SRCS:%.c=%.o)

a5: $(OBJS) 
	$(GCC) $(OBJS) -o a5

.c.o: 
	$(GCC) -c $*.c 

test: a5
	./a5 points.txt

clean: 
	rm -f a1 *.o output* *~