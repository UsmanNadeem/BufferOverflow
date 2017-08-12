#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <malloc.h>
#include <iostream>
#include <unistd.h>

using namespace std;



int main(int argc, char const *argv[])
{
	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			cout << endl << "************************************************" << endl;
			char command[50] = {};

			sprintf(command,"./attack %d %d",i, j);

			cout << "executing " << command << endl;
			// printf("****%s\n", command.c_str());
		    system(command);
		    // sleep(1);
		}
	}
	return 0;
}