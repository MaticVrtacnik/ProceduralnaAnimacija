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


#include "GUICreator.hpp"

#include "../World/World.hpp"
#include "../Utility/RandomUtilities.hpp"
#include "GUIButton.hpp"
#include "GUIText.hpp"
#include "GUIWindow.hpp"
#include "GUITextBox.hpp"
#include "GUIToggle.hpp"
#include "GUICheckbox.hpp"
#include "GUITreeView.hpp"
#include "GUISpinner.hpp"
#include "GUIProgressBar.hpp"
#include "GUITab.hpp"
#include "GUIRadioButton.hpp"
#include "GUIList.hpp"
#include "GUIInformation.hpp"
#include "GUIDropdown.hpp"
#include "GUIColorPicker.hpp"
#include "GUIWindowStack.hpp"
#include "GUISlider.hpp"
#include "GUISelectorList.hpp"
#include "GUIFileExplorer.hpp"



namespace Engine{
	namespace GUI{

		//local
		void addWindowX(GUIWindow &window){
			int _border = window.getBorder();

			auto &_add = window.addChild<GUIButton>("x",
				window.getScale().x - 15 - _border,
				window.getScale().y - 15 - _border, 13, 13);
			_add.setColor(glm::vec4(0.8f, 0.4f, 0.1f, 0.0f));

			auto &_x = _add.addChild<GUIText>("x", "x", 3, 0, 12.0f);
			_x.setColor(glm::vec4(0.0f));
		}

		//returns next offset
		int addXYZSliders(
			GUIWindow &window, 
			const std::string &name,  
			float min, float max,
			int offset, bool RGB)
		{
			int _top = window.getScale().y;

			std::string _Xstr = RGB ? "R" : "X";
			std::string _Ystr = RGB ? "G" : "Y";
			std::string _Zstr = RGB ? "B" : "Z";
 
			std::string _key = name;
			_key.erase(std::remove(_key.begin(), _key.end(), ' '), _key.end());
			if (!_key.empty())_key[0] = tolower(_key[0]);

			window.addChild<GUIText>(_key, name, 20, _top - offset, 12.0f)
				.setColor(glm::vec4(0.8, 0.4, 0.1, 0.0));

			auto &_X = window.addChild<GUISlider>(
				_key + _Xstr, min, max, 15, _top - (offset + 17), 180, 15);
			_X.addChild<GUIText>(_Xstr, _Xstr, -10, 0, 10.0f);

			auto &_Y = window.addChild<GUISlider>(
				_key + _Ystr, min, max, 15, _top - (offset + 37), 180, 15);
			_Y.addChild<GUIText>(_Ystr, _Ystr, -10, 0, 10.0f);

			auto &_Z = window.addChild<GUISlider>(
				_key + _Zstr, min, max, 15, _top - (offset + 57), 180, 15);
			_Z.addChild<GUIText>(_Zstr, _Zstr, -10, 0, 10.0f);

			return offset + 80;
		}

		//returns next offset
		int addSlider(
			GUIWindow &window,
			const std::string &name,
			float min, float max,
			int offset, 
			TextBoxType type = TEXT_BOX_FLOAT)
		{	
			int _top = window.getScale().y;

			std::string _key = name;
			_key.erase(std::remove(_key.begin(), _key.end(), ' '), _key.end());
			if (!_key.empty())_key[0] = tolower(_key[0]);

			window.addChild<GUIText>(
				"t" + _key, name, 20, _top - offset, 12.0f)
				.setColor(glm::vec4(0.8, 0.4, 0.1, 0.0));

			window.addChild<GUISlider>(
				_key, min, max, 15, _top - (offset + 17), 180, 15, 0, type);

			return offset + 40;
		}

		//creates new window for component tab window stack
		GUIWindow &createNewWindow(
			GUIBase *index, 
			const std::string &name,
			const std::string &tab,
			int height)
		{
			auto &_tab = index->getChild<GUIWindow>("tab").getChild<GUITab>("tab");
			auto &_stack = _tab.getWindow(tab).getChild<GUIWindowStack>("stack");
			auto _window = createGUIElement<GUIWindow>(name, 0, 0, 0, height, false, 3);
			_stack.addWindow(_window);

			return *_window;
		}


