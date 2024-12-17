### part 1  
```
./p1 < input
```
### part 2  
Wasn't sure how many times I had to run the emulated program, so I wrote
a small disassembler/transpiler.

Opcodes:
```
./disasm <example 
program = "015430"
0000:	00 01	adv 1
0002:	05 04	out A
0004:	03 00	jnz 0
```

C code:
```
$ ./disasm 1 <example 
program = "015430"
L0:
    A=A>>1;
    out(A);
    if (A) goto L0;
```

Use your input file instead of example, and plug the transpiled code into p2.c, adjust target string and len, compile, and run:
```
./p2
123456789123456789 # <--- your answer here
```
