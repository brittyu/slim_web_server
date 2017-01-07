test: server.o signal.o request.o
	gcc server.o signal.o request.o -o slim_web_server

server.o: server.c extern.h signal.h request.h
	gcc -c server.c

signal.o: signal.c signal.h extern.h
	gcc -c signal.c

request.o: request.c request.h
	gcc -c request.c

clean:
	rm *.o
