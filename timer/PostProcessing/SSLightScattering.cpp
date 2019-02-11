#include "SSLightScattering.hpp"

#include "../Shader/Shader.hpp"
#include "../Utility/PrintUtilities.hpp"
#include "../Utility/LogicUtilities.hpp"
#include "../Utility/MathUtilities.hpp"
#include "../Define/PrintDefines.hpp"


namespace Engine{
	namespace Graphics{
		namespace PostProcessing{

			SSLightScattering::SSLightScattering(
				std::shared_ptr<Shaders::Shader> shader,
				std::shared_ptr<Shaders::Shader> gaussianBlurShader, 
				unsigned int width, unsigned int height
			) :
				PostProcessingBase(shader, width / 2, height / 2, false),
				m_blur(gaussianBlurShader, width, height, GL_RGB16F, GL_RGB)
			{
				m_framebuffer.addTexture(Textures::createEmptyTexture(
					width / 2, height / 2, false, GL_RGB16F, GL_LINEAR));
				//TODO possibly change to 8bit channel RGB
			}

			SSLightScattering::~SSLightScattering(){

			}


			unsigned int SSLightScattering::useEffect(const glm::mat4 &viewMatrix,
				const glm::mat4 &projectionMatrix, const glm::vec3 &lightLocation,
				const glm::vec3 &lightScale, float decay, float density, float weight,
				unsigned int occludedLightTextureId)
			{
				PostProcessingBase::useEffect();

				glm::mat4 _viewProjectionMatrix(projectionMatrix * viewMatrix);
				//glm::vec3 _cameraRight(viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0]);
				//glm::vec3 _cameraUp(viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1]);

				glm::vec3 _cameraRight(viewMatrix[0]);
				glm::vec3 _cameraUp(viewMatrix[1]);

				

				glm::vec2 _screenLightLocation = Utilities::getScreenLocation(
					lightLocation, _viewProjectionMatrix);
				glm::vec2 _nScreenLightLocation = _screenLightLocation * 2.0f - 1.0f;

				//glm::vec2 _scaleFactors = -glm::sign(_nScreenLightLocation);
				glm::vec2 _scaleFactors = -glm::normalize(_screenLightLocation);
				glm::vec3 _scaleoffset = glm::vec3(lightScale.x * _scaleFactors.x,
					lightScale.y * _scaleFactors.y, 1.0f);


				glm::vec3 _edgeLocation = lightLocation;
				_edgeLocation += _cameraRight * lightScale.x * _scaleFactors.x;
				_edgeLocation += _cameraUp * lightScale.y * _scaleFactors.y;

				//TODO use actual screenspace edge maybe
				glm::vec2 _screenLightEdge = Utilities::getScreenLocation(
					_edgeLocation, _viewProjectionMatrix);
				_screenLightEdge = _screenLightEdge * 2.0f - 1.0f;

				//Utilities::print(_screenLightEdge);

				//Sun scattering falloff
				//TODO remove hardcoded
				const float _screenBorder = 1.0f;
				const float _lightBorderScale = 0.3f;

				//Utilities::print(_nScreenLightLocation);
				
				bool _insideX = Utilities::insideBorderScale(_nScreenLightLocation.x,
					_screenBorder, _lightBorderScale);
				bool _insideY = Utilities::insideBorderScale(_nScreenLightLocation.y,
					_screenBorder, _lightBorderScale);

				if (_insideX || _insideY){			
					glm::vec2 _absNScreenLightLocation = glm::abs(_nScreenLightLocation) - _screenBorder;

					float _screenFactor = glm::length(glm::vec2(
						(_absNScreenLightLocation.x / _lightBorderScale) * _insideX,
						(_absNScreenLightLocation.y / _lightBorderScale) * _insideY
					));

					float _maxLength = glm::length(glm::vec2(_insideX, _insideY));
					float _falloff = 1.0 - (_screenFactor / _maxLength);

					float _offsetX = glm::pow(glm::abs(_nScreenLightLocation.x) + 0.3f, 1.1f) * _lightBorderScale;
					_falloff = glm::smoothstep(_lightBorderScale + _offsetX, _screenBorder, _falloff);

					weight *= _falloff;

					//PRINT("INSIDE " << _falloff);
					//Utilities::print(_screenLightLocation);
				}

				unsigned int _blurredEffect = 0; 
				startTimer();
				{
					m_shader->setUniform("u_screenLightLocation", _screenLightLocation);
					m_shader->setUniform("u_decay", decay);
					m_shader->setUniform("u_density", density);
					m_shader->setUniform("u_weight", weight);

					Textures::bind2DTexture(occludedLightTextureId, 0);
					glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

					_blurredEffect = m_blur.useEffect(m_framebuffer.getTexture(0), 2, 1);
				}
				stopTimer();		


				return _blurredEffect;
			}

		}
	}
}
