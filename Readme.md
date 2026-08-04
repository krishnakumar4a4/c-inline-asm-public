# Creating annotations with inline assembly

This sample demonstrates how emitting a CodeView annotation from inline assembly can corrupt the debug information. As a result, symbols defined outside the inline assembly may be missing from the generated PDB.

## Build the sample

Run the following command:

```powershell
clang-cl /nologo /Zi /Od main.cpp /Femain.dll /link /dll /noentry /nodefaultlib /incremental:no /debug:full /pdb:main.pdb
```

The linker reports the following warnings, indicating that the object's debug information is corrupt:

```text
main-908b1b.obj : warning LNK4209: debugging information corrupt; recompile module; linking object as if no debug info
main-908b1b.obj : warning LNK4209: debugging information corrupt; recompile module; linking object as if no debug info
```

## Inspect the PDB

Dump the symbols and globals from the generated PDB:

```powershell
llvm-pdbutil dump -symbols -globals .\main.pdb
```

The output contains the `S_ANNOTATION` record with the values `b1`, `b2`, and `b3`, but the `my_local` and `my_global` symbols are missing.