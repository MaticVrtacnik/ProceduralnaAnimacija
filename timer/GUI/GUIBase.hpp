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


#ifndef GUI_BASE_HPP
#define GUI_BASE_HPP

#include <unordered_map>
#include <memory>
#include <algorithm>
#include <glm/glm.hpp>
#include <GL/glew.h>

#include "../Define/Defines.hpp"
#include "../Input/Input.hpp"
#include "../Shader/Shader.hpp"
#include "../Define/PrintDefines.hpp"
#include "../Define/Assert.hpp"
#include "../Texture/Texture.hpp"


namespace Engine{
	namespace GUI{

		static const glm::vec4 c_color(0, 0.4f, 0, 1.0f);
		static const glm::vec4 c_windowColor(0.15f, 0.15f, 0.16f, -0.15f);
		static const glm::vec4 c_elementColor1(0.15f, 0.15f, 0.16f, 0.0f);
		static const glm::vec4 c_elementColor2(0.25f, 0.25f, 0.27f, 0.0f);
		static const glm::vec4 c_textColor(1.0f, 1.0f, 1.0f, 0.0f);


		class GUIBase{
		protected:
			//using GUIArray = std::unordered_map<std::string, std::unique_ptr<GUIBase>>;
			using GUIArray = std::unordered_map<std::string, GUIBase*>;
			using TextureArray = std::unordered_map<std::string, unsigned int>;

			int m_x, m_y;
			int m_scaleX, m_scaleY;
			int m_border;

			bool m_visible;
			bool m_active;
			bool m_hovering;
			bool m_enabled;
			bool m_updated;

			unsigned int m_texture;
			glm::vec4 m_color;

			std::string m_name;
			GUIBase *m_parent;
			GUIArray m_children;
			std::vector<std::string> m_insertionOrder;

			virtual void onHover(){}
			virtual void onMouseDown(int mx, int my, bool left){}
			virtual void onMouseUp(int mx, int my, bool left){}
			virtual void onMouseHold(int mx, int my, bool left){}
			virtual void onButtonDown(int button) {}
			virtual void onButtonUp(int button) {}

			virtual void onChildAdded(GUIBase *child){}
			virtual void onParentChanged(){}

		public:
			GUIBase(
				const std::string &name,
				int x, int y,
				int scaleX, int scaleY,
				int border = 0,
				unsigned int texture = 0)
			{
				m_name = name;
				m_x = x;
				m_y = y;
				m_scaleX = scaleX;
				m_scaleY = scaleY;
				m_border = border;
				m_texture = texture;
				m_color = glm::vec4(0.0f);

				m_visible = true;
				m_enabled = true;
				m_active = false;
				m_updated = true;
			}

			virtual ~GUIBase(){}

			inline virtual void beforeDraw(Shaders::Shader &shader){
				shader.setUniform("u_color", m_color);
				shader.setUniform("u_scale", m_scaleX, m_scaleY);
				shader.setUniform("u_location", getLocation());

				//TODO add to GUI init (call only once)
				//shader.setUniform("u_texture", 0);
				Textures::bind2DTexture(m_texture, 0);
			}

			inline virtual void afterDraw(){

			}

			GUIBase *getIndex(){
				return m_parent == nullptr ? 
					this : m_parent->getIndex();
			}

			void handleInput(const Input::Input &input){
				int _mx = input.m_mouseX;
				int _my = input.m_mouseY;

				auto &_loc = getLocation();
				int _x1 = _loc.x;
				int _y1 = _loc.y;
				int _x2 = _loc.x + m_scaleX;
				int _y2 = _loc.y + m_scaleY;

				if (m_parent != nullptr){ //if outside of parents boundaries
					/*auto &_pLoc1 = m_parent->getLocation();
					auto &_pLoc2 = _pLoc1 + m_parent->getScale();
					_x1 = glm::max(_pLoc1.x, _x1);
					_y1 = glm::max(_pLoc1.y, _y1);
					_x2 = glm::min(_pLoc2.x, _x2);
					_y2 = glm::min(_pLoc2.y, _y2);*/
				}

				//check if mouse if over element
				if (_mx >= _x1 && _mx <= _x2 && 
					_my >= _y1 && _my <= _y2 && 
					m_enabled)
				{
					m_hovering = true;
					onHover();

					if (input.m_mouseLeftDown){
						onMouseDown(_mx, _my, true);
						setActive(true);
					}
					if (input.m_mouseRightDown){
						onMouseDown(_mx, _my, false);
						setActive(true);
					}
				}
				else if (input.m_mouseLeftDown || input.m_mouseRightDown){
					setActive(false);
				}else m_hovering = false;
				

				if (input.m_mouseLeftHold)onMouseHold(_mx, _my, true);
				if (input.m_mouseRightHold)onMouseHold(_mx, _my, false);	
				if (input.m_mouseLeftUp)onMouseUp(_mx, _my, true);
				if (input.m_mouseRightUp)onMouseUp(_mx, _my, false);

				//TODO maybe change to array 
				if (input.m_buttonLastDown != 0)onButtonDown(input.m_buttonLastDown);
				if (input.m_buttonLastUp != 0)onButtonUp(input.m_buttonLastUp);
				
			}

