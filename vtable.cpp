#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <malloc.h>
#include <iostream>

using namespace std;

void* destination = NULL;
char* attackString = "sh";
int arg1;
int arg2;
int debug;


class ParentOne
{
    public:
        void* buffer[3];
        virtual void doSomething1(char *string){
            cout << "called ParentOne::F1" << endl;

        }
        virtual void doSomething2(char *string){
            cout << "called ParentOne::F2" << endl;
        }
};

class ParentTwo
{
    public:
        
        virtual void foo1(char *string){   // attack
            printf("%d %d\n", arg1, arg2);
            // cout << "called ParentTwo::F1" << endl;
        }
        virtual void foo2(char *string)   // attack
        {
            printf("%d %d\n", arg1, arg2);
            // cout << "called ParentTwo::F2" << endl;
        }
};

class ParentThree
{
    public:
        
        void* buffer[3];
        virtual void foo1(char *string){
            cout << "called ParentThree::F1" << endl;
        }
        virtual void foo2(char *string)
        {
            cout << "called ParentThree::F2" << endl;
        }
};

class ParentFour
{
    public:
        
        virtual void ParentFourfoo1(char *string){   // attack
            printf("%d %d\n", arg1, arg2);
            // cout << "called ParentFour::F1" << endl;
        }
        virtual void ParentFourfoo2(char *string)   // attack
        {
            printf("%d %d\n", arg1, arg2);
            // cout << "called ParentFour::F2" << endl;
        }
};

class ParentFive
{
    public:
        
        virtual void ParentFivefoo1(char *string){   // attack
            printf("%d %d\n", arg1, arg2);
            // cout << "called ParentFive::F1" << endl;
        }
        virtual void ParentFivefoo2(char *string)   // attack
        {
            printf("%d %d\n", arg1, arg2);
            // cout << "called ParentFive::F2" << endl;
        }
};

class SingleInherit1:public ParentOne
{
    public:
        virtual void doSomething1(char *string)
        {
            cout << "called SingleInherit1::F1" << endl;
        }
        virtual void doSomething2(char *string){
            cout << "called SingleInherit1::F2" << endl;
        }

};
class SingleInherit2:public ParentOne
{
    public:
        virtual void doSomething1(char *string)
        {
            printf("%d %d\n", arg1, arg2);
            // cout << "called SingleInherit2::F1" << endl;
        }
        virtual void doSomething2(char *string){
            printf("%d %d\n", arg1, arg2);
            // cout << "called SingleInherit2::F2" << endl;
        }

};

class DoubleInheritChild1:public ParentThree, public ParentFour
{
    public:
        // virtual void doSomething1(char *string)
        // {
        //     cout << "called DoubleInheritChild1::F1" << endl;
        // }
        // virtual void doSomething2(char *string){
        //     cout << "called DoubleInheritChild1::F2" << endl;
        // }

        // virtual void foo1(char *string){
        //     cout << "called DoubleInheritChild1::F3" << endl;
        // }
        // virtual void foo2(char *string)
        // {
        //     cout << "called DoubleInheritChild1::F4" << endl;
        // }
        void overflow() {

            void** vPTRlocation = (void**)this;
            vPTRlocation += 4;// sizeof(void*)*4;  // 3 for array one for initial vptr

            *(vPTRlocation) = (void*)destination;
        }


};
class DoubleInheritChild2:public ParentThree, public ParentFour
{
    public:
        virtual void ParentFourfoo1(char *string)
        {
            printf("%d %d\n", arg1, arg2);
            // cout << "called DoubleInheritChild2::F1" << endl;
        }
        virtual void ParentFourfoo2(char *string){
            printf("%d %d\n", arg1, arg2);
            // cout << "called DoubleInheritChild2::F2" << endl;
        }

        virtual void ParentFivefoo1(char *string){
            printf("%d %d\n", arg1, arg2);
            // cout << "called DoubleInheritChild2::F3" << endl;
        }
        virtual void ParentFivefoo2(char *string)
        {
            printf("%d %d\n", arg1, arg2);
            // cout << "called DoubleInheritChild2::F4" << endl;
        }


};


void doSyscall(int x, char* string) {
            printf("%d %d\n", arg1, arg2);
    // cout << "at doSyscall function ATTACK successful\n";

    // cout << "calling system with args: " << string << endl;
    // system(string);
}