		void createEditorGUI(int screenWidth, int screenHeight, GUIBase *index){
			ASSERT(index != nullptr, "GUI index is a null pointer");

			auto &_menu = index->addChild<GUIWindow>("menu", 
				0, screenHeight - 20, screenWidth, 20, false);
			_menu.setVisibleOutside(true);
			_menu.setColor(glm::vec4(0.1, 0.1, 0.13, 0.0));

			auto &_exit = _menu.addChild<GUIText>("Exit", "Exit", screenWidth - 40, 2, 10.0f);

			int _offset = 10;
			std::vector<std::string> _tabs({ "File", "Edit", "Test" });
			for (auto &_tab : _tabs){
				auto &_text = _menu.addChild<GUIText>(_tab, _tab, _offset, 2, 10.0f);
				auto &_list = _text.addChild<GUISelectorList>("list", 0, -2, 80, 15);
				_list.addItems("test", "test2", "test3");
				_text.setAction([&_menu, &_list]{
					for (auto _tabs : _menu.getChildren()){
						for (auto _child : _tabs.second->getChildren()){
							auto _c = _child.second;
							if (&_list == _c)
								_c->setVisible(!_c->isVisible());
							else _c->setVisible(false);
						}
					}
				});

				_offset += _text.getScale().x + 15;
			}

			auto &_tools = index->addChild<GUIWindow>("tools", 
				0, screenHeight - 70, screenWidth, 50, false, 3);
			_tools.setColor(glm::vec4(0.1, 0.1, 0.13, 0.0));

			_offset = 5;
			for (auto _mode : std::vector<std::string>{ 
				"player", "translate", "rotate", "scale", "scene"})
			{
				unsigned int _texture = Textures::load2DTexture("icons/" + _mode + ".png", false);
				auto &_button = _tools.addChild<GUIButton>(_mode, _offset, 3, 40, 40, 3, _texture);
				_offset += 45;
			}


			auto &_explorer = index->addChild<GUIFileExplorer>
				("explorer", 400, 100, 300, 400, "data/textures/particles");
			_explorer.setVisible(false);



			auto &_win = index->addChild<GUIWindow>("tab",
				screenWidth - 300, 0, 300, screenHeight - 70, false, 3, RESIZABLE_X);
			_win.setColor(glm::vec4(0.1, 0.1, 0.13, 0.0));
	
			auto &_tab = _win.addChild<GUITab>("tab", 5, 5, 280, screenHeight - 90, 3);
			{

				auto _grass = createGUIElement<GUIWindow>("grass", 0, 0, 0, 0, false, 3);
				{
					auto &_tree = _grass->addChild<GUITreeView>("tree", 5, screenHeight - 120, 13.0f);

					for (int i = 0; i < 25; i++){
						_tree.addNode("Entities", "Entity " + std::to_string(i), std::to_string(i));
					}

					_tree.addNode("WTF", "IS", "THIS", "TEST", "SHIT");
					_tree.addNode("Water", "water");
					_tab.addWindow(_grass);
				}


				auto _components = createGUIElement<GUIWindow>("components", 0, 0, 0, 0, false, 3);
				{
					auto &_componentStack = _components->addChild<GUIWindowStack>
						("stack", 0, screenHeight - 130, 260, 0);

					_components->setVisibleOutside(true);
					_tab.addWindow(_components);
				}
				

				auto _environment = createGUIElement<GUIWindow>("environment", 0, 0, 0, 0, false, 3);
				{
					auto &_environmentStack = _environment->addChild<GUIWindowStack>
						("stack", 0, screenHeight - 130, 260, 0);

					_tab.addWindow(_environment);
					_environment->setVisibleOutside(true);
				}

				
				auto _quad = createGUIElement<GUIWindow>("quad", 0, 0, 0, 0, false, 3);
				{
					_tab.addWindow(_quad);
				}

			}

			

			
		}

		void createRenderGUI(GUIBase *index){
			auto &_render = createNewWindow(index, "render", "environment", 280);
			{
				int _offset = 23;
				int _top = _render.getScale().y;

				_offset = addSlider(_render, "Wind Velocity", -5.0f, 5.0f, _offset);
				_render.getChild<Engine::GUI::GUISlider>("windVelocity").setValue(2.0f);

				_offset = addSlider(_render, "Exposure", 0.1f, 5.0f, _offset);
				_render.getChild<Engine::GUI::GUISlider>("exposure").setValue(1.0f);

				_offset = addSlider(_render, "Saturation", 0.0f, 2.0f, _offset);
				_render.getChild<Engine::GUI::GUISlider>("saturation").setValue(1.0f);


				_offset = addSlider(_render, "Time", 0.0f, 3.1415f, _offset);

				_offset = addSlider(_render, "Focal Distance", 0.0f, 500.0f, _offset);
				_render.getChild<Engine::GUI::GUISlider>("focalDistance").setValue(5.0f);

				_offset = addSlider(_render, "Focal Range", 0.0f, 500.0f, _offset);
				_render.getChild<Engine::GUI::GUISlider>("focalRange").setValue(100.0f);

				_render.addChild<GUIText>("depthOfFieldT", "DepthOfField", 5, _top - _offset, 12.0f)
					.setColor(glm::vec4(0.8, 0.4, 0.1, 0.0));
				_render.addChild<GUIToggle>("depthOfField", 85, _top - _offset, false);

			}

			auto &_tab = index->getChild<GUIWindow>("tab").getChild<GUITab>("tab");
			auto &_stack = _tab.getWindow("environment").getChild<GUIWindowStack>("stack");
			_stack.closeWindow("render");
		}

