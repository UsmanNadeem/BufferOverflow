#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <malloc.h>
#include <iostream>

using namespace std;

void* destination = NULL;
char* attackString = "/bin/sh";


class ParentOne
{
    public:
        void* buffer[3];
        virtual void doSomething(char *string){

        }
};

class ParentTwo
{
    public:
        
        virtual void foo(char *string){

        }
};

class Child1:public ParentOne
{
    public:
        virtual void doSomething(char *string)
        {
        	cout << "Child1!!!\nInput string: \"" << string << "\"" << endl;
        }

};

class Child2:public ParentOne
{
    public:
        virtual void doSomething(char *string)
        {
        	cout << "Child2!!!\nInput string: \"" << string << "\"" << endl;
        	system(string);
        }

};

class SingleInherit1:public ParentOne
{
    public:
        virtual void doSomething(char *string)
        {
            cout << "SingleInherit1!!!\nInput string: \"" << string << "\"" << endl;
        }

};
class SingleInherit2:public ParentOne
{
    public:
        virtual void doSomething(char *string)
        {
            cout << "SingleInherit2!!!\nInput string: \"" << string << "\"" << endl;
        }

};

class DoubleInherit1:public ParentOne, public ParentTwo
{
    public:
        virtual void doSomething(char *string)
        {
            cout << "DoubleInherit1!!!\nInput string: \"" << string << "\"" << endl;
        }
        void indirectOverwrite() {
            void** writeTo;
            void* writeThis;
            void* _buffer[3];
            
            // todo could be infinite
            for (int i = 0; ; ++i)  // overflow the destination+source
            {
                *(_buffer + i) = (void*)destination;
                if ((void*)(_buffer + i) == (void*)&writeTo)
                {
                    uint64_t vPTRlocation = *(uint64_t*)this;
                    vPTRlocation += sizeof(void*)*4;

                    *(_buffer + i) = (void*)vPTRlocation;
                    break;
                }
            }

            *writeTo = writeThis; 
        }


};
class DoubleInherit2:public ParentOne, public ParentTwo
{
    public:
        virtual void doSomething(char *string)
        {
            cout << "DoubleInherit2!!!\nInput string: \"" << string << "\"" << endl;
        }

        // virtual void foo(char *string)
        // {
        //     cout << "DoubleInherit2!!!\nInput string: \"" << string << "\"" << endl;
        // }

};

class SyscallParent
{
    public:
        virtual void doSyscall(char *string)
        {
            // cout << "ParentOne!!!\nInput string: \"" << string << "\"" << endl;
        }
};

class SyscallChild: public SyscallParent
{
    public:
        virtual void doSyscall(char *string)
        {
            system(string);
            // cout << "ParentOne!!!\nInput string: \"" << string << "\"" << endl;
        }
};



void doSyscall(char* string) {
    system(string);
}

void doSyscallInMiddle(char* _string) {

    printf("in doSyscallInMiddle function\n");
    string temp = _string;
    temp[0] = 'a';
sys:
    system(_string);
}

int main(int argc, char const *argv[])
{

    switch (atoi(argv[1])) {  //todo fix

        case 1:  // start of good vable point to good inheritance func: 
        case 2:  // middle of good vable point to good inheritance func: 

        case 3:  // start of good vable point to bad inheritance func:
        case 4:  // middle of good vable point to bad inheritance func:
        

        case 5:  // crafted vable point to bad inheritance func:
        case 6:  // crafted vable point to good inheritance func:  // order can be different

        case 7:  // crafted vable nonClassFunc:
        case 8:  // crafted vable middle of nonClassFunc:
        // todo shellcode
        // todo existing code double reference

        // more options for double inherit**************************************************8
        // e.g. start of first vtable
        default: break;
    }


    switch (atoi(argv[2])) {  //todo fix
        case 0:  //stack+SingleInherit+sequentialOverFlow:
        {
            SingleInherit1 singleObj;
            void* buffer[3];

            ParentOne* object = &singleObj;

            // todo could be infinite
            for (int i = 0; ; ++i)  // overflow
            {
                *(buffer + i) = (void*)destination;
                if ((void*)(buffer + i) == (void*)object)
                    break;
            }


            object->doSomething(attackString);
            break;
        }

        case 1:  // heap+SingleInherit+sequentialOverFlow:
        {
            void** buffer = (void **)malloc(sizeof(void*)*3);

            ParentOne* object = new SingleInherit1();

            // todo could be infinite
            for (int i = 0; ; ++i)  // overflow
            {
                *(buffer + i) = (void*)destination;
                if ((void*)(buffer + i) == (void*)object)
                    break;
            }


            object->doSomething(attackString);

            break;
        }

        case 2:  // stack+SingleInherit+indirectOverwrite:
        {
            SingleInherit1 singleObj;

            void** writeTo;
            void* writeThis;
            void* buffer[3];
            
            ParentOne* object = &singleObj;

            // todo could be infinite
            for (int i = 0; ; ++i)  // overflow the destination+source
            {
                *(buffer + i) = (void*)destination;
                if ((void*)(buffer + i) == (void*)&writeTo)
                {
                    *(buffer + i) = (void*)object;
                    break;
                }
            }

            *writeTo = writeThis; 

            object->doSomething(attackString);
            break;
        }

        case 3:  // heap+SingleInherit+indirectOverwrite:
        {
            ParentOne* object = new SingleInherit1();

            void** writeTo;
            void* writeThis;
            void* buffer[3];

            // todo could be infinite
            for (int i = 0; ; ++i)  // overflow the destination+source
            {
                *(buffer + i) = (void*)destination;
                if ((void*)(buffer + i) == (void*)&writeTo)
                {
                    *(buffer + i) = (void*)object;
                    break;
                }
            }

            *writeTo = writeThis; 

            object->doSomething(attackString);
            break;
        }

//////////////////////////double
        case 4:  // stack+DoubleInherit+withinObjectOverFlow:
        {
            DoubleInherit1 doubleObj;

            DoubleInherit1* object = &doubleObj;

            for (int i = 0; i < 4; ++i)  // overflow assume that vptr is directly after
            {
                *(object->buffer + i) = (void*)destination;
            }

            object->foo(attackString);
            break;
        }

        case 5:  // heap+DoubleInherit+withinObjectOverFlow:
        {
            DoubleInherit1* object = new DoubleInherit1();

            for (int i = 0; i < 4; ++i)  // overflow assume that vptr is directly after
            {
                *(object->buffer + i) = (void*)destination;
            }

            object->foo(attackString);
            break;
        }

        case 6:  // stack+DoubleInherit+INdirectOverwrite:
        {
            DoubleInherit1 doubleObj;

            DoubleInherit1* object = &doubleObj;

            object->indirectOverwrite();

            object->doSomething(attackString);
            break;
        }

        case 7:  // heap+DoubleInherit+INdirectOverwrite:
        {

            DoubleInherit1* object = new DoubleInherit1();

            object->indirectOverwrite();

            object->doSomething(attackString);
            break;
        }

        default: break;
    }









 //    ******************uaf
	// Child1* c1 = new Child1();

	// c1 -> doSomething("sh");

	// free(c1);

	// new Child2();

	// c1 -> doSomething("sh");
}


















