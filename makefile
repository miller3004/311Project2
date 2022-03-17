text-client: socketClient.cc
	g++ socketClient.cc -o text-client
text-server: socketServer.cc
	g++ socketServer.cc -o text-server
clean:
	rm -f *.o text-server text-client

