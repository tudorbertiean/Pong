mypong : mypong.o
	gcc mypong.o -lncurses -o mypong
mypong.o : mypong.c paddle.h bounce.h
	gcc -c mypong.c

clean:
	rm mypong.o
