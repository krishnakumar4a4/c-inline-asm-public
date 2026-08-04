# Creating annotations using inline assembly
## Scenario 1: local variable + inline assembly
1. Generate main.obj using `clang-cl /nologo /c /Zi /Od /Fomain.obj main.cpp`
2. Running `llvm-readobj --codeview main.obj` shows error as below
```
CodeViewDebugInfo [
  Section: .debug$S (4)
  Magic: 0x4
C:\Program Files\LLVM\bin\llvm-readobj.exe: error: 'main.obj': Stream Error: The stream is too short to perform the requested operation.
```
3. Running `llvm-objdump -s -j '.debug$S' main.obj` shows
```                                                                             
main.obj:       file format coff-x86-64
Contents of section .debug$S:
 0000 04000000 f1000000 18000000 16001910  ................
 0010 09000000 00000300 62310062 32006233  ........b1.b2.b3
 0020 00000000 04000000 f1000000 90000000  ................
 0030 5e000111 00000000 433a5c55 73657273  ^.......C:\Users
 0040 5c6b7468 6f6b616c 615c736f 75726365  \********\source
 .
 .
0110 01000000 1e003e11 03100000 0000636f  ......>.......co
 0120 6d70696c 65725f64 65627567 5f6c6f63  mpiler_debug_loc
 0130 616c0000 0e004211 04000000 01000000  al....B.........
 ....
```
4. Running `clang-cl /nologo /Zi /Od main.cpp /Femain.dll /link /dll /noentry /nodefaultlib /incremental:no /debug:full /pdb:main.pdb` is successful with following warnings
```
main-908b1b.obj : warning LNK4209: debugging information corrupt; recompile module; linking object as if no debug info
main-908b1b.obj : warning LNK4209: debugging information corrupt; recompile module; linking object as if no debug info
```
5. Running `llvm-pdbutil dump -symbols -globals .\main.pdb` shows but no trace of `compiler_debug_local`
```
                          Symbols                           
============================================================
  Mod 0000 | `C:\Users\********\AppData\Local\Temp\main-908b1b.obj`: 
       4 | S_ANNOTATION [size = 24]
           addr = 0001:0009
           strings = [
                      b1
                      b2
                      b3]
Mod 0001 | `* Linker *`: 
```
## Scenario 2: multiple inline assemblies
1. Generate main.obj using `clang-cl /nologo /c /Zi /Od /Fomain.obj main.cpp`
2. Running `llvm-readobj --codeview main.obj` shows error as below
```
CodeViewDebugInfo [
  Section: .debug$S (4)
  Magic: 0x4
C:\Program Files\LLVM\bin\llvm-readobj.exe: error: 'main.obj': Stream Error: The stream is too short to perform the requested operation.
```
3. Running `llvm-objdump -s -j '.debug$S' main.obj` shows
```
main.obj:       file format coff-x86-64
Contents of section .debug$S:
 0000 04000000 f1000000 18000000 16001910  ................
 0010 00000000 00000300 62310062 32006233  ........b1.b2.b3
 0020 00000000 04000000 f1000000 18000000  ................
 0030 16001910 00000000 00000300 63310063  ............c1.c
 0040 32006333 00000000 04000000 f1000000  2.c3............
 0050 90000000 5e000111 00000000 433a5c55  ....^.......C:\U
 0060 73657273 5c6b7468 6f6b616c 615c736f  sers\********\so
 ....
```
4. Running `clang-cl /nologo /Zi /Od main.cpp /Femain.dll /link /dll /noentry /nodefaultlib /incremental:no /debug:full /pdb:main.pdb` is successful
5. Running `llvm-pdbutil dump -symbols -globals .\main.pdb` shows but no trace of S_ANNOTATION with c1, c2, c3
```
  Mod 0000 | `C:\Users\********\AppData\Local\Temp\main-d0ba56.obj`: 
       4 | S_ANNOTATION [size = 24]
           addr = 0001:0000
           strings = [
                      b1
                      b2
                      b3]
```