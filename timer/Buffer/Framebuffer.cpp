#include "Framebuffer.hpp"

#include <gl/glew.h>

#include "../Define/Assert.hpp"
#include "../Texture/Texture.hpp"


namespace Engine{
	namespace Buffers{

		Framebuffer::Framebuffer(){
		
		}

		Framebuffer::~Framebuffer(){
			if (m_depthTexture != 0){
				glDeleteTextures(1, &m_depthTexture);
			}

			glDeleteFramebuffers(1, &m_id);
		}


		void Framebuffer::bind(){
			glBindFramebuffer(GL_FRAMEBUFFER, m_id);
		}

		void Framebuffer::unbind(){
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}


		void Framebuffer::init(){
			m_hasDepth = false;
			m_depthTexture = 0;
			glGenFramebuffers(1, &m_id);
		}

		void Framebuffer::init(int width, int height, bool hasDepth, bool hasStencil){
			m_hasDepth = hasDepth;

			if (hasStencil){
				glGetError();

				PRINT("STENCIL");
				m_depthTexture = Textures::createEmptyTexture(width, height, false, 
					GL_DEPTH24_STENCIL8, GL_LINEAR, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8);

				GLenum _error = glGetError();
				if (_error){
					std::cout << "FBO texture loading error : '" <<
						gluErrorString(_error) << "'" << std::endl;
				}

			} else if (hasDepth){
				m_depthTexture = Textures::createEmptyTexture(
					width, height, true, GL_DEPTH_COMPONENT32, GL_LINEAR);
			} else{
				m_depthTexture = NULL;
			}


			glGenFramebuffers(1, &m_id);
			glBindFramebuffer(GL_FRAMEBUFFER, m_id);

			glFramebufferTexture2D(GL_FRAMEBUFFER,
				GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);


			if (hasStencil){
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
					GL_TEXTURE_2D, m_depthTexture, 0);

				GLenum _error = glGetError();
				if (_error){
					std::cout << "FBO texture loading error : '" <<
						gluErrorString(_error) << "'" << std::endl;
					PRINT("---------------------------------");
				}

			} else{
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
					GL_TEXTURE_2D, m_depthTexture, 0);
			}


			//ASSERT(checkErrors(), "Framebuffer not complete");
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		bool Framebuffer::checkErrors(){ // returns false if error occured
			int _framebufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
			bool _error = _framebufferStatus != GL_FRAMEBUFFER_COMPLETE;
			if (_error) std::cout << "FrameBuffer error: " << _framebufferStatus << std::endl;

			return !_error;
		}

		
		void Framebuffer::addTexture(unsigned int texture){
			GLint maxColorAttachments;
			glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxColorAttachments);
			ASSERT(static_cast<int>(m_textures.size()) < maxColorAttachments,
				"Exceeded max color attachments");

			bind();
			glFramebufferTexture2D(GL_FRAMEBUFFER,
				GL_COLOR_ATTACHMENT0 + m_textures.size(), GL_TEXTURE_2D, texture, 0);
			unbind();

			m_textures.push_back(texture);
		}

		const unsigned int &Framebuffer::getTexture(std::size_t id) const{
			ASSERT(id < m_textures.size(), "Texture id out of range");

			return m_textures[id];
		}

		const std::vector<unsigned int> &Framebuffer::getTextures() const{
			return m_textures;
		}

		void Framebuffer::setTexture(unsigned int texture, std::size_t id){
			ASSERT(id < m_textures.size(), "Texture id out of range");
			glFramebufferTexture2D(GL_FRAMEBUFFER,
				GL_COLOR_ATTACHMENT0 + id, GL_TEXTURE_2D, texture, 0);

			m_textures[id] = texture;
		}


		const unsigned int &Framebuffer::getDepthTexture() const{
			return m_depthTexture;
		}

		bool Framebuffer::hasDepth() const{
			return m_hasDepth;
		}

		void Framebuffer::setDepthTexture(unsigned int texture, bool deleteOld){
			if (deleteOld) glDeleteTextures(1, &m_depthTexture);
			m_depthTexture = texture;

			glFramebufferTexture2D(GL_FRAMEBUFFER,
				GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
		}


		//if num == 0 then all attachments will be set
		void Framebuffer::setDrawBuffers(std::size_t num){
			std::vector<GLenum> _attachments;
			std::size_t _numAttachments = num == 0 ? m_textures.size() : num;

			_attachments.reserve(_numAttachments);
			for (std::size_t i = 0; i < _numAttachments; ++i){
				_attachments.push_back(GL_COLOR_ATTACHMENT0 + i);
			}

			//TODO render buffer depth attachment 
			glDrawBuffers(_attachments.size(), _attachments.data());
		}

		void Framebuffer::setDrawBuffers(const std::vector<unsigned int> &buffers){
			ASSERT(!buffers.empty(), "No color attachments in drawBuffers");

			std::vector<GLenum> _attachments;
			_attachments.reserve(buffers.size());

			for (std::size_t i = 0; i < buffers.size(); ++i){
				_attachments.push_back(GL_COLOR_ATTACHMENT0 + buffers[i]);
			}

			//TODO render buffer depth attachment 
			glDrawBuffers(_attachments.size(), _attachments.data());
		}

	}
}
