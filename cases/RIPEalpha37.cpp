#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void* destination = NULL;
char* attackString = "\tAttacked!";
int arg1;
int arg2;
int debug;
int uaf = 0;

void doExit(int code) {
    printf("*******************************************************\n");
    if (code == 0) {
        printf("Attack Failed, Normal execution\n");
    } else if (code == 1) {
        printf("Attack Succeeded!\n");
    } else if (code == 2) {
        printf("Attack not performed, bad parameter options\n");
    } else if (code == 3) {
        printf("Attack not performed, could not get desired pointer addresses\n");
    }
    printf("*******************************************************\n");
    exit(code);
}

class ParentOne
{
    public:
        virtual void doSomething1(char *string){
            printf("\tcalled ParentOne::F1\n");
            doExit(0);
        }
        virtual void doSomething2(char *string){
            printf("\tcalled ParentOne::F2\n");
            doExit(0);
        }
};

class ParentTwo
{
    public:
        
        virtual void foo1(char *string){   // attack
            if (debug)
            {
                printf("\t\tin ParentTwo::F1\n");
            }
            printf("%s ", string);
            printf("%d %d\n", arg1, arg2);
            doExit(1);
        }
        virtual void foo2(char *string)   // attack
        {
            if (debug)
            {
                printf("\t\tin ParentTwo::F2\n");
            }
            printf("%s ", string);
            printf("%d %d\n", arg1, arg2);
            doExit(1);
        }
};

class ParentThree
{
    public:
        
        void* buffer[3];
        virtual void foo1(char *string){
            printf("\tcalled ParentThree::F1\n");
            doExit(0);
        }
        virtual void foo2(char *string)
        {
            printf("\tcalled ParentThree::F2\n");
            doExit(0);
        }
};

class ParentFour
{
    public:
        
        virtual void ParentFourfoo1(char *string){   // attack
            if (debug)
            {
                printf("\t\tin ParentFour::F1\n");
            }
            printf("%s ", string);
            printf("%d %d\n", arg1, arg2);
            doExit(1);
        }
        virtual void ParentFourfoo2(char *string)   // attack
        {
            if (debug)
            {
                printf("\t\tin ParentFour::F2\n");
            }
            printf("%s ", string);
            printf("%d %d\n", arg1, arg2);
            doExit(1);
        }
        virtual void ParentFourfoo3(char *string)
        {
            printf("\tcalled ParentFour::F3\n");
            doExit(0);
        }
};

class ParentFive
{
    public:
        
        virtual void ParentFivefoo1(char *string){   // attack
            if (debug)
            {
                printf("\t\tin ParentFive::F1\n");
            }
            printf("%s ", string);
            printf("%d %d\n", arg1, arg2);
            doExit(1);
        }
        virtual void ParentFivefoo2(char *string)   // attack
        {
            if (debug)
            {
                printf("\t\tin ParentFive::F2\n");
            }
            printf("%s ", string);
            printf("%d %d\n", arg1, arg2);
            doExit(1);
        }
};

class SingleInherit1:public ParentOne
{
    public:
        virtual void doSomething1(char *string)
        {
            printf("\tcalled SingleInherit1::F1\n");
            doExit(0);
        }
        virtual void doSomething2(char *string){
            printf("\tcalled SingleInherit1::F2\n");
            doExit(0);
        }

};
class SingleInherit2:public ParentOne
{
    public:
        virtual void doSomething1(char *string)
        {
            if (debug)
            {
                printf("\t\tin SingleInherit2::F1\n");
            }
            printf("%s ", string);
            printf("%d %d\n", arg1, arg2);
            doExit(1);
        }
        virtual void doSomething2(char *string){
            if (debug)
            {
                printf("\t\tin SingleInherit2::F2\n");
            }
            printf("%s ", string);
            printf("%d %d\n", arg1, arg2);
            doExit(1);
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
            if (uaf) {
                printf("\tUAF Attacked! ");
            } else {
                printf("\t\tAttacked! ");
            }
            printf("%d %d\n", arg1, arg2);
            if (debug)
            {
                printf("\t\tin DoubleInheritChild1::F1\n");
            }
            doExit(1);
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
            if (debug)
            {
                printf("\t\tin DoubleInheritChild2::F1\n");
            }
            printf("%s ", string);
            printf("%d %d\n", arg1, arg2);
            doExit(1);
        }
        virtual void ParentFourfoo2(char *string){
            if (debug)
            {
                printf("\t\tin DoubleInheritChild2::F2\n");
            }
            printf("%s ", string);
            printf("%d %d\n", arg1, arg2);
            doExit(1);
        }

        virtual void ParentFivefoo1(char *string){
            if (debug)
            {
                printf("\t\tin DoubleInheritChild2::F3\n");
            }
            printf("%s ", string);
            printf("%d %d\n", arg1, arg2);
            doExit(1);
        }
        virtual void ParentFivefoo2(char *string)
        {
            if (debug)
            {
                printf("\t\tin DoubleInheritChild2::F4\n");
            }
            printf("%s ", string);
            printf("%d %d\n", arg1, arg2);
            doExit(1);
        }


};


