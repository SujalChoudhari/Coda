# Define the version number
VERSION_NO = 1.1

# Compiler configurations
COMPILER = g++
DEBUG_FLAGS = -g -Wall -Wextra -Wpedantic
RELEASE_FLAGS = -O3
COMPILER_FLAGS = -Wl,--no-as-needed -ldl -std=c++17 -fPIC
DLL_COMPILER_FLAGS = -shared 
DEBUG_OUT_FILENAME = Coda_debug.out
RELEASE_OUT_FILENAME = Coda.out

# Output filenames for packages
OS_OUT_FILENAME = OS.so
RANDOM_OUT_FILENAME = Random.so
MATHS_OUT_FILENAME = Maths.so

# Define main project and package files
MAIN_PROJECT = Application/Application.cpp \
	Error/Raiser.cpp \
	Error/Manager.cpp \
	Frontend/Importer/Importer.cpp \
	Frontend/Lexer/Handlers.cpp \
	Frontend/Node/Node.cpp \
	Frontend/Lexer/Lexer.cpp \
	Frontend/Parser/ParseBinary.cpp \
	Frontend/Parser/ParseControlStatement.cpp \
	Frontend/Parser/ParseGeneral.cpp \
	Frontend/Parser/ParseObjectType.cpp \
	Frontend/Parser/Parser.cpp \
	Frontend/Tokens/Constants.cpp \
	Frontend/Tokens/Token.cpp \
	Main.cpp \
	Runtime/Environment/Environment.cpp \
	Runtime/Interpreter/EvaluateBinary.cpp \
	Runtime/Interpreter/EvaluateControlStructure.cpp \
	Runtime/Interpreter/EvaluateGeneral.cpp \
	Runtime/Interpreter/EvaluateObjectType.cpp \
	Runtime/Interpreter/Handler.cpp \
	Runtime/Interpreter/Interpreter.cpp \
	Runtime/NativeFunctions/NativeFunction.cpp \
	Runtime/RuntimeValue/Value.cpp \
	Utils/ArgParser.cpp \
	Utils/FileReader.cpp

OS_PACKAGE =dll/OS/FileSystem.cpp \
	dll/OS/OS.cpp \

RANDOM_PACKAGE = dll/Random/Random.cpp

MATHS_PACKAGE = dll/Maths/Basic.cpp\
	dll/Maths/BasicFunctions.cpp\
	dll/Maths/Trignometry.cpp


# Define directories for debug and release modes
DEBUG_OBJDIR = ./bin/Debug/Linux/
RELEASE_OBJDIR = ./bin/Release/Linux/

# Set object directory based on mode
ifeq ($(MODE),release)
    OBJDIR = $(RELEASE_OBJDIR)
    COMPILER_FLAGS += $(RELEASE_FLAGS)
    OUT_FILENAME = $(RELEASE_OUT_FILENAME)
else
    OBJDIR = $(DEBUG_OBJDIR)
    COMPILER_FLAGS += $(DEBUG_FLAGS)
    OUT_FILENAME = $(DEBUG_OUT_FILENAME)
endif

# Generate object paths for main, OS, Random, and Maths files
MAIN_OBJECTS = $(patsubst %.cpp,$(OBJDIR)%.o,$(MAIN_PROJECT))
OS_OBJECTS = $(patsubst %.cpp,$(OBJDIR)%.o,$(OS_PACKAGE))
RANDOM_OBJECTS = $(patsubst %.cpp,$(OBJDIR)%.o,$(RANDOM_PACKAGE))
MATHS_OBJECTS = $(patsubst %.cpp,$(OBJDIR)%.o,$(MATHS_PACKAGE))


# Build targets
debug: $(OBJDIR) $(MAIN_OBJECTS)
	$(COMPILER) $(COMPILER_FLAGS) $(DEBUG_FLAGS) $(MAIN_OBJECTS) -o $(OBJDIR)$(OUT_FILENAME)

release: $(OBJDIR) $(MAIN_OBJECTS)
	$(COMPILER) $(COMPILER_FLAGS) $(RELEASE_FLAGS) $(MAIN_OBJECTS) -o $(OBJDIR)$(OUT_FILENAME)

os: $(OBJDIR) $(OS_OBJECTS)
	$(COMPILER) $(COMPILER_FLAGS) $(DLL_COMPILER_FLAGS) $(OS_OBJECTS) -o $(OBJDIR)$(OS_OUT_FILENAME)
	cp -f $(CURDIR)/dll/OS/os.coda $(OBJDIR)os.coda

random: $(OBJDIR) $(RANDOM_OBJECTS)
	$(COMPILER) $(COMPILER_FLAGS) $(DLL_COMPILER_FLAGS) $(RANDOM_OBJECTS) -o $(OBJDIR)$(RANDOM_OUT_FILENAME)
	cp -f $(CURDIR)/dll/Random/random.coda $(OBJDIR)random.coda

maths: $(OBJDIR) $(MATHS_OBJECTS)
	$(COMPILER) $(COMPILER_FLAGS) $(DLL_COMPILER_FLAGS) $(MATHS_OBJECTS) -o $(OBJDIR)$(MATHS_OUT_FILENAME)
	cp -f $(CURDIR)/dll/Maths/maths.coda $(OBJDIR)maths.coda

# Rule for creating object files
$(OBJDIR)%.o: %.cpp
	mkdir -p $(dir $@)
	$(COMPILER) $(COMPILER_FLAGS) $(DLL_COMPILER_FLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

# Target for publishing
package:
	make release
	make random MODE=release
	make maths MODE=release
	make os MODE=release
	tar -cvzf Coda_v$(VERSION_NO).tar.gz -C $(RELEASE_OBJDIR) \
	$(RELEASE_OUT_FILENAME) \
	$(OS_OUT_FILENAME) \
	$(RANDOM_OUT_FILENAME) \
	$(MATHS_OUT_FILENAME) \
	os.coda \
	random.coda \
	maths.coda

# Clean up target
clean:
	rm -fr ./bin
	rm -f *.o
	rm -f *.out