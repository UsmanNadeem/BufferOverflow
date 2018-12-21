#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <malloc.h>
#include <iostream>
#include <unistd.h>

using namespace std;


int foo(int argc, char const *argv[], std::string fileName);

int main(int argc, char const *argv[]) {

	std::cout << "\n*******************************************************\n";
	std::cout << "Running Original Benchmark\n";
	int a = foo (argc, argv, "/home/linux/Desktop/bench/attack");

	// std::cout << "Running CPI compiled Benchmark\n";
	// int b = foo (argc, argv, "/home/linux/Desktop/bench/attack.cpi"); 

	// std::cout << "Running CPS compiled Benchmark\n";
	// int c = foo (argc, argv, "/home/linux/Desktop/bench/attack.cps"); 

	// std::cout << "Running SS compiled Benchmark\n";
	// int d = foo (argc, argv, "/home/linux/Desktop/bench/attack.ss"); 

	std::cout << "\n\n*******************************************************\n";
	std::cout << "Original Attacks Succeeded: " << a << "/" << 90 << std::endl; 
	// std::cout << "CPI Attacks Succeeded: " << b << "/" << 90 << std::endl; 
	// std::cout << "CPS Attacks Succeeded: " << c << "/" << 90 << std::endl; 
	// std::cout << "SS Attacks Succeeded: " << d << "/" << 90 << std::endl; 
	return 0;
}

int foo(int argc, char const *argv[], std::string fileName)
{
	int debug = 0;

	if (argc == 2 && strcmp(argv[1], "d") == 0 )
	{
		debug = 1;
	}

	const int total = 90;
	int currentNum = 0;
	int numPassed = 0;
	for (int i = 0; i < 12; ++i)
	{
		for (int j = 0; j < 16; ++j)
		{	
			if (i < 2)
				if (j>7&&j<12)
					continue;
			if (i>7&&i<12)
				if (j < 8)
					continue;

			if (j == 4 
			|| j == 5
			|| j == 6
			|| j == 7
			|| j == 12
			|| j == 13
			|| j == 14
			|| j == 15
			)
				continue;

			char command[50] = {};
			if (debug)
				sprintf(command,"%s %d %d d", fileName.c_str(), i, j);
			else
				sprintf(command,"%s %d %d", fileName.c_str(), i, j);

			if(debug) cout << "executing " << command << endl;
				cout << command << endl;

			// std::cout << "Now running test #" << ++currentNum << "/" << total << ":\n";

			int status = system(command);

			if (WIFEXITED(status)) {
				int result = WEXITSTATUS(status);
				if (result == 1) ++numPassed;
			}
			// std::cout << "\n\n";

			if (j == 0
			|| j == 2
			|| j == 4
			|| j == 6
			|| j == 8
			|| j == 10
			|| j == 12
			|| j == 14
			/*^^ stack*/
			|| j == 1
			|| j == 5
			|| j == 7
			|| j == 9
			|| j == 13
			|| j == 15
			/*redundant copies*/
			)
				continue;

			char commandUAF[50] = {};
			if (debug)
				sprintf(commandUAF,"%s %d %d u d", fileName.c_str(), i, j);
			else
				sprintf(commandUAF,"%s %d %d u", fileName.c_str(), i, j);
			if(debug) std::cout << "executing " << commandUAF << endl;
				cout << commandUAF << endl;

			// std::cout << "Now running test #" << ++currentNum << "/" << total << ":\n";
			status = system(command);
			if (WIFEXITED(status)) {
				int result = WEXITSTATUS(status);
				if (result == 1) ++numPassed;
			}
		}
	}


	return numPassed;
}

/*0 = normal exec
-1 = attacked
-2 = bad option
-3 = attack failed*/