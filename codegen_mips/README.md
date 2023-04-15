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
and it will **generate** the code of the given file.

To remove compiled files
```
make clean
```
To see report of program you may test it as well in **final** folder. For testing type:
```
make
cd final
./run-mips
```
and a file called `run.output` will be generated to inspect and debug the output of code.

To run a mips assembly file
```
spim -file <filename.s>
```

# Additional notes
The mips assembly code may not work with `spim` if there are too many instructions. For example, if you go to `max_instructions_test.s` you might notice the fact that roughly 16300 lines/instructions is the threshold for spim.

So even if the code generator outputs the correct assembly code, it may fail to compile the moment the assembly file is too large for spim to handle.