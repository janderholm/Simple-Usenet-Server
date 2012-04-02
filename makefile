CXX       = g++
CXXFLAGS  = -std=c++0x -pipe -O2 -Wall -W -pedantic-errors
CXXFLAGS += -Wmissing-braces -Wparentheses
OBJS      = *.o client server disktest dbtest

all: server client

clean:
	$(RM) server $(OBJS) 

server: server.o utils.o inmemorydb.o diskdb.o
	$(CXX) $(CXXFLAGS) server.o utils.o inmemorydb.o diskdb.o clientserver/*.cc -o server

disktest:
	$(CXX) $(CXXFLAGS) disktest.o diskdb.o -o disktest

client: client.o utils.o
	$(CXX) $(CXXFLAGS) client.o utils.o clientserver/*.cc -o client

dbtest: inmemorydb.o
	$(CXX) $(CXXFLAGS) dbtest.cc inmemorydb.o -o dbtest
	./dbtest && echo "SUCCESS!" || echo "FAIL!"
	@$(RM) dbtest

server.o: server.cc
	$(CXX) -c $(CXXFLAGS) server.cc

client.o: client.cc
	$(CXX) -c $(CXXFLAGS) client.cc

utils.o: utils.cc
	$(CXX) -c $(CXXFLAGS) utils.cc

inmemorydb.o: inmemorydb.cc
	$(CXX) -c $(CXXFLAGS) inmemorydb.cc

diskdb.o: diskdb.cc
	$(CXX) -c $(CXXFLAGS) diskdb.cc




