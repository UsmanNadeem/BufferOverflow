#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <malloc.h>
#include <iostream>

using namespace std;

void* destination = NULL;
char* attackString = "\tAttacked!";
int arg1;
int arg2;
int debug;
int uaf = 0;

class ParentOne
{
    public:
        virtual void doSomething1(char *string){
            cout << "\tcalled ParentOne::F1" << endl;

        }
        virtual void doSomething2(char *string){
            cout << "\tcalled ParentOne::F2" << endl;
        }
};

class ParentTwo
{
    public:
        
        virtual void foo1(char *string){   // attack
            cout << string << " ";
            printf("%d %d\n", arg1, arg2);
            if (debug)
            {
                printf("\tin ParentTwo::F1\n");
            }
            exit(1);
        }
        virtual void foo2(char *string)   // attack
        {
            cout << string << " ";
            printf("%d %d\n", arg1, arg2);
            if (debug)
            {
                printf("\tin ParentTwo::F2\n");
            }
            exit(1);
        }
};

class ParentThree
{
    public:
        
        void* buffer[3];
        virtual void foo1(char *string){
            cout << "\tcalled ParentThree::F1" << endl;
        }
        virtual void foo2(char *string)
        {
            cout << "\tcalled ParentThree::F2" << endl;
        }
};

class ParentFour
{
    public:
        
        virtual void ParentFourfoo1(char *string){   // attack
            cout << string << " ";
            printf("%d %d\n", arg1, arg2);
            if (debug)
            {
                printf("\tin ParentFour::F1\n");
            }
            exit(1);
        }
        virtual void ParentFourfoo2(char *string)   // attack
        {
            cout << string << " ";
            printf("%d %d\n", arg1, arg2);
            if (debug)
            {
                printf("\tin ParentFour::F2\n");
            }
            exit(1);
        }
        virtual void ParentFourfoo3(char *string)
        {
            cout << "\tcalled ParentFour::F3" << endl;
        }
};

class ParentFive
{
    public:
        
        virtual void ParentFivefoo1(char *string){   // attack
            cout << string << " ";
            printf("%d %d\n", arg1, arg2);
            if (debug)
            {
                printf("\tin ParentFive::F1\n");
            }
            exit(1);
        }
        virtual void ParentFivefoo2(char *string)   // attack
        {
            cout << string << " ";
            printf("%d %d\n", arg1, arg2);
            if (debug)
            {
                printf("\tin ParentFive::F2\n");
            }
            exit(1);
        }
};

class SingleInherit1:public ParentOne
{
    public:
        virtual void doSomething1(char *string)
        {
            cout << "\tcalled SingleInherit1::F1" << endl;
        }
        virtual void doSomething2(char *string){
            cout << "\tcalled SingleInherit1::F2" << endl;
        }

};
class SingleInherit2:public ParentOne
{
    public:
        virtual void doSomething1(char *string)
        {
            cout << string << " ";
            printf("%d %d\n", arg1, arg2);
            if (debug)
            {
                printf("\tin SingleInherit2::F1\n");
            }
            exit(1);
        }
        virtual void doSomething2(char *string){
            cout << string << " ";
            printf("%d %d\n", arg1, arg2);
            if (debug)
            {
                printf("\tin SingleInherit2::F2\n");
            }
            exit(1);
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
        virtual void DoubleInheritChild1Foo1() {
            if (uaf)
            cout << "\tUAF Attacked! ";
            else
            cout << "\tAttacked! ";
            printf("%d %d\n", arg1, arg2);
            if (debug)
            {
                printf("\tin DoubleInheritChild1::F1\n");
            }
            exit(1);
        }
        void overflow() {

            void** vPTRlocation = (void**)this;
            vPTRlocation += 4;// sizeof(void*)*4;  // 3 for array one for initial vptr

            *(vPTRlocation) = (void*)destination;
        }


};
class DoubleInheritChild2:public ParentFour, public ParentFive
{
    public:
        virtual void ParentFourfoo1(char *string)
        {
            cout << string << " ";
            printf("%d %d\n", arg1, arg2);
            if (debug)
            {
                printf("\tin DoubleInheritChild2::F1\n");
            }
            exit(1);
        }
        virtual void ParentFourfoo2(char *string){
            cout << string << " ";
            printf("%d %d\n", arg1, arg2);
            if (debug)
            {
                printf("\tin DoubleInheritChild2::F2\n");
            }
            exit(1);
        }

