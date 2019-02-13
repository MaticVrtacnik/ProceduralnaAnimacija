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


#ifndef DEFERRED_RENDERER_HPP
#define DEFERRED_RENDERER_HPP

//#define DYNAMIC_SHADOWS

#include <memory>

#include "../Buffer/ArrayBuffer.hpp"
#include "../Buffer/Framebuffer.hpp"
#include "../Shader/Shader.hpp"


namespace Engine{
	namespace Graphics{

		enum GBuffer{
			GBUFFER_LOCATION = 0,
			GBUFFER_NORMAL,
			GBUFFER_VELOCITY,
			GBUFFER_ALBEDO,
			GBUFFER_RMOE
		};


		class Renderer{
		public:
			glm::mat4 m_paintCollisionViewTransform = glm::mat4(1.0f);

		private:
			glm::vec3 m_sunLocation;
			glm::mat4 m_sunViewMatrix;
			glm::mat4 m_sunProjectionMatrix;
			glm::mat4 m_sunViewProjectioMatrix;
			
			Buffers::Framebuffer m_gFramebuffer;
			Buffers::Framebuffer m_shadowFramebuffer;
			Buffers::Framebuffer m_occludedFramebuffer;
			Buffers::Framebuffer m_paintDepthFramebuffer;

			Buffers::Framebuffer m_finalFramebuffer;
			unsigned int m_screenWidth, m_screenHeight;

			Buffers::ArrayBuffer m_screenQuadBuffer;
			unsigned int m_screenQuadVertexArray = 0;

		private:
			void initScreenQuad();

			void initGFramebuffer();

			void initPaintDepthFramebuffer();

			void initShadowFramebuffer();

			void initOccludedFramebuffer();

			void initFinalFramebuffer();

		public:
			//static const unsigned int c_shadowResolution = 1024;
			static const unsigned int c_shadowResolution = 2048;
			//static const unsigned int c_shadowResolution = 4096;
			//static const unsigned int c_shadowResolution = 8192;

			Renderer(unsigned int screenWidth, unsigned int screenHeight);

			~Renderer();


			//before system update 
			void clearFramebuffers();


			//Sun location, matrix
			void setSunLocation(const glm::vec3 &location);

			const glm::vec3 &getSunLocation() const;

			const glm::mat4 &getSunViewMatrix() const;

			const glm::mat4 &getSunProjectionMatrix() const;

			const glm::mat4 &getSunViewProjectionMatrix() const;


			//Framebuffers
			Buffers::Framebuffer &getGFramebuffer();

			Buffers::Framebuffer &getShadowFramebuffer();

			Buffers::Framebuffer &getOccludedFramebuffer();

			Buffers::Framebuffer &getPaintDepthFramebuffer();

			Buffers::Framebuffer &getFinalFramebuffer();


			//Gframebuffer
			const unsigned int &getDepthTexture() const;

			const unsigned int &getLocationTexture() const;

			const unsigned int &getNormalTexture() const;

			const unsigned int &getAlbedoTexture() const;

			const unsigned int &getVelocityTexture() const;

			const unsigned int &getRoughMetalOccluEmissTexture() const;


			//ShadowBuffer
			const unsigned int &getShadowTexture() const;

			//OccludedLightsBuffer
			unsigned int getScreenQuadVertexArray() const;

			//Paint Depth
			unsigned int getPaintDepthTexture() const;

		};

	}
}

#endif //DEFERRED_RENDERER_HPP