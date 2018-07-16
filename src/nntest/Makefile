CC	= g++
CFLAGS 	= -g -Wall -std=c++14 -O3
LDFLAGS	= -lm
TARGET = ga
OBJS = main.o ga_driver.o person.o base_logger.o
VALGRIND = valgrind --tool=memcheck --verbose --leak-check=full --track-origins=yes --show-leak-kinds=all

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS)
valgrind: $(TARGET)
	$(VALGRIND) ./$(TARGET)
ga_driver.o: ga_driver.cpp ga_driver.h 
	$(CC) $(CFLAGS) -c ga_driver.cpp

person.o: person.cpp
	$(CC) $(CFLAGS) -c person.cpp
	
base_logger.o: base_logger.cpp
	$(CC) $(CFLAGS) -c base_logger.cpp
main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

clean:
	rm *.o *.log $(TARGET)
