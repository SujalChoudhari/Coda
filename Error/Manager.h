#pragma once

namespace Coda {
	namespace Error {

		class Manager {
		public:
			static void raiseLexerError();
			static void raiseParserError();
			static void raiseRuntimeError();
			static void raiseError();
			static bool isSafe();
			static void reset();

		private:
			static int mErrorCount;
			static int mLexerErrorCount;
			static int mParserErrorCount;
			static int mRuntimeErrorCount;

		};

	}
}