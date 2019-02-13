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


#include "MatrixComponent.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>


namespace Engine{
	namespace Entities{
		namespace Components{

			MatrixComponent::MatrixComponent(){
				setMatrix();
				updateStartTransforms();
			}

			MatrixComponent::MatrixComponent(const Location &location) :
				m_location(location), m_rotationCenter(location)
			{
				setMatrix();
				updateStartTransforms();
			}

			MatrixComponent::MatrixComponent(const Rotation &rotation) :
				 m_rotation(rotation)
			{
				setMatrix();
				updateStartTransforms();
			}

			MatrixComponent::MatrixComponent(const Scale &scale) :
				m_scale(scale)
			{
				setMatrix();
				updateStartTransforms();
			}

			MatrixComponent::MatrixComponent(const Location &location, const Rotation &rotation) :
				m_location(location), m_rotationCenter(location), m_rotation(rotation)
			{
				setMatrix();
				updateStartTransforms();
			}

			MatrixComponent::MatrixComponent(const Location &location, const Scale &scale) :
				m_location(location), m_rotationCenter(location), m_scale(scale)
			{
				setMatrix();
				updateStartTransforms();
			}

			MatrixComponent::MatrixComponent(const Rotation &rotation, const Scale &scale) :
				m_rotation(rotation), m_scale(scale)
			{
				setMatrix();
				updateStartTransforms();
			}

			MatrixComponent::MatrixComponent(const Location &location, const Rotation &rotation, const Scale &scale) :
				m_location(location), m_rotationCenter(location), m_rotation(rotation), m_scale(scale)
			{
				setMatrix();
				updateStartTransforms();
			}

			/*MatrixComponent::MatrixComponent(Location &&location) :
				m_location(std::move(location))
			{
				setMatrix();
				updateStartTransforms();
			}

			MatrixComponent::MatrixComponent(Rotation &&rotation) :
				m_rotation(std::move(rotation))
			{
				setMatrix();
				updateStartTransforms();
			}

			MatrixComponent::MatrixComponent(Scale &&scale) :
				m_scale(std::move(scale)),
			{
				setMatrix();
				updateStartTransforms();
			}

			MatrixComponent::MatrixComponent(Location &&location, Rotation &&rotation) :
				m_location(std::move(location)), m_rotation(std::move(rotation))
			{
				setMatrix();
				updateStartTransforms();
			}

			MatrixComponent::MatrixComponent(Location &&location, Scale &&scale) :
				m_location(std::move(location)), m_scale(std::move(scale))
			{
				setMatrix();
				updateStartTransforms();
			}

			MatrixComponent::MatrixComponent(Rotation &&rotation, Scale &&scale) :
				 m_rotation(std::move(rotation)), m_scale(std::move(scale))
			{
				setMatrix();
				updateStartTransforms();
			}

			MatrixComponent::MatrixComponent(Location &&location, Rotation &&rotation, Scale &&scale) :
				m_location(std::move(location)), m_rotation(std::move(rotation)), m_scale(std::move(scale))
			{
				setMatrix();
				updateStartTransforms();
			}*/



			const glm::mat4 &MatrixComponent::getMatrix(){
				if (!m_updated)setMatrix();

				return m_matrix;
			}

			const glm::mat4 &MatrixComponent::getPreviousMatrix(){
				if (!m_updated)setMatrix();

				return m_previousMatrix;
			}

			void MatrixComponent::setMatrix(){
				if (m_updated)return;

				m_updated = true;
				m_previousMatrix = m_matrix;

				Location _offset(m_location - m_rotationCenter);
				bool _rotateAround = m_location != m_rotationCenter;

				m_matrix = glm::mat4(1.0);
				m_matrix *= glm::translate(m_location);
				if (_rotateAround) m_matrix *= glm::translate(-_offset);
				m_matrix *= glm::toMat4(m_rotation);
				if (_rotateAround) m_matrix *= glm::translate(_offset);
				m_matrix *= glm::scale(m_scale);

				/*if (m_scale.x == 0.0) WARNING("Scale.x is 0!");
				if (m_scale.y == 0.0) WARNING("Scale.y is 0!");
				if (m_scale.z == 0.0) WARNING("Scale.z is 0!");*/
			}



			void MatrixComponent::resetTransforms(){
				setLocation(m_startLocation);
				setRotation(m_startRotation);
				setScale(m_startScale);
			}

			void MatrixComponent::updateStartTransforms(){
				m_startLocation = m_location;
				m_startRotation = m_rotation;
				m_startScale = m_scale;
			}



			const Location &MatrixComponent::getRotationCenter() const{
				return m_rotationCenter;
			}

			void MatrixComponent::resetRotationCenter(){
				m_rotationCenter = m_location;
			}

			void MatrixComponent::setRotationCenter(const Location &center){
				m_rotationCenter = center;
				m_updated = false;
			}

			void MatrixComponent::setRotationCenter(Location &&center){
				m_rotationCenter = std::move(center);
				m_updated = false;
			}

			void MatrixComponent::setRotationCenter(float x, float y, float z){
				m_rotationCenter = Location(x, y, z);
				m_updated = false;
			}



			const Location &MatrixComponent::getLocation() const{
				return m_location;
			}

			const Location &MatrixComponent::getStartLocation() const{
				return m_startLocation;
			}

			const Location &MatrixComponent::getPreviousLocation() const{
				return m_previousLocation;
			}

			const float MatrixComponent::getDistanceFromOrigin() const{
				return glm::length(glm::vec3(m_location.x, m_location.y, m_location.z));
			}

