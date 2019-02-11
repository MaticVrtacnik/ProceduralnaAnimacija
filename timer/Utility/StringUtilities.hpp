#ifndef STRING_UTILITIES_HPP
#define STRING_UTILITIES_HPP

#include <string>
#include <vector>

namespace Engine{
	namespace Utilities{

		std::string strLower(const std::string &string);

		std::string strUpper(const std::string &string);

		std::string removeFormat(const std::string &string);

		std::vector<std::string> splitString(const std::string &string, char delimiter = ' ');

	}
}

#endif //STRING_UTILITIES_HPP