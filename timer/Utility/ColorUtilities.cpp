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


#include "ColorUtilities.hpp"


namespace Engine{
	namespace Utilities{

		//Expected H[0, 360], SV[0.0, 1.0] Returns RGB[0,1]
		glm::vec3 getColorWheel(float h, float s, float v){
			int _i = (int) floor(h / 60.0f) % 6;
			float _f = h / 60.0f - floor(h / 60.0f);
			float _p = v * (float) (1 - s);
			float _q = v * (float) (1 - s * _f);
			float _t = v * (float) (1 - (1 - _f) * s);

			switch (_i) {
				case 0: return glm::vec3(v, _t, _p);
				case 1: return glm::vec3(_q, v, _p);
				case 2: return glm::vec3(_p, v, _t);
				case 3: return glm::vec3(_p, _q, v);
				case 4: return glm::vec3(_t, _p, v);
				case 5: return glm::vec3(v, _p, _q);
				default: return glm::vec3(0.0);
			}
		}


		//Expects RGB[0,1] Returns HSV[0,1]
		glm::vec3 rgb2hsv(const glm::vec3 &rgb){
			return rgb2hsv(rgb.r, rgb.g, rgb.b);
		}

		//Expects RGB[0,1] Returns HSV[0,1]
		glm::vec3 rgb2hsv(float r, float g, float b){
			float h, s, v; // h:0-360.0, s:0.0-1.0, v:0.0-1.0

			float max = glm::max(glm::max(r, g), b);
			float min = glm::min(glm::min(r, g), b);

			v = max;

			if (max == 0.0f) {
				s = h = 0.0f;
			} else if (max - min == 0.0f) {
				s = h = 0.0f;
			} else {
				s = (max - min) / max;

				if (max == r) {
					h = 60.0f * ((g - b) / (max - min)) + 0.0f;
				} else if (max == g) {
					h = 60.0f * ((b - r) / (max - min)) + 120.0f;
				} else {
					h = 60.0 * ((r - g) / (max - min)) + 240.0f;
				}
			}

			if (h < 0.0f) h += 360.0f;

			return glm::vec3(h / 360.0f, s, v);
		}


		//Expects HSV[0,1] Returns RGB[0,1]
		glm::vec3 hsv2rgb(const glm::vec3 &hsv){
			return hsv2rgb(hsv.r, hsv.g, hsv.b);
		}

		//Expects HSV[0,1] Returns RGB[0,1]
		glm::vec3 hsv2rgb(float h, float s, float v){
			h = h * 360.0f; // 0-360

			float r, g, b; // 0.0-1.0

			int   hi = (int) (h / 60.0f) % 6;
			float f = (h / 60.0f) - hi;
			float p = v * (1.0f - s);
			float q = v * (1.0f - s * f);
			float t = v * (1.0f - s * (1.0f - f));

			switch (hi) {
				case 0: r = v, g = t, b = p; break;
				case 1: r = q, g = v, b = p; break;
				case 2: r = p, g = v, b = t; break;
				case 3: r = p, g = q, b = v; break;
				case 4: r = t, g = p, b = v; break;
				case 5: r = v, g = p, b = q; break;
			}

			return glm::vec3(r, g, b);
		}

	}
}
