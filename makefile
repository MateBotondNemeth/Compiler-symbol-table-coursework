all: symbol_table_part_a symbol_table_part_b

symbol_table_part_a:
	g++ symbol_table_part_a.cpp -o symbol_table

symbol_table_part_b:
	gcc -o tree.obj -c tree.cpp
	ar rcs tree.lib tree.obj
    g++ symbol_table_part_b.cpp tree.lib -o symbol_table

clean:
    del *.obj
	del *.exe
	del *.lib
	del *.txt