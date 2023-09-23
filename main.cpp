#include <iostream>

#include "./Application/Application.h"
#include "Application/Coda.h"

int main(int argc, char** argv) {
	Coda::Application app = Coda::Application();
	app.run(argc,argv);
}