        virtual void ParentFivefoo1(char *string){
            cout << string << " ";
            printf("%d %d\n", arg1, arg2);
            if (debug)
            {
                printf("\tin DoubleInheritChild2::F3\n");
            }
            exit(1);
        }
        virtual void ParentFivefoo2(char *string)
        {
            cout << string << " ";
            printf("%d %d\n", arg1, arg2);
            if (debug)
            {
                printf("\tin DoubleInheritChild2::F4\n");
            }
            exit(1);
        }


};


void doSyscall(int x, char* string) {
    cout << string << " ";
    printf("%d %d\n", arg1, arg2);

    if (debug)
    {
        printf("\tin doSyscall func\n");
    }
    exit(1);
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
            if (arg2 >= 12)  // double inherit
            {
                newPtr = (void**) malloc(size);
                *newPtr = destination;
                void* temp = (void*) new DoubleInheritChild1();
                memcpy((void*)(newPtr), temp, size > sizeof(DoubleInheritChild1) ? sizeof(DoubleInheritChild1) : size);
                free (temp);
                // newPtr = (void**) new DoubleInheritChild1();
                break;
            }

            newPtr = (void**) new SingleInherit2();

            break;
        }
        case 1: {
            if (debug) 
                cout << "UAF middle of good inheritance" << endl;

            newPtr = (void**) malloc(size);
            *newPtr = destination;

            break;
        }
        case 2: {
            if (debug)
                cout << "uaf with invalid inheritance" << endl;
            
            if (arg2 == 9 || arg2 == 11)  // double inherit overwrite 2nd vptr
            {
                newPtr = (void**) malloc(size);
                void* temp = (void*) new ParentTwo();
                memcpy((void*)(newPtr + 4), temp, size-4 > sizeof(ParentTwo) ? sizeof(ParentTwo) : size-4);
                free (temp);
            } 
            else
            {
                if (size == sizeof(ParentTwo))
                {
                    newPtr = (void**) new ParentTwo();
                } else
                {
                    newPtr = (void**) malloc(size);
                    void* temp = (void*) new ParentTwo();
                    memcpy((void*)(newPtr), temp, size > sizeof(ParentTwo) ? sizeof(ParentTwo) : size);
                    free (temp);
                }
            }

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

            if (arg2 == 9 || arg2 == 11)  // double inherit overwrite 2nd vptr
            {
                newPtr = (void**) malloc(size);
                void* temp = (void*) new ParentFour();
                memcpy((void*)(newPtr + 4), temp, size-4 > sizeof(ParentFour) ? sizeof(ParentFour) : size-4);
                free (temp);
            } 
            else
            {
                if (size == sizeof(ParentFour))
                {
                    newPtr = (void**) new ParentFour();
                } else
                {
                    newPtr = (void**) malloc(size);
                    void* temp = (void*) new ParentFour();
                    memcpy((void*)(newPtr), temp, size > sizeof(ParentFour) ? sizeof(ParentFour) : size);
                    free (temp);
                }
            }

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

            if (arg2 == 9 || arg2 == 11)  // double inherit overwrite 2nd vptr
            {
                newPtr = (void**) malloc(size);
                void* temp = (void*) new ParentFive();
                memcpy((void*)(newPtr + 4), temp, size-4 > sizeof(ParentFive) ? sizeof(ParentFive) : size-4);
                free (temp);
            } 
            else
            {
                if (size == sizeof(ParentFive))
                {
                    newPtr = (void**) new ParentFive();
                } else
                {
                    newPtr = (void**) malloc(size);
                    void* temp = (void*) new ParentFive();
                    memcpy((void*)(newPtr), temp, size > sizeof(ParentFive) ? sizeof(ParentFive) : size);
                    free (temp);
                }
            }

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
            exit(2);
            break;
        }
    }


    if (/*debug && */newPtr!=ptr)
    {
        cout << "UAF new pointer not in the same location" << endl;
        // printf("Old = %p New = %p\n", ptr, newPtr);
        exit(3);
    }
}

