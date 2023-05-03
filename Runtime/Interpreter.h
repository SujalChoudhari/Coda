#pragma once

#include "Value.h"
#include "Type.h"
#include "../Frontend/Node/NodeType.h"
#include "../Frontend/Node/Node.h"

namespace Coda {
	namespace Runtime {

		class Interpreter {

		public:
			Runtime::Value evaluate(FrontEnd::Node astNode);
		};

	}
}