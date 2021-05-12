com: server.c client.c
	gcc server.c -o wcserver
	gcc client.c -o client
	
clean:
	rm wcserver client