#ifndef OIL_PAINT_HPP
#define OIL_PAINT_HPP

#include "../PostProcessingBase.hpp"


namespace Engine{
	namespace Graphics{
		namespace PostProcessing{

			class OilPaint : public PostProcessingBase{
			private:

			public:
				OilPaint(std::shared_ptr<Shaders::Shader> shader, 
					unsigned int width, unsigned int height);

				~OilPaint();


				unsigned int useEffect(unsigned int textureId, float radius);

			};

		}
	}
}

#endif //ifndef OIL_PAINT_HPP