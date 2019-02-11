#ifndef SIMPLE_SHAPES_HPP
#define SIMPLE_SHAPES_HPP

#include <memory>


namespace Engine{ namespace Meshes{
	struct Mesh;
} }


namespace Engine{
	namespace Meshes{

		std::unique_ptr<Mesh> getTriangle(unsigned int texture, float scale);

		std::unique_ptr<Mesh> getQuad(unsigned int texture, float scale);

		std::unique_ptr<Mesh> getCircle(unsigned int texture, float scale, float slices);

		std::unique_ptr<Mesh> getCube(unsigned int texture, float scale);

		std::unique_ptr<Mesh> getCube(unsigned int texture, float scaleX, float scaleY, float scaleZ);

		std::unique_ptr<Mesh> getSphere(unsigned int texture, float scale, float slices, float stacks);

	}
}

#endif //SIMPLE_SHAPES_HPP