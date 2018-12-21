# RIPE ALPHA

##### Files: ##### 
There are 90 files with names `RIPEalphaXX.cpp`. Ranging from `RIPEalpha1.cpp` to `RIPEalpha90.cpp`. Each file corresponds to one of the 90 combinations as mentioned in the submitted report.

##### How to compile: ##### 
The files can be compiled with `clang++ -m32 RIPEalpha1.cpp -o RIPEalpha1`. The `-m32` option is to specify 32-bit compilation. To compile with CPI would be similar, except that there would be more CPI specific parameters.


##### How to run: ##### 
There is one optional command line argument to the program. `d` can be passed to enable some useful additional information. e.g. `./RIPEalpha1 d` or just `./RIPEalpha1` will also work.

##### Output of the attack scenarios: ##### 
The success outcome of an attack is printed on the screen. It can also be determined from the exit status of the program.
            1. Attack did not succeed, i.e. the execution was normal: Exit Code = 0; Output: `Attack Failed, Normal execution`  
            2. Attack succeeded, i.e. the execution was diverted to a malicious target line in the code: Exit Code = 1; Output: `Attack Succeeded!`  
            3. Attack could not performed. This could be because of two reasons, for use-after-free attacks pointer of the newly created object was not in the same location as the recently freed object. Second case is when overflowing a buffer, the memory layout of the program is such that the object is placed before the buffer. For an attack to happen the buffer needs to be placed before an object so that a sequential overflow will overwrite the buffer.
            Restarting the attack scenario several times might change the outcome in this case. Exit Code = 3; Output: `Attack not performed, could not get desired pointer addresses`.

##### Sample output: ##### 

Below you can see the output of RIPEalpha2 with debug mode disabled.

            *******************************************************
            Running test # 2/90:
                Destination: Val.Vtbl.Strt
                Object Atacked: SingleInherit
                Method: heap+sequentialOverFlow
                Attacked! 0 1
            *******************************************************
            Attack Succeeded!
            *******************************************************


Below you can see the output of RIPEalpha2 with debug mode enabled.

            *******************************************************
            Running test # 2/90:
                Destination: Val.Vtbl.Strt
                Object Atacked: SingleInherit
                Method: heap+sequentialOverFlow
                    object = 0x8a62018  buffer = 0x8a62008
                    old vpointer = 0x804f238
                    new vpointer = 0x804f310
                    vpointer to copy = 0x804f310
                    calling ParentOne::SingleInherit1::F1
                    in SingleInherit2::F1
                Attacked! 0 1
            *******************************************************
            Attack Succeeded!
            *******************************************************


The `Destination` is the type of malicious target. `Object Atacked` is the type of object that is attacked. `Method` is the type of attack, e.g. in this case attack on heap objects by sequentially overflowing them. More details are present in the report.

In this case the buffer was allocated at address 0x8a62008, physically before the object, which was at 0x8a62018. The initial VPTR of the object was pointing to the VTable at address 0x804f238, and our malicious target is supposed to be a VTable at 0x804f310. Value of `new vpointer` in the output shows that the VPTR was successfuly overwritten to point to target address. The original call was to method `F1` of class `SingleInherit1` as seen in the output. The line `in SingleInherit2::F1` tells us that instead of the original function, a function of some other class was called. Meanining that the attack was successful. 

In the case of a failed attack the output would have said `called SingleInherit1::F1`, and, `Attack Failed, Normal execution`
