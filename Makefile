CLIENTSERVERPATH = clientserver

CXX       = g++
CXXFLAGS  = -std=c++0x -pipe -O2 -Wall -W -pedantic-errors
CXXFLAGS += -Wmissing-braces -Wparentheses -I$(CLIENTSERVERPATH)
OBJS      = *.o client server disktest dbtest



all: serverdisk servermem client

clean:
	$(RM) $(OBJS) bin/*

servermem: bin servermem.o utils.o inmemorydb.o
	$(CXX) $(CXXFLAGS) servermem.o utils.o inmemorydb.o \
	$(CLIENTSERVERPATH)/*.cc -o bin/servermem

serverdisk: bin serverdisk.o utils.o diskdb.o
	$(CXX) $(CXXFLAGS) serverdisk.o utils.o diskdb.o \
	$(CLIENTSERVERPATH)/*.cc -o bin/serverdisk

disktest:
	$(CXX) $(CXXFLAGS) disktest.o diskdb.o -o disktest

client: bin client.o utils.o
	$(CXX) $(CXXFLAGS) client.o utils.o $(CLIENTSERVERPATH)/*.cc \
	-o bin/client

dbtest: inmemorydb.o
	$(CXX) $(CXXFLAGS) dbtest.cc inmemorydb.o -o dbtest
	./dbtest && echo "SUCCESS!" || echo "FAIL!"
	@$(RM) dbtest

servermem.o: src/server.cc
	$(CXX) -c $(CXXFLAGS) src/server.cc -o servermem.o

serverdisk.o: src/server.cc
	$(CXX) -c -D DISKDB $(CXXFLAGS) src/server.cc -o serverdisk.o

client.o: src/client.cc
	$(CXX) -c $(CXXFLAGS) src/client.cc

utils.o: src/utils.cc
	$(CXX) -c $(CXXFLAGS) src/utils.cc

inmemorydb.o: src/inmemorydb.cc
	$(CXX) -c $(CXXFLAGS) src/inmemorydb.cc

diskdb.o: src/diskdb.cc
	$(CXX) -c $(CXXFLAGS) src/diskdb.cc

bin:
	mkdir -p bin/
