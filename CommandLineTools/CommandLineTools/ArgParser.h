#pragma once
#include <vector>

namespace CLT {

	enum class ArgumentType {
		None = 0,
		Option,
		Value,
		EXEPath
	};
	const char* ArgumentTypeToString(ArgumentType type);

	struct Argument {
		const char* value;
		ArgumentType type;

		operator const char*() { return value; }
	};

	std::vector<Argument> ParseArgs(int argc, char** argv);

}