#use `make` to create a 'debug.main.exe' - Debug file
#    this wil always open a terminal window besides the game
#    The DEBUG_BUILD flag will also be set during compilation

#use `make run` to build and execute the current debug build

#use `make release` to create 'main.exe' release file 
#    this will not open a console window uppon start there are no outputs made

#use `make clean` to remove all compiled files 

TARGET:=main
CPP:=g++

LIBS:= -L./modules/RGSDL/lib -lRGSDL -lSDL2main -lSDL2 -lSDL2_image
COMMON_FLAGS:=-I./modules -std=c++2a -Wno-unknown-pragmas

DEBUG_FLAGS:=-Wall -g -DDEBUG_BUILD
RELEASE_FLAGS:=-mwindows 

CPPFILES:=$(shell find ./src -name *.cpp | xargs)
OBJFILES:=$(patsubst ./%.cpp,build/%.o,$(CPPFILES))

debug:FLAGS:=$(COMMON_FLAGS) $(DEBUG_FLAGS)
debug: $(OBJFILES)
	$(CPP) $^ $(FLAGS) $(LIBS) -o $@.$(TARGET) 

gdb: debug
	gdb ./$^.$(TARGET)

run: debug
	./$^.$(TARGET)

release:FLAGS:=$(COMMON_FLAGS) $(RELEASE_FLAGS)
release: $(OBJFILES)
	$(CPP) $^ $(FLAGS) $(LIBS) -o $(TARGET)

build/%.o: ./%.cpp
	$(shell mkdir -p `dirname $@`)
	$(CPP) $^ -c $(FLAGS) -o $@

.phony: clean

clean:
	$(shell rm -rf ./build)
	$(shell rm -f  *.$(TARGET) $(TARGET))
