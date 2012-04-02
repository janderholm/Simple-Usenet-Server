CXX       = g++
CXXFLAGS  = -std=c++0x -pipe -O2 -Wall -W -pedantic-errors
CXXFLAGS += -Wmissing-braces -Wparentheses
OBJS      = *.o client server disktest dbtest

all: server client

clean:
	$(RM) server $(OBJS) 

server server.cc utils.cc inmemorydb.cc:
	$(CXX) $(CXXFLAGS) server.cc utils.cc inmemorydb.cc diskdb.cc clientserver/*.cc -o server

disktest:
	$(CXX) $(CXXFLAGS) disktest.cc diskdb.cc -o disktest

client client.cc utils.cc:
	$(CXX) $(CXXFLAGS) client.cc utils.cc clientserver/*.cc -o client

dbtest:
	$(CXX) $(CXXFLAGS) dbtest.cc inmemorydb.cc -o dbtest
	./dbtest && echo "SUCCESS!" || echo "FAIL!"
	@$(RM) dbtest
