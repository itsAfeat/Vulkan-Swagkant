#include "Swagkant.hpp"

int main() {
	SwagkantApp app;

	try {
		app.run("Schwag");
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}