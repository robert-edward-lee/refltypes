test:
	@gcc -Wall -Wextra -pedantic -I. tests/mminfotest.c -o test.exe
	@./test.exe

preproc:
	@gcc -E -I. tests/mminfotest.c > _mminfotest.c
