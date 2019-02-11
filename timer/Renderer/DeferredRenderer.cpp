#include "DeferredRenderer.hpp"

#include <glm\gtx\transform.hpp>

#include "../Texture/Texture.hpp"
#include "../Utility/MathUtilities.hpp"
#include "../Define/Assert.hpp"
#include "../Define/PrintDefines.hpp"


namespace Engine{
	namespace Graphics{

		void Renderer::initGFramebuffer(){
			m_gFramebuffer.init(m_screenWidth, m_screenHeight, true, true);

			//LOCATION-XYZ, LINEAR_DEPTH-W
			m_gFramebuffer.addTexture(Textures::createEmptyTexture(
				m_screenWidth, m_screenHeight, false, GL_RGBA32F, GL_NEAREST, GL_RGBA, GL_FLOAT));

			//NORMAL-XYZ
			m_gFramebuffer.addTexture(Textures::createEmptyTexture(
				m_screenWidth, m_screenHeight, false, GL_RGB16F, GL_NEAREST, GL_RGB, GL_FLOAT));

			//VELOCITY-XY
			m_gFramebuffer.addTexture(Textures::createEmptyTexture(
				m_screenWidth, m_screenHeight, false, GL_RG16F, GL_NEAREST, GL_RG, GL_FLOAT));

			//ALBEDO-RGB, OPACITY-A
			m_gFramebuffer.addTexture(Textures::createEmptyTexture(
				m_screenWidth, m_screenHeight, false, GL_RGBA8, GL_NEAREST, GL_RGBA, GL_FLOAT));

			//ROUGHNESS-R, METALLIC-G, OCCLUSION-B, EMISSIVE-A
			m_gFramebuffer.addTexture(Textures::createEmptyTexture(
				m_screenWidth, m_screenHeight, false, GL_RGBA8, GL_NEAREST, GL_RGBA, GL_FLOAT));


			m_gFramebuffer.bind();
			{
				//TODO render buffer depth attachment 
				m_gFramebuffer.setDrawBuffers();

				if (!m_gFramebuffer.checkErrors()){
					PRINT("FRAMEBUFFER ERROR \n");
				}
			}
			m_gFramebuffer.unbind();
		}

		void Renderer::initShadowFramebuffer(){
			m_shadowFramebuffer.init(c_shadowResolution, c_shadowResolution, true);
			m_shadowFramebuffer.addTexture(Textures::createEmptyTexture(
				c_shadowResolution, c_shadowResolution, false, GL_RG32F, GL_LINEAR, GL_RG));
				//c_shadowResolution, c_shadowResolution, false, GL_R32F, GL_LINEAR, GL_RGBA));

			Textures::bind2DTexture(m_shadowFramebuffer.getTexture(0));

			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

			GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

			Textures::unbind2DTexture();
		}

		void Renderer::initOccludedFramebuffer(){
			/*m_occludedFramebuffer.init(m_screenWidth / 2, m_screenHeight / 2, true);
			m_occludedFramebuffer.addTexture(Textures::createEmptyTexture(
				m_screenWidth / 2, m_screenHeight / 2, false, GL_RGBA, GL_LINEAR));*/

			m_occludedFramebuffer.init(m_screenWidth, m_screenHeight, false);
			m_occludedFramebuffer.addTexture(Textures::createEmptyTexture(
				m_screenWidth, m_screenHeight, false, GL_RGBA, GL_LINEAR));
		}

		void Renderer::initPaintDepthFramebuffer(){
			m_paintDepthFramebuffer.init(m_screenWidth / 2, m_screenHeight / 2, true);
			m_paintDepthFramebuffer.addTexture(Textures::createEmptyTexture(
				m_screenWidth / 2, m_screenHeight / 2, false, GL_R32F, GL_LINEAR, GL_RED));
			m_paintDepthFramebuffer.addTexture(Textures::createEmptyTexture(
				m_screenWidth / 2, m_screenHeight / 2, false, GL_RG8, GL_LINEAR, GL_RG));

			m_paintDepthFramebuffer.bind();
			{
				m_paintDepthFramebuffer.setDrawBuffers();
			}
			m_paintDepthFramebuffer.unbind();

			//TODO maybe add texture for movement perspective
		}


		void Renderer::initFinalFramebuffer(){

			m_finalFramebuffer.init(m_screenWidth, m_screenHeight, false);

			m_finalFramebuffer.addTexture(Textures::createEmptyTexture(
				m_screenWidth, m_screenHeight, false, GL_RGBA16F, GL_LINEAR, GL_RGBA, GL_FLOAT));

			//BLOOM
			m_finalFramebuffer.addTexture(Textures::createEmptyTexture(
				m_screenWidth, m_screenHeight, false, /*GL_RGBA8*/ GL_RGBA16F, GL_LINEAR, GL_RGBA, GL_FLOAT));

			m_finalFramebuffer.bind();
			m_finalFramebuffer.setDrawBuffers();
			ASSERT(m_finalFramebuffer.checkErrors(), "Deferred Framebuffer not complete");

			m_finalFramebuffer.unbind();
		}

		void Renderer::initScreenQuad(){
			static std::vector<GLfloat> _vertexData({
				-1.0f, -1.0f, 0.0f,
				1.0f, -1.0f, 0.0f,
				-1.0f, 1.0f, 0.0f,
				1.0f, 1.0f, 0.0f
			});

			glGenVertexArrays(1, &m_screenQuadVertexArray);
			glBindVertexArray(m_screenQuadVertexArray);

			m_screenQuadBuffer.loadBuffer(_vertexData, GL_ARRAY_BUFFER);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glBindVertexArray(0);
		}


