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
	int uaf = 0;

	if (argc == 2 && strcmp(argv[1], "d") == 0 )
	{
		debug = 1;
	}

	if (argc == 2 && strcmp(argv[1], "u") == 0 )
	{
		uaf = 1;
	}

	if (argc == 3 && strcmp(argv[2], "d") == 0 )
	{
		debug = 1;
	}

	if (argc == 3 && strcmp(argv[1], "u") == 0 )
	{
		uaf = 1;
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
				if (uaf)
				{
					sprintf(command,"/home/linux/Desktop/bench/attack %d %d u d",i, j);
				} else {
					sprintf(command,"/home/linux/Desktop/bench/attack %d %d d",i, j);
				}
			} else {
				if (uaf)
				{
					sprintf(command,"/home/linux/Desktop/bench/attack %d %d u",i, j);
				} else {
					sprintf(command,"/home/linux/Desktop/bench/attack %d %d",i, j);
				}
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