CC=clang

TestAssn1: test_assign1_1.o dberror.o storage_mgr.o
	cc  -o TestAssn1 test_assign1_1.o dberror.o storage_mgr.o -g
test_assign1_1.o: test_assign1_1.c test_helper.h dberror.h storage_mgr.h
	cc   -c test_assign1_1.c -g
dberror.o: dberror.c dberror.h
	cc  -c dberror.c -g
storage_mgr.o: storage_mgr.c storage_mgr.h dberror.h
	cc -c storage_mgr.c -g

clean:
	rm -f *.o TestAssn1
