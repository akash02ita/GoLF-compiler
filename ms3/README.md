# How to compile and run
To make the project, type
```
make
```
which should produce an executable `golf`.

To run `golf`, type
```
golf <FILE-PATH>
```
and it will **parse** the contents of the given file.

_Scanning feature_ is not supported yet, but it will be soon added in future or please visit `ms1` branch.

To remove compiled files
```
make clean
```
which should remove the executable file and the object files as well.

To see report of program you may test it as well in **tests* folder. For testing type:
```
make
cd tests
./run
```
and a file called `run.output` will be generated to inspect and debug the output of code.