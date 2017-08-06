

void func1 (int a) {
	a++;
}

void func2 (int a) {
	a--;
}
void func3 (void (*funcPtrArgument)(int)) {
	funcPtrArgument = & func2;

	funcPtrArgument(123);
}

void (*funcPtrVariable)(int);

int main()
{
	void (*funcPtrArray[2])(int);

	funcPtrArray[0] = &func1;
	funcPtrArray[1] = &func2;

	funcPtrArray[0](231);

	funcPtrVariable = & func1;


	func3(funcPtrVariable);

	
	funcPtrVariable(123);



	//funcPtrVariable++;

	// foo(1);
	// func3(foo);

	return 1;
}