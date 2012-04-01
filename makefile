
all: server

clean:
	$(RM) server

server server.cc utils.cc inmemorydb.cc:
	g++ -Wall --std=c++0x server.cc utils.cc inmemorydb.cc clientserver/*.cc -o server

disk:
	g++ -Wall --std=c++0x server.cc diskdb.cc clientserver/*.cc -o server

disktest:
	g++ -Wall --std=c++0x disktest.cc diskdb.cc -o disktest

client:
	g++ -Wall --std=c++0x client.cc utils.cc clientserver/*.cc -o client

dbtest:
	g++ -Wall --std=c++0x dbtest.cc inmemorydb.cc -o dbtest
	./dbtest && echo "SUCCESS!" || echo "FAIL!"
	@rm dbtest
