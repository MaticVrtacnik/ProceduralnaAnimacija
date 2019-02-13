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


#ifndef GUI_TREE_VIEW_HPP
#define GUI_TREE_VIEW_HPP

#include "GUIList.hpp"
#include "../Define/Assert.hpp"

namespace Engine{
	namespace GUI{
		
		class GUITreeView : public GUIBase{
		private:
			unsigned int m_arrowDownTexture;
			unsigned int m_arrowRightTexture;
			unsigned int m_nodeTexture;
			float m_size;

			void setNodeFunction(GUIButton &button){
				auto _action = [this, &button](){
					auto &_children = button.getChildren();
					auto &_order = button.getInsertionOrder();
					auto _first = _children[_order[0]];

					for (auto &_key : _order){
						auto _child = _children[_key];
						if (_child != _first){
							_child->setVisible(!_child->isVisible());
						}
					}

					if (_children.size() < 2)return; //TODO HACK dunno added as workaround

					bool _visible = _children[_order[1]]->isVisible();
					button.setTexture(_visible ? m_arrowDownTexture : m_arrowRightTexture);
					updateLocations(&this->getChild<GUIButton>("index"));
				};

				button.setAction(_action);	
				button.getChild<GUIText>(button.getName()).setAction(_action);
			}

			int numVisibleChildren(GUIBase *child){
				int _sum = 1;
				for (auto _child : child->getChildren()){
					if (_child.second->isVisible()) //also includes text (sum / 2)
						_sum += numVisibleChildren(_child.second);
				}return _sum;
			}

			//returns absolute max location
			//for calculating new scale
			glm::ivec2 getMaxLocation(GUIBase *child){ 
				glm::ivec2 _max(0, 0);
				for (auto &_child : child->getChildren()){
					auto _c = _child.second;
					auto &_loc = _c->getLocation();
					auto &_nLoc = _loc + _c->getScale();
					auto _mLoc = getMaxLocation(_c);

					if (_mLoc.x > _max.x) _max.x = _mLoc.x;
					if (_mLoc.y > _max.y) _max.y = _mLoc.y;
				}

				return _max;
			}

			void updateLocations(GUIBase *child){	
				auto &_children = child->getChildren();
				auto &_order = child->getInsertionOrder();

				for (auto _it = _order.begin(); _it != _order.end(); ++_it){
					auto _child = _children[*_it];

					if (!_child->getChildren().empty()){ //if its not final node (text)
						int _sum = 0;
						for (auto _it2 = _order.begin(); _it2 != _it; ++_it2){
							_sum += numVisibleChildren(_children[*_it2]);
						}

						int _y = static_cast<int>(child->getScale().y - 2.0f * m_size - (_sum / 2.0f) * m_size);
						_child->setLocation(_child->getRelativeLocation().x, _y);
					}
					else{//text
						_child->setLocation(_child->getRelativeLocation().x, -1);
					}

					updateLocations(_child);

				}
			}

		public:
			GUITreeView(
				const std::string &name, 
				int x, int y, 
				float size,
				const std::string &indexName = "World"
			)
				: GUIBase(name, x, y, 0, 0)
			{
				m_color = c_elementColor1;
				m_size = size;

				m_nodeTexture = Textures::load2DTexture("icons/cube.png", false);
				m_arrowRightTexture = Textures::load2DTexture("icons/folderClosed.png", false);
				m_arrowDownTexture = Textures::load2DTexture("icons/folderOpened.png", false);

				int _size = static_cast<int>(m_size);
				auto &_button = addChild<GUIButton>(
					"index", 5, m_scaleY - 10 - _size, _size, _size, 0, m_arrowRightTexture);

				_button.addChild<GUIText>(
					"index", indexName, 5 + _size, -1, m_size);

				_button.setColor(glm::vec4(-0.2f, -0.4f, -0.7f, 0.0f));
				setNodeFunction(_button);
			}

			~GUITreeView(){
				glDeleteTextures(1, &m_nodeTexture);
				glDeleteTextures(1, &m_arrowDownTexture);
				glDeleteTextures(1, &m_arrowRightTexture);
			}

			void updateLocations(){
				updateLocations(&getChild<GUIButton>("index"));
			}		
			
			template <typename... Args>
			GUIButton &addNode(Args&&... args){
				return addNode<>(std::vector<std::string>{ 
					std::forward<Args>(args)... });
			}

			template <>
			GUIButton &addNode<std::vector<std::string>>(std::vector<std::string>&& paths){
				GUIBase *_child = &getChild<GUIButton>("index");
		
				for (std::size_t i = 0; i < paths.size(); ++i){
					auto &_order = _child->getInsertionOrder();
					auto _string = std::move(paths[i]);
					//ASSERT(_string != "index", "Index node duplicate");

					if (std::find(_order.begin(), _order.end(), _string) == _order.end()){
						int _size = static_cast<int>(m_size);
						auto &_button = _child->addChild<GUIButton>(
							_string, _size, 0, _size, _size);

						_button.addChild<GUIText>(_string, _string, _size, -1, m_size - 2.0f);
					}

					_child->setTexture(m_arrowRightTexture);

					auto &_button = _child->getChild<GUIButton>(_string);
					_child = &_button;
		
					setNodeFunction(_button);
					_child->setVisible(false);
					_child->setColor(glm::vec4(-0.2f, -0.4f, -0.7f, 0.0f));
				}


				auto _button = static_cast<GUIButton*>(_child);//TODO (open component tab?) -> final node
				_button->setAction([]{});
				_button->getChild<GUIText>(_button->getName()).setAction([]{});
				_button->setColor(glm::vec4(0.0f));
				_button->setTexture(m_nodeTexture);

				//TODO slow af
				/*auto &_index = getChild<GUIButton>("index");
				updateLocations(&_index);*/

				return *_button;
			}

		};
	}
}

#endif //GUI_TREE_VIEW_HPP