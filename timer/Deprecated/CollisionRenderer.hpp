#ifndef COLLISION_RENDERER_HPP
#define COLLISION_RENDERER_HPP

#include <btBulletDynamicsCommon.h>
#include <GL\GLU.H>
#include <glm\glm.hpp>


namespace Engine{
	namespace Physics{

		class CollisionRenderer{
		private:
			GLUquadric *m_quadricObject;

		public:
			CollisionRenderer();

			~CollisionRenderer();


			void drawBody(btRigidBody *body) const;

			void renderCube(btCollisionShape *collisionShape, const glm::mat4 &transform) const;

			void renderPlane(btCollisionShape *collisionShape, const glm::mat4 &transform) const;

			void renderSphere(btCollisionShape *collisionShape, const glm::mat4 &transform) const;

			void renderCapsule(btCollisionShape *collisionShape,const glm::mat4 &transform) const;

			void renderConvexHull(btCollisionShape *collisionShape, const glm::mat4 &transform) const;

			void drawMesh(btCollisionShape *collisionShape, const glm::mat4 &transform) const;

		};
	}
}

#endif //COLLISION_RENDERER_HPP