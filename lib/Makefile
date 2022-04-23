all: libjp.a

libjp.a: ../src/*.cpp
	$(CC) -c $(FLAGS) -I../inc $^
	ar rvs $@ *.o
