CPP      = g++
OBJ      = main.o
LINKOBJ  = main.o
LIBS     = -m32 -lopengl32 -lglu32 -lfreeglut
BIN      = load_model
RM       = rm -f

.PHONY: all all-before all-after clean clean-custom

all: all-before $(BIN) all-after

clean: clean-custom
	${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CPP) $(LINKOBJ) -o $(BIN) $(LIBS)

main.o: main.cpp
	$(CPP) -c main.cpp -o main.o -std=c++11 -m32
