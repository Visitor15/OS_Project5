objects = main.o memory_manager.o computer.o

all: $(objects)
	g++ -o project5A $(objects)

computer.o : computer.cpp computer.h
	g++ -Wall -c -g computer.cpp

memory_manager.o : memory_manager.cpp memory_manager.h
	g++ -Wall -c -g memory_manager.cpp

main.o : main.cpp
	g++ -Wall -c -g main.cpp

clean :
	rm -f $(objects) log*.txt core