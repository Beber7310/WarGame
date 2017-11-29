#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>

#include "guiManager.h"

#include "configuration.h"

#include "openCV.h"

using namespace std;

int main (int argc, char* argv[]) {

	runtcp();
	rendertask();

}

