assembler: assembler.o pre_assembler.o first_pass.o second_pass.o encoding.o data.o define_variable.o
	gcc -Wall -ansi -pedantic assembler.o pre_assembler.o first_pass.o second_pass.o encoding.o data.o define_variable.o -o assembler
assembler.o: assembler.c
	gcc -c -Wall -ansi -pedantic assembler.c -o assembler.o
pre_assembler.o: pre_assembler.c
	gcc -c -Wall -ansi -pedantic pre_assembler.c -o pre_assembler.o
first_pass.o: first_pass.c
	gcc -c -Wall -ansi -pedantic first_pass.c -o first_pass.o
data.o: data.c
	gcc -c -Wall -ansi -pedantic data.c -o data.o
second_pass.o: second_pass.c
	gcc -c -Wall -ansi -pedantic second_pass.c -o second_pass.o
encoding.o: encoding.c
	gcc -c -Wall -ansi -pedantic encoding.c -o encoding.o
define_variable.o: define_variable.c
	gcc -c -Wall -ansi -pedantic define_variable.c -o define_variable.o
