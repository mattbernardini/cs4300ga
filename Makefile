CC	= g++
CFLAGS 	= -g -Wall -std=c++14 -fopenmp -O3
LDFLAGS	= -lm -fopenmp -static -lquadmath
TARGET = ga
OBJS = main.o ga_driver.o person.o 

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS)

ga_driver.o: ga_driver.cpp ga_driver.h 
	$(CC) $(CFLAGS) -c ga_driver.cpp

person.o: person.cpp
	$(CC) $(CFLAGS) -c person.cpp
	
linkedlist.o: linkedlist.cpp
	$(CC) $(CFLAGS) -c linkedlist.cpp
	
main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

clean:
	rm *.o $(TARGET)