void doUAF (void* ptr, int size) {
    void** newPtr = NULL;
    switch (arg1)
    {
        case 0: {
            if (debug)
                cout << "UAF with valid inheritance" << endl;

            newPtr = (void**) new SingleInherit2();

            break;
        }
        case 1: {
            if (debug) 
                cout << "UAF middle of good inheritance" << endl;

            newPtr = (void**) malloc(size);
            if (arg2 == 9 || arg2 == 11 || arg2 == 13 || arg2 == 15)  // double inherit overwrite 2nd vptr
                newPtr[4] = destination;
            else
                *newPtr = destination;

            break;
        }
        case 2: {
            if (debug)
                cout << "uaf with invalid inheritance" << endl;

            newPtr = (void**) new ParentTwo();

            break;
        }
        case 3: {
            if (debug) 
                cout << "uaf middle of invalid inheritance" << endl;
            newPtr = (void**) malloc(size);
            if (arg2 == 9 || arg2 == 11)  // double inherit overwrite 2nd vptr
                newPtr[4] = destination;
            else
                *newPtr = destination;
            break;
        }
        case 4: {
            if (debug) 
                cout << "uaf crafted vtable with valid inheritance" << endl;
            newPtr = (void**) malloc(size);
            if (arg2 == 9 || arg2 == 11)  // double inherit overwrite 2nd vptr
                newPtr[4] = destination;
            else
                *newPtr = destination;
            break;
        }
        case 5: {
            if (debug) 
                cout << "uaf crafted vtable with invalid inheritance" << endl;
            newPtr = (void**) malloc(size);
            if (arg2 == 9 || arg2 == 11)  // double inherit overwrite 2nd vptr
                newPtr[4] = destination;
            else
                *newPtr = destination;
            break;
        }
        case 6: {
            if (debug) 
                cout << "uaf nonClassFunc" << endl;
            newPtr = (void**) malloc(size);
            if (arg2 == 9 || arg2 == 11)  // double inherit overwrite 2nd vptr
                newPtr[4] = destination;
            else
                *newPtr = destination;
            break;
        }
        case 7: {
            newPtr = (void**) malloc(size);

            if (arg2 == 9 || arg2 == 11)  // double inherit overwrite 2nd vptr
                newPtr[4] = destination;
            else
                *newPtr = destination;

            if (debug) 
            {
                cout << "uaf middle of a nonClassFunc" << endl;
                printf("location of syslbl/destination = %p\n", (void*)(*((void***)*newPtr)));
            }
            break;
        }
        case 8: {
            if (debug) 
                cout << "uaf start of a vtable of other parent in double inheritance" << endl;
            newPtr = (void**) malloc(size);

            if (arg2 == 9 || arg2 == 11)  // double inherit overwrite 2nd vptr
            {
                *newPtr = destination;
                break;
            }

            void* temp = (void*) new ParentFour();
            memcpy((void*)(newPtr + 4), temp, size-4 > sizeof(ParentFour) ? sizeof(ParentFour) : size-4);
            free (temp);

            break;
        }
        case 9: {
            if (debug) 
                cout << "uaf middle of a vtable of other parent in double inheritance" << endl;
            newPtr = (void**) malloc(size);

            if (arg2 == 9 || arg2 == 11)  // double inherit overwrite 2nd vptr
                newPtr[4] = destination;
            else
                *newPtr = destination;
            break;
        }
        case 10: {
            if (debug) 
                cout << "uaf start of a vtable of third unrelated parent in double inheritance" << endl;

            newPtr = (void**) malloc(size);
            if (arg2 == 9 || arg2 == 11)  // double inherit overwrite 2nd vptr
            {
                *newPtr = destination;
                break;
            }
            void* temp = (void*) new ParentFive();
            memcpy((void*)(newPtr + 4), temp, size-4 > sizeof(ParentFive) ? sizeof(ParentFive) : size-4);
            free (temp);

            break;
        }
        case 11: {
            if (debug) 
                cout << "uaf middle of a vtable of third unrelated parent in double inheritance" << endl;
            newPtr = (void**) malloc(size);

            if (arg2 == 9 || arg2 == 11)  // double inherit overwrite 2nd vptr
                newPtr[4] = destination;
            else
                *newPtr = destination;

            break;
        }
            // if (arg2 == 9 || arg2 == 11 || arg2 == 13 || arg2 == 15)  // double inherit overwrite 2nd vptr
            //     newPtr[4] = *newPtr;


        default: {
            cout << "bad first option\n";
            exit(0);
            break;
        }
    }


    if (debug && newPtr!=ptr)
    {
        cout << "UAF new pointer not in the same location" << endl;
        printf("Old = %p New = %p\n", ptr, newPtr);
        exit(0);
    }
}

