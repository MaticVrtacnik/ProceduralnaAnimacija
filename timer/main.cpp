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


#include "Engine.hpp"

#include <SDL2\SDL.h>

#define GLEW_STATIC
#undef main


void printGPUInfo(){
	int value;

	std::cout << "GPU Vendor: " << glGetString(GL_VENDOR) << '\n';
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << '\n';
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << '\n';
	std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << '\n';

	SDL_GL_GetAttribute(SDL_GL_CONTEXT_FLAGS, &value);
	std::cout << "Debug context: " << (value & SDL_GL_CONTEXT_DEBUG_FLAG ? "yes" : "no") << '\n';

	SDL_GL_GetAttribute(SDL_GL_ACCELERATED_VISUAL, &value);
	std::cout << "HW accelerated: " << (value ? "yes" : "no") << '\n';

	SDL_GL_GetAttribute(SDL_GL_DOUBLEBUFFER, &value);
	std::cout << "Double buffered: " << (value ? "yes" : "no") << '\n';

	SDL_GL_GetAttribute(SDL_GL_MULTISAMPLESAMPLES, &value);
	std::cout << "FSAA samples: " << value << '\n';

	SDL_GL_GetAttribute(SDL_GL_DEPTH_SIZE, &value);
	std::cout << "Depth bits: " << value << '\n';

	SDL_GL_GetAttribute(SDL_GL_STENCIL_SIZE, &value);
	std::cout << "Stencil bits: " << value << '\n';

	SDL_GL_GetAttribute(SDL_GL_RED_SIZE, &value);
	std::cout << "RGBA bits: " << value << " ";
	SDL_GL_GetAttribute(SDL_GL_GREEN_SIZE, &value);
	std::cout << value << " ";
	SDL_GL_GetAttribute(SDL_GL_BLUE_SIZE, &value);
	std::cout << value << " ";
	SDL_GL_GetAttribute(SDL_GL_ALPHA_SIZE, &value);
	std::cout << value << '\n';

	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &value);
	std::cout << "Max Texture Units: " << value << std::endl;

	glGetIntegerv(GL_MAX_UNIFORM_LOCATIONS, &value);
	std::cout << "Max Uniforms: " << value << std::endl;

	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &value);
	std::cout << "Max Vertex Attributes: " << value << std::endl;

	std::cout << "***************************************" << std::endl << std::endl;
}

int main(){
	int _flags = SDL_WINDOW_OPENGL;

#ifdef RESIZABLE
	_flags |= SDL_WINDOW_RESIZABLE;
#endif

#ifdef FULLSCREEN
	_flags |= SDL_WINDOW_FULLSCREEN;
#endif



	/*SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);*/
	
	SDL_Window *_screen = SDL_CreateWindow("Engine v1.0.0", 50, 50,
		DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, _flags);
	if (_screen == nullptr){
		return 0;
	}

	SDL_GLContext _context = SDL_GL_CreateContext(_screen);
	glewInit();
	printGPUInfo();
	

	int _return =  Application().run(_screen);

	SDL_Quit();
	SDL_GL_DeleteContext(_context);
	SDL_DestroyWindow(_screen);

	return _return;
}