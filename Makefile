#use `make` to create a 'debug.main.exe' - Debug file
#    this wil always open a terminal window besides the game
#    The DEBUG_BUILD flag will also be set during compilation

#use `make run` to build and execute the current debug build

#use `make release` to create 'main.exe' release file 
#    this will not open a console window uppon start there are no outputs made

#use `make clean` to remove all compiled files 

TARGET:=RGSDL
CPP:=g++

LIBS:= -I./headers -lSDL2main -lSDL2 -lSDL2_image
COMMON_FLAGS:=-std=c++2a -Wno-unknown-pragmas

DEBUG_FLAGS:=-Wall -g -DDEBUG_BUILD
RELEASE_FLAGS:= 

CPPFILES:=$(shell find ./src -name *.cpp | xargs)
OBJFILES:=$(patsubst ./%.cpp,build/%.o,$(CPPFILES))

lib:FLAGS:=$(COMMON_FLAGS) $(RELEASE_FLAGS)
lib: $(OBJFILES)
	$(shell rm -rf ./lib)
	$(shell mkdir -p lib)
	ar q lib/lib$(TARGET).a $^

build/%.o: ./%.cpp
	$(shell mkdir -p `dirname $@`)
	$(CPP) $^ -c $(FLAGS) -o $@

.phony: clean

clean:
	$(shell rm -rf ./build)
	$(shell rm -rf ./lib)
	$(shell rm -f  *.$(TARGET) $(TARGET))
