#ifndef MOTION_BLUR_HPP
#define MOTION_BLUR_HPP

#include "PostProcessingBase.hpp"


namespace Engine{
	namespace Graphics{
		namespace PostProcessing{

			class MotionBlur : public PostProcessingBase{
			private:

			public:
				MotionBlur(std::shared_ptr<Shaders::Shader> shader, 
					unsigned int width, unsigned int height);

				~MotionBlur();


				unsigned int useEffect(float fps, unsigned int textureId,
					unsigned int velocityTextureId);

			};

		}
	}
}

#endif //MOTION_BLUR_HPP