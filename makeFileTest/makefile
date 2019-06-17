CC = g++
CFALGS = -std=c++11
all: main.cpp sum.o
	$(CC) $(CFALGS) main.cpp sum.o -o main
sum: sum.cpp 
	$(CC) $(CFALGS) sum.cpp -c
clean:
	rm ./sum