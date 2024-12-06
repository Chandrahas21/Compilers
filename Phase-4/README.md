# This is directory for Semantic phase.

2. To compile the code, follow the following command:
    ```bash
    make
    ```

## Usage

Once you’ve compile the code, you can test the input programs.

### Running Input Programs

1. **To Run the input2.txt:** 

    ```bash
    make run NUM=2
    ```
    ---> It will run the input2.txt file.
2. **To Print the sybol table for input2.txt:**

    ```bash
    make run NUM=2 VERBOSE=-v
    ```
3. **Default input program:**

   ---> Default input program is input.txt.To run it follow the following command.
    ```bash
    make run
    ```
   ---> It will run input.txt program

    ```bash
    make run VERBOSE=-v
    ```
   ---> Above command will print symbol table for input.txt program.

