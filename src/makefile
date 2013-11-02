objects = main.o memory_manager.o computer.o

all: $(objects)
	g++ -o project5A $(objects)

computer.o : computer.cpp computer.h
	g++ -Wall -c computer.cpp

memory_manager.o : memory_manager.cpp memory_manager.h
	g++ -Wall -c memory_manager.cpp

main.o : main.cpp
	g++ -Wall -c main.cpp

clean :
	rm -f $(objects)