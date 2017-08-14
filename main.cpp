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
	int debug = 0;

	if (argc == 2 && strcmp(argv[1], "d") == 0 )
	{
		debug = 1;
	}
	for (int i = 0; i < 12; ++i)
	{
		for (int j = 0; j < 16; ++j)
		{
            if(debug)
			cout << endl << "************************************************" << endl;
			char command[50] = {};

			if (debug)
			{
				sprintf(command,"./attack %d %d d",i, j);
			} else {
				sprintf(command,"./attack %d %d",i, j);
			}

            if(debug)
			cout << "executing " << command << endl;
			// printf("****%s\n", command.c_str());
		    system(command);
		    // sleep(1);
		}
	}
	return 0;
}