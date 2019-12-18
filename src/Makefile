CC = gcc
COMPILER_FLAGS = -std=c99 -fstack-protector-all -g
LINKER_FLAGS = -lm
EXEC_NAME = siguel
ODIR = obj
OBJECTS = main.o distance.o heapsort.o geometry.o object.o circle.o rectangle.o block.o equipment.o \
    segment.o vertex.o building.o wall.o file_util.o svg.o commands.o query.o data.o \
    redblack_tree.o hash_table.o point.o text.o files.o commerce.o commerce_type.o \
    person.o polygon.o pathfind.o binary_heap.o graph_node.o
INTERACTION = interaction.o
INTERACTION_GUI = interaction_gui.o gui.o

OBJ = $(patsubst %,$(ODIR)/%,$(OBJECTS))
OBJ_INT = $(patsubst %,$(ODIR)/%,$(INTERACTION))
OBJ_INT_GUI = $(patsubst %,$(ODIR)/%,$(INTERACTION_GUI))

all: $(EXEC_NAME)

$(EXEC_NAME): $(OBJ) $(OBJ_INT)
	$(CC) $(OBJ) $(OBJ_INT) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(EXEC_NAME) -Wall

$(EXEC_NAME)_gui: $(OBJ) $(OBJ_INT_GUI)
	$(CC) $(OBJ) $(OBJ_INT_GUI) $(COMPILER_FLAGS) -D CURSES_ON $(LINKER_FLAGS) -lncurses -o $(EXEC_NAME) -Wall

$(ODIR)/main.o: main.c modules/util/file_util.h modules/sig/object.h commands.h $(ODIR)
	$(CC) -c $< -o $@ $(COMPILER_FLAGS)

$(ODIR)/distance.o: modules/aux/distance.c modules/aux/distance.h modules/sig/equipment.h
	$(CC) -c $< -o $@ $(COMPILER_FLAGS)

$(ODIR)/segment.o: modules/aux/segment.c modules/aux/segment.h modules/aux/vertex.h
	$(CC) -c $< -o $@ $(COMPILER_FLAGS)

$(ODIR)/vertex.o: modules/aux/vertex.c modules/aux/vertex.h modules/aux/segment.h
	$(CC) -c $< -o $@ $(COMPILER_FLAGS)

$(ODIR)/point.o: modules/aux/point.c modules/aux/point.h
	$(CC) -c $< -o $@ $(COMPILER_FLAGS)

$(ODIR)/heapsort.o: modules/aux/heapsort.c modules/aux/heapsort.h
	$(CC) -c $< -o $@ $(COMPILER_FLAGS)

$(ODIR)/geometry.o: modules/sig/geometry.c modules/sig/geometry.h modules/sig/object.h
	$(CC) -c $< -o $@ $(COMPILER_FLAGS)

$(ODIR)/object.o: modules/sig/object.c modules/sig/object.h
	$(CC) -c $< -o $@ $(COMPILER_FLAGS)

$(ODIR)/circle.o: modules/sig/circle.c modules/sig/circle.h
	$(CC) -c $< -o $@ $(COMPILER_FLAGS)

$(ODIR)/rectangle.o: modules/sig/rectangle.c modules/sig/rectangle.h
	$(CC) -c $< -o $@ $(COMPILER_FLAGS)

$(ODIR)/text.o: modules/sig/text.c modules/sig/text.h
	$(CC) -c $< -o $@ $(COMPILER_FLAGS)

$(ODIR)/block.o: modules/sig/block.c modules/sig/block.h
	$(CC) -c $< -o $@ $(COMPILER_FLAGS)

$(ODIR)/equipment.o: modules/sig/equipment.c modules/sig/equipment.h
	$(CC) -c $< -o $@ $(COMPILER_FLAGS)

$(ODIR)/building.o: modules/sig/building.c modules/sig/building.h
	$(CC) -c $< -o $@ $(COMPILER_FLAGS)

$(ODIR)/wall.o: modules/sig/wall.c modules/sig/wall.h
	$(CC) -c $< -o $@ $(COMPILER_FLAGS)

$(ODIR)/file_util.o: modules/util/file_util.c modules/util/file_util.h
	$(CC) -c $< -o $@ $(COMPILER_FLAGS)

$(ODIR)/svg.o: modules/util/svg.c modules/util/svg.h modules/sig/object.h modules/sig/circle.h modules/sig/rectangle.h
	$(CC) -c $< -o $@ $(COMPILER_FLAGS)

$(ODIR)/commands.o: commands.c commands.h modules/util/file_util.h modules/util/svg.h modules/sig/geometry.h modules/sig/object.h
	$(CC) -c $< -o $@ $(COMPILER_FLAGS)

$(ODIR)/query.o: query.c query.h modules/util/svg.h modules/sig/geometry.h modules/sig/object.h
	$(CC) -c $< -o $@ $(COMPILER_FLAGS)

$(ODIR)/data.o: data.c data.h modules/data_structures/redblack_tree.h modules/data_structures/hash_table.h modules/aux/point.h
	$(CC) -c $< -o $@ $(COMPILER_FLAGS)

$(ODIR)/redblack_tree.o: modules/data_structures/redblack_tree.c modules/data_structures/redblack_tree.h
	$(CC) -c $< -o $@ $(COMPILER_FLAGS)

$(ODIR)/hash_table.o: modules/data_structures/hash_table.c modules/data_structures/hash_table.h
	$(CC) -c $< -o $@ $(COMPILER_FLAGS)

$(ODIR)/files.o: modules/util/files.c modules/util/files.h
	$(CC) -c $< -o $@ $(COMPILER_FLAGS)

$(ODIR)/commerce_type.o: modules/sig/commerce_type.c modules/sig/commerce_type.h
	$(CC) -c $< -o $@ $(COMPILER_FLAGS)

$(ODIR)/commerce.o: modules/sig/commerce.c modules/sig/commerce.h modules/sig/building.h modules/sig/commerce_type.h
	$(CC) -c $< -o $@ $(COMPILER_FLAGS)

$(ODIR)/person.o: modules/sig/person.c modules/sig/person.h modules/sig/building.h
	$(CC) -c $< -o $@ $(COMPILER_FLAGS)

$(ODIR)/interaction.o: interaction.c interaction.h
	$(CC) -c $< -o $@ $(COMPILER_FLAGS)

$(ODIR)/interaction_gui.o: interaction.c interaction.h
	$(CC) -c $< -o $@ $(COMPILER_FLAGS) -lncurses -D CURSES_ON

$(ODIR)/polygon.o: modules/aux/polygon.c modules/aux/polygon.h
	$(CC) -c $< -o $@ $(COMPILER_FLAGS)

$(ODIR)/gui.o: gui.c gui.h
	$(CC) -c $< -o $@ $(COMPILER_FLAGS) -lncurses -D CURSES_ON

$(ODIR)/pathfind.o: pathfind.c pathfind.h
	$(CC) -c $< -o $@ $(COMPILER_FLAGS)

$(ODIR)/binary_heap.o: modules/data_structures/binary_heap.c modules/data_structures/binary_heap.h
	$(CC) -c $< -o $@ $(COMPILER_FLAGS)

$(ODIR)/graph_node.o: modules/aux/graph_node.c modules/aux/graph_node.h
	$(CC) -c $< -o $@ $(COMPILER_FLAGS)

$(ODIR):
	mkdir $@

clean:
	rm -rf obj/*.o
