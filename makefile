ASM = nasm
FLAGS = -f elf64 -g -I src/
LINKER = ld

SOURCES = src/forthress.asm
INCS = src/util.inc src/words.inc src/macro.inc
OBJECTS = obj/forthress.o

EXECUTABLE = forthress

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(LINKER) $(OBJECTS) -o $@

$(OBJECTS): $(SOURCES) $(INCS)
	mkdir -p obj
	$(ASM) $(FLAGS) $(SOURCES)  -o $@

clean:
	rm -rf obj/
	rm $(EXECUTABLE)
