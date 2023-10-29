COMPILER = g++
COMPILER_FLAGS = -Wl,--no-as-needed -ldl -std=c++17
DLL_COMPILER_FLAGS =  -shared -fPIC

OUT_FILENAME = Coda.out
OS_OUT_FILENAME = OS.so
RANDOM_OUT_FILENAME = Random.so
MATHS_OUT_FILENAME = Maths.so

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

OBJDIR = ./bin/Release/Linux/

MAIN_OBJECTS = $(patsubst %.cpp,$(OBJDIR)%.o,$(MAIN_PROJECT))
OS_OBJECTS = $(patsubst %.cpp,$(OBJDIR)%.o,$(OS_PACKAGE))
RANDOM_OBJECTS = $(patsubst %.cpp,$(OBJDIR)%.o,$(RANDOM_PACKAGE))
MATHS_OBJECTS = $(patsubst %.cpp,$(OBJDIR)%.o,$(MATHS_PACKAGE))

all: $(OBJDIR) $(MAIN_OBJECTS)
	$(COMPILER) $(COMPILER_FLAGS) $(MAIN_OBJECTS) -o $(OBJDIR)$(OUT_FILENAME)

os: $(OBJDIR) $(OS_OBJECTS)
	$(COMPILER) $(DLL_COMPILER_FLAGS) $(OS_OBJECTS) -o $(OBJDIR)$(OS_OUT_FILENAME)
	cp -f $(CURDIR)/dll/OS/os.coda $(OBJDIR)os.coda

random: $(OBJDIR) $(RANDOM_OBJECTS)
	$(COMPILER) $(DLL_COMPILER_FLAGS) $(RANDOM_OBJECTS) -o $(OBJDIR)$(RANDOM_OUT_FILENAME)
	cp -f $(CURDIR)/dll/Random/random.coda $(OBJDIR)random.coda

maths: $(OBJDIR) $(MATHS_OBJECTS)
	$(COMPILER) $(DLL_COMPILER_FLAGS) $(MATHS_OBJECTS) -o $(OBJDIR)$(MATHS_OUT_FILENAME)
	cp -f $(CURDIR)/dll/Maths/maths.coda $(OBJDIR)maths.coda


$(OBJDIR)%.o: %.cpp
	mkdir -p $(dir $@)
	$(COMPILER) $(COMPILER_FLAGS) $(DLL_COMPILER_FLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -fr ./bin
	rm -f $(MAIN_OBJECTS) $(OBJDIR)$(OUT_FILENAME)