int main(int argc, char const *argv[])
{
    // stdout = freopen("/home/linux/Desktop/bench/tmp", "w", stdout);
    if (argc < 3)
    {
        cout << "not enough params\n";
        exit(2);
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

    if(uaf) {
        attackString = "\tUAF Attacked!";
    }
// SET DESTINATIONS
    switch (atoi(argv[1])) {  //todo fix

        case 0:  // start of good vable point to good inheritance func: 
        {
            if (atoi(argv[2]) > 7 && atoi(argv[2]) < 12)  // double inherit
            {
                if(debug)
                cout << "not possible for now\n";
                exit(2);
            }

            cout << "\tDestination: Val.Vtbl.Strt" << endl;
            if (uaf) break; // if uaf enabled then we just create an object of that type

            // if (debug)

            if (atoi(argv[2]) >= 12)  // double inherit
            {
                DoubleInheritChild1* object2 = new DoubleInheritChild1();
                void* vtableO2 = (void*)*(void**)object2;
                destination = vtableO2;
                delete object2;
                break;
            }


            // P1 obj of SI1 points to SI2
            ParentOne* object2 = new SingleInherit2();
            void* vtableO2 = (void*)*(void**)object2;
            destination = vtableO2;
            delete object2;
            break;
        }


        case 1:  // middle of good vable point to good inheritance func: 
        {
            if (atoi(argv[2]) > 7 && atoi(argv[2]) < 12)  // double inherit
            {
                if(debug)
                cout << "not possible for now\n";
                exit(2);
            }

            // if (uaf) break; // if uaf enabled then we just create an object of that type

            
            if (debug)
            cout << "point to the middle of a vtable with valid inheritance" << endl;
            cout << "\tDestination: Val.Vtbl.Midl" << endl;

            if (atoi(argv[2]) >= 12)  // double inherit
            {
                DoubleInheritChild1* object2 = new DoubleInheritChild1();
                void* vtableO2 = (void*)*(void**)object2;
                destination = (void*)((uint64_t)vtableO2 + sizeof(void*) + sizeof(void*));
                delete object2;
                break;
            }

            // P1 obj of SI1 points to SI2 func 2
            ParentOne* object2 = new SingleInherit2();
            void* vtableO2 = (void*)*(void**)object2;
            destination = (void*)((uint64_t)vtableO2 + sizeof(void*));
            
            delete object2;
            break;
        }

        case 2:  // start of good vable point to bad inheritance func:
        {
            cout << "\tDestination: InVal.Vtbl.Strt" << endl;
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
            cout << "\tDestination: InVal.Vtbl.Midl" << endl;

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
            cout << "\tDestination: Val.Vtbl.Crafted" << endl;


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
            cout << "\tDestination: InVal.Vtbl.Crafted" << endl;

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
            cout << "\tDestination: nonClassFunc" << endl;

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
            cout << "\tDestination: nonClassFunc.Midl" << endl;

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
                exit(2);            
            }

            cout << "\tDestination: Val.Vtbl.Strt DoubleInherit" << endl;
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
                exit(2);            
            }

            // if (uaf) break; // if uaf enabled then we just create an object of that type

            
            if (debug)
            cout << "point to the middle of a vtable of other parent in double inheritance" << endl;
            cout << "\tDestination: Val.Vtbl.Midl DoubleInherit" << endl;

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
                exit(2);            
            }

            cout << "\tDestination: InVal.Vtbl.Strt DoubleInherit" << endl;
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
                exit(2);            
            }

            // if (uaf) break; // if uaf enabled then we just create an object of that type

            
            if (debug)
            cout << "point to the middle of a vtable of third unrelated parent in double inheritance" << endl;
            cout << "\tDestination: InVal.Vtbl.Midl DoubleInherit" << endl;

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
            exit(2);
            break;
        }
    }


    switch (atoi(argv[2])) {  //todo fix
        case 0:  //stack+SingleInherit+sequentialOverFlow:
        {
            // if (debug)
            cout << "\tObject Atacked: SingleInherit\n";
            cout << "\tMethod: stack+sequentialOverFlow\n";
            SingleInherit1 singleObj;
            void* buffer[3];

            ParentOne* object = &singleObj;
            if (uaf)
            {
                if (debug) {
                    cout << "no UAF on stack" << endl;
                }
                exit(2);
            }

            if (((void*)object < (void*)buffer))
            {
                if (debug) {
                    cout << "*****cant overflow...object before buffer" << endl;
                    printf("object = %p\tbuffer = %p\n", object, buffer);
                }
                exit(3);
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
                cout << "calling ParentOne::SingleInherit1::F1" << endl;
            object->doSomething1(attackString);
            break;
        }

        case 1:  // heap+SingleInherit+sequentialOverFlow:
        {
            cout << "\tObject Atacked: SingleInherit\n";
            if (!uaf)
            cout << "\tMethod: heap+sequentialOverFlow\n";        
            else 
            cout << "\tMethod: heap+UAF\n";        

            // void** buffer = (void **)malloc(sizeof(void*)*3);
            void** buffer = new void*[1];   // stick with 1 otherwise get memory dump
            // void** buffer = (void **)malloc(0x30);
// printf("%d\n", sizeof(SingleInherit1));
// printf("%d\n", sizeof(void*));
            ParentOne* object = new SingleInherit1();

            if (debug)
            {
                printf("object = %p\tbuffer = %p\n", object, buffer);
                printf("old vpointer = %p\n", (void*)*(void**)object);
            }

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
                exit(3);
            }


            for (int i = 0;i<((uint64_t)object - (uint64_t)buffer) ; ++i)  // overflow
            {
                // cout << ((uint64_t)object - (uint64_t)buffer)<< "   "<<i << endl;
                *(buffer + i) = (void*)destination;
                if ((void*)(buffer + i) == (void*)object)
                    break;
            }

            // free(buffer);   // dont free metadata might crash due to corruption


            attack1:    
                if (debug)
                {
                    printf("new vpointer = %p\n", (void*)*(void**)object);
                    printf("vpointer to copy = %p\n", destination);
                    cout << "calling ParentOne::SingleInherit1::F1" << endl;
                }

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
                exit(2);
            }
            cout << "\tObject Atacked: SingleInherit\n";
            cout << "\tMethod: stack+indirectOverwrite\n";   

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
            cout << "calling ParentOne::SingleInherit1::F1" << endl;
            object->doSomething1(attackString);
            break;
        }

        case 3:  // heap+SingleInherit+indirectOverwrite:
        {
            cout << "\tObject Atacked: SingleInherit\n";
            if (!uaf)
            cout << "\tMethod: heap+indirectOverwrite\n";        
            else 
            cout << "\tMethod: heap+UAF\n";

            ParentOne* object = new SingleInherit1();

            if (debug)
                printf("old vpointer = %p\n", (void*)*(void**)object);

            if (uaf)
            {
                delete object;
                doUAF(object, sizeof(SingleInherit1));
                goto attack3; 
            }

            *(void**)object = destination;
            
            
            attack3:
                if (debug)
                {
                    printf("new vpointer = %p\n", (void*)*(void**)object);
                    printf("vpointer to copy = %p\n", destination);
                    cout << "calling ParentOne::SingleInherit1::F1" << endl;
                }
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
                exit(2);
            }
            cout << "\tObject Atacked: SingleInherit\n";
            cout << "\tMethod: stack+sequentialOverFlow\n";

            SingleInherit1 singleObj;
            void* buffer[3];

            SingleInherit1* object = &singleObj;

            if (((void*)object < (void*)buffer))
            {
                if (debug)
                cout << "*****cant overflow...object before buffer" << endl;
                if (debug)
                printf("object = %p\tbuffer = %p\n", object, buffer);
                exit(3);
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
            cout << "calling SingleInherit1::SingleInherit1::F1" << endl;
            object->doSomething1(attackString);
            break;
        }

        case 5:  // heap+SingleInherit+sequentialOverFlow:
        {

            cout << "\tObject Atacked: SingleInherit\n";
            if (!uaf)
            cout << "\tMethod: heap+sequentialOverFlow\n";        
            else 
            cout << "\tMethod: heap+UAF\n";
            // void** buffer = (void **)malloc(sizeof(void*)*3);
            void** buffer = new void*[1];   // stick with 1 otherwise get memory dump
            // void** buffer = (void **)malloc(0x30);
// printf("%d\n", sizeof(SingleInherit1));
// printf("%d\n", sizeof(void*));
            SingleInherit1* object = new SingleInherit1();

            if (debug)
            {
                printf("object = %p\tbuffer = %p\n", object, buffer);
                printf("old vpointer = %p\n", (void*)*(void**)object);
            }

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
                exit(3);
            }


            for (int i = 0;i<((uint64_t)object - (uint64_t)buffer) ; ++i)  // overflow
            {
                // cout << ((uint64_t)object - (uint64_t)buffer)<< "   "<<i << endl;
                *(buffer + i) = (void*)destination;
                if ((void*)(buffer + i) == (void*)object)
                    break;
            }

            // free(buffer);   // dont free metadata might crash due to corruption

            attack5:
                if(debug)
                {
                    printf("new vpointer = %p\n", (void*)*(void**)object);
                    printf("vpointer to copy = %p\n", destination);
                    cout << "calling SingleInherit1::SingleInherit1::F1" << endl;
                }
                    
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
                exit(2);
            }
            cout << "\tObject Atacked: SingleInherit\n";
            cout << "\tMethod: stack+indirectOverwrite\n";        

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
            cout << "calling SingleInherit1::SingleInherit1::F1" << endl;
            object->doSomething1(attackString);
            break;
        }

        case 7:  // heap+SingleInherit+indirectOverwrite:
        {
            cout << "\tObject Atacked: SingleInherit\n";
            if (!uaf)
            cout << "\tMethod: heap+indirectOverwrite\n";        
            else 
            cout << "\tMethod: heap+UAF\n";

            if (debug)
            cout << "heap+SingleInherit+indirectOverwrite\n";
            SingleInherit1* object = new SingleInherit1();

            if (debug)
                printf("old vpointer = %p\n", (void*)*(void**)object);

            if (uaf)
            {
                delete object;
                doUAF(object, sizeof(SingleInherit1));
                goto attack7; 
            }


            *(void**)object = destination;
            
            
            attack7:
                if(debug)
                {
                    printf("new vpointer = %p\n", (void*)*(void**)object);
                    printf("vpointer to copy = %p\n", destination);
                    cout << "calling SingleInherit1::SingleInherit1::F1" << endl;
                }
                    
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
                exit(2);
            }

            cout << "\tObject Atacked: DoubleInheritChild\n";
            cout << "\tMethod: stack+withinObjectOverFlow\n";        

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
            cout << "calling DoubleInheritChild1::DoubleInheritChild1::F3 ParentFour::F1" << endl;
            object->ParentFourfoo1(attackString);
            break;
        }

        case 9:  // heap+DoubleInherit+withinObjectOverFlow:
        {
            cout << "\tObject Atacked: DoubleInheritChild\n";
            if (!uaf)
            cout << "\tMethod: heap+withinObjectOverFlow\n";        
            else 
            cout << "\tMethod: heap+UAF\n";

            DoubleInheritChild1* object = new DoubleInheritChild1();

            if (debug)
                printf("old vpointer = %p\n", (void*)*((void**)object + 4));

            if (uaf)
            {
                delete object;
                doUAF(object, sizeof(DoubleInheritChild1));
                goto attack9; 
            }

            for (int i = 0; i < 4; ++i)  // overflow assume that vptr is directly after
            {
                *(object->buffer + i) = (void*)destination;
            }


            attack9:
                if(debug)
                {
                    printf("new vpointer = %p\n", (void*)*((void**)object + 4));
                    printf("vpointer to copy = %p\n", destination);
                    cout << "calling DoubleInheritChild1::DoubleInheritChild1::F3 ParentFour::F1" << endl;
                }

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
                exit(2);
            }
            cout << "\tObject Atacked: DoubleInheritChild\n";
            cout << "\tMethod: stack+indirectOverwrite\n";        

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
            cout << "calling DoubleInheritChild1::DoubleInheritChild1::F3 ParentFour::F1" << endl;
            object->ParentFourfoo1(attackString);
            break;
        }

        case 11:  // heap+DoubleInherit+INdirectOverwrite:
        {

            cout << "\tObject Atacked: DoubleInheritChild\n";
            if (!uaf)
            cout << "\tMethod: heap+indirectOverwrite\n";        
            else 
            cout << "\tMethod: heap+UAF\n";

            DoubleInheritChild1* object = new DoubleInheritChild1();

            if (debug)
                printf("old vpointer = %p\n", (void*)*((void**)object + 4));

            if (uaf)
            {
                delete object;
                doUAF(object, sizeof(DoubleInheritChild1));
                goto attack11; 
            }


            object->overflow();


            attack11:
                if(debug)
                {
                    printf("new vpointer = %p\n", (void*)*((void**)object + 4));
                    printf("vpointer to copy = %p\n", destination);
                    cout << "calling DoubleInheritChild1::DoubleInheritChild1::F3 ParentFour::F1" << endl;
                }

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
                exit(2);
            }

            cout << "\tObject Atacked: DoubleInheritParent\n";
            cout << "\tMethod: stack+sequentialOverFlow\n";        

            if (arg1 > 1)
            {
                ParentThree singleObj;
                void* buffer[3];

                ParentThree* object = &singleObj;

                if (((void*)object < (void*)buffer))
                {
                    if (debug)
                    cout << "*****cant overflow...object before buffer" << endl;
                    if (debug)
                    printf("object = %p\tbuffer = %p\n", object, buffer);
                    exit(3);
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
                    cout << "calling ParentThree::ParentThree::F1" << endl;
                object->foo1(attackString);
                break;
            }

            else if (arg1 == 0)
            {
                ParentFour singleObj;
                void* buffer[3];

                ParentFour* object = &singleObj;

                if (((void*)object < (void*)buffer))
                {
                    if (debug)
                    cout << "*****cant overflow...object before buffer" << endl;
                    if (debug)
                    printf("object = %p\tbuffer = %p\n", object, buffer);
                    exit(3);
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
                    cout << "calling ParentFour::ParentFour::F3" << endl;
                object->ParentFourfoo3(attackString);
                break;
            }

            else if (arg1 == 1)
            {
                ParentFour singleObj;
                void* buffer[3];

                ParentFour* object = &singleObj;

                if (((void*)object < (void*)buffer))
                {
                    if (debug)
                    cout << "*****cant overflow...object before buffer" << endl;
                    if (debug)
                    printf("object = %p\tbuffer = %p\n", object, buffer);
                    exit(3);
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
                    cout << "calling ParentFour::ParentFour::F1" << endl;
                object->ParentFourfoo1(attackString);
                break;
            }
        }

        case 13:  // heap+DoubleInheritChild+sequentialOverFlow:
        {

            cout << "\tObject Atacked: DoubleInheritParent\n";
            if (!uaf)
            cout << "\tMethod: heap+sequentialOverFlow\n";        
            else 
            cout << "\tMethod: heap+UAF\n";

            if (arg1 > 1)
            {
                void** buffer = new void*[1];   // stick with 1 otherwise get memory dump

                ParentThree* object = new ParentThree();

                if (debug) 
                {
                    printf("object = %p\tbuffer = %p\n", object, buffer);
                    printf("old vpointer = %p\n", (void*)*(void**)object);
                }

                if (uaf)
                {
                    delete object;
                    doUAF(object, sizeof(ParentThree));
                    goto attack13a; 
                }

                if (((void*)object < (void*)buffer))
                {
                    if (debug)
                    cout << "*****cant overflow...object before buffer" << endl;
                    exit(3);
                }


                for (int i = 0;i<((uint64_t)object - (uint64_t)buffer) ; ++i)  // overflow
                {
                    *(buffer + i) = (void*)destination;
                    if ((void*)(buffer + i) == (void*)object)
                        break;
                }

                // free(buffer);   // dont free metadata might crash due to corruption

                attack13a:
                    if(debug)
                    {
                        printf("new vpointer = %p\n", (void*)*(void**)object);
                        printf("vpointer to copy = %p\n", destination);
                        cout << "calling ParentThree::ParentThree::F1" << endl;
                    }

                    object->foo1(attackString);

                    break;

            }
            else if (arg1 == 0)
            {
                void** buffer = new void*[1];   // stick with 1 otherwise get memory dump

                ParentFour* object = new ParentFour();

                if (debug) 
                {
                    printf("object = %p\tbuffer = %p\n", object, buffer);
                    printf("old vpointer = %p\n", (void*)*(void**)object);
                }

                if (uaf)
                {
                    delete object;
                    doUAF(object, sizeof(ParentFour));
                    goto attack13b; 
                }

                if (((void*)object < (void*)buffer))
                {
                    if (debug)
                    cout << "*****cant overflow...object before buffer" << endl;
                    exit(3);
                }


                for (int i = 0;i<((uint64_t)object - (uint64_t)buffer) ; ++i)  // overflow
                {
                    *(buffer + i) = (void*)destination;
                    if ((void*)(buffer + i) == (void*)object)
                        break;
                }

                // free(buffer);   // dont free metadata might crash due to corruption

                attack13b:
                    if(debug)
                    {
                        printf("new vpointer = %p\n", (void*)*(void**)object);
                        printf("vpointer to copy = %p\n", destination);
                        cout << "calling ParentFour::ParentFour::F3" << endl;
                    }

                    object->ParentFourfoo3(attackString);

                    break;
            }
            else if (arg1 == 1)
            {
                void** buffer = new void*[1];   // stick with 1 otherwise get memory dump

                ParentFour* object = new ParentFour();

                if (debug) 
                {
                    printf("object = %p\tbuffer = %p\n", object, buffer);
                    printf("old vpointer = %p\n", (void*)*(void**)object);
                }

                if (uaf)
                {
                    delete object;
                    doUAF(object, sizeof(ParentFour));
                    goto attack13c; 
                }

                if (((void*)object < (void*)buffer))
                {
                    if (debug)
                    cout << "*****cant overflow...object before buffer" << endl;
                    exit(3);
                }


                for (int i = 0;i<((uint64_t)object - (uint64_t)buffer) ; ++i)  // overflow
                {
                    *(buffer + i) = (void*)destination;
                    if ((void*)(buffer + i) == (void*)object)
                        break;
                }

                // free(buffer);   // dont free metadata might crash due to corruption

                attack13c:
                    if(debug)
                    {
                        printf("new vpointer = %p\n", (void*)*(void**)object);
                        printf("vpointer to copy = %p\n", destination);
                        cout << "calling ParentFour::ParentFour::F1" << endl;
                    }

                    object->ParentFourfoo1(attackString);

                    break;
            }
                
        }

        case 14:  // stack+DoubleInheritChild+indirectOverwrite:
        {
            if (uaf)
            {
                if (debug) {
                    cout << "no UAF on stack" << endl;
                }
                exit(2);
            }
            cout << "\tObject Atacked: DoubleInheritParent\n";
            cout << "\tMethod: stack+indirectOverwrite\n";        

            if (debug)
                cout << "stack+DoubleInheritChild+indirectOverwrite\n";


            if (arg1 > 1)
            {
                ParentThree singleObj;
                ParentThree* object = &singleObj;

                if (debug)
                    printf("old vpointer = %p\n", (void*)*(void**)object);

                *(void**)object = destination;

                if (debug) 
                {
                    printf("new vpointer = %p\n", (void*)*(void**)object);
                    printf("vpointer to copy = %p\n", destination);
                    cout << "calling ParentThree::ParentThree::F1" << endl;
                }

                object->foo1(attackString);
                break;
            }
            else if (arg1 == 0)
            {
                ParentFour singleObj;
                ParentFour* object = &singleObj;

                if (debug)
                    printf("old vpointer = %p\n", (void*)*(void**)object);

                *(void**)object = destination;

                if (debug) 
                {
                    printf("new vpointer = %p\n", (void*)*(void**)object);
                    printf("vpointer to copy = %p\n", destination);
                    cout << "calling ParentFour::ParentFour::F3" << endl;
                }

                object->ParentFourfoo3(attackString);
                break;

            }
            else if (arg1 == 1)
            {
                ParentFour singleObj;
                ParentFour* object = &singleObj;

                if (debug)
                    printf("old vpointer = %p\n", (void*)*(void**)object);

                *(void**)object = destination;

                if (debug) 
                {
                    printf("new vpointer = %p\n", (void*)*(void**)object);
                    printf("vpointer to copy = %p\n", destination);
                    cout << "calling ParentFour::ParentFour::F1" << endl;
                }

                object->ParentFourfoo1(attackString);
                break;

            }
        }

        case 15:  // heap+DoubleInheritChild+indirectOverwrite:
        {

            cout << "\tObject Atacked: DoubleInheritParent\n";
            if (!uaf)
            cout << "\tMethod: heap+withinObjectOverFlow\n";        
            else 
            cout << "\tMethod: heap+UAF\n";


            if (arg1 > 1)
            {
                ParentThree* object = new ParentThree();

                if (debug)
                    printf("old vpointer = %p\n", (void*)*(void**)object);

                if (uaf)
                {
                    delete object;
                    doUAF(object, sizeof(ParentThree));
                    goto attack15a; 
                }


                *(void**)object = destination;
                
                
                attack15a:
                    if(debug)
                    {
                        printf("new vpointer = %p\n", (void*)*(void**)object);
                        printf("vpointer to copy = %p\n", destination);
                        cout << "calling ParentThree::ParentThree::F1" << endl;
                    }

                    object->foo1(attackString);
                    break;
            }
            else if (arg1 == 0)
            {   
                ParentFour* object = new ParentFour();

                if (debug)
                    printf("old vpointer = %p\n", (void*)*(void**)object);

                if (uaf)
                {
                    delete object;
                    doUAF(object, sizeof(ParentFour));
                    goto attack15b; 
                }


                *(void**)object = destination;
                
                
                attack15b:
                    if(debug)
                    {
                        printf("new vpointer = %p\n", (void*)*(void**)object);
                        printf("vpointer to copy = %p\n", destination);
                        cout << "calling ParentFour::ParentFour::F3" << endl;
                    }

                    object->ParentFourfoo3(attackString);
                    break;
            }
            else if (arg1 == 1)
            {   
                ParentFour* object = new ParentFour();

                if (debug)
                    printf("old vpointer = %p\n", (void*)*(void**)object);

                if (uaf)
                {
                    delete object;
                    doUAF(object, sizeof(ParentFour));
                    goto attack15c; 
                }


                *(void**)object = destination;
                
                
                attack15c:
                    if(debug)
                    {
                        printf("new vpointer = %p\n", (void*)*(void**)object);
                        printf("vpointer to copy = %p\n", destination);
                        cout << "calling ParentFour::ParentFour::F1" << endl;
                    }

                    object->ParentFourfoo1(attackString);
                    break;
            }

        }

        case 50: goto syslbl;  // just so that compiler does not remove it

        default: {
            cout << "bad second option\n";
            exit(2);
            break;
        }
    }



    return 0;

syslbl:
    // system(attackString);
    // printf("%s\n", "at syslbl ATTACK successful\n");
    // printf("%d %d\n", arg1, arg2);
    // printf("", arg1, arg2);
    if(uaf)
    cout << flush << "\tUAF Attacked! " << arg1 << " " << arg2;
    else
    cout << flush << "\tAttacked! " << arg1 << " " << arg2;
    if(debug)
        cout << "at syslbl ATTACK successful \n";
    exit(1);


 //    ******************uaf
	// Child1* c1 = new Child1();

	// c1 -> doSomething("sh");

	// free(c1);

	// new Child2();

	// c1 -> doSomething("sh");
}


















