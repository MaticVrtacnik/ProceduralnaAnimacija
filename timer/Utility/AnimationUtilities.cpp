/*
MIT License

Copyright (c) 2019 MaticVrtacnik

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/


#include "AnimationUtilities.hpp"

#include <glm\gtx\transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "../Animation/AnimationSkeleton.hpp"
#include "../Component/CollisionComponent.hpp"
#include "../Component/MatrixComponent.hpp"
#include "../Component/PlayerControllerComponent.hpp"
#include "../Component/SkeletalAnimationComponent.hpp"
#include "../Entity/Entity.hpp"
#include "../Utility/CollisionUtilities.hpp"
#include "../Utility/PrintUtilities.hpp"
#include "../World/World.hpp"


namespace Engine{
	namespace Utilities{
		using namespace Entities;
		using namespace Components;


		glm::mat4 convertMat4(const aiMatrix4x4 &matrix){
			return glm::mat4(
				matrix.a1, matrix.b1, matrix.c1, matrix.d1,
				matrix.a2, matrix.b2, matrix.c2, matrix.d2,
				matrix.a3, matrix.b3, matrix.c3, matrix.d3,
				matrix.a4, matrix.b4, matrix.c4, matrix.d4
			);
		}



		glm::mat4 correctTransformForRotatedBody(
			const glm::mat4 &transform, unsigned int boneId)
		{
			//TODO remove hardcoded arms 

			//return transform;


			if (boneId >= 3 && boneId < 6){
				return transform * glm::rotate(90.0f, 0.0f, 0.0f, 1.0f);
			}else if (boneId >= 6 && boneId < 9){
				return transform * glm::rotate(-90.0f, 0.0f, 0.0f, 1.0f);
			}else{
				return transform;
			}
		}


		void correctFinalTransforms(const Animation::AnimationSkeleton &skeleton,
			std::vector<glm::mat4> *transforms)
		{
			for (int numTransforms = transforms->size(), i = 0; i < numTransforms; ++i){
				auto &_transform = (*transforms)[i];

				_transform = correctTransformForRotatedBody(_transform, i);
			}
		}




		glm::mat4 setCollisionBodyTransformFromBone(
			const std::string &key, Entity &skeletonEntity)
		{
			auto &_matrixComponent = skeletonEntity.getComponent<MatrixComponent>();
			auto &_collisionComponent = skeletonEntity.getComponent<CollisionComponent>();
			auto &_animationComponent = skeletonEntity.getComponent<SkeletalAnimationComponent>();

			auto &_skeleton = _animationComponent.getSkeleton();
			auto &_boneMap = _skeleton.m_boneMap;

			//Unable to find desired bone
			if (_boneMap.find(key) == _boneMap.end()){
				PRINT("Unable to find bone '" << key << "'");
				return glm::mat4(1.0f);
			}


			unsigned int _boneId = _boneMap[key];
			auto &_ragdollOffset = _collisionComponent.getRagdollBoundingBoxes()[_boneId];
			glm::vec3 _location = (_ragdollOffset[0] + _ragdollOffset[1]) * 0.5f;

			glm::mat4 _collisionTransform = _matrixComponent.getMatrix() *
				_animationComponent.getOffsetTransforms()[_boneId] * glm::translate(_location);

			_collisionTransform = correctTransformForRotatedBody(_collisionTransform, _boneId);
		

			btTransform _bodyTransform;
			btScalar *_m = glm::value_ptr(_collisionTransform);
			_bodyTransform.setFromOpenGLMatrix(_m);
			_collisionComponent.getBody(_boneId)->setWorldTransform(_bodyTransform);

			return _collisionTransform;
		}

		/*glm::mat4 setCollisionBodyTransformFromBone(
			const std::string &key, Entity &skeletonEntity,
			const Location &location, const Rotation &rotation, const Scale &scale)
		{
			//TODO maybe
		}*/


		glm::mat4 getBoneTransformFromCollisionBody(const std::string &key,
			Entities::Entity &skeletonEntity, const btRigidBody &collisionBody)
		{
			glm::mat4 _transform(1.0f);
			collisionBody.getWorldTransform().getOpenGLMatrix(glm::value_ptr(_transform));

			//_transform = _transform * glm::scale(glm::vec3(0.05f));
			//_transform = _transform * glm::rotate(-90.0f, 1.0f, 0.0f, 0.0f);

			//_transform = glm::mat4(glm::transpose(glm::inverse(glm::mat3(_transform))));

			//Utilities::print(_transform[3]);

			auto &_matrixComponent = skeletonEntity.getComponent<MatrixComponent>();
			auto &_playerComponent = skeletonEntity.getComponent<PlayerControllerComponent>();

			btVector3 _min, _max;
			_playerComponent.getCollisionShape()->getAabb(btTransform::getIdentity(), _min, _max);
			//collisionBody.getAabb();

			glm::vec3 _location = (Physics::convertVec3(_min + _max)) * 0.5f;
			//_transform *= glm::translate(_location);
			//_transform *= glm::translate(glm::vec3(0.0f, -20.0f, 0.0f));

			//Utilities::print(_location);

			

			/*glm::mat4 _playerTransform(1.0f);
			_playerComponent.getBody()->getWorldTransform().getOpenGLMatrix(glm::value_ptr(_playerTransform));
			glm::mat4 _playerInverseTransform = glm::translate(-_playerComponent.getLocation());*/

			glm::mat4 _boneTransform = glm::inverse(_matrixComponent.getMatrix()) * _transform;
			_boneTransform *= glm::scale(glm::vec3(0.05f));

			//_boneTransform = _playerInverseTransform * _transform;

			return _boneTransform;
		}


		glm::mat4 setBoneTransformFromCollisionBody(
			const std::string &key, Entity &skeletonEntity)
		{
			auto &_matrixComponent = skeletonEntity.getComponent<MatrixComponent>();
			auto &_collisionComponent = skeletonEntity.getComponent<CollisionComponent>();
			auto &_animationComponent = skeletonEntity.getComponent<SkeletalAnimationComponent>();

			auto &_skeleton = _animationComponent.getSkeleton();
			auto &_boneMap = _skeleton.m_boneMap;

			//Unable to find desired bone
			if (_boneMap.find(key) == _boneMap.end()){
				PRINT("Unable to find bone '" << key << "'");
				return glm::mat4(1.0f);
			}


			unsigned int _boneId = _boneMap[key];
			glm::mat4 _transform(1.0f);
			_collisionComponent.getBodies()[_boneId]->getWorldTransform()
				.getOpenGLMatrix(glm::value_ptr(_transform));

			_transform = correctTransformForRotatedBody(_transform, _boneId);


			auto &_ragdollOffset = _collisionComponent.getRagdollBoundingBoxes()[_boneId];
			glm::vec3 _location = (_ragdollOffset[0] + _ragdollOffset[1]) * 0.5f;
			_transform *= glm::translate(-_location);

			//glm::mat4 _inverseOffset = _animationComponent.getSkeleton().m_boneTransforms[i].m_offset			
			//_inverseOffset *= _inverseOffset;


			//TODO UPDATE
			auto _transforms = _animationComponent.getOffsetTransforms();
			_transforms[_boneId] = glm::inverse(_matrixComponent.getMatrix()) * _transform;
			//_animationComponent.setOffsetTransforms(_transforms);
			_animationComponent.updateOffsetTransforms();


			return _transform;
		}

		/*glm::mat4 setBoneTransformFromCollisionBody(
			const std::string &key, Entity &skeletonEntity,
			const Location &location, const Rotation &rotation, const Scale &scale)
		{
			//TODO maybe
		}*/


		int g_c = 0;
		std::vector<glm::mat4> setCollisionBodyTransformsFromBones(Entity &skeletonEntity){
			auto &_matrixComponent = skeletonEntity.getComponent<MatrixComponent>();
			auto &_collisionComponent = skeletonEntity.getComponent<CollisionComponent>();
			auto &_animationComponent = skeletonEntity.getComponent<SkeletalAnimationComponent>();

			//glm::mat4 _modelMatrix = _matrixComponent.getMatrix();
			glm::mat4 _modelMatrix = glm::translate(_matrixComponent.getLocation()) *
				glm::toMat4(_matrixComponent.getRotation());

			std::vector<glm::mat4> _boneTransforms = _animationComponent.getOffsetTransforms();
			auto &_ragdollBoundingBoxes = _collisionComponent.getRagdollBoundingBoxes();

			std::vector<glm::mat4> _collisionBodyTransforms(_boneTransforms.size(), glm::mat4(1.0f));
			for (int i = 0; i < _boneTransforms.size(); ++i)
			{
				auto &_body = _collisionComponent.getBody(i);
				if (!_body->isKinematicObject()){
					continue;
				}

				auto &_ragdollOffset = _ragdollBoundingBoxes[i];
				glm::vec3 _location = (_ragdollOffset[0] + _ragdollOffset[1]) * 0.5f;

				//Utilities::print(_location);

				glm::mat4 _originalOffset = _animationComponent.getSkeleton().m_boneTransforms[i].m_bvhOffset;
				_originalOffset[3] = _originalOffset[3] * glm::vec4(_matrixComponent.getScale(), 1.0f);

				//glm::vec3 _location(_originalOffset[3]);
				//_location = _location * _matrixComponent.getScale();

				//Utilities::print(_animationComponent.getSkeleton().m_boneTransforms[i].m_originalOffset);

				glm::mat4 _boneTransform = _boneTransforms[i];
				glm::mat4 _scaledBoneTransform = glm::mat4(glm::mat3(_boneTransform)); //Rotation only
				_scaledBoneTransform = _boneTransform;

				//Scale translation by matrix scale
				_scaledBoneTransform[3] = _boneTransform[3] * glm::vec4(_matrixComponent.getScale(), 1.0f);

				glm::mat4 _collisionTransform = _modelMatrix *_scaledBoneTransform * glm::translate(_location);
				
				// 
				//_collisionTransform = _modelMatrix * glm::translate(_location);

				//_collisionTransform = correctTransformForRotatedBody(_collisionTransform, i);
				_collisionBodyTransforms[i] = _collisionTransform;


				btTransform _bodyTransform;
				_bodyTransform.setFromOpenGLMatrix(glm::value_ptr(_collisionTransform));
				_body->setWorldTransform(_bodyTransform);

				if (i == 0 && ++g_c > 60){
					btTransform _transform;
					_transform.setFromOpenGLMatrix(glm::value_ptr(_matrixComponent.getMatrix()));

					//Utilities::print(glm::eulerAngles(Physics::convertQuat(_transform.getRotation())));
					//PRINT(_transform.getRotation().getY());
					//Utilities::print(glm::transpose(glm::inverse(_boneTransforms[i])));
					//PRINT("==========================================");

					g_c = 0;
				}

				/*auto _motionState = _body->getMotionState();
				if (_motionState != nullptr){
					_motionState->setWorldTransform(_bodyTransform);
				}*/
			}


			return _collisionBodyTransforms;
		}


		//Ragdoll, dynamic skirt
		std::vector<glm::mat4> setBoneTransformsFromCollisionBodies(Entity &skeletonEntity){
			auto &_matrixComponent = skeletonEntity.getComponent<MatrixComponent>();
			auto &_collisionComponent = skeletonEntity.getComponent<CollisionComponent>();
			auto &_animationComponent = skeletonEntity.getComponent<SkeletalAnimationComponent>();

			auto &_skeleton = _animationComponent.getSkeleton();

			auto &_bodies = _collisionComponent.getBodies();
			auto &_ragdollBoundingBoxes = _collisionComponent.getRagdollBoundingBoxes();
			glm::mat4 _inverseModelMatrix(glm::inverse(_matrixComponent.getMatrix()));
			//glm::mat4 _inverseModelMatrix = _matrixComponent.getMatrix();

			//Location _rootLocation = Physics::convertVec3(_bodies[0]->getWorldTransform().getOrigin());
			//_inverseModelMatrix = _inverseModelMatrix * glm::translate(-_rootLocation);

			auto &_boneTransforms = _animationComponent.getTransforms();
			if (_boneTransforms.size() < _bodies.size()){
				//Invalid transforms size
				return std::vector<glm::mat4>();
			}

			glm::mat4 _originalOffset = _skeleton.m_boneTransforms[0].m_originalOffset;
			glm::vec4 _translation = _boneTransforms[0][3] - _originalOffset[3]; // parent bone
			

			//Utilities::print(_skeleton.m_boneTransforms[0].m_originalOffset);

			for (int i = 0; i < _bodies.size(); ++i)
			{
				auto &_body = _bodies[i];
				if (_body->isKinematicObject()){
					continue;
				}


				glm::mat4 _transform(1.0f);
				_body->getWorldTransform().getOpenGLMatrix(glm::value_ptr(_transform));
				//_transform *= glm::rotate(180.0f, glm::vec3(1.0f, 0.0f, 0.0f));


				//_transform =  _transform;
				//_transform = correctTransformForRotatedBody(_transform, i);

				//_transform *= glm::rotate(90.0f, 1.0f, 0.0f, 0.0f);
				                                                                           

				auto &_ragdollOffset = _ragdollBoundingBoxes[i];
				glm::vec3 _location = (_ragdollOffset[0] + _ragdollOffset[1]) * 0.5f;
				//_transform *= glm::translate(-_location);

				//glm::mat4 _inverseOffset = _animationComponent.getSkeleton().m_boneTransforms[i].m_offset			
				//_inverseOffset *= _inverseOffset;

				//Keep the same offsets
				/*glm::vec4 _translation = _boneTransforms[0][3]; // parent bone
				Utilities::print(_translation);*/

				glm::mat4 _originalOffset = glm::mat4(glm::mat3(_skeleton.m_boneTransforms[i].m_originalOffset));
				//_originalOffset = glm::transpose(_originalOffset);

				//_boneTransforms[i] = _transform;
				_boneTransforms[i] = _inverseModelMatrix * _transform * _originalOffset;
				_boneTransforms[i] *= glm::scale(_matrixComponent.getScale());
				_boneTransforms[i][3] = glm::vec4(glm::vec3(0.0f, _translation.z, -_translation.y), 1.0f); // glm::vec4(0, 0, 0, 1);
			
				//0 Scale matrix
				//_boneTransforms[i] = _inverseModelMatrix * glm::mat4(0.0f) * _originalOffset;
			}

			//_animationComponent.setTransforms(_boneTransforms);
			_animationComponent.updateOffsetTransforms();

			//PRINT("SKELEOTN INV TRANSFORM");
			//Utilities::print(_skeleton.m_globalInverseTransform);



			return _boneTransforms;
		}



		void setTransformLocation(const Animation::AnimationSkeleton &skeleton,
			std::vector<glm::mat4> *transforms, const std::string &boneName, const Location &location)
		{

		}

		void setTransformRotation(const Animation::AnimationSkeleton &skeleton,
			std::vector<glm::mat4> *transforms, const std::string &boneName, const Rotation &rotation)
		{
			unsigned int _boneId = skeleton.getBoneId(boneName);

			//TODO maybe use Scale as well
			glm::mat4 _identity(1.0f);
			_identity = glm::toMat4(rotation);
			_identity[3] = (*transforms)[_boneId][3];
			(*transforms)[_boneId] = _identity;
		}

	}
}
