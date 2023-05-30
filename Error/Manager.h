#pragma once

namespace Coda {
	namespace Error {
		/*
			Manager keeps track of errors.
			User can fetch the if there are any errors and reset the error count.
		*/
		class Manager {
		public:
			/*
				Increment the error count.
			*/
			static void raiseError();
			/*
				If there are any errors, returns false.
				@return True if there are no errors, false otherwise.
			*/
			static bool isSafe();

			/*
			Resets the error counter to 0.
			*/
			static void reset();

		private:
			static int mErrorCount;
		};

	} // namespace Error
} // namespace Coda