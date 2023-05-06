#pragma once

namespace Coda {
	namespace Error {

		class Manager {
		public:

			static void raiseError();
			static bool isSafe();
			static void reset();

		private:
			static int mErrorCount;
		};

	}
}