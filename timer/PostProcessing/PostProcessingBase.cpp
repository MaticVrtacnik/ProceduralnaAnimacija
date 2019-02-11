#include "PostProcessingBase.hpp"

#include "../Shader/Shader.hpp"
#include "../Define/Assert.hpp"


namespace Engine{
	namespace Graphics{
		namespace PostProcessing{

			PostProcessingBase::PostProcessingBase(std::shared_ptr<Shaders::Shader> shaderPtr,
				unsigned int textureWidth, unsigned int textureHeight, bool hasDepth
			) :
				m_shader(shaderPtr),
				m_textureWidth(textureWidth), m_textureHeight(textureHeight)
			{
				m_framebuffer.init(textureWidth, textureHeight, hasDepth);

				glGenQueries(1, &m_timerStartQuery);
				glGenQueries(1, &m_timerEndQuery);
			}

			PostProcessingBase::PostProcessingBase(const PostProcessingBase &object) :
				m_framebuffer(object.m_framebuffer),
				m_shader(object.m_shader),
				m_textureWidth(object.m_textureWidth),
				m_textureHeight(object.m_textureHeight),
				m_timerStartQuery(object.m_timerStartQuery),
				m_timerEndQuery(object.m_timerEndQuery)
			{
			}

			PostProcessingBase::PostProcessingBase(PostProcessingBase &&object) :
				m_framebuffer(std::move(object.m_framebuffer)),
				m_shader(std::move(object.m_shader)),
				m_textureWidth(std::move(object.m_textureWidth)),
				m_textureHeight(std::move(object.m_textureHeight)),
				m_timerStartQuery(std::move(object.m_timerStartQuery)),
				m_timerEndQuery(std::move(object.m_timerEndQuery))
			{
			}

			PostProcessingBase::~PostProcessingBase(){
				glDeleteQueries(1, &m_timerStartQuery);
				glDeleteQueries(1, &m_timerEndQuery);
			}


			unsigned int PostProcessingBase::useEffect(){
				glViewport(0, 0, m_textureWidth, m_textureHeight);

				m_shader->bind();
				m_framebuffer.bind();

				if (!m_framebuffer.hasDepth()) glClear(GL_COLOR_BUFFER_BIT);
				else glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				return 0; //Default Value -> NOT USED
			}

			const unsigned int &PostProcessingBase::getResult(const std::size_t &index) const{
				ASSERT(m_framebuffer.getTextures().size() > index, 
					"Framebuffer texture index out of range");

				return m_framebuffer.getTexture(index);
			}


			void PostProcessingBase::startTimer(){
				glQueryCounter(m_timerStartQuery, GL_TIMESTAMP);
			}

			void PostProcessingBase::stopTimer(){
				glQueryCounter(m_timerEndQuery, GL_TIMESTAMP);
			}

			double PostProcessingBase::getEffectDuration() const{
				GLuint64 _gpuStartTime, _gpuEndTime;
				glGetQueryObjectui64v(m_timerStartQuery, GL_QUERY_RESULT, &_gpuStartTime);
				glGetQueryObjectui64v(m_timerEndQuery, GL_QUERY_RESULT, &_gpuEndTime);

				return (_gpuEndTime - _gpuStartTime) / 1000000.0;
			}


			const unsigned int &PostProcessingBase::getTextureWidth() const{
				return m_textureWidth;
			}

			const unsigned int &PostProcessingBase::getTextureHeight() const{
				return m_textureHeight;
			}

		}
	}
}
