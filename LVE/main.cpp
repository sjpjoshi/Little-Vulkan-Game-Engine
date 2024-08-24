#include "first_app.hpp"

// ideally all we will need for now
#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main() {
	// calling the function 
	lve::FirstApp app{};

	// not necessary but good practice for now
	try {
		app.run(); 

	} // try
	catch (const std::exception& e) {
		std::cerr << e.what() << "\n";
		return EXIT_FAILURE;

	} // catch

	return EXIT_SUCCESS;
	 
} // main