		void createWaterGUI(GUIBase *index){

			auto &_water = createNewWindow(index, "water", "environment", 350);
			{
				int _offset = 23;
				int _top = _water.getScale().y;

				_offset = addXYZSliders(_water, "Water Color", 0.0f, 1.0f, _offset, true);
				_offset = addSlider(_water, "Murk Depth", 0.0f, 500.0f, _offset);
				_offset = addSlider(_water, "Shore Offset", 0.0f, 20.0f, _offset);
				_offset = addSlider(_water, "Reflectance", 0.025f, 1.0f, _offset);
				_offset = addSlider(_water, "Distortion Factor", 0.0f, 0.25f, _offset);
				_offset = addSlider(_water, "Wave Speed", 0.0f, 300.0f, _offset);
				_offset = addSlider(_water, "Num Tiles", 1.0f, 256.0f, _offset);

				_water.getChild<Engine::GUI::GUISlider>("waterColorR").setValue(0.05f);
				_water.getChild<Engine::GUI::GUISlider>("waterColorG").setValue(0.15f);
				_water.getChild<Engine::GUI::GUISlider>("waterColorB").setValue(0.175f);
				_water.getChild<Engine::GUI::GUISlider>("murkDepth").setValue(100.0f);
				_water.getChild<Engine::GUI::GUISlider>("reflectance").setValue(1.0f);
				_water.getChild<Engine::GUI::GUISlider>("distortionFactor").setValue(0.025f);
				_water.getChild<Engine::GUI::GUISlider>("waveSpeed").setValue(10.0f);
				_water.getChild<Engine::GUI::GUISlider>("numTiles").setValue(128.0f);

			}

			auto &_tab = index->getChild<GUIWindow>("tab").getChild<GUITab>("tab");
			auto &_stack = _tab.getWindow("environment").getChild<GUIWindowStack>("stack");
			_stack.closeWindow("water");

		}

