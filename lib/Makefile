all: libjp.a

libjp.a: ../src/*.cpp
	$(CC) -c -fPIC $(FLAGS) -I../inc $^
	ar rvs $@ *.o
