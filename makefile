

dbtest:
	g++ --std=c++0x dbtest.cc inmemorydb.cc -o dbtest
	./dbtest && echo "SUCCESS!" || echo "FAIL!"
	rm dbtest
