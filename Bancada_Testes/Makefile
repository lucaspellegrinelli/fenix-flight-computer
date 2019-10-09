main_cpp_path := pc_bordo.cpp
other_cpp_path := arduino_simulated_code/lib
build_name := bancada

other_cpp := $(wildcard $(other_cpp_path)/*.cpp)

all:
	g++ $(main_cpp_path) $(other_cpp) -o $(build_name) -I arduino_simulated_code -I arduino_simulated_code/lib

clean:
	@rm -rf $(build_name)
