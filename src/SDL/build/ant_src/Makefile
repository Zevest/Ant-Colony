CC= gcc
LDFLAGS = -Wall
CD = cd
SDLBUILD = `sdl2-config --cflags --libs`
LIBS = -lm
DIR = 
SRC = graph.c arrayList.c random.c vector.c
MAIN = sdl_graph.c
BUILD = 
OBJ = $(SRC:.c=.o)
EXEC = ./app

all:	$(BUILD)$(EXEC) test clean

$(BUILD)$(EXEC): $(OBJ) 
	$(CC) $(MAIN) *.o -o $(BUILD)$(EXEC) $(LIBS) $(SDLBUILD)

$(OBJ): $(SRC)
	$(CC) $(LDFLAGS) -c $(SRC) $(LIBS) $(SDLBUILD)

	
test:	$(BUILD)$(EXEC)
	$(BUILD)$(EXEC)

clean:
	rm $(DIR)*.o

