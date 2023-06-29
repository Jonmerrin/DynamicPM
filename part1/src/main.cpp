// Support Code written by Michael D. Shah
// Last Updated: 6/11/21
// Please do not redistribute without asking permission.

// Functionality that we created
#include "SDLGraphicsProgram.hpp"
#include <iostream>

int main(int argc, char** argv){

	if(argc < 2){
        std::cout << "Please specify a tolerance level and a .obj model" << std::endl;
        return 0;
    }
	float tolerance = 0.0;
	try {
		tolerance = std::stof(argv[1]);
	} catch(const std::exception& e) {
        std::cout << "Please provide a valid float for the tolerance." << std::endl;
        return 0;
	}
	if(argc < 3){
        std::cout << "Please specify a path to a .obj model" << std::endl;
        return 0;
    }

	std::string path;
	path = std::string(argv[2]);

	SDLGraphicsProgram mySDLGraphicsProgram(1280,720, tolerance, path);
	// Run our program forever
	mySDLGraphicsProgram.Loop();
	// When our program ends, it will exit scope, the
	// destructor will then be called and clean up the program.
	return 0;
}
