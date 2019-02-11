#include "StringUtilities.hpp"

#include <algorithm>

namespace Engine{
	namespace Utilities{

		std::string strLower(const std::string &string){
			std::string _string = string;
			std::transform(_string.begin(), _string.end(), _string.begin(), tolower);

			return _string;
		}

		std::string strUpper(const std::string &string){
			std::string _string = string;
			std::transform(_string.begin(), _string.end(), _string.begin(), toupper);

			return _string;
		}

		std::string removeFormat(const std::string &string){
			std::string::size_type _dotId = string.find_last_of('.');
			std::string _formatless = _dotId == std::string::npos ? 
				string : string.substr(0, _dotId);

			return _formatless;
		}

		std::vector<std::string> splitString(const std::string &string, char delimiter){
			size_t _last = 0, _next = 0;
			std::vector<std::string> _substrings;

			while ((_next = string.find(delimiter, _last)) != std::string::npos){
				_substrings.push_back(string.substr(_last, _next - _last));
				_last = _next + 1;
			} _substrings.push_back(string.substr(_last));

			return _substrings;
		}

	}
}
