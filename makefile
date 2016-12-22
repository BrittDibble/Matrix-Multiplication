all: pipe.o create_matrix.o serial.o parallel.o horizontal_parallel.o vertical_parallel.0
pipe.o: pipe.cpp
	g++ pipe.cpp -o pipe
create_matrix.o: create_matrix.cpp
	g++ create_matrix.cpp -o create_matrix
serial.o: serial.cpp
	g++ serial.cpp -lpthread -o serial
parallel.o: parallel.cpp
	g++ parallel.cpp -lpthread -o parallel
horizontal_parallel.o: horizontal_parallel.cpp
	g++ horizontal_parallel.cpp -lpthread -o horizontal_parallel
vertical_parallel.0: vertical_parallel.cpp
	g++ vertical_parallel.cpp -lpthread -o vertical_parallel