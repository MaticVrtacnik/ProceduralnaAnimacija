#ifndef BITWISE_UTILITIES_HPP
#define BITWISE_UTILITIES_HPP

namespace Engine{
	namespace Utilities{

		bool checkBit(int variable, short bit);

		int toggleBit(int variable, short bit);

		int enableBit(int variable, short bit);

		int disableBit(int variable, short bit);

		bool isBigEndian();

		int convertDataToInteger(char *data, int n);

	}
}

#endif //BITWISE_UTILITIES_HPP