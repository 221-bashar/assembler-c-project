# Assembler Project in C  

## 📌 Overview  
This project is an **assembler written in C, developed as part of a university course. The assembler reads assembly source files (`.as`) and produces the corresponding machine code and supporting output files. It implements a **two-pass process** (first pass & second pass) for symbol resolution and code generation.  

## 🛠 Features  
- Pre-assembler stage (macro handling and preprocessing).  
- First pass for symbol table creation and label detection.  
- Second pass for encoding instructions and resolving references.  
- Support for external and entry symbols.  
- Generates output files:  
  - `.ob` – Object file (machine code).  
  - `.ent` – Entry symbols.  
  - `.ext` – External symbols.  

## 📂 Project Structure  
```
project/
├── encoding.c / encoding.h
├── first_pass.c / first_pass.h
├── second_pass.c / second_pass.h
├── pre_assembler.c / pre_assembler.h
├── define_variable.c / define_variable.h
├── data.c / data.h
├── makefile
├── ps.as          # Example input file
├── ps.ob          # Example object output
├── ps.ent / ps.ext# Example symbol outputs
└── ...
```

## ⚡ Compilation  
Use the provided `makefile` to build the project:  
```bash
make
```
This will compile all modules and generate the assembler executable.  

To clean build files:  
```bash
make clean
```

## ▶️ Usage  
Run the assembler with an assembly source file:  
```bash
./assembler ps.as
```
This will produce `.ob`, `.ent`, and `.ext` files as output.  

## 🧪 Example  
Input file (`ps.as`):  
```asm
; sample assembly file
MAIN:   mov r3, LENGTH
        add r2,
         r3
        stop
LENGTH: .data 6
```

Output files include:  
- `ps.ob` (object code)  
- `ps.ent` (entries)  
- `ps.ext` (externals, if used)  

## 📚 Skills Gained  
- Low-level programming in C  
- Modular software design  
- Understanding of assembler internals and machine code translation  
- Working with symbol tables and memory management  
