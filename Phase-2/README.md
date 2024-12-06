# Phase-2 readme

---> Phase 2 folder has lexerReturn.l file, main lex file which should be linked with parsing phase and Test folder is for testing the lex phase.

---> Test folder contains lexerDetailed.l, lexerReplace.l, makefile and two directories for input and output files.

## To test , go to Test folder and follow the following commands:
```bash
make
./lexerDetailed ./Input_Files/<inputFile_name>.txt ./Output_Files/<outputFile_name>.txt
```

## Example:
```bash
make
./lexerDetailed ./Input_Files/input1.txt ./Output_Files/output1.txt
```
