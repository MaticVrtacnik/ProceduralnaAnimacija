#ifndef FINALIZE_SYSTEM_HPP
#define FINALIZE_SYSTEM_HPP

#include <glm/glm.hpp>
#include "System.hpp"
#include "../Buffer/ArrayBuffer.hpp"


namespace Engine{ namespace Graphics{ namespace PostProcessing{
	class HDR;
	class GaussianBlur;
	class TextureSizeChanger;
	class MotionBlur;
	class SAO;
	class SSLightScattering;
	class CombineEffects;
	class CombineTextures;
} } }


namespace Engine{
	namespace Entities{
		namespace Systems{
			using namespace Graphics::PostProcessing;

			class FinalizeSystem : public System{
			private:
				double m_lightingTimer = 0.0, m_bloomTimer = 0.0;
				unsigned int m_timerStartQuery = 0;
				unsigned int m_timerEndQuery = 0;

			private:
				unsigned int m_screenTexture = 0;
				std::vector<unsigned int> m_bloomTextures;

				unsigned int m_screenQuadVertexArray = 0;
				Buffers::ArrayBuffer m_screenQuadBuffer;

			private:
				Entity getSunEntity();

			public:
				std::unique_ptr<Graphics::PostProcessing::HDR> m_HDR;
				std::unique_ptr<Graphics::PostProcessing::GaussianBlur> m_gaussianBlur;
				std::unique_ptr<Graphics::PostProcessing::GaussianBlur> m_shadowBlur;
				std::unique_ptr<Graphics::PostProcessing::TextureSizeChanger> m_textureSizeChanger;
				std::unique_ptr<Graphics::PostProcessing::MotionBlur> m_motionBlur;
				std::unique_ptr<Graphics::PostProcessing::SAO> m_SAO;
				std::unique_ptr<Graphics::PostProcessing::SSLightScattering> m_sunScattering;
				std::unique_ptr<Graphics::PostProcessing::SSLightScattering> m_lightScattering;
				std::unique_ptr<Graphics::PostProcessing::CombineEffects> m_combineEffects;
				std::unique_ptr<Graphics::PostProcessing::CombineTextures> m_combineTextures;
				std::unique_ptr<Graphics::PostProcessing::CombineTextures> m_combineReflections;

			public:
				FinalizeSystem();

				~FinalizeSystem();


				void init() override;

				void setLightUniforms();


				void preUpdate(float fps) override;

				void update(float fps) override;

				void postUpdate(float fps) override;


				void renderDeferred();

				const std::vector<unsigned int> &getBloomTextures() const;

				const unsigned int &getScreenTexture() const;

			};

		}
	}
}

#endif //FINALIZE_SYSTEM_HPP