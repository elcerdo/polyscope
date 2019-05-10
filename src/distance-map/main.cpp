#include "polyscope/polyscope.h"

int main(int argc, char** argv) {
		polyscope::options::autocenterStructures = true;

		polyscope::init();

		polyscope::show();

		return 0;
}
