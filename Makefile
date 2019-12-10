CC= gcc
LDFLAGS = -Wall
LIBS = -lm
DIR = src/
SRC = $(DIR)graph.c $(DIR)arrayList.c $(DIR)random.c $(DIR)vector.c
BUILD = build/
OBJ = $(SRC:.c=.o)
EXEC = app

all:	$(BUILD)$(EXEC) test clean
	

$(BUILD)$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(BUILD)$(EXEC) $(LIBS)

$(EXEC): $(SRC)
	$(CC) $(LDFLAGS) -c $(SRC) $(LIBS)

test:	$(BUILD)$(EXEC)
	$(BUILD)$(EXEC)

clean:
	rm $(DIR)*.o

