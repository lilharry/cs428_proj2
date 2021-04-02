all: server client1 client2

server: server.cpp 
	g++ server.cpp -o server

client1: client1.cpp
	g++ client1.cpp -o client1

client2: client2.cpp
	g++ client2.cpp -o client2

clean:
	rm client1
	rm client2
	rm server

run: all
	./server &
	./client1
	./client2
