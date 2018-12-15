# output:  main.o aes.o hashTable.o 
# 	g++ -std=c++11 -g main.o aes.o hashTable.o -o output
# main.o: main.cpp
# 	g++ -std=c++11 -c main.cpp
# aes.o: aes.cpp aes.h
# 	g++ -std=c++11 -c aes.cpp
# hashTable.o: hashTable.cpp hashTable.h
# 	g++ -std=c++11 -c hashTable.cpp
# clean:
# 	rm *.o output 

OBJS	= main.o aes.o hashTable.o
SOURCE	= main.cpp aes.cpp hashTable.cpp
HEADER	= aes.h hashTable.h
OUT	= output
CC	 = g++
FLAGS	 = -g -c -Wall
LFLAGS	 =

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

main.o: main.cpp
	$(CC) $(FLAGS) main.cpp

aes.o: aes.cpp
	$(CC) $(FLAGS) aes.cpp

hashTable.o: hashTable.cpp
	$(CC) $(FLAGS) hashTable.cpp


clean:
	rm -f $(OBJS) $(OUT)