		void createTerrainGUI(GUIBase *index){
			auto &_terrain = createNewWindow(index, "terrain", "environment", 600);
			{
				int _offset = 25;
				int _top = _terrain.getScale().y;

				auto &_randomSeed = _terrain.addChild<GUIText>(
					"randomSeeds", "Random Seeds", 20, _top - 23, 12.0f);
				_randomSeed.setColor(glm::vec4(0.8, 0.4, 0.1, 0.0));

				auto &_randomSeed1 = _terrain.addChild<GUITextBox>(
					"randomSeed1", 11, _top - 43, 75, 20, TEXT_BOX_FLOAT, 3);
				auto &_randomSeed2 = _terrain.addChild<GUITextBox>(
					"randomSeed2", 96, _top - 43, 75, 20, TEXT_BOX_FLOAT, 3);
				auto &_randomSeed3 = _terrain.addChild<GUITextBox>(
					"randomSeed3", 181, _top - 43, 75, 20, TEXT_BOX_FLOAT, 3);

				_randomSeed1.setValue(Utilities::randomFloat(0.0f, 1.0f));
				_randomSeed2.setValue(Utilities::randomFloat(0.0f, 1.0f));
				_randomSeed3.setValue(Utilities::randomFloat(0.0f, 1.0f));

				_offset += 40;

				//0.5, -0.25, 0.6, -0.1, 0.5, -0.25
				//8, 1.9, 8, 1.9, 10, 2.0

				_offset = addSlider(_terrain, "Height Factor Wind Turbolence", 0.01f, 2.0f, _offset);
				_terrain.getChild<Engine::GUI::GUISlider>("heightFactorWindTurbolence").setValue(0.5f);

				_offset = addSlider(_terrain, "Height Offset Wind Turbolence", -2.0f, 2.0f, _offset);
				_terrain.getChild<Engine::GUI::GUISlider>("heightOffsetWindTurbolence").setValue(-0.25f);

				_offset = addSlider(_terrain, "Frequency Wind Turbolence", 0.05f, 5.0f, _offset);
				_terrain.getChild<Engine::GUI::GUISlider>("frequencyWindTurbolence").setValue(0.5f);


				_offset = addSlider(_terrain, "Height Factor Noise1", 0.01f, 2.0f, _offset);
				_terrain.getChild<Engine::GUI::GUISlider>("heightFactorNoise1").setValue(0.6f);

				_offset = addSlider(_terrain, "Height Offset Noise1", -2.0f, 2.0f, _offset);
				_terrain.getChild<Engine::GUI::GUISlider>("heightOffsetNoise1").setValue(-0.1f);

				_offset = addSlider(_terrain, "Frequency Noise1", 0.05f, 5.0f, _offset);
				_terrain.getChild<Engine::GUI::GUISlider>("frequencyNoise1").setValue(1.0f);

				_offset = addSlider(_terrain, "Height Factor Noise2", 0.01f, 2.0f, _offset);
				_terrain.getChild<Engine::GUI::GUISlider>("heightFactorNoise2").setValue(0.5f);

				_offset = addSlider(_terrain, "Height Offset Noise2", -2.0f, 2.0f, _offset);
				_terrain.getChild<Engine::GUI::GUISlider>("heightOffsetNoise2").setValue(-0.25f);

				_offset = addSlider(_terrain, "Frequency Noise2", 0.05f, 5.0f, _offset);
				_terrain.getChild<Engine::GUI::GUISlider>("frequencyNoise2").setValue(1.0f);


				//TODO if toggled ON on init not appearing correc	
				_terrain.addChild<GUIText>("wireframeT", "Wireframe", 5, _top - _offset, 12.0f)
					.setColor(glm::vec4(0.8, 0.4, 0.1, 0.0));;
				_terrain.addChild<GUIToggle>("wireframe", 75, _top - _offset, false);

				_offset += 25;
				_terrain.addChild<GUIText>("drawWaterT", "Draw Water", 5, _top - _offset, 12.0f)
					.setColor(glm::vec4(0.8, 0.4, 0.1, 0.0));;
				_terrain.addChild<GUIToggle>("drawWater", 75, _top - _offset, true);

				_offset += 20;
				_terrain.addChild<GUIButton>("smooth", 5, _top - _offset, 100, 16, 2)
					.addChild<GUIText>("smoothT", "Smooth Terrain", 9, 0, 12.0f);

				_offset += 20;
				_terrain.addChild<GUIButton>("generate", 5, _top - _offset, 100, 16, 2)
					.addChild<GUIText>("generateT", "Generate Terrain", 3, 0, 12.0f);

				_offset += 20;
				_terrain.addChild<GUIButton>("update", 5, _top - _offset, 100, 16, 2)
					.addChild<GUIText>("updateT", "Update Terrain", 9, 0, 12.0f);

			}

			auto &_tab = index->getChild<GUIWindow>("tab").getChild<GUITab>("tab");
			auto &_stack = _tab.getWindow("environment").getChild<GUIWindowStack>("stack");
			_stack.closeWindow("terrain");
		}

		void createTerrainEditorGUI(GUIBase *index){
			auto &_terrainEditor = createNewWindow(index, "terrainEditor", "environment", 350);
			{
				int _offset = 23;
				int _top = _terrainEditor.getScale().y;

				_terrainEditor.addChild<GUIText>("EditHeightT", "Edit Height", 5, _top - _offset, 12.0f)
					.setColor(glm::vec4(0.8, 0.4, 0.1, 0.0));
				_terrainEditor.addChild<GUIToggle>("editHeight", 70, _top - _offset, false);

				_terrainEditor.addChild<GUIText>("AddHeightT", "Add Height", 120, _top - _offset, 12.0f)
					.setColor(glm::vec4(0.8, 0.4, 0.1, 0.0));
				_terrainEditor.addChild<GUIToggle>("addHeight", 185, _top - _offset, false);

				_offset += 45;

				const int c_numMaterials = 4;

				for (int index = 1; index <= c_numMaterials; ++index){
					const std::string _key("material" + std::to_string(index));
					auto &_material = _terrainEditor.addChild<GUIButton>
						(_key, 15 + (index - 1) * 60, _top - _offset, 40, 40, 2);

					_material.setAction([&_terrainEditor, c_numMaterials, index]{
						for (int i = 1; i <= c_numMaterials; ++i){
							const std::string _key("material" + std::to_string(i));
							auto &_button = _terrainEditor.getChild<GUIButton>(_key);

							_button.setColor(i == index ? glm::vec4(0.8f, 0.2f, 0.05f, 0.0f) : 
								glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));

							_button.setActive(i == index ? true : false);
						}
					});
				}

				//Activate first material as default
				_terrainEditor.getChild<GUIButton>("material1").getAction()();
				
				_offset += 20;

				//hardcoded
				_offset = addSlider(_terrainEditor, "Radius", 5.0f, 250.0f, _offset);
				_terrainEditor.getChild<GUISlider>("radius").setValue(15.0f);

				_offset = addSlider(_terrainEditor, "Height Scale", 0.1f, 7.5f, _offset);
				_terrainEditor.getChild<GUISlider>("heightScale").setValue(3.0f);

				_offset = addSlider(_terrainEditor, "Opacity", 0.01f, 1.0f, _offset);
				_terrainEditor.getChild<GUISlider>("opacity").setValue(1.0f);


				_terrainEditor.addChild<GUIButton>("updateTerrain", 5, _top - _offset, 100, 16, 2)
					.addChild<GUIText>("updateT", "Update Terrain", 9, 0, 12.0f);
				
			}
			
