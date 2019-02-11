#ifndef LINEARIZE_DEPTH_HPP
#define LINEARIZE_DEPTH_HPP

#include "PostProcessingBase.hpp"


namespace Engine{
	namespace Graphics{
		namespace PostProcessing{


			class LinearizeDepth : public PostProcessingBase{
			private:

			public:
				LinearizeDepth(std::shared_ptr<Shaders::Shader> shader, 
					unsigned int width, unsigned int height);

				~LinearizeDepth();


				unsigned int useEffect(unsigned int depthTexture);

			};

		}
	}
}

#endif //LINEARIZE_DEPTH_HPP