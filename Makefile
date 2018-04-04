CC = c++
CFLAGS = -g -Wall -std=c++14
TARGET = ga
OBJS = main.o ga_driver.o person.o
$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) -v $(LDFLAGS)
ga_driver.o: ga_driver.cpp
	$(CC) $(CFLAGS) -c ga_driver.cpp

person.o: person.cpp
	$(CC) $(CFLAGS) -c person.cpp

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

clean:
	rm *.o $(TARGET)
