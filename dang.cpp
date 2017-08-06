#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <iostream>

using namespace std;

class SingleInheritClass
{
    public:
        virtual void doSomething(char *string)
        {
            cout << "Parent!!!\nInput string: \"" << string << "\"" << endl;
        }
};

class Child1:public SingleInheritClass
{
    public:
        virtual void doSomething(char *string)
        {
        	cout << "Child1!!!\nInput string: \"" << string << "\"" << endl;
        }

};

class Child2:public SingleInheritClass
{
    public:
        virtual void doSomething(char *string)
        {
        	cout << "Attacking!!!\nChild2 executing string: \"" << string << "\"" << endl;
        	system(string);
        }

};

int main()
{
	Child1* c1 = new Child1();

	c1 -> doSomething("sh");

	free(c1);

	new Child2();

	c1 -> doSomething("sh");
}