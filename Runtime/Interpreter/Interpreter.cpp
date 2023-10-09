#include "Interpreter.h"


std::stack<Coda::Error::Position> Coda::Runtime::Interpreter::callStack;
namespace Coda {
	namespace Runtime {
		Interpreter::Interpreter() {
			Interpreter::callStack = std::stack<Position>();
		}

		ValuePtr Interpreter::interpret(const Frontend::Node& astNode, Environment& env)
		{
			IF_ERROR_RETURN_VALUE_PTR;

			Value value = Value();
			value.startPosition = astNode.startPosition;
			value.endPosition = astNode.endPosition;


			switch (astNode.type) {
				case Frontend::NodeType::IDENTIFIER:
					return evaluateIdentifier(astNode, env);
				case Frontend::NodeType::BYTE_LITERAL:
					value.type = Type::BYTE;
					value.value = astNode.value;
					break;
				case Frontend::NodeType::INTEGER_LITERAL:
					value.type = Type::INT;
					value.value = astNode.value;
					break;
				case Frontend::NodeType::LONG_INT_LITERAL:
					value.type = Type::LONG;
					value.value = astNode.value;
					break;
				case Frontend::NodeType::FLOATING_POINT_LITERAL:
					value.type = Type::FLOAT;
					value.value = astNode.value;
					break;
				case Frontend::NodeType::DOUBLE_LITERAL:
					value.type = Type::DOUBLE;
					value.value = astNode.value;
					break;
				case Frontend::NodeType::STRING_LITERAL:
					value.type = Type::STRING;
					value.value = astNode.value;
					break;
				case Frontend::NodeType::CHARACTER_LITERAL:
					value.type = Type::CHAR;
					value.value = astNode.value;
					break;
				case Frontend::NodeType::OBJECT_LITERAL:
					return evaluateObjectExpression(astNode, env);
				case Frontend::NodeType::SCOPE_LITERAL:
					return evaluateScopeExpression(astNode, env);
				case Frontend::NodeType::LIST_LITERAL:
					return evaluateListExpression(astNode, env);
				case Frontend::NodeType::CALL_EXPRESSION:
					auto res = evaluateCallExpression(astNode, env);
					if (Error::Manager::isSafe())
						Interpreter::callStack.pop();
					return res;
				case Frontend::NodeType::NATIVE_CALL_EXPRESSION:
					auto res = evaluateNativeCallExpression(astNode, env);
					if (Error::Manager::isSafe())
						Interpreter::callStack.pop();
					return res;
				case Frontend::NodeType::BLOCK_STATEMENT:
					return evaluateBlockExpression(astNode, env);
				case Frontend::NodeType::UNARY_EXPRESSION:
					return evaluateUnaryExpression(astNode, env);
				case Frontend::NodeType::BINARY_EXPRESSION:
					return evaluateBinaryExpression(astNode, env);
				case Frontend::NodeType::MEMBER_EXPRESSION:
					auto res = evaluateMemberExpression(astNode, env);
					if (Error::Manager::isSafe())
						Interpreter::callStack.pop();
					return res;
				case Frontend::NodeType::VARIABLE_DECLARATION:
					return evaluateVariableDeclaration(astNode, env);
				case Frontend::NodeType::FUNCTION_LITERAL:
					return evaluateFunctionDeclaration(astNode, env);
				case Frontend::NodeType::CONSTANT_DECLARATION:
					return evaluateVariableDeclaration(astNode, env, true);
				case Frontend::NodeType::ASSIGNMENT_EXPRESSION:
					return evaluateAssignmentExpression(astNode, env);
				case Frontend::NodeType::IF_EXPRESSION:
					return evaluateIfExpression(astNode, env);
				case Frontend::NodeType::FOR_EXPRESSION:
					return evaluateForExpression(astNode, env);
				case Frontend::NodeType::FOR_IN_EXPRESSION:
					return evaluateForInExpression(astNode, env);
				case Frontend::NodeType::WHILE_EXPRESSION:
					return evaluateWhileExpression(astNode, env);
				case Frontend::NodeType::DO_WHILE_EXPRESSION:
					return evaluateDoWhileExpression(astNode, env);
				case Frontend::NodeType::JUMP_EXPRESSION:
					return evaluateJumpExpression(astNode, env);
				default:
					Error::Runtime::raise("Unrecognized ASTNode '" + astNode.value + "'");
			}

			return std::make_shared<Value>(value);
		}

		ValuePtr Interpreter::evaluateProgram(const Frontend::Program& program, Environment& env)
		{
			IF_ERROR_RETURN_VALUE_PTR;
			std::shared_ptr<Value> lastEvaluated = nullptr;
			for (Frontend::Node statement : program.body) {
				lastEvaluated = interpret(statement, env);
			}

			return lastEvaluated;
		}
	} // namespace Runtime
} // namespace Coda

