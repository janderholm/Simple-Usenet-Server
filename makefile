
all: server

clean:
	$(RM) server

server:
	g++ -Wall --std=c++0x server.cc inmemorydb.cc clientserver/*.cc -o server


dbtest:
	g++ -Wall --std=c++0x dbtest.cc inmemorydb.cc -o dbtest
	./dbtest && echo "SUCCESS!" || echo "FAIL!"
	@rm dbtest
