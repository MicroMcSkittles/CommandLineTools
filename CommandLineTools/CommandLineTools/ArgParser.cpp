#include "CommandLineTools/ArgParser.h"

namespace CLT {
	const char* ArgumentTypeToString(ArgumentType type)
	{
		switch (type) {
		case ArgumentType::EXEPath: return "Program Path";
		case ArgumentType::Option:  return "Option";
		case ArgumentType::Value:   return "Value";
		}
		return "Unknown";
	}
	std::vector<Argument> ParseArgs(int argc, char** argv)
	{
		std::vector<Argument> arguments;

		// Store the path to the exe file, it will always be the first argument
		arguments.push_back({ argv[0], ArgumentType::EXEPath });

		// Loop through each argument
		for (int i = 1; i < argc; ++i) {
			const char* arg = argv[i];

			ArgumentType type;
			switch (arg[0])
			{
			case '-': type = ArgumentType::Option; break;
			default:  type = ArgumentType::Value; break;
			}

			arguments.push_back({ arg, type });
		}

		return arguments;
	}

}