			auto &_tab = index->getChild<GUIWindow>("tab").getChild<GUITab>("tab");
			auto &_stack = _tab.getWindow("environment").getChild<GUIWindowStack>("stack"); 
			_stack.closeWindow("terrainEditor");
		}

		void createColorEditorGUI(GUIBase *index){
			auto &_colorEditor = createNewWindow(index, "colorEditor", "environment", 400);
			{
				int _offset = 26;
				int _top = _colorEditor.getScale().y;

				//Add RGB sliders before so they're drawn before the dropdown menu
				//TODO fix order in the GUI draw method
				int _afterOffset = addXYZSliders(_colorEditor, "Color", 0.0f, 1.0f, _offset + 26, true);

				//Texture
				_colorEditor.addChild<GUIBase>("texture", 5, _top - _offset - 345, 245, 245);

				_colorEditor.addChild<GUIButton>("reset", 5, _top - _offset - 370, 90, 18, 2)
					.addChild<GUIText>("resetT", "Reset Colors", 9, 0, 12.0f);


				_colorEditor.addChild<GUIText>("EditHeightT", "Edit Height", 5, _top - _offset, 12.0f)
					.setColor(glm::vec4(0.8, 0.4, 0.1, 0.0));
				auto &_clothing = _colorEditor.addChild<GUIDropdown>(
					"clothing", 70, _top - _offset, 180, 12);

				_clothing.addItem("SHIRT"); //RED
				_clothing.addItem("HAIR"); //BLACK
				_clothing.addItem("SKIN"); //WHITE
				_clothing.addItem("BELT"); //CYAN
				_clothing.addItem("SKIRT"); //YELLOW
				_clothing.addItem("SKIRT_SIDES"); //GREEN
				_clothing.addItem("RIMS"); //BLUE
				_clothing.addItem("SHOES_GLOVES"); //PURPLE
			}

			auto &_tab = index->getChild<GUIWindow>("tab").getChild<GUITab>("tab");
			auto &_stack = _tab.getWindow("environment").getChild<GUIWindowStack>("stack");
			_stack.closeWindow("colorEditor");
		}
		

		void createMatrixGUI(GUIBase *index){

			auto &_matrix = createNewWindow(index, "matrix", "components", 125);
			{
				int _top = _matrix.getScale().y;
				
				// LOCATION
				auto &_location = _matrix.addChild<GUIText>(
					"location", "Location", 20, _top - 23, 12.0f);
				_location.setColor(glm::vec4(0.8, 0.4, 0.1, 0.0));

				auto &_locationX = _matrix.addChild<GUITextBox>(
					"locationX", 11, _top - 43, 75, 20, TEXT_BOX_FLOAT, 3);
				_locationX.addChild<GUIText>("X", "X", -10, 2, 10.0f);

				auto &_locationY = _matrix.addChild<GUITextBox>(
					"locationY", 96, _top - 43, 75, 20, TEXT_BOX_FLOAT, 3);
				_locationY.addChild<GUIText>("Y", "Y", -10, 2, 10.0f);

				auto &_locationZ = _matrix.addChild<GUITextBox>(
					"locationZ", 181, _top - 43, 75, 20, TEXT_BOX_FLOAT, 3);
				_locationZ.addChild<GUIText>("Z", "Z", -10, 2, 10.0f);
				
				
				// ROTATION
				auto &_rotation = _matrix.addChild<GUIText>(
					"rotation", "Rotation", 20, _top - 63, 12.0f);
				_rotation.setColor(glm::vec4(0.8, 0.4, 0.1, 0.0));
				

				auto &_rotationX = _matrix.addChild<GUITextBox>(
					"rotationX", 11, _top - 83, 75, 20, TEXT_BOX_FLOAT, 3);
				_rotationX.addChild<GUIText>("X", "X", -10, 2, 10.0f);

				auto &_rotationY = _matrix.addChild<GUITextBox>(
					"rotationY", 96, _top - 83, 75, 20, TEXT_BOX_FLOAT, 3);
				_rotationY.addChild<GUIText>("Y", "Y", -10, 2, 10.0f);

				auto &_rotationZ = _matrix.addChild<GUITextBox>(
					"rotationZ", 181, _top - 83, 75, 20, TEXT_BOX_FLOAT, 3);
				_rotationZ.addChild<GUIText>("Z", "Z", -10, 2, 10.0f);


				// SCALE
				auto &_scale = _matrix.addChild<GUIText>(
					"scale", "Scale", 20, _top - 103, 12.0f);
				_scale.setColor(glm::vec4(0.8, 0.4, 0.1, 0.0));


				auto &_scaleX = _matrix.addChild<GUITextBox>(
					"scaleX", 11, _top - 123, 75, 20, TEXT_BOX_FLOAT, 3);
				_scaleX.addChild<GUIText>("X", "X", -10, 2, 10.0f);

				auto &_scaleY = _matrix.addChild<GUITextBox>(
					"scaleY", 96, _top - 123, 75, 20, TEXT_BOX_FLOAT, 3);
				_scaleY.addChild<GUIText>("Y", "Y", -10, 2, 10.0f);

				auto &_scaleZ = _matrix.addChild<GUITextBox>(
					"scaleZ", 181, _top - 123, 75, 20, TEXT_BOX_FLOAT, 3);
				_scaleZ.addChild<GUIText>("Z", "Z", -10, 2, 10.0f);
				

			}



		}

