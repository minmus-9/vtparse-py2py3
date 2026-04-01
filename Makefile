PYTHON:=python3
RUBY_GENERATION_FILES = vtparse_gen_c_tables.rb vtparse_tables.rb

all: vtparse_table.c vtparse_table.h test libvtparse.a vtparse.py

clean:
	rm -f vtparse_table.c vtparse_table.h test vtparse.o vtparse_table.o libvtparse.a
	rm -f vtparse_table.ppc

vtparse.py:	vtparse_table.c vtparse_table.h vtparse_gen.py vtparse_gen.inc
	gcc -E - < vtparse_table.c > vtparse_table.ppc
	$(PYTHON) vtparse_gen.py < vtparse_table.ppc > $@

vtparse_table.c: $(RUBY_GENERATION_FILES)
	ruby -I . vtparse_gen_c_tables.rb

vtparse_table.h: $(RUBY_GENERATION_FILES)
	ruby -I . vtparse_gen_c_tables.rb

test: vtparse.c vtparse.h vtparse_table.c vtparse_table.h vtparse_test.c
	gcc -Wall -o test vtparse_test.c vtparse.c vtparse_table.c

libvtparse.a: vtparse.o vtparse_table.o
	rm -f $@
	ar r $@ $^
	ranlib $@

.c.o:
	gcc -O3 -Wall -o $@ -c $<


.PHONY: all clean

