normal: slim_normal_server.o signal.o request.o slim_normal.o
	gcc slim_normal_server.o signal.o request.o slim_normal.o -o slim_normal_web_server

select: slim_select_server.o signal.o request.o slim_select.o
	gcc slim_select_server.o signal.o request.o slim_select.o -o slim_select_web_server

poll: slim_poll_server.o signal.o request.o slim_poll.o
	gcc slim_poll_server.o signal.o request.o slim_poll.o -o slim_poll_web_server

slim_select_server.o: slim_select_server.c extern.h signal.h request.h
	gcc -c slim_select_server.c

slim_normal_server.o: slim_normal_server.c extern.h signal.h request.h
	gcc -c slim_normal_server.c

slim_poll_server.o: slim_poll_server.c extern.h signal.h request.h
	gcc -c slim_poll_server.c

signal.o: signal.c signal.h extern.h
	gcc -c signal.c

request.o: request.c request.h
	gcc -c request.c

slim_normal.o: normal.c normal.h
	gcc -c slim_normal.c

slim_select.o: slim_select.c slim_select.h
	gcc -c slim_select.c

slim_poll.o: slim_poll.c slim_poll.h
	gcc -c slim_poll.c

clean:
	rm *.o