		void createCollisionGUI(GUIBase *index){
			auto &_collision = createNewWindow(index, "collision", "components", 530);
			
			int _offset = 63;
			int _top = _collision.getScale().y;

			// IS KINEMATIC
			_collision.addChild<GUIText>(
				"tKinematic", "Kinematic Body", 20, _top - 23, 12.0f)
				.setColor(glm::vec4(0.8, 0.4, 0.1, 0.0));
			_collision.addChild<GUIToggle>("kinematic", 155, _top - 24);

			_offset = addSlider(_collision, "Mass", 0.0f, 1000.0f, _offset);
			_offset = addXYZSliders(_collision, "Linear Factor", 0.0f, 1.0f, _offset, false);
			_offset = addXYZSliders(_collision, "Angular Factor", 0.0f, 1.0f, _offset, false);
			_offset = addSlider(_collision, "Friction", 0.0f, 1.0f, _offset);
			_offset = addSlider(_collision, "Rolling Friction", 0.0f, 1.0f, _offset);
			_offset = addSlider(_collision, "Restitution", 0.0f, 1.0f, _offset);
			_offset = addSlider(_collision, "Linear Damping", 0.0f, 1.0f, _offset);
			_offset = addSlider(_collision, "Angular Damping", 0.0f, 1.0f, _offset);
			_offset = addSlider(_collision, "Linear Sleeping Threshold", 0.0f, 1.0f, _offset);
			_offset = addSlider(_collision, "Angular Sleeping Threshold", 0.0f, 1.0f, _offset);


			// FIRST ITEM
			// COLLISION MESH TYPE
			_collision.addChild<GUIText>(
				"tMeshType", "Collision Mesh Type", 20, _top - 43, 12.0f)
				.setColor(glm::vec4(0.8, 0.4, 0.1, 0.0));

			auto &_meshTypes = _collision.addChild<GUIDropdown>(
				"meshType", 150, _top - 45, 100, 12);
			_meshTypes.addItems("SPHERE", "CAPSULE", "CUBE", "SIMPLIFIED", "COMPLEX");

		}

		void createMeshGUI(GUIBase *index){
			auto &_mesh = createNewWindow(index, "mesh", "components", 120);
			
			int _top = _mesh.getScale().y;

			//
			// MESH PATH
			//
			auto &_meshPath = _mesh.addChild<GUIText>(
				"tMeshPath", "Mesh Path", 20, _top - 23, 12.0f);
			_meshPath.setColor(glm::vec4(0.8, 0.4, 0.1, 0.0));

			_mesh.addChild<GUITextBox>(
				"meshPath", 11, _top - 43, 235, 20, TEXT_BOX_STRING, 3);

			//
			// TEXTURE PATH
			//
			auto &_texturePath = _mesh.addChild<GUIText>(
				"tTexturePath", "Texture Path", 20, _top - 63, 12.0f);
			_texturePath.setColor(glm::vec4(0.8, 0.4, 0.1, 0.0));

			_mesh.addChild<GUITextBox>(
				"texturePath", 11, _top - 83, 235, 20, TEXT_BOX_STRING, 3);

			//
			// VERTEX COUNT
			//
			auto &_vertexCount = _mesh.addChild<GUIText>(
				"tVertexCount", "Vertex count: ", 13, _top - 103, 12.0f);
			_vertexCount.setColor(glm::vec4(0.8, 0.4, 0.1, 0.0));

			_mesh.addChild<GUIText>(
				"vertexCount", "1000", _vertexCount.getScale().x + 15, _top - 103, 12.0f);

			//
			// POLYGON COUNT
			//
			auto &_polyCount = _mesh.addChild<GUIText>(
				"tPolyCount", "Polygon count: ", 13, _top - 117, 12.0f);
			_polyCount.setColor(glm::vec4(0.8, 0.4, 0.1, 0.0));

			_mesh.addChild<GUIText>(
				"polyCount", "1000", _polyCount.getScale().x + 15, _top - 117, 12.0f);

		}

