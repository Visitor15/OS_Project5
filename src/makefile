objects = main.o process_builder.o memory_manager.o computer.o

all: $(objects)
	g++ -o project5A $(objects)

computer.o : computer.cpp computer.h
	g++ -c computer.cpp

memory_manager.o : memory_manager.cpp memory_manager.h process_builder.h backing_store.h frame_table.h
	g++ -c memory_manager.cpp

process_builder.o : process_builder.cpp process_builder.h page_fault_exception.h process_manager.h
	g++ -c process_builder.cpp

main.o : main.cpp
	g++ -c main.cpp

clean :
	rm -f $(objects) log*.txt core
