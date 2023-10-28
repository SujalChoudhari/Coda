COMPILER = g++
COMPILER_FLAGS = -Wl,--no-as-needed -ldl -Wall -Wextra -Werror

OUT_FILENAME = Coda.out
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
        dll/Maths/BaiscFunctions.cpp\
        dll/Maths/Trignometry.cpp

OBJDIR = ./bin/Release/Linux/

MAIN_OBJECTS = $(MAIN_PROJECT:.cpp=.o)
OS_OBJECTS = $(OS_PACKAGE:.cpp=.o)
RANDOM_OBJECTS = $(RANDOM_PACKAGE:.cpp=.o)
MATHS_OBJECTS = $(MATHS_PACKAGE:.cpp=.o)


all:  $(OBJDIR) $(MAIN_OBJECTS)
	$(COMPILER) $(COMPILER_FLAGS) $(MAIN_OBJECTS) -o $(OUT_FILENAME)

os: $(OBJDIR) $(OS_OBJECTS)
	$(COMPILER) $(COMPILER_FLAGS) $(OS_OBJECTS) -o $(OUT_FILENAME)

random: $(OBJDIR) $(RANDOM_OBJECTS)
	$(COMPILER) $(COMPILER_FLAGS) $(RANDOM_OBJECTS) -o $(OUT_FILENAME)

maths: $(OBJDIR) $(MATHS_OBJECTS)
	$(COMPILER) $(COMPILER_FLAGS) $(MATHS_OBJECTS) -o $(OUT_FILENAME)

$(OBJDIR)%.o: %.cpp
	$(COMPILER) $(COMPILER_FLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -f $(MAIN_OBJECTS) $(OBJDIR)$(OUT_FILENAME)
