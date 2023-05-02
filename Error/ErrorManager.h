#pragma once
class ErrorManager {
public:
	static void raiseLexerError();
	static void raiseParserError();
	static void raiseError();
	static bool isSafe();

private:
	static int mErrorCount;
	static int mLexerErrorCount;
	static int mParserErrorCount;

};
