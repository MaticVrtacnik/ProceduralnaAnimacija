#ifndef MATRIX_COMPONENT_HPP
#define MATRIX_COMPONENT_HPP

#include "Component.hpp"
#include "../TransformVariables.hpp"


namespace Engine{
	namespace Entities{
		namespace Components{

			class MatrixComponent : public Component{
			private: 
				bool m_updated = false;

			public: 
				bool m_cameraTransform = true;

			private:
				glm::mat4	m_matrix			= glm::mat4(1.0f);
				glm::mat4	m_previousMatrix	= glm::mat4(1.0f);

				Location	m_rotationCenter	= Location(0.0f);

				Location	m_startLocation		= Location(0.0f);
				Location	m_location			= Location(0.0f);
				Location	m_previousLocation	= Location(0.0f);

				Rotation	m_startRotation		= Rotation(0.0f);
				Rotation	m_rotation			= Rotation(0.0f);
				Rotation	m_previousRotation	= Rotation(0.0f);

				Scale		m_startScale		= Scale(1.0f);
				Scale		m_scale				= Scale(1.0f);
				Scale		m_previousScale		= Scale(1.0f);

			private:
				void setMatrix();

			public:
				MatrixComponent();

				MatrixComponent(const Location &location);

				MatrixComponent(const Rotation &rotation);

				MatrixComponent(const Scale &scale);

				MatrixComponent(const Location &location, const Rotation &rotation);

				MatrixComponent(const Location &location, const Scale &scale);

				MatrixComponent(const Rotation &rotation, const Scale &scale);

				MatrixComponent(const Location &location, const Rotation &rotation, const Scale &scale);

				/*
				//TODO maybe not used
				MatrixComponent(Location &&location);

				MatrixComponent(Rotation &&rotation);

				MatrixComponent(Scale &&scale);

				MatrixComponent(Location &&location, Rotation &&rotation);

				MatrixComponent(Location &&location, Scale &&scale);

				MatrixComponent(Rotation &&rotation, Scale &&scale);

				MatrixComponent(Location &&location, Rotation &&rotation, Scale &&scale);
				*/


				const glm::mat4 &getMatrix();

				const glm::mat4 &getPreviousMatrix();


				void resetTransforms();

				void updateStartTransforms();


				void resetRotationCenter();

				const Location &getRotationCenter() const;

				void setRotationCenter(const Location &center);

				void setRotationCenter(Location &&center);

				void setRotationCenter(float x, float y, float z);

		
				const Location &getLocation() const;

				const Location &getStartLocation() const;

				const Location &getPreviousLocation() const;

				const float getDistanceFromOrigin() const;

				void setLocation(const Location &location);

				void setLocation(Location &&location);

				void setLocation(float x, float y, float z);

				void setLocationX(float x);

				void setLocationY(float y);

				void setLocationZ(float z);


				const Rotation &getRotation() const;

				const Rotation &getStartRotation() const;

				const Rotation &getPreviousRotation() const;

				void setRotation(const Rotation &rotation);

				void setRotation(Rotation &&rotation);

				void setRotation(const glm::vec3 &angles);

				void setRotation(glm::vec3 &&angles);

				void setRotation(float x, float y, float z);


				const Scale &getScale() const;

				const Scale &getStartScale() const;

				const Scale &getPreviousScale() const;

				void setScale(const Scale &scale);

				void setScale(Scale &&scale);

				void setScale(float x, float y, float z);

				void setScaleX(float x);

				void setScaleY(float y);

				void setScaleZ(float z);


				void increaseLocation(const Location &change);

				void increaseLocation(Location &&location);

				void increaseLocation(float x, float y, float z);

				void increaseLocationX(float x);

				void increaseLocationY(float y);

				void increaseLocationZ(float z);


				void increaseRotation(const Rotation &change);

				void increaseRotation(Rotation &&change);

				void increaseRotation(float x, float y, float z);

				void increaseRotationX(float angle);

				void increaseRotationY(float angle);

				void increaseRotationZ(float angle);


				void increaseScale(const Scale &change);

				void increaseScale(Scale &&change);

				void increaseScale(float x, float y, float z);

				void increaseScaleX(float x);

				void increaseScaleY(float y);

				void increaseScaleZ(float z);

			};

		}
	}
}

#endif //MATRIX_COMPONENT_HPP