void doSyscall(int x, char* string) {

    if (debug)
    {
        printf("\t\tin doSyscall func\n");
    }
    printf("%s ", string);
    printf("%d %d\n", arg1, arg2);
    doExit(1);
    // cout << "at doSyscall function ATTACK successful\n";

    // cout << "calling system with args: " << string << endl;
    // system(string);
}





void doUAF (void* ptr, int size) {
    void** newPtr = NULL;
    switch (arg1)
    {
        case 0: {
            if (debug) {
                printf("\t\tUAF with valid inheritance\n");
            }
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
            if (debug) {
                printf("\t\tUAF middle of good inheritance\n");
            }

            newPtr = (void**) malloc(size);
            *newPtr = destination;

            break;
        }
        case 2: {
            if (debug) {
                printf("\t\tuaf with invalid inheritance\n");
            }
            
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
            if (debug) {
                printf("\t\tuaf middle of invalid inheritance\n");
            }

            newPtr = (void**) malloc(size);

            if (arg2 == 9 || arg2 == 11)  // double inherit overwrite 2nd vptr
                newPtr[4] = destination;
            else
                *newPtr = destination;

            break;
        }
        case 4: {
            if (debug) {
                printf("\t\tuaf crafted vtable with valid inheritance\n");
            }
            newPtr = (void**) malloc(size);
            if (arg2 == 9 || arg2 == 11)  // double inherit overwrite 2nd vptr
                newPtr[4] = destination;
            else
                *newPtr = destination;

            break;
        }
        case 5: {
            if (debug) {
                printf("\t\tuaf crafted vtable with invalid inheritance\n");
            }
            newPtr = (void**) malloc(size);
            if (arg2 == 9 || arg2 == 11)  // double inherit overwrite 2nd vptr
                newPtr[4] = destination;
            else
                *newPtr = destination;
            break;
        }
        case 6: {
            if (debug) {
                printf("\t\tuaf nonClassFunc\n");
            } 
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
                printf("\t\tuaf middle of a nonClassFunc\n");
                printf("\t\tlocation of syslbl/destination = %p\n", (void*)(*((void***)*newPtr)));
            }
            break;
        }
        case 8: {
            if (debug) {
                printf("\t\tuaf start of a vtable of other parent in double inheritance\n");
            }

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
            if (debug) {
                printf("\t\tuaf middle of a vtable of other parent in double inheritance\n");
            }
            newPtr = (void**) malloc(size);

            if (arg2 == 9 || arg2 == 11)  // double inherit overwrite 2nd vptr
                newPtr[4] = destination;
            else
                *newPtr = destination;
            break;
        }
        case 10: {
            if (debug) {
                printf("\t\tuaf start of a vtable of third unrelated parent in double inheritance\n");
            }

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
            if (debug) {
                printf("\t\tuaf middle of a vtable of third unrelated parent in double inheritance\n");
            }
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
            printf("bad first option\n");
            doExit(2);
            break;
        }
    }


    if (/*debug && */newPtr!=ptr)
    {
        printf("UAF new pointer not in the same location\n");
        // printf("Old = %p New = %p\n", ptr, newPtr);
        doExit(3);
    }
}