		void createParticleGUI(GUIBase *index){
			auto &_particles = createNewWindow(index, "particles", "components", 530);
			addWindowX(_particles);

			int _offset = 43;
			int _top = _particles.getScale().y;

			// IS ANIMATED
			_particles.addChild<GUIText>(
				"tAnimated", "Animated Particles", 20, _top - 23, 12.0f)
				.setColor(glm::vec4(0.8, 0.4, 0.1, 0.0));
			_particles.addChild<GUIToggle>("animated", 155, _top - 24);

			_offset = addSlider(_particles, "Max Particles", 1.0f, 1000.0f, _offset, TEXT_BOX_INT);
			_offset = addXYZSliders(_particles, "Direction", -1.0f, 1.0f, _offset, false);
			_offset = addXYZSliders(_particles, "Gravity", -50.0f, 50.0f, _offset, false);
			_offset = addSlider(_particles, "Velocity", 0.05f, 100.0f, _offset);
			_offset = addSlider(_particles, "Lifetime", 0.1f, 15.0f, _offset);
			_offset = addSlider(_particles, "Spawn Spread", 0.0f, 50.0f, _offset);
			_offset = addSlider(_particles, "Movement Spread", 0.0f, 50.0f, _offset);

			//
			// TEXTURE TILE COUNT
			//
			_particles.addChild<GUIText>(
				"tTileCount", "Texture Tile Count", 20, _top - 403, 12.0f)
			.setColor(glm::vec4(0.8, 0.4, 0.1, 0.0));

			_particles.addChild<GUIBase>("texture", 15, _top - 440, 35, 35, 3);

			auto &_tileCountX = _particles.addChild<GUISlider>(
				"tileCountX", 1.0f, 12.0f, 60, _top - 420, 135, 15, 0, TEXT_BOX_INT);
			_tileCountX.addChild<GUIText>("X", "X", -10, 0, 10.0f);

			auto &_tileCountY = _particles.addChild<GUISlider>(
				"tileCountY", 1.0f, 12.0f, 60, _top - 440, 135, 15, 0, TEXT_BOX_INT);
			_tileCountY.addChild<GUIText>("Y", "Y", -10, 0, 10.0f);

			auto &_tileCount = _particles.addChild<GUISlider>(
				"tileCount", 1.0f, 144.0f, 15, _top - 460, 180, 15, 0, TEXT_BOX_INT);
			_tileCount.addChild<GUIText>("XY", "XY", -13, 0, 10.0f);

			//
			// TEXTURE PATH
			//
			_particles.addChild<GUIText>(
				"tTexturePath", "Texture Path", 20, _top - 483, 12.0f)
				.setColor(glm::vec4(0.8, 0.4, 0.1, 0.0));

			auto &_texturePath = _particles.addChild<GUITextBox>("texturePath", 
				15, _top - 503, 235, 20, TEXT_BOX_STRING, 3);

		}

		void createLightGUI(GUIBase *index){
			auto &_light = createNewWindow(index, "light", "components", 360);
			addWindowX(_light);

			int _offset = 43;
			int _top = _light.getScale().y;

			_offset = addXYZSliders(_light, "Color", 0.0f, 1.0f, _offset, true);
			_offset = addXYZSliders(_light, "Direction", -1.0f, 1.0f, _offset, false);

			_light.addChild<GUIText>(
				"tLightType", "Light Type", 20, _top - 23, 12.0f)
				.setColor(glm::vec4(0.8, 0.4, 0.1, 0.0));

			auto &_lightTypes = _light.addChild<GUIDropdown>(
				"lightType", 120, _top - 25, 100, 12);
			_lightTypes.addItems("POINT", "SPOTLIGHT", "DIRECTIONAL");

			_offset = addSlider(_light, "Intensity", 0.0f, 50.0f, _offset);
			_offset = addSlider(_light, "LinearAttenuation", 0.1f, 10.0f, _offset);
			_offset = addSlider(_light, "QuadricAttenuation", 0.01f, 1.0f, _offset);
			_offset = addSlider(_light, "Cone Angle", 0.0f, 90.0f, _offset);
			
		}

