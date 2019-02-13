/*
MIT License

Copyright (c) 2019 MaticVrtacnik

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/


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