		static const float c_sSize = 150.0f;
		Renderer::Renderer(unsigned int screenWidth, unsigned int screenHeight) :
			m_screenWidth(screenWidth), m_screenHeight(screenHeight), 
			m_sunProjectionMatrix(glm::ortho(-c_sSize, c_sSize, -c_sSize, c_sSize, 250.0f, 1500.0f))
		{
			initScreenQuad();
			initGFramebuffer();
			initShadowFramebuffer();
			initOccludedFramebuffer();
			initPaintDepthFramebuffer();
			initFinalFramebuffer();

			const glm::vec3 c_defaultSunLocation(-170.0f, 570.0f, 550.0f);
			setSunLocation(c_defaultSunLocation);
		}

		Renderer::~Renderer(){
			//delete depth? automatic = framebuffer texture depth
			std::cout << "DELETED RENDERER" << std::endl;

			for (auto &_texture : m_gFramebuffer.getTextures()){
				glDeleteTextures(1, &_texture);
			}
				
			for (auto &_texture : m_shadowFramebuffer.getTextures()){
				glDeleteTextures(1, &_texture);
			}

			for (auto &_texture : m_occludedFramebuffer.getTextures()){
				glDeleteTextures(1, &_texture);
			}

			for (auto &_texture : m_finalFramebuffer.getTextures()){
				glDeleteTextures(1, &_texture);
			}
		}



		void Renderer::clearFramebuffers(){
			glClearColor(0, 0, 0, 1);

			getFinalFramebuffer().bind();
			{
				glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT);
			}

			getShadowFramebuffer().bind();
			{
				glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			}

			getOccludedFramebuffer().bind();
			{
				glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT);
			}

			getPaintDepthFramebuffer().bind();
			{
				glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			}

			getGFramebuffer().bind();
			{		
				//glClearColor(1, 0.8, 0.55, 1);
				glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			}

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			{
				glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			}			
		}


		void Renderer::setSunLocation(const glm::vec3 &location){
			m_sunLocation = location;

			//Update matrix
			m_sunViewMatrix = glm::mat4(1.0f);

			glm::vec3 _rotation = Utilities::getEulerRotationFromVector(-m_sunLocation);
			m_sunViewMatrix *= glm::rotate(-_rotation.z, 0.0f, 0.0f, 1.0f);
			m_sunViewMatrix *= glm::rotate(-_rotation.x, 1.0f, 0.0f, 0.0f);
			m_sunViewMatrix *= glm::rotate(-_rotation.y, 0.0f, 1.0f, 0.0f);
			m_sunViewMatrix *= glm::translate(-m_sunLocation);

			m_sunViewProjectioMatrix = m_sunProjectionMatrix * m_sunViewMatrix;
		}

		const glm::vec3 &Renderer::getSunLocation() const{
			return m_sunLocation;
		}

		const glm::mat4 &Renderer::getSunViewMatrix() const{
			return m_sunViewMatrix;
		}

		const glm::mat4 &Renderer::getSunProjectionMatrix() const{
			return m_sunProjectionMatrix;
		}

		const glm::mat4 &Renderer::getSunViewProjectionMatrix() const{
			return m_sunViewProjectioMatrix;
		}


		Buffers::Framebuffer &Renderer::getGFramebuffer(){
			return m_gFramebuffer;
		}

		Buffers::Framebuffer &Renderer::getPaintDepthFramebuffer(){
			return m_paintDepthFramebuffer;
		}

		Buffers::Framebuffer &Renderer::getShadowFramebuffer(){
			return m_shadowFramebuffer;
		}

		Buffers::Framebuffer &Renderer::getOccludedFramebuffer(){
			return m_occludedFramebuffer;
		}

		Buffers::Framebuffer &Renderer::getFinalFramebuffer(){
			return m_finalFramebuffer;
		}


		//GFRAMEBUFFER
		const unsigned int &Renderer::getDepthTexture() const{
			return m_gFramebuffer.getDepthTexture();
		}

		const unsigned int &Renderer::getLocationTexture() const{
			return m_gFramebuffer.getTexture(GBUFFER_LOCATION);
		}

		const unsigned int &Renderer::getNormalTexture() const{
			return m_gFramebuffer.getTexture(GBUFFER_NORMAL);
		}

		const unsigned int &Renderer::getAlbedoTexture() const{
			return m_gFramebuffer.getTexture(GBUFFER_ALBEDO);
		}

		const unsigned int &Renderer::getVelocityTexture() const{
			return m_gFramebuffer.getTexture(GBUFFER_VELOCITY);
		}

		const unsigned int &Renderer::getRoughMetalOccluEmissTexture() const{
			return m_gFramebuffer.getTexture(GBUFFER_RMOE);
		}


		//SHADOW FRAMEBUFFER
		const unsigned int &Renderer::getShadowTexture() const{
			return m_shadowFramebuffer.getTexture(0);
		}


		//OCCLUDED LIGHT FRAMEBUFFER
		//EMPTY

		unsigned int Renderer::getScreenQuadVertexArray() const{
			return m_screenQuadVertexArray;
		}


		unsigned int Renderer::getPaintDepthTexture() const{
			return m_paintDepthFramebuffer.getTexture(0);
		}


	}
}