			bool isHovering() const{
				/*bool _childHover = false;
				for (auto &_child : m_children){
					if (_child.second->isHovering()){
						_childHover = true; 
						break;
					}
				}*/
				//TODO check
				return m_hovering; // || _childHover;
			}

			bool isActive() const{
				return m_active;
			}

			void setActive(bool active){
				if (m_active != active){
					if (active){
						if (m_parent != nullptr){
							m_parent->setActive(true);
						}
					}
					else{
						for (auto &_child : m_children){
							_child.second->setActive(false);
						}
					}

					m_active = active;
				}
			}

			void setVisible(bool visible){
				m_visible = visible;
			}

			bool isUpdated(){
				if (m_updated){
					m_updated = false;
					return true;
				}else return false;
			}

			void setUpdated(bool updated){
				m_updated = updated;
			}

			bool isEnabled() const{
				return m_enabled;
			}
			
			void setEnabled(bool enabled){
				if (enabled != m_enabled){
					auto _add = glm::vec4(0, 0, 0, -0.3f);
					for (auto &_child : m_children){
						_child.second->setEnabled(enabled);
					}

					m_color += (enabled ? -_add : _add);
					m_enabled = enabled;		
				}
			}

			void disable(){
				setEnabled(false);
			}

			void enable(){
				setEnabled(true);
			}

			bool isVisible() const{
				return m_visible;
			}

			void setBorder(int border){
				m_border = border;
			}

			int getBorder() const{
				return m_border;
			}

			void setLocation(int x, int y){		
				m_x = x;
				m_y = y;
			}

			void setAbsoluteLocation(int x, int y){
				auto &_loc = getLocation();
				m_x += (x - _loc.x);
				m_y += (y - _loc.y);
			}

			void setScale(int x, int y){
				m_scaleX = x;
				m_scaleY = y;
			}

			void setColor(const glm::vec4 &color){
				m_color = color;
			}

			const glm::vec4 &getColor() const{
				return m_color;
			}

			const std::string &getName() const{
				return m_name;
			}

			glm::ivec2 getScale() const{
				return glm::ivec2(m_scaleX, m_scaleY);
			}

			glm::ivec2 getRelativeLocation() const{
				return glm::ivec2(m_x, m_y);
			}

			glm::ivec2 getLocation() const{
				return m_parent == nullptr ? glm::ivec2(m_x, m_y) :
					glm::ivec2(m_x, m_y) + m_parent->getLocation();
			}

			unsigned int getTexture() const{
				return m_texture;
			}

			void setTexture(unsigned int texture){
				m_texture = texture;
			}

			void setParent(GUIBase *parent){
				m_parent = parent;
				onParentChanged();
			}

			const std::vector<std::string> &getInsertionOrder() const{
				return m_insertionOrder;
			}

			GUIArray &getChildren(){
				return m_children;
			}

			GUIBase *getParent() const{
				return m_parent;
			}

			
			template <typename T, typename... Args>
			T &addChild(Args&&... args){
				static_assert(std::is_base_of<GUIBase, T>::value,
					"GUI element not inherited from GUIBase");
				
				auto _child = new T{ std::forward <Args>(args)... };
				addChild(_child);
				return *_child;
			}


			void addChild(GUIBase *child){
				if (child == nullptr){		
					ASSERT(0, "Child is a null pointer");
					return;
				}

				child->m_parent = this;

				auto &_rLoc = child->getRelativeLocation();
				child->setLocation(_rLoc.x + m_border, _rLoc.y + m_border);

				auto &_name = child->getName();
				if (m_children.find(_name) != m_children.end()){
					WARNING("Child with that name already exists (" << _name << ")");
					delete child;
					ASSERT(0, "");
					return;
				}

				m_insertionOrder.push_back(_name);
				m_children[_name] = child;
				
				//TODO CONVERT TO SMART PTR
				//m_children[_child->getName()] = std::make_unique<T>(_child);
				//m_children[_child->getName()] = std::make_unique<T>(_child);

				onChildAdded(child);
			}

			bool hasChild(const std::string &name){
				return m_children.find(name) != m_children.end();
			}

			template <typename T> 
			T &getChild(const std::string &name){
				static_assert(std::is_base_of<GUIBase, T>::value,
					"GUI element not inherited from GUIBase");

				return static_cast<T&>(*m_children[name]);// .get();
			}

			void removeChild(const std::string &name){
				//m_children.erase(name);
				//onChildRemoved...?
			}

		};



		template <typename T, typename... Args>
		T *createGUIElement(Args&&... args){
			static_assert(std::is_base_of<GUIBase, T>::value,
				"GUI element not inherited from GUIBase");

			return new T{ std::forward <Args>(args)... };
		}


	}
}

#endif //GUI_BASE_HPP