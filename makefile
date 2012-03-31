
all: server

clean:
	$(RM) server

server:
	g++ -Wall --std=c++0x server.cc inmemorydb.cc clientserver/*.cc -o server

disk:
	g++ -Wall --std=c++0x server.cc diskdb.cc clientserver/*.cc -o server

disktest:
	g++ -Wall --std=c++0x disktest.cc diskdb.cc -o disktest

dbtest:
	g++ -Wall --std=c++0x dbtest.cc inmemorydb.cc -o dbtest
	./dbtest && echo "SUCCESS!" || echo "FAIL!"
	@rm dbtest