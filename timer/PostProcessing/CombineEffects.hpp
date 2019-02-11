#ifndef COMBINE_EFFECTS_HPP
#define COMBINE_EFFECTS_HPP

#include <glm\glm.hpp>

#include "PostProcessingBase.hpp"


namespace Engine{
	namespace Graphics{
		namespace PostProcessing{

			class CombineEffects : public PostProcessingBase{
			private:

			public:
				CombineEffects(std::shared_ptr<Shaders::Shader> shader, 
					unsigned int width, unsigned int height);

				~CombineEffects();


				unsigned int useEffect(const glm::mat4 &viewMatrix, unsigned int screenTextureId,
					unsigned int lensFlareTextureId, unsigned int lightScatteringTextureId,
					unsigned int sunLightScatteringTextureId);
				
			};

		}
	}
}

#endif //COMBINE_EFFECTS_HPP