compiler_args := -O3 -std=c++11

main_cpp_path := pc_bordo.cpp
other_cpp_path := Arduino/lib
build_name := bancada

other_cpp := $(wildcard $(other_cpp_path)/*.cpp)

all:
	g++ $(compiler_args) $(main_cpp_path) $(other_cpp) -o $(build_name) -I Arduino -I Arduino/lib

clean:
	@rm -rf $(build_name)