int main(int argc, char const *argv[])
{
    if (argc < 3)
    {
        cout << "not enough params\n";
        exit(0);
    }

    debug = 0;
    if (argc == 4 && strcmp(argv[3], "d") == 0) 
    {
        debug = 1;   
    }
    if (argc == 5 && strcmp(argv[4], "d") == 0) 
    {
        debug = 1;   
    }

    int uaf = 0;
    if (argc == 4 && strcmp(argv[3], "u") == 0) 
    {
        uaf = 1;   
    }

    if (argc == 5 && strcmp(argv[3], "u") == 0) 
    {
        uaf = 1;   
    }

    arg1 = atoi(argv[1]);
    arg2 = atoi(argv[2]);


// SET DESTINATIONS
    switch (atoi(argv[1])) {  //todo fix

        case 0:  // start of good vable point to good inheritance func: 
        {
            if (atoi(argv[2]) > 7)  // double inherit
            {
                if(debug)
                cout << "not possible for now\n";
                exit(0);
                // DoubleInheritChild1* object2 = new DoubleInheritChild2();
                // void* vtableO2 = (void*)*((void**)object2 +4);  // 1 ptr + 3 for buffer
                // destination = vtableO2;
                // delete object2;
                // break;
            }

            if (uaf) break; // if uaf enabled then we just create an object of that type

            if (debug)
            cout << "point to the start of a vtable with valid inheritance" << endl;

            // P1 obj of SI1 points to SI2
            ParentOne* object2 = new SingleInherit2();
            void* vtableO2 = (void*)*(void**)object2;
            destination = vtableO2;
            delete object2;
            break;
        }


        case 1:  // middle of good vable point to good inheritance func: 
        {
            if (atoi(argv[2]) > 7)  // double inherit
            {
                if(debug)
                cout << "not possible for now\n";
                exit(0);
                // DoubleInheritChild1* object2 = new DoubleInheritChild2();
                // void* vtableO2 = (void*)*((void**)object2 +4);  // 1 ptr + 3 for buffer
                // destination = (void*)((uint64_t)vtableO2 + sizeof(void*));  // second ptr
                // delete object2;
                // break;
            }

            // if (uaf) break; // if uaf enabled then we just create an object of that type

            
            if (debug)
            cout << "point to the middle of a vtable with valid inheritance" << endl;

            // P1 obj of SI1 points to SI2 func 2
            ParentOne* object2 = new SingleInherit2();
            void* vtableO2 = (void*)*(void**)object2;
            destination = (void*)((uint64_t)vtableO2 + sizeof(void*));
            
            delete object2;
            break;
        }

        case 2:  // start of good vable point to bad inheritance func:
        {
            if (uaf) break; // if uaf enabled then we just create an object of that type

            
            if (debug)
            cout << "point to the start of a vtable with invalid inheritance" << endl;

            if (atoi(argv[2]) > 7)  // double inherit
            {
                ParentTwo* object2 = new ParentTwo();
                void* vtableO2 = (void*)*((void**)object2);  // 1 ptr + 3 for buffer
                destination = (void*)vtableO2;  // first ptr
                delete object2;
                break;
            }


            // P1 obj of SI1 points to P2
            ParentTwo* object2 = new ParentTwo();
            void* vtableO2 = (void*)*(void**)object2;
            destination = vtableO2;
            
            delete object2;
            break;
        }

        case 3:  // middle of good vable point to bad inheritance func:
        {

            // if (uaf) break; // if uaf enabled then we just create an object of that type

            
            if (debug)
            cout << "point to the middle of a vtable with invalid inheritance" << endl;

            if (atoi(argv[2]) > 7)  // double inherit
            {
                ParentTwo* object2 = new ParentTwo();
                void* vtableO2 = (void*)*((void**)object2);  // 1 ptr + 3 for buffer
                destination = (void*)((uint64_t)vtableO2 + sizeof(void*));  // second ptr
                delete object2;
                break;
            }

            // P1 obj of SI1 points to P2 func 2
            ParentTwo* object2 = new ParentTwo();
            void* vtableO2 = (void*)*(void**)object2;
            destination = (void*)((uint64_t)vtableO2 + sizeof(void*));
            
            delete object2;
            break;
        }

        case 4:  // crafted vable point to good inheritance func:
        {

            // if (uaf) break; // if uaf enabled then we just create an object of that type

            
            if (debug)
            cout << "point to a crafted vtable with valid inheritance" << endl;


            if (atoi(argv[2]) > 7)  // double inherit
            {
                void** buffer = (void**)malloc (sizeof(void*) * 8);  // make it longer than necessary
                DoubleInheritChild1* object2 = new DoubleInheritChild1();
                void* vtableO2 = (void*)*((void**)object2 + 4);  // 1 initial vptr + 3 for buffer + 1 for second method

                for (int i = 0; i < 8; ++i)
                {
                    *(buffer+i) = (void*) *(((void**)vtableO2) + 1);
                }

                destination = buffer;
                delete object2;
                break;
            }

            // make a buffer and use it as a vtable point to SI2
            void** buffer = (void**)malloc (sizeof(void*) * 8);  // make it longer than necessary
            ParentOne* object2 = new SingleInherit2();
            void* vtableO2 = (void*)*(void**)object2;

            for (int i = 0; i < 8; ++i)
            {
                *(buffer+i) = *(((void**)vtableO2));
            }

            destination = buffer;
            
            delete object2;
            break;
        }

        case 5:  // crafted vable point to bad inheritance func:
        {

            // if (uaf) break; // if uaf enabled then we just create an object of that type

            
            if (debug)
            cout << "point to a crafted vtable with invalid inheritance" << endl;

            // dont need this just point to P3 object 
            if (atoi(argv[2]) > 7)  // double inherit
            {
                // void** buffer = (void**)malloc (sizeof(void*) * 8);  // make it longer than necessary
                // DoubleInheritChild2* object2 = new DoubleInheritChild2();
                // void* vtableO2 = (void*)*((void**)object2 +4);  // 1 ptr + 3 for buffer
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
            // if (uaf) break; // if uaf enabled then we just create an object of that type

            
            if (debug)
            cout << "point to a nonClassFunc" << endl;

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
            // if (uaf) break; // if uaf enabled then we just create an object of that type

            
            if (debug)
            cout << "point to the middle of a nonClassFunc" << endl;

            // make a buffer and use it as a vtable point to syslbl
            void** buffer = (void**)malloc (sizeof(void*) * 8);  // make it longer than necessary

            for (int i = 0; i < 8; ++i)
            {
                *(buffer+i) = &&syslbl;
            }

            if (debug)
            {
                printf("location of syslbl = %p\n", &&syslbl);
            }

            destination = buffer;
            break;
        }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        case 8:  // point to the start of a vtable of other parent in double inheritance 
        {
            if (atoi(argv[2]) < 8)  // single inherit
            {
                if(debug)
                cout << "not possible for now\n";
                exit(0);            
            }

            if (uaf) break; // if uaf enabled then we just create an object of that type

            
            if (debug)
            cout << "point to the start of a vtable of other parent in double inheritance" << endl;

            ParentFour* object2 = new ParentFour();
            void* vtableO2 = (void*)*(void**)object2;
            destination = vtableO2;
            delete object2;
            break;
        }

        case 9:  // point to the middle of a vtable of other parent in double inheritance
        {
            if (atoi(argv[2]) < 8)  // single inherit
            {
                if(debug)
                cout << "not possible for now\n";
                exit(0);            
            }

            // if (uaf) break; // if uaf enabled then we just create an object of that type

            
            if (debug)
            cout << "point to the middle of a vtable of other parent in double inheritance" << endl;

            ParentFour* object2 = new ParentFour();
            void* vtableO2 = (void*)*(void**)object2;
            destination = (void*)((uint64_t)vtableO2 + sizeof(void*));
            
            delete object2;
            break;
        }

        case 10:  // point to the start of a vtable of third unrelated parent in double inheritance 
        {
            if (atoi(argv[2]) < 8)  // single inherit
            {
                if(debug)
                cout << "not possible for now\n";
                exit(0);            
            }

            if (uaf) break; // if uaf enabled then we just create an object of that type

            
            if (debug)
            cout << "point to the start of a vtable of third unrelated parent in double inheritance" << endl;

            ParentFive* object2 = new ParentFive();
            void* vtableO2 = (void*)*(void**)object2;
            destination = vtableO2;
            delete object2;
            break;
        }

        case 11:  // point to the middle of a vtable of third unrelated parent in double inheritance
        {
            if (atoi(argv[2]) < 8)  // single inherit
            {
                if(debug)
                cout << "not possible for now\n";
                exit(0);            
            }

            // if (uaf) break; // if uaf enabled then we just create an object of that type

            
            if (debug)
            cout << "point to the middle of a vtable of third unrelated parent in double inheritance" << endl;

            ParentFive* object2 = new ParentFive();
            void* vtableO2 = (void*)*(void**)object2;
            destination = (void*)((uint64_t)vtableO2 + sizeof(void*));
            
            delete object2;
            break;
        }
        // todo shellcode
        // todo existing code double reference

        default: {
            cout << "bad first option\n";
            exit(0);
            break;
        }
    }


    switch (atoi(argv[2])) {  //todo fix
        case 0:  //stack+SingleInherit+sequentialOverFlow:
        {
            if (debug)
            cout << "stack+SingleInherit+sequentialOverFlow\n";
            SingleInherit1 singleObj;
            void* buffer[3];

            ParentOne* object = &singleObj;
            if (uaf)
            {
                if (debug) {
                    cout << "no UAF on stack" << endl;
                }
                exit(0);
            }

            if (((void*)object < (void*)buffer))
            {
                if (debug) {
                    cout << "*****cant overflow...object before buffer" << endl;
                    printf("object = %p\tbuffer = %p\n", object, buffer);
                }
                exit(0);
            }

            if (debug) {
                printf("object = %p\tbuffer = %p\n", object, buffer);
                printf("old vpointer = %p\n", (void*)*(void**)object);
            }

            // printf("address of buffer = %p \naddress of object = %p\n\n", &buffer[0], &singleObj);
            for (int i = 0; i<((uint64_t)object - (uint64_t)buffer); ++i)  // overflow
            {
                *(buffer + i) = (void*)destination;
                if ((void*)(buffer + i) == (void*)object)
                    break;
            }

            if (debug) 
            {
                printf("new vpointer = %p\n", (void*)*(void**)object);
                printf("vpointer to copy = %p\n", destination);
            }

            if(debug)
                cout << "calling SingleInherit1::F1" << endl;
            object->doSomething1(attackString);
            break;
        }

        case 1:  // heap+SingleInherit+sequentialOverFlow:
        {
            if (debug)
            cout << "heap+SingleInherit+sequentialOverFlow\n";
            // void** buffer = (void **)malloc(sizeof(void*)*3);
            void** buffer = new void*[1];   // stick with 1 otherwise get memory dump
            // void** buffer = (void **)malloc(0x30);
// printf("%d\n", sizeof(SingleInherit1));
// printf("%d\n", sizeof(void*));
            ParentOne* object = new SingleInherit1();

            if (uaf)
            {
                delete object;
                doUAF(object, sizeof(SingleInherit1));
                goto attack1; 
            }

            if (((void*)object < (void*)buffer))
            {
                if (debug)
                cout << "*****cant overflow...object before buffer" << endl;
                if (debug)
                printf("object = %p\tbuffer = %p\n", object, buffer);
                exit(0);
            }

            if (debug)
                printf("object = %p\tbuffer = %p\n", object, buffer);
            if (debug)
            printf("old vpointer = %p\n", (void*)*(void**)object);

            for (int i = 0;i<((uint64_t)object - (uint64_t)buffer) ; ++i)  // overflow
            {
                // cout << ((uint64_t)object - (uint64_t)buffer)<< "   "<<i << endl;
                *(buffer + i) = (void*)destination;
                if ((void*)(buffer + i) == (void*)object)
                    break;
            }

            // free(buffer);   // dont free metadata might crash due to corruption
            if (debug)
            printf("new vpointer = %p\n", (void*)*(void**)object);
            if (debug)
            printf("vpointer to copy = %p\n", destination);


            attack1:    
                if(debug)
                    cout << "calling SingleInherit1::F1" << endl;

                object->doSomething1(attackString);

                break;
        }

        case 2:  // stack+SingleInherit+indirectOverwrite:
        {
            if (uaf)
            {
                if (debug) {
                    cout << "no UAF on stack" << endl;
                }
                exit(0);
            }

            if (debug)
            cout << "stack+SingleInherit+indirectOverwrite\n";
            SingleInherit1 singleObj;

            ParentOne* object = &singleObj;

            if (debug)
            printf("old vpointer = %p\n", (void*)*(void**)object);

            *(void**)object = destination;

            if (debug)
            printf("new vpointer = %p\n", (void*)*(void**)object);
            if (debug)
            printf("vpointer to copy = %p\n", destination);
            if(debug)
            cout << "calling SingleInherit1::F1" << endl;
            object->doSomething1(attackString);
            break;
        }

        case 3:  // heap+SingleInherit+indirectOverwrite:
        {
            if (debug)
            cout << "heap+SingleInherit+indirectOverwrite\n";
            ParentOne* object = new SingleInherit1();

            if (uaf)
            {
                delete object;
                doUAF(object, sizeof(SingleInherit1));
                goto attack3; 
            }

            if (debug)
            printf("old vpointer = %p\n", (void*)*(void**)object);

            *(void**)object = destination;
            
            if (debug)
            printf("new vpointer = %p\n", (void*)*(void**)object);
            if (debug)
            printf("vpointer to copy = %p\n", destination);
            
            attack3:
                if(debug)
                    cout << "calling SingleInherit1::F1" << endl;
                object->doSomething1(attackString);
                break;
        }




        case 4:  //stack+SingleInherit+sequentialOverFlow:
        {
            if (uaf)
            {
                if (debug) {
                    cout << "no UAF on stack" << endl;
                }
                exit(0);
            }

            if (debug)
            cout << "stack+SingleInherit+sequentialOverFlow\n";
            SingleInherit1 singleObj;
            void* buffer[3];

            SingleInherit1* object = &singleObj;

            if (((void*)object < (void*)buffer))
            {
                if (debug)
                cout << "*****cant overflow...object before buffer" << endl;
                if (debug)
                printf("object = %p\tbuffer = %p\n", object, buffer);
                exit(0);
            }

            if (debug)
                printf("object = %p\tbuffer = %p\n", object, buffer);

            if (debug)
                printf("old vpointer = %p\n", (void*)*(void**)object);
            // printf("address of buffer = %p \naddress of object = %p\n\n", &buffer[0], &singleObj);
            for (int i = 0; i<((uint64_t)object - (uint64_t)buffer); ++i)  // overflow
            {
                *(buffer + i) = (void*)destination;
                if ((void*)(buffer + i) == (void*)object)
                    break;
            }

            if (debug) 
            {
                printf("new vpointer = %p\n", (void*)*(void**)object);
                printf("vpointer to copy = %p\n", destination);
            }

            if(debug)
            cout << "calling SingleInherit1::F1" << endl;
            object->doSomething1(attackString);
            break;
        }

        case 5:  // heap+SingleInherit+sequentialOverFlow:
        {

            if (debug)
            cout << "heap+SingleInherit+sequentialOverFlow\n";
            // void** buffer = (void **)malloc(sizeof(void*)*3);
            void** buffer = new void*[1];   // stick with 1 otherwise get memory dump
            // void** buffer = (void **)malloc(0x30);
// printf("%d\n", sizeof(SingleInherit1));
// printf("%d\n", sizeof(void*));
            SingleInherit1* object = new SingleInherit1();

            if (uaf)
            {
                delete object;
                doUAF(object, sizeof(SingleInherit1));
                goto attack5; 
            }

            if (((void*)object < (void*)buffer))
            {
                if (debug)
                cout << "*****cant overflow...object before buffer" << endl;
                if (debug)
                printf("object = %p\tbuffer = %p\n", object, buffer);
                exit(0);
            }

            if (debug)
                printf("object = %p\tbuffer = %p\n", object, buffer);
            if (debug)
            printf("old vpointer = %p\n", (void*)*(void**)object);

            for (int i = 0;i<((uint64_t)object - (uint64_t)buffer) ; ++i)  // overflow
            {
                // cout << ((uint64_t)object - (uint64_t)buffer)<< "   "<<i << endl;
                *(buffer + i) = (void*)destination;
                if ((void*)(buffer + i) == (void*)object)
                    break;
            }

            // free(buffer);   // dont free metadata might crash due to corruption
            if (debug)
            printf("new vpointer = %p\n", (void*)*(void**)object);
            if (debug)
            printf("vpointer to copy = %p\n", destination);

            attack5:
                if(debug)
                cout << "calling SingleInherit1::F1" << endl;
                object->doSomething1(attackString);

                break;
        }

        case 6:  // stack+SingleInherit+indirectOverwrite:
        {
            if (uaf)
            {
                if (debug) {
                    cout << "no UAF on stack" << endl;
                }
                exit(0);
            }

            if (debug)
            cout << "stack+SingleInherit+indirectOverwrite\n";
            SingleInherit1 singleObj;

            SingleInherit1* object = &singleObj;

            if (debug)
            printf("old vpointer = %p\n", (void*)*(void**)object);

            *(void**)object = destination;

            if (debug)
            printf("new vpointer = %p\n", (void*)*(void**)object);
            if (debug)
            printf("vpointer to copy = %p\n", destination);
            if(debug)
            cout << "calling SingleInherit1::F1" << endl;
            object->doSomething1(attackString);
            break;
        }

        case 7:  // heap+SingleInherit+indirectOverwrite:
        {
            if (debug)
            cout << "heap+SingleInherit+indirectOverwrite\n";
            SingleInherit1* object = new SingleInherit1();

            if (uaf)
            {
                delete object;
                doUAF(object, sizeof(SingleInherit1));
                goto attack7; 
            }

            if (debug)
            printf("old vpointer = %p\n", (void*)*(void**)object);

            *(void**)object = destination;
            
            if (debug)
            printf("new vpointer = %p\n", (void*)*(void**)object);
            if (debug)
            printf("vpointer to copy = %p\n", destination);
            
            attack7:
                if(debug)
                cout << "calling SingleInherit1::F1" << endl;
                object->doSomething1(attackString);
                break;
        }

//////////////////////////double
        case 8:  // stack+DoubleInherit+withinObjectOverFlow:
        {
            if (uaf)
            {
                if (debug) {
                    cout << "no UAF on stack" << endl;
                }
                exit(0);
            }

            if (debug)
            cout << "stack+DoubleInherit+withinObjectOverFlow\n";
            DoubleInheritChild1 doubleObj;

            DoubleInheritChild1* object = &doubleObj;

            if (debug)
            printf("old vpointer = %p\n", (void*)*((void**)object + 4));

            for (int i = 0; i < 4; ++i)  // overflow assume that vptr is directly after
            {
                *(object->buffer + i) = (void*)destination;
            }

            if (debug)
            printf("new vpointer = %p\n", (void*)*((void**)object + 4));
            if (debug)
            printf("vpointer to copy = %p\n", destination);

            if(debug)
            cout << "calling DoubleInheritChild1::F3 ParentFour::F1" << endl;
            object->ParentFourfoo1(attackString);
            break;
        }

        case 9:  // heap+DoubleInherit+withinObjectOverFlow:
        {
            if (debug)
                cout << "heap+DoubleInherit+withinObjectOverFlow\n";

            DoubleInheritChild1* object = new DoubleInheritChild1();

            if (uaf)
            {
                delete object;
                doUAF(object, sizeof(DoubleInheritChild1));
                goto attack9; 
            }

            if (debug)
            printf("old vpointer = %p\n", (void*)*((void**)object + 4));

            for (int i = 0; i < 4; ++i)  // overflow assume that vptr is directly after
            {
                *(object->buffer + i) = (void*)destination;
            }

            if (debug)
            printf("new vpointer = %p\n", (void*)*((void**)object + 4));
            if (debug)
            printf("vpointer to copy = %p\n", destination);

            attack9:
                if(debug)
                cout << "calling DoubleInheritChild1::F3 ParentFour::F1" << endl;
                object->ParentFourfoo1(attackString);
                break;
        }

        case 10:  // stack+DoubleInherit+INdirectOverwrite:
        {
            if (uaf)
            {
                if (debug) {
                    cout << "no UAF on stack" << endl;
                }
                exit(0);
            }

            if (debug)
            cout << "stack+DoubleInherit+INdirectOverwrite\n";
            DoubleInheritChild1 doubleObj;

            DoubleInheritChild1* object = &doubleObj;

            if (debug)
            printf("old vpointer = %p\n", (void*)*((void**)object + 4));

            object->overflow();

            if (debug)
            printf("new vpointer = %p\n", (void*)*((void**)object + 4));
            if (debug)
            printf("vpointer to copy = %p\n", destination);

            if(debug)
            cout << "calling DoubleInheritChild1::F3 ParentFour::F1" << endl;
            object->ParentFourfoo1(attackString);
            break;
        }

        case 11:  // heap+DoubleInherit+INdirectOverwrite:
        {

            if (debug)
                cout << "heap+DoubleInherit+INdirectOverwrite\n";

            DoubleInheritChild1* object = new DoubleInheritChild1();

            if (uaf)
            {
                delete object;
                doUAF(object, sizeof(DoubleInheritChild1));
                goto attack11; 
            }

            if (debug)
            printf("old vpointer = %p\n", (void*)*((void**)object + 4));

            object->overflow();

            if (debug)
            printf("new vpointer = %p\n", (void*)*((void**)object + 4));
            if (debug)
            printf("vpointer to copy = %p\n", destination);

            attack11:
                if(debug)
                    cout << "calling DoubleInheritChild1::F3 ParentFour::F1" << endl;
                object->ParentFourfoo1(attackString);
                break;
        }
////////////////////////////////////////////////////////////////////////////////////////////////////

        case 12:  //stack+DoubleInheritChild+sequentialOverFlow:
        {
            if (uaf)
            {
                if (debug) {
                    cout << "no UAF on stack" << endl;
                }
                exit(0);
            }

            if (debug)
            cout << "stack+DoubleInheritChild+sequentialOverFlow\n";
            ParentThree singleObj;
            void* buffer[3];

            ParentThree* object = &singleObj;

            if (((void*)object < (void*)buffer))
            {
                if (debug)
                cout << "*****cant overflow...object before buffer" << endl;
                if (debug)
                printf("object = %p\tbuffer = %p\n", object, buffer);
                exit(0);
            }

            if (debug)
                printf("object = %p\tbuffer = %p\n", object, buffer);

            if (debug)
                printf("old vpointer = %p\n", (void*)*(void**)object);

            for (int i = 0; i<((uint64_t)object - (uint64_t)buffer); ++i)  // overflow
            {
                *(buffer + i) = (void*)destination;
                if ((void*)(buffer + i) == (void*)object)
                    break;
            }

            if (debug) 
            {
                printf("new vpointer = %p\n", (void*)*(void**)object);
                printf("vpointer to copy = %p\n", destination);
            }

            if(debug)
            cout << "calling ParentThree::F1" << endl;
            object->foo1(attackString);
            break;
        }

        case 13:  // heap+DoubleInheritChild+sequentialOverFlow:
        {
            if (debug)
            cout << "heap+DoubleInheritChild+sequentialOverFlow\n";

            void** buffer = new void*[1];   // stick with 1 otherwise get memory dump

            ParentThree* object = new ParentThree();

            if (uaf)
            {
                delete object;
                doUAF(object, sizeof(ParentThree));
                goto attack13; 
            }

            if (((void*)object < (void*)buffer))
            {
                if (debug)
                cout << "*****cant overflow...object before buffer" << endl;
                if (debug)
                printf("object = %p\tbuffer = %p\n", object, buffer);
                exit(0);
            }

            if (debug)
                printf("object = %p\tbuffer = %p\n", object, buffer);
            if (debug)
            printf("old vpointer = %p\n", (void*)*(void**)object);

            for (int i = 0;i<((uint64_t)object - (uint64_t)buffer) ; ++i)  // overflow
            {
                // cout << ((uint64_t)object - (uint64_t)buffer)<< "   "<<i << endl;
                *(buffer + i) = (void*)destination;
                if ((void*)(buffer + i) == (void*)object)
                    break;
            }

            // free(buffer);   // dont free metadata might crash due to corruption
            if (debug)
            printf("new vpointer = %p\n", (void*)*(void**)object);
            if (debug)
            printf("vpointer to copy = %p\n", destination);


            attack13:
                if(debug)
                    cout << "calling ParentThree::F1" << endl;
                object->foo1(attackString);

                break;
        }

        case 14:  // stack+DoubleInheritChild+indirectOverwrite:
        {
            if (uaf)
            {
                if (debug) {
                    cout << "no UAF on stack" << endl;
                }
                exit(0);
            }

            if (debug)
            cout << "stack+DoubleInheritChild+indirectOverwrite\n";
            ParentThree singleObj;

            ParentThree* object = &singleObj;

            if (debug)
            printf("old vpointer = %p\n", (void*)*(void**)object);

            *(void**)object = destination;

            if (debug)
            printf("new vpointer = %p\n", (void*)*(void**)object);
            if (debug)
            printf("vpointer to copy = %p\n", destination);
            if(debug)
            cout << "calling ParentThree::F1" << endl;
            object->foo1(attackString);
            break;
        }

        case 15:  // heap+DoubleInheritChild+indirectOverwrite:
        {
            if (debug)
                cout << "heap+DoubleInheritChild+indirectOverwrite\n";

            ParentThree* object = new ParentThree();

            if (uaf)
            {
                delete object;
                doUAF(object, sizeof(ParentThree));
                goto attack15; 
            }

            if (debug)
            printf("old vpointer = %p\n", (void*)*(void**)object);

            *(void**)object = destination;
            
            if (debug)
            printf("new vpointer = %p\n", (void*)*(void**)object);
            if (debug)
            printf("vpointer to copy = %p\n", destination);
            
            attack15:
                if(debug)
                    cout << "calling ParentThree::F1" << endl;

                object->foo1(attackString);
                break;
        }

        case 50: goto syslbl;  // just so that compiler does not remove it

        default: {
            cout << "bad second option\n";
            exit(0);
            break;
        }
    }



    return 0;

syslbl:
    // system(attackString);
    // printf("%s\n", "at syslbl ATTACK successful\n");
    printf("%d %d\n", arg1, arg2);
    if(debug)
    cout << "at syslbl ATTACK successful\n";
    return 0;



 //    ******************uaf
	// Child1* c1 = new Child1();

	// c1 -> doSomething("sh");

	// free(c1);

	// new Child2();

	// c1 -> doSomething("sh");
}


