			void MatrixComponent::setLocation(const Location &location){
				Location _change = location - m_location;
				m_location = m_location + _change;
				m_rotationCenter = m_rotationCenter + _change;
				m_updated = false;
			}

			void MatrixComponent::setLocation(Location &&location){
				Location _change = std::move(location) - m_location;
				m_location = m_location + _change;
				m_rotationCenter = m_rotationCenter + _change;
				m_updated = false;
			}

			void MatrixComponent::setLocation(float x, float y, float z){
				Location _change = Location(x, y, z) - m_location;
				m_location = m_location + _change;
				m_rotationCenter = m_rotationCenter + _change;
				m_updated = false;
			}

			void MatrixComponent::setLocationX(float x){
				float _change = x - m_location.x;
				m_location.x += _change;
				m_rotationCenter.x += _change;
				m_updated = false;
			}

			void MatrixComponent::setLocationY(float y){
				float _change = y - m_location.y;
				m_location.y += _change;
				m_rotationCenter.y += _change;
				m_updated = false;
			}

			void MatrixComponent::setLocationZ(float z){
				float _change = z - m_location.z;
				m_location.z += _change;
				m_rotationCenter.z += _change;
				m_updated = false;
			}



			const Rotation &MatrixComponent::getRotation() const{
				return m_rotation;
			}

			const Rotation &MatrixComponent::getStartRotation() const{
				return m_startRotation;
			}

			const Rotation &MatrixComponent::getPreviousRotation() const{
				return m_previousRotation;
			}

			void MatrixComponent::setRotation(const Rotation &rotation){
				m_rotation = rotation;
				m_updated = false;
			}

			void MatrixComponent::setRotation(Rotation &&rotation){
				m_rotation = std::move(rotation);
				m_updated = false;
			}

			void MatrixComponent::setRotation(const glm::vec3 &angles){
				m_rotation = Rotation(angles);
				m_updated = false;
			}

			void MatrixComponent::setRotation(glm::vec3 &&angles){
				m_rotation = Rotation(std::move(angles));
				m_updated = false;
			}

			void MatrixComponent::setRotation(float x, float y, float z){
				m_rotation = Rotation(x, y, z);
				m_updated = false;
			}



			const Scale &MatrixComponent::getScale() const{
				return m_scale;
			}
			
			const Scale &MatrixComponent::getStartScale() const{
				return m_startScale;
			}
			
			const Scale &MatrixComponent::getPreviousScale() const{
				return m_previousScale;
			}

			void MatrixComponent::setScale(const Scale &scale){
				m_scale = scale;
				m_updated = false;
			}

			void MatrixComponent::setScale(Scale &&scale){
				m_scale = std::move(scale);
				m_updated = false;
			}

			void MatrixComponent::setScale(float x, float y, float z){
				m_scale = Scale(x, y, z);
				m_updated = false;
			}

			void MatrixComponent::setScaleX(float x){
				m_scale.x = x;
				m_updated = false;
			}

			void MatrixComponent::setScaleY(float y){
				m_scale.y = y;
				m_updated = false;
			}

			void MatrixComponent::setScaleZ(float z){
				m_scale.z = z;
				m_updated = false;
			}



			void MatrixComponent::increaseLocation(const Location &change){
				m_location = m_location + change;
				m_rotationCenter = m_rotationCenter + change;
				m_updated = false;
			}
		
			//TODO remove? doesnt make sense?
			void MatrixComponent::increaseLocation(Location &&location){
				m_location = m_location + std::move(location);
				m_updated = false;
			}

			void MatrixComponent::increaseLocation(float x, float y, float z){
				Location _change(x, y, z);
				m_location = m_location + _change;
				m_rotationCenter = m_rotationCenter + _change;
				m_updated = false;
			}

			void MatrixComponent::increaseLocationX(float x){
				increaseLocation(x, 0.0f, 0.0f);
			}

			void MatrixComponent::increaseLocationY(float y){
				increaseLocation(0.0f, y, 0.0f);
			}

			void MatrixComponent::increaseLocationZ(float z){
				increaseLocation(0.0f, 0.0f, z);
			}



			void MatrixComponent::increaseRotation(const Rotation &change){
				m_rotation = change * m_rotation;
				m_updated = false;
			}

			void MatrixComponent::increaseRotation(Rotation &&change){
				m_rotation = std::move(change) * m_rotation;
				m_updated = false;
			}

			void MatrixComponent::increaseRotation(float x, float y, float z){
				m_rotation = Rotation(x, y, z) * m_rotation;
				m_updated = false;
			}

			void MatrixComponent::increaseRotationX(float angle){
				increaseRotation(angle, 0.0f, 0.0f);
			}

			void MatrixComponent::increaseRotationY(float angle){
				increaseRotation(0.0f, angle, 0.0f);
			}

			void MatrixComponent::increaseRotationZ(float angle){
				increaseRotation(0.0f, 0.0f, angle);
			}



			void MatrixComponent::increaseScale(const Scale &change){
				m_scale = m_scale + change;
				m_updated = false;
			}

			void MatrixComponent::increaseScale(Scale &&change){
				m_scale = m_scale + std::move(change);
				m_updated = false;
			}

			void MatrixComponent::increaseScale(float x, float y, float z){
				m_scale = m_scale + Scale(x, y, z);
				m_updated = false;
			}

			void MatrixComponent::increaseScaleX(float x){
				increaseScale(x, 0.0f, 0.0f);
			}

			void MatrixComponent::increaseScaleY(float y){
				increaseScale(0.0f, y, 0.0f);
			}

			void MatrixComponent::increaseScaleZ(float z){
				increaseScale(0.0f, 0.0f, z);
			}

		}
	}
}