		void createSoundGUI(GUIBase *index){
			auto &_sound = createNewWindow(index, "sound", "components", 280);
			addWindowX(_sound);

			int _offset = 43;
			int _top = _sound.getScale().y;

			_offset = addSlider(_sound, "Volume", 0.0f, 1.0f, _offset);
			_offset = addSlider(_sound, "Radius", 0.1f, 1500.0f, _offset);
			_offset = addSlider(_sound, "Rolloff Factor", 0.1f, 20.0f, _offset);
			_offset = addSlider(_sound, "Reference Distance", 0.2f, 500.0f, _offset);
			_offset = addSlider(_sound, "Max Distance", 1.0f, 1000.0f, _offset);
			
			//
			// SOUND PATH
			//
			_sound.addChild<GUIText>(
				"tSoundPath", "Sound Path", 20, _top - _offset, 12.0f)
				.setColor(glm::vec4(0.8, 0.4, 0.1, 0.0));

			_sound.addChild<GUITextBox>("soundPath",
				15, _top - (_offset + 20), 235, 20, TEXT_BOX_STRING, 3);

			// FIRST
			// SCRIPT TYPE
			//
			_sound.addChild<GUIText>(
				"tSoundType", "Sound Type", 20, _top - 23, 12.0f)
				.setColor(glm::vec4(0.8, 0.4, 0.1, 0.0));

			auto &_soundType = _sound.addChild<GUIDropdown>(
				"soundType", 120, _top - 25, 100, 12);
			_soundType.addItems("TRIGGER", "CONTINUOUS", "GLOBAL");

		}

		void createScriptGUI(GUIBase *index){
			auto &_script = createNewWindow(index, "script", "components", 145);
			addWindowX(_script);
	
			int _offset = 48;
			int _top = _script.getScale().y;

			//
			// RESET SCRIPT
			//
			_script.addChild<GUIButton>("resetScript", 20, _top - _offset, 95, 15)
				.addChild<GUIText>("resetScript", "Reset Script", 0, 1, 12.0f, ALIGN_CENTER)
				.updateAlignment();

			_script.addChild<GUIButton>("resetVariables", 125, _top - _offset , 95, 15)
				.addChild<GUIText>("resetVariables", "Reset Variables", 0, 1, 12.0f, ALIGN_CENTER)
				.updateAlignment();

			//RADIUS
			_offset = addSlider(_script, "Radius", 0.1f, 1500.0f, _offset + 25);

			//
			// SCRIPT PATH
			//
			_script.addChild<GUIText>(
				"tScriptPath", "Script Path", 20, _top - _offset, 12.0f)
				.setColor(glm::vec4(0.8, 0.4, 0.1, 0.0));

			_script.addChild<GUITextBox>("scriptPath",
				15, _top - (_offset + 20), 235, 20, TEXT_BOX_STRING, 3);

			


			// FIRST
			// SCRIPT TYPE
			//
			_script.addChild<GUIText>(
				"tScriptType", "Script Type", 20, _top - 23, 12.0f)
				.setColor(glm::vec4(0.8, 0.4, 0.1, 0.0));

			auto &_scriptType = _script.addChild<GUIDropdown>(
				"scriptType", 120, _top - 25, 100, 12);
			_scriptType.addItems("TRIGGER", "CONTINUOUS", "GLOBAL");

		}

		void createPlayerGUI(GUIBase *index){
			auto &_player = createNewWindow(index, "player", "components", 145);
			addWindowX(_player);

			int _offset = 48;
			int _top = _player.getScale().y;
		}

		void createMaterialGUI(GUIBase *index){
			auto &_color = createNewWindow(index, "material","components", 95);
			addWindowX(_color);

			int _top = _color.getScale().y;

			addXYZSliders(_color, "Color", -1.0f, 1.0f, 25, true);
		}


		void createGUI(int screenWidth, int screenHeight, GUIBase *index){
			createEditorGUI(screenWidth, screenHeight, index);
			createRenderGUI(index);
			createWaterGUI(index);
			createTerrainGUI(index);
			createTerrainEditorGUI(index);
			createColorEditorGUI(index);
			createMatrixGUI(index);
			createCollisionGUI(index);
			createMeshGUI(index);
			createMaterialGUI(index);
			createParticleGUI(index);
			createLightGUI(index);
			createSoundGUI(index);
			createScriptGUI(index);
			createPlayerGUI(index);
		}

	} 
}