#include "BitwiseUtilities.hpp"

namespace Engine{
	namespace Utilities{

		bool checkBit(int variable, short bit){
			return (variable & bit) != 0;
		}

		int toggleBit(int variable, short bit){
			return (variable ^ (1 << bit));
		}

		int enableBit(int variable, short bit){
			return (variable | (1 << bit));
		}

		int disableBit(int variable, short bit){
			return (variable & ~(1 << bit));
		}

		bool isBigEndian(){
			int x = 1;
			return !((char*)&x)[0];
		}

		int convertDataToInteger(char *data, int n){
			int x = 0;

			if (isBigEndian()){
				for (int i = 0; i < n; ++i)
					((char*)&x)[3 - i] = data[i];
			}
			else{
				for (int i = 0; i < n; ++i)
					((char*)&x)[i] = data[i];
			}

			return x;
		}

	}
}