int main(int argc, char const *argv[])
{

    printf("\n*******************************************************\n");
    printf("Running test # %d/%d:\n", 37, 90);

    debug = 0;
    if (argc == 2 && strcmp(argv[1], "d") == 0) 
    {
        debug = 1;   
    }

    arg1 = 4;
    arg2 = 9;
    uaf = 0;
    
    if(uaf) {
        attackString = "\tUAF Attacked!";
    }
// SET DESTINATIONS
    switch (arg1) {

        case 0:  // start of good vable point to good inheritance func: 
        {
            if (arg2 > 7 && arg2 < 12)  // double inherit
            {
                if (debug) {
                    printf("\t\tnot possible for now\n");
                }
                doExit(2);
            }

            printf("\tDestination: Val.Vtbl.Strt\n");
            if (uaf) break; // if uaf enabled then we just create an object of that type

            // if (debug)

            if (arg2 >= 12)  // double inherit
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
            if (arg2 > 7 && arg2 < 12)  // double inherit
            {
                if (debug) {
                    printf("\t\tnot possible for now\n");
                }
                doExit(2);
            }

            // if (uaf) break; // if uaf enabled then we just create an object of that type

            
            if (debug) {
                printf("\t\tpoint to the middle of a vtable with valid inheritance\n");
            }
            printf("\tDestination: Val.Vtbl.Midl\n");

            if (arg2 >= 12)  // double inherit
            {
                DoubleInheritChild1* object2 = new DoubleInheritChild1();
                void* vtableO2 = (void*)*(void**)object2;
                destination = (void*)((unsigned int)vtableO2 + sizeof(void*) + sizeof(void*));
                delete object2;
                break;
            }

            // P1 obj of SI1 points to SI2 func 2
            ParentOne* object2 = new SingleInherit2();
            void* vtableO2 = (void*)*(void**)object2;
            destination = (void*)((unsigned int)vtableO2 + sizeof(void*));
            
            delete object2;
            break;
        }

        case 2:  // start of good vable point to bad inheritance func:
        {
            printf("\tDestination: InVal.Vtbl.Strt\n");
            if (uaf) break; // if uaf enabled then we just create an object of that type

            
            if (debug) {
                printf("\t\tpoint to the start of a vtable with invalid inheritance\n");
            }

            if (arg2 > 7)  // double inherit
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

            
            if (debug) {
                printf("\t\tpoint to the middle of a vtable with invalid inheritance\n");
            }
            printf("\tDestination: InVal.Vtbl.Midl\n");

            if (arg2 > 7)  // double inherit
            {
                ParentTwo* object2 = new ParentTwo();
                void* vtableO2 = (void*)*((void**)object2);  // 1 ptr + 3 for buffer
                destination = (void*)((unsigned int)vtableO2 + sizeof(void*));  // second ptr
                delete object2;
                break;
            }

            // P1 obj of SI1 points to P2 func 2
            ParentTwo* object2 = new ParentTwo();
            void* vtableO2 = (void*)*(void**)object2;
            destination = (void*)((unsigned int)vtableO2 + sizeof(void*));
            
            delete object2;
            break;
        }

        case 4:  // crafted vable point to good inheritance func:
        {

            // if (uaf) break; // if uaf enabled then we just create an object of that type

            
            if (debug) {
                printf("\t\tpoint to a crafted vtable with valid inheritance\n");
            }
            printf("\tDestination: Val.Vtbl.Crafted\n");


            if (arg2 > 7)  // double inherit
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

            
            if (debug) {
                printf("\t\tpoint to a crafted vtable with invalid inheritance\n");
            }
            printf("\tDestination: InVal.Vtbl.Crafted\n");

            // dont need this just point to P3 object 
            if (arg2 > 7)  // double inherit
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

            
            if (debug) {
                printf("\t\tpoint to a nonClassFunc\n");
            }
            printf("\tDestination: nonClassFunc\n");

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

            
            if (debug) {
                printf("\t\tpoint to the middle of a nonClassFunc\n");
            }
            printf("\tDestination: nonClassFunc.Midl\n");

            // make a buffer and use it as a vtable point to syslbl
            void** buffer = (void**)malloc (sizeof(void*) * 8);  // make it longer than necessary

            for (int i = 0; i < 8; ++i)
            {
                *(buffer+i) = &&syslbl;
            }

            if (debug)
            {
                printf("\t\tlocation of syslbl = %p\n", &&syslbl);
            }

            destination = buffer;
            break;
        }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
        case 8:  // point to the start of a vtable of other parent in double inheritance 
        {
            if (arg2 < 8)  // single inherit
            {
                if (debug) {
                    printf("\t\tnot possible for now\n");
                }
                doExit(2);            
            }

            printf("\tDestination: Val.Vtbl.Strt DoubleInherit\n");
            if (uaf) break; // if uaf enabled then we just create an object of that type

            
            if (debug) {
                printf("\t\tpoint to the start of a vtable of other parent in double inheritance\n");
            }

            ParentFour* object2 = new ParentFour();
            void* vtableO2 = (void*)*(void**)object2;
            destination = vtableO2;
            delete object2;
            break;
        }

        case 9:  // point to the middle of a vtable of other parent in double inheritance
        {
            if (arg2 < 8)  // single inherit
            {
                if (debug) {
                    printf("\t\tnot possible for now\n");
                }
                doExit(2);            
            }

            // if (uaf) break; // if uaf enabled then we just create an object of that type

            
            if (debug) {
                printf("\t\tpoint to the middle of a vtable of other parent in double inheritance\n");
            }
            printf("\tDestination: Val.Vtbl.Midl DoubleInherit\n");

            ParentFour* object2 = new ParentFour();
            void* vtableO2 = (void*)*(void**)object2;
            destination = (void*)((unsigned int)vtableO2 + sizeof(void*));
            
            delete object2;
            break;
        }

        case 10:  // point to the start of a vtable of third unrelated parent in double inheritance 
        {
            if (arg2 < 8)  // single inherit
            {
                if (debug) {
                    printf("\t\tnot possible for now\n");
                }
                doExit(2);            
            }

            printf("\tDestination: InVal.Vtbl.Strt DoubleInherit\n");
            if (uaf) break; // if uaf enabled then we just create an object of that type

            
            if (debug) {
                printf("\t\tpoint to the start of a vtable of third unrelated parent in double inheritance\n");
            }

            ParentFive* object2 = new ParentFive();
            void* vtableO2 = (void*)*(void**)object2;
            destination = vtableO2;
            delete object2;
            break;
        }

        case 11:  // point to the middle of a vtable of third unrelated parent in double inheritance
        {
            if (arg2 < 8)  // single inherit
            {
                if (debug) {
                    printf("\t\tnot possible for now\n");
                }
                doExit(2);            
            }

            // if (uaf) break; // if uaf enabled then we just create an object of that type

            
            if (debug) {
                printf("\t\tpoint to the middle of a vtable of third unrelated parent in double inheritance\n");
            }
            printf("\tDestination: InVal.Vtbl.Midl DoubleInherit\n");

            ParentFive* object2 = new ParentFive();
            void* vtableO2 = (void*)*(void**)object2;
            destination = (void*)((unsigned int)vtableO2 + sizeof(void*));
            
            delete object2;
            break;
        }
        // todo shellcode
        // todo existing code double reference

        default: {
            printf("bad first option\n");
            doExit(2);
            break;
        }
    }


    switch (arg2) {  //todo fix
        case 0:  //stack+SingleInherit+sequentialOverFlow:
        {
            // if (debug)
            printf("\tObject Atacked: SingleInherit\n");
                printf("\tMethod: stack+sequentialOverFlow\n");
            SingleInherit1 singleObj;
            void* buffer[3];

            ParentOne* object = &singleObj;
            if (uaf)
            {
                if (debug) {
                    printf("\t\tno UAF on stack\n");
                }
                doExit(2);
            }

            if (((void*)object < (void*)buffer))
            {
                if (debug) {
                    printf("\t\t*****cant overflow...object before buffer\n");
                    printf("\t\tobject = %p\tbuffer = %p\n", object, buffer);
                }
                doExit(3);
            }

            if (debug) {
                printf("\t\tobject = %p\tbuffer = %p\n", object, buffer);
                printf("\t\told vpointer = %p\n", (void*)*(void**)object);
            }

            // printf("address of buffer = %p \naddress of object = %p\n\n", &buffer[0], &singleObj);
            for (int i = 0; i<((unsigned int)object - (unsigned int)buffer); ++i)  // overflow
            {
                *(buffer + i) = (void*)destination;
                if ((void*)(buffer + i) == (void*)object)
                    break;
            }

            if (debug) 
            {
                printf("\t\tnew vpointer = %p\n", (void*)*(void**)object);
                printf("\t\tvpointer to copy = %p\n", destination);
            }

            if (debug) {
                printf("\t\tcalling ParentOne::SingleInherit1::F1\n");
            }
            object->doSomething1(attackString);
            break;
        }

        case 1:  // heap+SingleInherit+sequentialOverFlow:
        {
            printf("\tObject Atacked: SingleInherit\n");
            if (!uaf) {
                printf("\tMethod: heap+sequentialOverFlow\n");
            }
            else {
                printf("\tMethod: heap+UAF\n");
            }

            // void** buffer = (void **)malloc(sizeof(void*)*3);
            void** buffer = new void*[1];   // stick with 1 otherwise get memory dump
            // void** buffer = (void **)malloc(0x30);
// printf("%d\n", sizeof(SingleInherit1));
// printf("%d\n", sizeof(void*));
            ParentOne* object = new SingleInherit1();

            if (debug)
            {
                printf("\t\tobject = %p\tbuffer = %p\n", object, buffer);
                printf("\t\told vpointer = %p\n", (void*)*(void**)object);
            }

            if (uaf)
            {
                delete object;
                doUAF(object, sizeof(SingleInherit1));
                goto attack1; 
            }

            if (((void*)object < (void*)buffer))
            {
                if (debug) {
                    printf("\t\t*****cant overflow...object before buffer\n");
                }
                doExit(3);
            }


            for (int i = 0;i<((unsigned int)object - (unsigned int)buffer) ; ++i)  // overflow
            {
                // cout << ((unsigned int)object - (unsigned int)buffer)<< "   "<<i << endl;
                *(buffer + i) = (void*)destination;
                if ((void*)(buffer + i) == (void*)object)
                    break;
            }

            // free(buffer);   // dont free metadata might crash due to corruption


            attack1:    
                if (debug)
                {
                    printf("\t\tnew vpointer = %p\n", (void*)*(void**)object);
                    printf("\t\tvpointer to copy = %p\n", destination);
                    printf("\t\tcalling ParentOne::SingleInherit1::F1\n");
                }

                object->doSomething1(attackString);

                break;
        }

        case 2:  // stack+SingleInherit+indirectOverwrite:
        {
            if (uaf)
            {
                if (debug) {
                    printf("\t\tno UAF on stack\n");
                }
                doExit(2);
            }
            printf("\tObject Atacked: SingleInherit\n");
            printf("\tMethod: stack+indirectOverwrite\n");

            if (debug) {
                printf("\t\tstack+SingleInherit+indirectOverwrite\n");
            }
            SingleInherit1 singleObj;

            ParentOne* object = &singleObj;

            if (debug)
            printf("\t\told vpointer = %p\n", (void*)*(void**)object);

            *(void**)object = destination;

            if (debug) {
                printf("\t\tnew vpointer = %p\n", (void*)*(void**)object);
                printf("\t\tvpointer to copy = %p\n", destination);
                printf("\t\tcalling ParentOne::SingleInherit1::F1\n");
            }
            object->doSomething1(attackString);
            break;
        }

        case 3:  // heap+SingleInherit+indirectOverwrite:
        {
            printf("\tObject Atacked: SingleInherit\n");
            if (!uaf) {
                printf("\tMethod: heap+indirectOverwrite\n");
            } else {
                printf("\tMethod: heap+UAF\n");
            }

            ParentOne* object = new SingleInherit1();

            if (debug)
                printf("\t\told vpointer = %p\n", (void*)*(void**)object);

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
                    printf("\t\tnew vpointer = %p\n", (void*)*(void**)object);
                    printf("\t\tvpointer to copy = %p\n", destination);
                    printf("\t\tcalling ParentOne::SingleInherit1::F1\n");
                }
                object->doSomething1(attackString);
                break;
        }




        case 4:  //stack+SingleInherit+sequentialOverFlow:
        {
            if (uaf)
            {
                if (debug) {
                    printf("\t\tno UAF on stack\n");
                }
                doExit(2);
            }
            printf("\tObject Atacked: SingleInherit\n");
            printf("\tMethod: stack+sequentialOverFlow\n");
            SingleInherit1 singleObj;
            void* buffer[3];

            SingleInherit1* object = &singleObj;

            if (((void*)object < (void*)buffer))
            {
                if (debug) {
                    printf("\t\t*****cant overflow...object before buffer\n");
                    printf("\t\tobject = %p\tbuffer = %p\n", object, buffer);
                }
                doExit(3);
            }

            if (debug) {
                printf("\t\tobject = %p\tbuffer = %p\n", object, buffer);
            }

            if (debug) {
                printf("\t\told vpointer = %p\n", (void*)*(void**)object);
            }
            // printf("address of buffer = %p \naddress of object = %p\n\n", &buffer[0], &singleObj);
            for (int i = 0; i<((unsigned int)object - (unsigned int)buffer); ++i)  // overflow
            {
                *(buffer + i) = (void*)destination;
                if ((void*)(buffer + i) == (void*)object)
                    break;
            }

            if (debug) 
            {
                printf("\t\tnew vpointer = %p\n", (void*)*(void**)object);
                printf("\t\tvpointer to copy = %p\n", destination);
            }

            if (debug) {
                printf("\t\tcalling SingleInherit1::SingleInherit1::F1\n");
            }
            object->doSomething1(attackString);
            break;
        }

        case 5:  // heap+SingleInherit+sequentialOverFlow:
        {

            printf("\tObject Atacked: SingleInherit\n");
            if (!uaf) {
                printf("\tMethod: heap+sequentialOverFlow\n");
            } else {
                printf("\tMethod: heap+UAF\n");
            }
            // void** buffer = (void **)malloc(sizeof(void*)*3);
            void** buffer = new void*[1];   // stick with 1 otherwise get memory dump
            // void** buffer = (void **)malloc(0x30);
// printf("%d\n", sizeof(SingleInherit1));
// printf("%d\n", sizeof(void*));
            SingleInherit1* object = new SingleInherit1();

            if (debug)
            {
                printf("\t\tobject = %p\tbuffer = %p\n", object, buffer);
                printf("\t\told vpointer = %p\n", (void*)*(void**)object);
            }

            if (uaf)
            {
                delete object;
                doUAF(object, sizeof(SingleInherit1));
                goto attack5; 
            }


            if (((void*)object < (void*)buffer))
            {
                if (debug) {
                    printf("\t\t*****cant overflow...object before buffer\n");
                }
                doExit(3);
            }


            for (int i = 0;i<((unsigned int)object - (unsigned int)buffer) ; ++i)  // overflow
            {
                // cout << ((unsigned int)object - (unsigned int)buffer)<< "   "<<i << endl;
                *(buffer + i) = (void*)destination;
                if ((void*)(buffer + i) == (void*)object)
                    break;
            }

            // free(buffer);   // dont free metadata might crash due to corruption

            attack5:
                if (debug)
                {
                    printf("\t\tnew vpointer = %p\n", (void*)*(void**)object);
                    printf("\t\tvpointer to copy = %p\n", destination);
                    printf("\t\tcalling SingleInherit1::SingleInherit1::F1\n");
                }
                    
                object->doSomething1(attackString);

                break;
        }

        case 6:  // stack+SingleInherit+indirectOverwrite:
        {
            if (uaf)
            {
                if (debug) {
                    printf("\t\tno UAF on stack\n");
                }
                doExit(2);
            }
            printf("\tObject Atacked: SingleInherit\n");
            printf("\tMethod: stack+indirectOverwrite\n");

            SingleInherit1 singleObj;

            SingleInherit1* object = &singleObj;

            if (debug) {
                printf("\t\told vpointer = %p\n", (void*)*(void**)object);
            }

            *(void**)object = destination;

            if (debug) {
                printf("\t\tnew vpointer = %p\n", (void*)*(void**)object);
                printf("\t\tvpointer to copy = %p\n", destination);
                printf("\t\tcalling SingleInherit1::SingleInherit1::F1\n");
            }
            object->doSomething1(attackString);
            break;
        }

        case 7:  // heap+SingleInherit+indirectOverwrite:
        {
            printf("\tObject Atacked: SingleInherit\n");
            if (!uaf) {
                printf("\tMethod: heap+indirectOverwrite\n");
            } else {
                printf("\tMethod: heap+UAF\n");
            } 

            if (debug) {
                printf("\t\theap+SingleInherit+indirectOverwrite\n");
            }
            SingleInherit1* object = new SingleInherit1();

            if (debug){
                printf("\t\told vpointer = %p\n", (void*)*(void**)object);
            }

            if (uaf)
            {
                delete object;
                doUAF(object, sizeof(SingleInherit1));
                goto attack7; 
            }


            *(void**)object = destination;
            
            
            attack7:
                if (debug)
                {
                    printf("\t\tnew vpointer = %p\n", (void*)*(void**)object);
                    printf("\t\tvpointer to copy = %p\n", destination);
                    printf("\t\tcalling SingleInherit1::SingleInherit1::F1\n");
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
                    printf("\t\tno UAF on stack\n");
                }
                doExit(2);
            }

            printf("\tObject Atacked: DoubleInheritChild\n");
            printf("\tMethod: stack+withinObjectOverFlow\n");

            DoubleInheritChild1 doubleObj;

            DoubleInheritChild1* object = &doubleObj;

            if (debug)
            printf("\t\told vpointer = %p\n", (void*)*((void**)object + 4));

            for (int i = 0; i < 4; ++i)  // overflow assume that vptr is directly after
            {
                *(object->buffer + i) = (void*)destination;
            }

            if (debug)
            printf("\t\tnew vpointer = %p\n", (void*)*((void**)object + 4));
            if (debug)
            printf("\t\tvpointer to copy = %p\n", destination);

            if (debug) {
                printf("\t\tcalling DoubleInheritChild1::DoubleInheritChild1::F3 ParentFour::F1\n");
            }
            object->ParentFourfoo1(attackString);
            break;
        }

        case 9:  // heap+DoubleInherit+withinObjectOverFlow:
        {
            printf("\tObject Atacked: DoubleInheritChild\n");
            if (!uaf) {
                printf("\tMethod: heap+withinObjectOverFlow\n");
            } else {
                printf("\tMethod: heap+UAF\n");
            } 

            DoubleInheritChild1* object = new DoubleInheritChild1();

            if (debug)
                printf("\t\told vpointer = %p\n", (void*)*((void**)object + 4));

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
                if (debug)
                {
                    printf("\t\tnew vpointer = %p\n", (void*)*((void**)object + 4));
                    printf("\t\tvpointer to copy = %p\n", destination);
                    printf("\t\tcalling DoubleInheritChild1::DoubleInheritChild1::F3 ParentFour::F1\n");
                }

                object->ParentFourfoo1(attackString);
                break;
        }

        case 10:  // stack+DoubleInherit+INdirectOverwrite:
        {
            if (uaf)
            {
                if (debug) {
                    printf("\t\tno UAF on stack\n");
                }
                doExit(2);
            }
            printf("\tObject Atacked: DoubleInheritChild\n");
            printf("\tMethod: stack+indirectOverwrite\n");

            DoubleInheritChild1 doubleObj;

            DoubleInheritChild1* object = &doubleObj;

            if (debug) {
                printf("\t\told vpointer = %p\n", (void*)*((void**)object + 4));
            }

            object->overflow();

            if (debug) {
                printf("\t\tnew vpointer = %p\n", (void*)*((void**)object + 4));
                printf("\t\tvpointer to copy = %p\n", destination);
                printf("\t\tcalling DoubleInheritChild1::DoubleInheritChild1::F3 ParentFour::F1\n");
            }
            object->ParentFourfoo1(attackString);
            break;
        }

        case 11:  // heap+DoubleInherit+INdirectOverwrite:
        {

            printf("\tObject Atacked: DoubleInheritChild\n");
            if (!uaf) {
                printf("\tMethod: heap+indirectOverwrite\n");
            } else {
                printf("\tMethod: heap+UAF\n");
            }

            DoubleInheritChild1* object = new DoubleInheritChild1();

            if (debug) {
                printf("\t\told vpointer = %p\n", (void*)*((void**)object + 4));
            }

            if (uaf)
            {
                delete object;
                doUAF(object, sizeof(DoubleInheritChild1));
                goto attack11; 
            }


            object->overflow();


            attack11:
                if (debug)
                {
                    printf("\t\tnew vpointer = %p\n", (void*)*((void**)object + 4));
                    printf("\t\tvpointer to copy = %p\n", destination);
                    printf("\t\tcalling DoubleInheritChild1::DoubleInheritChild1::F3 ParentFour::F1\n");
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
                    printf("\t\tno UAF on stack\n");
                }
                doExit(2);
            }

            printf("\tObject Atacked: DoubleInheritParent\n");
            printf("\tMethod: stack+sequentialOverFlow\n");

            if (arg1 > 1)
            {
                ParentThree singleObj;
                void* buffer[3];

                ParentThree* object = &singleObj;

                if (((void*)object < (void*)buffer))
                {
                    if (debug) {
                        printf("\t\t*****cant overflow...object before buffer\n");
                    printf("object = %p\tbuffer = %p\n", object, buffer);
                    }

                    doExit(3);
                }

                if (debug) {
                    printf("\t\tobject = %p\tbuffer = %p\n", object, buffer);
                    printf("\t\told vpointer = %p\n", (void*)*(void**)object);
                }

                for (int i = 0; i<((unsigned int)object - (unsigned int)buffer); ++i)  // overflow
                {
                    *(buffer + i) = (void*)destination;
                    if ((void*)(buffer + i) == (void*)object)
                        break;
                }

                if (debug) 
                {
                    printf("\t\tnew vpointer = %p\n", (void*)*(void**)object);
                    printf("\t\tvpointer to copy = %p\n", destination);
                }

                if (debug) {
                    printf("\t\tcalling ParentThree::ParentThree::F1\n");
                }
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
                    if (debug) {
                        printf("\t\t*****cant overflow...object before buffer\n");
                        printf("\t\tobject = %p\tbuffer = %p\n", object, buffer);
                    }
                    doExit(3);
                }

                if (debug)
                    printf("\t\tobject = %p\tbuffer = %p\n", object, buffer);

                if (debug)
                    printf("\t\told vpointer = %p\n", (void*)*(void**)object);

                for (int i = 0; i<((unsigned int)object - (unsigned int)buffer); ++i)  // overflow
                {
                    *(buffer + i) = (void*)destination;
                    if ((void*)(buffer + i) == (void*)object)
                        break;
                }

                if (debug) 
                {
                    printf("\t\tnew vpointer = %p\n", (void*)*(void**)object);
                    printf("\t\tvpointer to copy = %p\n", destination);
                }

                if (debug) {
                    printf("\t\tcalling ParentFour::ParentFour::F3\n");
                }
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
                    if (debug) {
                        printf("\t\t*****cant overflow...object before buffer\n");
                        printf("\t\tobject = %p\tbuffer = %p\n", object, buffer);
                    }
                    doExit(3);
                }

                if (debug)
                    printf("\t\tobject = %p\tbuffer = %p\n", object, buffer);

                if (debug)
                    printf("\t\told vpointer = %p\n", (void*)*(void**)object);

                for (int i = 0; i<((unsigned int)object - (unsigned int)buffer); ++i)  // overflow
                {
                    *(buffer + i) = (void*)destination;
                    if ((void*)(buffer + i) == (void*)object)
                        break;
                }

                if (debug) 
                {
                    printf("\t\tnew vpointer = %p\n", (void*)*(void**)object);
                    printf("\t\tvpointer to copy = %p\n", destination);
                }

                if (debug) {
                    printf("\t\tcalling ParentFour::ParentFour::F1\n");
                }
                object->ParentFourfoo1(attackString);
                break;
            }
        }

        case 13:  // heap+DoubleInheritChild+sequentialOverFlow:
        {

            printf("\tObject Atacked: DoubleInheritParent\n");
            if (!uaf) {
                printf("\tMethod: heap+sequentialOverFlow\n");
            } else {
                printf("\tMethod: heap+UAF\n");
            } 

            if (arg1 > 1)
            {
                void** buffer = new void*[1];   // stick with 1 otherwise get memory dump

                ParentThree* object = new ParentThree();

                if (debug) 
                {
                    printf("\t\tobject = %p\tbuffer = %p\n", object, buffer);
                    printf("\t\told vpointer = %p\n", (void*)*(void**)object);
                }

                if (uaf)
                {
                    delete object;
                    doUAF(object, sizeof(ParentThree));
                    goto attack13a; 
                }

                if (((void*)object < (void*)buffer))
                {
                    if (debug) {
                        printf("\t\t*****cant overflow...object before buffer\n");
                    }
                    doExit(3);
                }


                for (int i = 0;i<((unsigned int)object - (unsigned int)buffer) ; ++i)  // overflow
                {
                    *(buffer + i) = (void*)destination;
                    if ((void*)(buffer + i) == (void*)object)
                        break;
                }

                // free(buffer);   // dont free metadata might crash due to corruption

                attack13a:
                    if (debug)
                    {
                        printf("\t\tnew vpointer = %p\n", (void*)*(void**)object);
                        printf("\t\tvpointer to copy = %p\n", destination);
                        printf("\t\tcalling ParentThree::ParentThree::F1\n");
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
                    printf("\t\tobject = %p\tbuffer = %p\n", object, buffer);
                    printf("\t\told vpointer = %p\n", (void*)*(void**)object);
                }

                if (uaf)
                {
                    delete object;
                    doUAF(object, sizeof(ParentFour));
                    goto attack13b; 
                }

                if (((void*)object < (void*)buffer))
                {
                    if (debug) {
                        printf("\t\t*****cant overflow...object before buffer\n");
                    }
                    doExit(3);
                }


                for (int i = 0;i<((unsigned int)object - (unsigned int)buffer) ; ++i)  // overflow
                {
                    *(buffer + i) = (void*)destination;
                    if ((void*)(buffer + i) == (void*)object)
                        break;
                }

                // free(buffer);   // dont free metadata might crash due to corruption

                attack13b:
                    if (debug)
                    {
                        printf("\t\tnew vpointer = %p\n", (void*)*(void**)object);
                        printf("\t\tvpointer to copy = %p\n", destination);
                        printf("\t\tcalling ParentFour::ParentFour::F3\n");
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
                    printf("\t\tobject = %p\tbuffer = %p\n", object, buffer);
                    printf("\t\told vpointer = %p\n", (void*)*(void**)object);
                }

                if (uaf)
                {
                    delete object;
                    doUAF(object, sizeof(ParentFour));
                    goto attack13c; 
                }

                if (((void*)object < (void*)buffer))
                {
                    if (debug) {
                        printf("\t\t*****cant overflow...object before buffer\n");
                    }
                    doExit(3);
                }


                for (int i = 0;i<((unsigned int)object - (unsigned int)buffer) ; ++i)  // overflow
                {
                    *(buffer + i) = (void*)destination;
                    if ((void*)(buffer + i) == (void*)object)
                        break;
                }

                // free(buffer);   // dont free metadata might crash due to corruption

                attack13c:
                    if (debug)
                    {
                        printf("\t\tnew vpointer = %p\n", (void*)*(void**)object);
                        printf("\t\tvpointer to copy = %p\n", destination);
                        printf("\t\tcalling ParentFour::ParentFour::F1\n");
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
                    printf("\t\tno UAF on stack\n");
                }
                doExit(2);
            }
            printf("\tObject Atacked: DoubleInheritParent\n");
            printf("\tMethod: stack+indirectOverwrite\n");

            if (debug) {
                printf("\t\tstack+DoubleInheritChild+indirectOverwrite\n");
            }

            if (arg1 > 1)
            {
                ParentThree singleObj;
                ParentThree* object = &singleObj;

                if (debug) {
                    printf("\t\told vpointer = %p\n", (void*)*(void**)object);
                }

                *(void**)object = destination;

                if (debug) 
                {
                    printf("\t\tnew vpointer = %p\n", (void*)*(void**)object);
                    printf("\t\tvpointer to copy = %p\n", destination);
                    printf("\t\tcalling ParentThree::ParentThree::F1\n");
                }

                object->foo1(attackString);
                break;
            }
            else if (arg1 == 0)
            {
                ParentFour singleObj;
                ParentFour* object = &singleObj;

                if (debug)
                    printf("\t\told vpointer = %p\n", (void*)*(void**)object);

                *(void**)object = destination;

                if (debug) 
                {
                    printf("\t\tnew vpointer = %p\n", (void*)*(void**)object);
                    printf("\t\tvpointer to copy = %p\n", destination);
                    printf("\t\tcalling ParentFour::ParentFour::F3\n");
                }

                object->ParentFourfoo3(attackString);
                break;

            }
            else if (arg1 == 1)
            {
                ParentFour singleObj;
                ParentFour* object = &singleObj;

                if (debug) {
                    printf("\t\told vpointer = %p\n", (void*)*(void**)object);
                }

                *(void**)object = destination;

                if (debug) 
                {
                    printf("\t\tnew vpointer = %p\n", (void*)*(void**)object);
                    printf("\t\tvpointer to copy = %p\n", destination);
                    printf("\t\tcalling ParentFour::ParentFour::F1\n");
                }

                object->ParentFourfoo1(attackString);
                break;

            }
        }

        case 15:  // heap+DoubleInheritChild+indirectOverwrite:
        {

            printf("\tObject Atacked: DoubleInheritParent\n");
            if (!uaf) {
                printf("\tMethod: heap+withinObjectOverFlow\n");
            } else {
                printf("\tMethod: heap+UAF\n");
            } 


            if (arg1 > 1)
            {
                ParentThree* object = new ParentThree();

                if (debug)
                    printf("\t\told vpointer = %p\n", (void*)*(void**)object);

                if (uaf)
                {
                    delete object;
                    doUAF(object, sizeof(ParentThree));
                    goto attack15a; 
                }


                *(void**)object = destination;
                
                
                attack15a:
                    if (debug)
                    {
                        printf("\t\tnew vpointer = %p\n", (void*)*(void**)object);
                        printf("\t\tvpointer to copy = %p\n", destination);
                        printf("\t\tcalling ParentThree::ParentThree::F1\n");
                    }

                    object->foo1(attackString);
                    break;
            }
            else if (arg1 == 0)
            {   
                ParentFour* object = new ParentFour();

                if (debug)
                    printf("\t\told vpointer = %p\n", (void*)*(void**)object);

                if (uaf)
                {
                    delete object;
                    doUAF(object, sizeof(ParentFour));
                    goto attack15b; 
                }


                *(void**)object = destination;
                
                
                attack15b:
                    if (debug)
                    {
                        printf("\t\tnew vpointer = %p\n", (void*)*(void**)object);
                        printf("\t\tvpointer to copy = %p\n", destination);
                        printf("\t\tcalling ParentFour::ParentFour::F3\n");
                    }

                    object->ParentFourfoo3(attackString);
                    break;
            }
            else if (arg1 == 1)
            {   
                ParentFour* object = new ParentFour();

                if (debug)
                    printf("\t\told vpointer = %p\n", (void*)*(void**)object);

                if (uaf)
                {
                    delete object;
                    doUAF(object, sizeof(ParentFour));
                    goto attack15c; 
                }


                *(void**)object = destination;
                
                
                attack15c:
                    if (debug)
                    {
                        printf("\t\tnew vpointer = %p\n", (void*)*(void**)object);
                        printf("\t\tvpointer to copy = %p\n", destination);
                        printf("\t\tcalling ParentFour::ParentFour::F1\n");
                    }

                    object->ParentFourfoo1(attackString);
                    break;
            }

        }

        case 50: goto syslbl;  // just so that compiler does not remove it

        default: {
            printf("bad second option\n");
            doExit(2);
            break;
        }
    }



syslbl:
    // system(attackString);
    // printf("%s\n", "at syslbl ATTACK successful\n");
    // printf("%d %d\n", arg1, arg2);
    // printf("", arg1, arg2);
    if (debug) {
        printf("\t\tat syslbl ATTACK successful\n");
    }
    if(uaf) {
        // cout << flush << "\tUAF Attacked! " << arg1 << " " << arg2;
        printf("\tUAF Attacked! %d %d\n", arg1, arg2);
    } else {
        // cout << flush << "\tAttacked! " << arg1 << " " << arg2;
        printf("\tAttacked! %d %d\n", arg1, arg2);
    }
    doExit(1);


 //    ******************uaf
	// Child1* c1 = new Child1();

	// c1 -> doSomething("sh");

	// free(c1);

	// new Child2();

	// c1 -> doSomething("sh");
}


