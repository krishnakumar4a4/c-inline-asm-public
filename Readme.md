# Creating annotations with inline assembly

This code demonstrates how emitting CodeView `S_ANNOTATION` records via inline assembly corrupts the PDB, causing all the other symbols to be lost.

## Steps to Reproduce
### Build the sample

Run the following command:

```powershell
clang-cl /nologo /Zi /Od main.cpp /Femain.dll /link /dll /noentry /nodefaultlib /incremental:no /debug:full /pdb:main.pdb
```

The build will report the following warnings indicating that debug info has been corrupted:

```text
main-908b1b.obj : warning LNK4209: debugging information corrupt; recompile module; linking object as if no debug info
main-908b1b.obj : warning LNK4209: debugging information corrupt; recompile module; linking object as if no debug info
```

### Inspect the PDB

Upon build completion, dump the symbols and globals from the generated PDB:

```powershell
llvm-pdbutil dump -symbols -globals main.pdb
```

The output will contain the `S_ANNOTATION` record with the values `b1`, `b2`, and `b3`, but the symbols `my_local` and `my_global` will be missing.
