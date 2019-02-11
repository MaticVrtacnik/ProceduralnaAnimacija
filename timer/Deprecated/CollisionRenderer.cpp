#include "CollisionRenderer.hpp"

#include "../Define/Defines.hpp"
#include "../Define/Assert.hpp"
#include "../Utility/CollisionUtilities.hpp"
#include <glm\gtc\type_ptr.hpp>


namespace Engine{
	namespace Physics{

		CollisionRenderer::CollisionRenderer(){
			m_quadricObject = gluNewQuadric();
		}

		CollisionRenderer::~CollisionRenderer(){
			gluDeleteQuadric(m_quadricObject);
		}


		void CollisionRenderer::drawBody(btRigidBody *body) const{
			if (body == nullptr){
				ASSERT(0, "Body is a null pointer");
				return;
			}

			//Variables required everywhere
			btCollisionShape *_collisionShape = body->getCollisionShape();
			glm::mat4 _transform; 
			body->getWorldTransform().getOpenGLMatrix(glm::value_ptr(_transform));

			switch (_collisionShape->getShapeType())
			{
				case BOX_SHAPE_PROXYTYPE: renderCube(_collisionShape, _transform); break;					
				case STATIC_PLANE_PROXYTYPE: renderPlane(_collisionShape, _transform); break;		
				case SPHERE_SHAPE_PROXYTYPE: renderSphere(_collisionShape, _transform); break;					
				case CAPSULE_SHAPE_PROXYTYPE: renderCapsule(_collisionShape, _transform); break;				
				case TRIANGLE_MESH_SHAPE_PROXYTYPE: drawMesh(_collisionShape, _transform); break;
				case CONVEX_HULL_SHAPE_PROXYTYPE: renderConvexHull(_collisionShape, _transform); break;
				default: break;
			}
		}

		void CollisionRenderer::renderCube(btCollisionShape *collisionShape, 
			const glm::mat4 &transform) const
		{
			if (collisionShape->getShapeType() != BOX_SHAPE_PROXYTYPE){
				ASSERT(0, "Collision shape is not a cube");
				return;
			}

			glPushMatrix();		
			glMultMatrixf(glm::value_ptr(transform));

			glBegin(GL_QUADS);
			{
				glm::vec3 _extents(Physics::convertVec3(static_cast<btBoxShape*>
					(collisionShape)->getHalfExtentsWithoutMargin()));

				glVertex3f(-_extents.x, -_extents.y, _extents.z);
				glVertex3f(-_extents.x, _extents.y, _extents.z);
				glVertex3f(_extents.x, _extents.y, _extents.z);
				glVertex3f(_extents.x, -_extents.y, _extents.z);

				glVertex3f(-_extents.x, -_extents.y, -_extents.z);
				glVertex3f(-_extents.x, _extents.y, -_extents.z);
				glVertex3f(_extents.x, _extents.y, -_extents.z);
				glVertex3f(_extents.x, -_extents.y, -_extents.z);

				glVertex3f(_extents.x, -_extents.y, -_extents.z);
				glVertex3f(_extents.x, _extents.y, -_extents.z);
				glVertex3f(_extents.x, _extents.y, _extents.z);
				glVertex3f(_extents.x, -_extents.y, _extents.z);

				glVertex3f(-_extents.x, -_extents.y, -_extents.z);
				glVertex3f(-_extents.x, _extents.y, -_extents.z);
				glVertex3f(-_extents.x, _extents.y, _extents.z);
				glVertex3f(-_extents.x, -_extents.y, _extents.z);

				glVertex3f(-_extents.x, _extents.y, _extents.z);
				glVertex3f(-_extents.x, _extents.y, -_extents.z);
				glVertex3f(_extents.x, _extents.y, -_extents.z);
				glVertex3f(_extents.x, _extents.y, _extents.z);

				glVertex3f(-_extents.x, -_extents.y, _extents.z);
				glVertex3f(-_extents.x, -_extents.y, -_extents.z);
				glVertex3f(_extents.x, -_extents.y, -_extents.z);
				glVertex3f(_extents.x, -_extents.y, _extents.z);
			}
			glEnd();

			glPopMatrix();
		}

		void CollisionRenderer::renderPlane(btCollisionShape *collisionShape, 		
			const glm::mat4 &transform) const
		{
			if (collisionShape->getShapeType() != STATIC_PLANE_PROXYTYPE){
				ASSERT(0, "Collision shape is not a plane");
				return;
			}

			glPushMatrix();
			glMultMatrixf(glm::value_ptr(transform));

			glBegin(GL_QUADS);
			{
				glVertex3f(-WORLD_SIZE, 0.0f, WORLD_SIZE);
				glVertex3f(-WORLD_SIZE, 0.0f, -WORLD_SIZE);
				glVertex3f(WORLD_SIZE, 0.0f, -WORLD_SIZE);
				glVertex3f(WORLD_SIZE, 0.0f, WORLD_SIZE);
			}	
			glEnd();

			glPopMatrix();
		}

		void CollisionRenderer::renderSphere(btCollisionShape *collisionShape,
			const glm::mat4 &transform) const
		{
			if (collisionShape->getShapeType() != SPHERE_SHAPE_PROXYTYPE){
				ASSERT(0, "Collision shape is not a sphere");
				return;
			}

			glPushMatrix();
			glMultMatrixf(glm::value_ptr(transform));

			float _radius = static_cast<btSphereShape*>(collisionShape)->getRadius();
			gluSphere(m_quadricObject, _radius, 20, 20);

			glPopMatrix();
		}

		void CollisionRenderer::renderCapsule(btCollisionShape *collisionShape, 
			const glm::mat4 &transform) const
		{
			glPushMatrix();
			glMultMatrixf(glm::value_ptr(transform));

			btCapsuleShape *_capsule = static_cast<btCapsuleShape*>(collisionShape);
			const float _radius = _capsule->getRadius();
			const float _sphereHeightScale = _capsule->getHalfHeight() / _radius;

			glScalef(1.0f, 2 * _sphereHeightScale, 1.0f);
			gluSphere(m_quadricObject, _radius, 20, 20);

			glPopMatrix();
		}

		void CollisionRenderer::renderConvexHull(btCollisionShape *collisionShape,
			const glm::mat4 &transform) const
		{
			/*if (collisionShape->getShapeType() != SPHERE_SHAPE_PROXYTYPE){
				ASSERT(0, "Collision shape is not a sphere");
				return;
			}*/

			glPushMatrix();
			glMultMatrixf(glm::value_ptr(transform));

			auto _convexHullShape = static_cast<btConvexHullShape*>(collisionShape);
			glm::mat2x3 _boundingBox = Physics::getBoundingBox(_convexHullShape);

			glBegin(GL_POINTS);

			auto _points = _convexHullShape->getPoints();
			auto _numPoints = _convexHullShape->getNumPoints();
			for (int i = 0; i < _numPoints; ++i){
				auto &_p = _points[i];

				glVertex3f(_p.getX(), _p.getY(), _p.getZ());
			}
			glEnd();

			auto _poly = _convexHullShape->getConvexPolyhedron();
			

			float _radius = glm::length(_boundingBox[1] - _boundingBox[0]) * 0.5f;
			//gluSphere(m_quadricObject, _radius, 20, 20);
			//PRINT(_radius);

			glPopMatrix();
		}

		void CollisionRenderer::drawMesh(btCollisionShape *collisionShape,
			const glm::mat4 &transform) const
		{
			ASSERT(0, "TODO IMPLEMENT");
			//TODO
			//btTriangleMeshShape *shape = body->getCollisionShape();
		}

	}
}
