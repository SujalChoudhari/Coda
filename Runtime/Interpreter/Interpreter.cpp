#include "Interpreter.h"

namespace Coda {
	namespace Runtime {
		ValuePtr Interpreter::interpret(const Frontend::Node& astNode, Environment& env)
		{
			IF_ERROR_RETURN_VALUE_PTR;

			Value value = Value();
			value.startPosition = astNode.startPosition;
			value.endPosition = astNode.endPosition;


			if (astNode.type == Frontend::NodeType::IDENTIFIER) {
				return evaluateIdentifier(astNode, env);
			}
			else if (astNode.type == Frontend::NodeType::BYTE_LITERAL) {
				value.type = Type::BYTE;
				value.value = astNode.value;
			}
			else if (astNode.type == Frontend::NodeType::INTEGER_LITERAL) {
				value.type = Type::INT;
				value.value = astNode.value;
			}
			else if (astNode.type == Frontend::NodeType::LONG_INT_LITERAL) {
				value.type = Type::LONG;
				value.value = astNode.value;
			}
			else if (astNode.type == Frontend::NodeType::FLOATING_POINT_LITERAL) {
				value.type = Type::FLOAT;
				value.value = astNode.value;
			}
			else if (astNode.type == Frontend::NodeType::DOUBLE_LITERAL) {
				value.type = Type::DOUBLE;
				value.value = astNode.value;
			}
			else if (astNode.type == Frontend::NodeType::STRING_LITERAL) {
				value.type = Type::STRING;
				value.value = astNode.value;
			}
			else if (astNode.type == Frontend::NodeType::CHARACTER_LITERAL) {
				value.type = Type::CHAR;
				value.value = astNode.value;
			}
			else if (astNode.type == Frontend::NodeType::OBJECT_LITERAL) {
				return evaluateObjectExpression(astNode, env);
			}
			else if (astNode.type == Frontend::NodeType::SCOPE_LITERAL) {
				return evaluateScopeExpression(astNode, env);
			}
			else if (astNode.type == Frontend::NodeType::LIST_LITERAL) {
				return evaluateListExpression(astNode, env);
			}
			else if (astNode.type == Frontend::NodeType::CALL_EXPRESSION) {
				return evaluateCallExpression(astNode, env);
			}
			else if (astNode.type == Frontend::NodeType::BLOCK_STATEMENT) {
				return evaluateBlockExpression(astNode, env);
			}
			else if (astNode.type == Frontend::NodeType::UNARY_EXPRESSION) {
				return evaluateUnaryExpression(astNode, env);
			}
			else if (astNode.type == Frontend::NodeType::BINARY_EXPRESSION) {
				return evaluateBinaryExpression(astNode, env);
			}
			else if (astNode.type == Frontend::NodeType::MEMBER_EXPRESSION) {
				return evaluateMemberExpression(astNode, env);
			}
			else if (astNode.type == Frontend::NodeType::VARIABLE_DECLARATION) {
				return evaluateVariableDeclaration(astNode, env);
			}
			else if (astNode.type == Frontend::NodeType::FUNCTION_LITERAL) {
				return evaluateFunctionDeclaration(astNode, env);
			}
			else if (astNode.type == Frontend::NodeType::CONSTANT_DECLARATION) {
				return evaluateVariableDeclaration(astNode, env, true);
			}
			else if (astNode.type == Frontend::NodeType::ASSIGNMENT_EXPRESSION) {
				return evaluateAssignmentExpression(astNode, env);
			}
			else if (astNode.type == Frontend::NodeType::IF_EXPRESSION) {
				return evaluateIfExpression(astNode, env);
			}
			else if (astNode.type == Frontend::NodeType::FOR_EXPRESSION) {
				return evaluateForExpression(astNode, env);
			}
			else if (astNode.type == Frontend::NodeType::WHILE_EXPRESSION) {
				return evaluateWhileExpression(astNode, env);
			}
			else if (astNode.type == Frontend::NodeType::DO_WHILE_EXPRESSION) {
				return evaluateDoWhileExpression(astNode, env);
			}
			else if (astNode.type == Frontend::NodeType::JUMP_EXPRESSION) {
				return evaluateJumpExpression(astNode, env);
			}
			else {
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

