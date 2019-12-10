CC= gcc
LDFLAGS = -Wall
SDLBUILD = `src/SDL/build/sdl2-config --cflags --libs`
LIBS = -lm
DIR = src/SDL/build/ant_src/
SRC = $(DIR)graph.c $(DIR)arrayList.c $(DIR)random.c $(DIR)vector.c $(DIR)sdl_graph.c
BUILD = build/
OBJ = $(SRC:.c=.o)
EXEC = app

all:	$(BUILD)$(EXEC) test clean
	

$(BUILD)$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(BUILD)$(EXEC) $(LIBS)

$(EXEC): $(SRC)
	$(CC) $(LDFLAGS) -c $(SRC) $(LIBS) $(SDLBUILD)

test:	$(BUILD)$(EXEC)
	$(BUILD)$(EXEC)

clean:
	rm $(DIR)*.o

