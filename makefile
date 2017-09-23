d: a.out
	./a.out
a.out: main.cpp binary_index_tree.h.gch disjoint_set.h.gch segment_tree.h.gch
	g++ main.cpp
binary_index_tree.h.gch: binary_index_tree.h data_structure.h.gch
	g++ binary_index_tree.h
disjoint_set.h.gch: disjoint_set.h data_structure.h.gch
	g++ disjoint_set.h
segment_tree.h.gch: segment_tree.h data_structure.h.gch
	g++ segment_tree.h
data_structure.h.gch: data_structure.h
	g++ data_structure.h