.PHONY: refltest
refltest:
	@gcc -Wall -Wextra -pedantic -I. tests/mminfotest.c -o $@
	@./$@

preproc:
	@gcc -E -I. tests/mminfotest.c > _mminfotest.c

clean:
	@rm -f refltest* _*.c *.exe *.out
