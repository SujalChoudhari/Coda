#include "Error.h"
#include "../Utils/Colors.h"
#include "../Runtime/Interpreter/Interpreter.h"

namespace Coda {
    namespace Error {
        std::string formatPosition(const Position& pos) {
            std::string positionInfo = "Line: " + std::to_string(pos.line) +
                ", Char: " + std::to_string(pos.character);
            if (!pos.scope.empty()) {
                positionInfo += ", Scope: " + pos.scope;
            }
            return positionInfo;
        }

        std::string formatLineWithPointer(const Position& startPos, const Position& endPos) {
            std::string lineInfo;
            if (!startPos.lineText.empty()) {
                lineInfo = endPos.lineText + "\n";
                lineInfo += std::string(startPos.character - 1, '-') + "^";
                if (endPos.character - startPos.character > 2) {
                    lineInfo += std::string(endPos.character - startPos.character - 2, '~') + "^";
                }
            }
            return lineInfo;
        }

        void printError(const std::string& prefix, const std::string& error) {
            std::cout << Utils::Colors::ACCENT << "[" << prefix << "]: " <<
                Utils::Colors::ERROR << error << Utils::Colors::RESET << std::endl;
        }

        void Importer::raise(std::string error) {
            Manager::raiseError();
            printError("IMPTR", error);
        }

        void Lexer::raise(std::string error, const Position& startPos, const Position& endPos) {
            Manager::raiseError();
            printError("LEXER", error);
            std::cout << Utils::Colors::WARNING << formatPosition(startPos) << Utils::Colors::RESET << std::endl;
            std::cout << formatLineWithPointer(startPos, endPos) << std::endl;
        }

        void Parser::raise(std::string error, const Position& startPos, const Position& endPos) {
            Manager::raiseError();
            printError("PARSER", error);
            std::cout << formatLineWithPointer(startPos, endPos) << Utils::Colors::RESET << std::endl;
        }

        void Runtime::raise(std::string error, std::stack<Position>& callStack, const Position& startPos, const Position& endPos) {
            Manager::raiseError();

            std::cout << "\nTrackback:\n";
            while (!callStack.empty()) {
                std::cout << formatPosition(callStack.top()) << std::endl;
                callStack.pop();
            }
            std::cout << formatLineWithPointer(startPos, endPos) << Utils::Colors::RESET << std::endl;
            printError("RNTIME", error);

        }

        void Runtime::raise(std::string error) {
            Manager::raiseError();
            printError("RNTIME", error);
        }
    }
}
