#include "GUISystem.hpp"

#include "../World/World.hpp"
#include "../World/WorldVariables.hpp"
#include "../Collision/Collision.hpp"
#include "../Utility/CollisionUtilities.hpp"
#include "../Component/ComponentIncluder.hpp"
#include "../GUI/GUI.hpp"
#include "../GUI/GUIIncluder.hpp"


namespace Engine{
	namespace GUIHelper{

		inline void setSliderValue(
			GUI::GUIWindow &window,
			const std::string &name,
			float value)
		{
			window.getChild<GUI::GUISlider>(name).setValue(value);
		}

		inline void setSliderXYZValues(
			GUI::GUIWindow &window,
			const std::string &name,
			const glm::vec3 &values,
			bool RGB)
		{
			setSliderValue(window, name + (RGB ? "R" : "X"), values.x);
			setSliderValue(window, name + (RGB ? "G" : "Y"), values.y);
			setSliderValue(window, name + (RGB ? "B" : "Z"), values.z);
		}


		inline void setTextboxValue(
			GUI::GUIWindow &window,
			const std::string &name,
			float value)
		{
			window.getChild<GUI::GUITextBox>(name).setValue(value);
		}

		inline void setTextboxXYZValues(
			GUI::GUIWindow &window,
			const std::string &name,
			const glm::vec3 &values,
			bool RGB)
		{
			setTextboxValue(window, name + (RGB ? "R" : "X"), values.x);
			setTextboxValue(window, name + (RGB ? "G" : "Y"), values.y);
			setTextboxValue(window, name + (RGB ? "B" : "Z"), values.z);
		}

	}
}


namespace Engine{
	namespace Entities{
		namespace Systems{
			using namespace Physics;


			GUISystem::GUISystem(){

			}

			GUISystem::~GUISystem(){

			}



			void GUISystem::init(){
				auto &_variables = getWorld().getVariables();
				
			}

			void GUISystem::preUpdate(float fps){
				//TODO update GUI for selected entity
			}

			void GUISystem::update(float fps){

			}

			void GUISystem::postUpdate(float fps){

			}



			void GUISystem::setGUIComponent(const std::string &windowName, int windowHeight, bool on){
				auto &_windowsStack = getWorld().getVariables().m_gui->getIndex().
					getChild<GUI::GUIWindow>("tab").getChild<GUI::GUITab>("tab").
					getWindow("components").getChild<GUI::GUIWindowStack>("stack");


				auto &_window = _windowsStack.getWindow(windowName);
				auto &_button = _window.getChild<GUI::GUIButton>("x");

				auto &_x = _button.getChild<GUI::GUIText>("x");
				_x.setString(on ? "x" : "+", GUI::ALIGN_LEFT);
				_x.setLocation(_x.getRelativeLocation().x,
					_x.getRelativeLocation().y);

				int _newScaleY = on ? windowHeight : 25;
				_button.setLocation(_button.getRelativeLocation().x,
					_newScaleY - 15 - _window.getBorder());

				_window.setScale(_window.getScale().x, _newScaleY);
				_window.setLocation(0, -_newScaleY);

				for (auto &_child : _window.getChildren()){
					//we dont want to close button for adding/deleting components
					if (_child.second != &_button){
						_child.second->setVisible(on);
						if (on)_child.second->setUpdated(true);
					}
				}

				//decide if window will be closed on selected entity switch
				_windowsStack.setWindowVisible(windowName, on);
			}
			
			void GUISystem::setGUIValuesFromEntity(const Entity &entity){
				auto &_windowsStack = getWorld().getVariables().m_gui->getIndex().
					getChild<GUI::GUIWindow>("tab").getChild<GUI::GUITab>("tab").
					getWindow("components").getChild<GUI::GUIWindowStack>("stack");

				if (!entity.isValid()){
					for (auto &_child : _windowsStack.getChildren()){
						_windowsStack.setWindowVisible(_child.second->getName(), false, false);
					}
					_windowsStack.updateLocations();
					_windowsStack.setEnabled(false);
					return;
				}else{
					_windowsStack.setEnabled(true);
				}


				//Set stack windows visible if entity has the required component
				setGUIComponent("material", 95, entity.hasComponent<MaterialComponent>());
				setGUIComponent("light", 360, entity.hasComponent<LightComponent>());
				setGUIComponent("player", 145, entity.hasComponent<PlayerControllerComponent>());


				// MATRIX
				if (entity.hasComponent<MatrixComponent>())
				{
					auto &_matrix = entity.getComponent<MatrixComponent>();

					auto &_matrixGUI = _windowsStack.getWindow("matrix");
					{
						GUIHelper::setTextboxXYZValues(_matrixGUI, "location",
							_matrix.getLocation(), false);
						GUIHelper::setTextboxXYZValues(_matrixGUI, "rotation",
							glm::eulerAngles(_matrix.getRotation()), false);
						GUIHelper::setTextboxXYZValues(_matrixGUI, "scale",
							_matrix.getScale(), false);
					}
				}

				// COLLISION
				if (entity.hasComponent<CollisionComponent>())
				{
					auto &_collision = entity.getComponent<CollisionComponent>();
					auto _body = _collision.getBody();

					auto &_collisionGUI = _windowsStack.getWindow("collision");
					{
						_collisionGUI.getChild<GUI::GUIToggle>("kinematic").setOn(_collision.isKinematic());
						_collisionGUI.getChild<GUI::GUIDropdown>("meshType").setValue(_collision.getMeshType());

						GUIHelper::setSliderXYZValues(_collisionGUI, "linearFactor",
							Physics::convertVec3(_body->getLinearFactor()), false);
						GUIHelper::setSliderXYZValues(_collisionGUI, "angularFactor",
							Physics::convertVec3(_body->getAngularFactor()), false);

						auto &_mass = _collisionGUI.getChild<GUI::GUISlider>("mass");
						bool _isComplex = _collision.getMeshType() == Physics::COLLISION_COMPLEX;
						_mass.setEnabled(!_isComplex); //complex meshes cannot be dynamic

						GUIHelper::setSliderValue(_collisionGUI, "mass", _isComplex ? 0.0f : _collision.getMass());
						GUIHelper::setSliderValue(_collisionGUI, "friction", _body->getFriction());
						GUIHelper::setSliderValue(_collisionGUI, "rollingFriction", _body->getRollingFriction());
						GUIHelper::setSliderValue(_collisionGUI, "restitution", _body->getRestitution());
						GUIHelper::setSliderValue(_collisionGUI, "linearDamping", _body->getLinearDamping());
						GUIHelper::setSliderValue(_collisionGUI, "angularDamping", _body->getAngularDamping());
						GUIHelper::setSliderValue(_collisionGUI, "linearSleepingThreshold", _body->getLinearSleepingThreshold());
						GUIHelper::setSliderValue(_collisionGUI, "angularSleepingThreshold", _body->getAngularSleepingThreshold());
					}
				}
				

				// MESH
				if (entity.hasComponent<MeshComponent>())
				{
					auto &_renderer = entity.getComponent<MeshComponent>();
					auto &_mesh = _renderer.getMesh();
					auto _meshGUI = _windowsStack.getWindow("mesh");
					{
						_meshGUI.getChild<GUI::GUITextBox>("meshPath").setValue(_mesh.m_path);

						_meshGUI.getChild<GUI::GUIText>("vertexCount").setString(
							std::to_string(_mesh.m_vertexData.size()), GUI::ALIGN_LEFT);

						_meshGUI.getChild<GUI::GUIText>("polyCount").setString(
							std::to_string(_mesh.m_indices.size() / 3), GUI::ALIGN_LEFT);
					}
				}


				// MATERIAL
				if (entity.hasComponent<MaterialComponent>())
				{
					auto &_color = entity.getComponent<MaterialComponent>();
					auto &_materialGUI = _windowsStack.getWindow("material"); 
					{
						GUIHelper::setSliderXYZValues(_materialGUI, "color", _color.getColor(), true);
					}
				}



				// LIGHT
				if (entity.hasComponent<LightComponent>())
				{
					auto &_light = entity.getComponent<LightComponent>();	
					auto &_lightGUI = _windowsStack.getWindow("light");
					{
						GUIHelper::setSliderXYZValues(_lightGUI, "color", _light.m_color, true);
						GUIHelper::setSliderXYZValues(_lightGUI, "direction", _light.getDirection(), false);
						GUIHelper::setSliderValue(_lightGUI, "intensity", _light.m_intensity);
						GUIHelper::setSliderValue(_lightGUI, "linearAttenuation", _light.m_linearAttenuation);
						GUIHelper::setSliderValue(_lightGUI, "quadricAttenuation", _light.m_quadricAttenuation);
						GUIHelper::setSliderValue(_lightGUI, "coneAngle", _light.getConeAngle());

						_lightGUI.getChild<GUI::GUIDropdown>("lightType").setValue(_light.getType());
						_lightGUI.getChild<GUI::GUISlider>("coneAngle").setEnabled(_light.getType() == LIGHT_SPOTLIGHT);
					}
				}

				// AI CONTROLLER
				//TODO change to NPC CONTROLLER
				if (entity.hasComponent<PlayerControllerComponent>())
				{
					auto &_playerController = entity.getComponent<PlayerControllerComponent>();
					auto &_playerGUI = _windowsStack.getWindow("player");
					{

					}
				}

			}

			void GUISystem::setEntityValuesFromGUI(const Entity &entity,
				const std::set<Entity*> &selectedEntities)
			{
				if (!entity.isValid()){
					PRINT("Can't set GUI values from invalid entity");
					return;
				}


				auto &_variables = getWorld().getVariables();
				auto &_resourceManager = getWorld().getResourceManager();

				auto &_gui = _variables.m_gui->getIndex();
				auto &_explorer = _gui.getChild<GUI::GUIFileExplorer>("explorer");
				auto &_windows = _gui.getChild<GUI::GUIWindow>("tab").getChild<GUI::GUITab>("tab")
					.getWindow("components").getChild<GUI::GUIWindowStack>("stack");


				auto &_matrix = entity.getComponent<MatrixComponent>();
				auto &_collision = entity.getComponent<CollisionComponent>();
				const auto &_body = _collision.getBody();


				auto &_matrixGUI = _windows.getWindow("matrix");
				{

					auto &_locationX = _matrixGUI.getChild<GUI::GUITextBox>("locationX");
					auto &_locationY = _matrixGUI.getChild<GUI::GUITextBox>("locationY");
					auto &_locationZ = _matrixGUI.getChild<GUI::GUITextBox>("locationZ");

					if (_locationX.isUpdated() || _locationY.isUpdated() || _locationZ.isUpdated()){
						Location _newLocation(_locationX.getFloatValue(),
							_locationY.getFloatValue(), _locationZ.getFloatValue());

						_matrix.setLocation(_newLocation);
						_variables.m_collision->setBodyLocation(*_body, _newLocation);

						//TODO
					}

					//TODO TRANSFORM COLLISION BODY

					//TODO CONVERT TO EULER ANGLES
					auto &_rotationX = _matrixGUI.getChild<GUI::GUITextBox>("rotationX");
					auto &_rotationY = _matrixGUI.getChild<GUI::GUITextBox>("rotationY");
					auto &_rotationZ = _matrixGUI.getChild<GUI::GUITextBox>("rotationZ");

					if (_rotationX.isUpdated() || _rotationY.isUpdated() || _rotationZ.isUpdated()){
						_matrix.setRotation(Rotation(_rotationX.getFloatValue(),
							_rotationY.getFloatValue(), _rotationZ.getFloatValue()));
					}


					auto &_scaleX = _matrixGUI.getChild<GUI::GUITextBox>("scaleX");
					auto &_scaleY = _matrixGUI.getChild<GUI::GUITextBox>("scaleY");
					auto &_scaleZ = _matrixGUI.getChild<GUI::GUITextBox>("scaleZ");

					if (_scaleX.isUpdated() || _scaleY.isUpdated() || _scaleZ.isUpdated()){
						Scale _newScale(_scaleX.getFloatValue(), _scaleY.getFloatValue(), _scaleZ.getFloatValue());
						_variables.m_collision->setBodyScale(*_body, _newScale);
						_matrix.setScale(_newScale);
					}

				}


				auto &_collisionGUI = _windows.getWindow("collision");
				{
					// KINEMATIC
					auto &_kinematic = _collisionGUI.getChild<GUI::GUIToggle>("kinematic");
					if (_kinematic.isUpdated()){
						for (auto &_entity : selectedEntities){
							_entity->getComponent<CollisionComponent>().setKinematicVariable(_kinematic.isOn());
						}
					}


					// MASS
					auto &_meshType = _collisionGUI.getChild<GUI::GUIDropdown>("meshType");
					auto &_mass = _collisionGUI.getChild<GUI::GUISlider>("mass");
					_mass.setEnabled(_meshType.getValueId() != Physics::COLLISION_COMPLEX);

					if (_mass.isUpdated()){
						for (auto &_entity : selectedEntities)
							_entity->getComponent<CollisionComponent>().setMassVariable(_mass.getFloatValue());
					}


					// COLLISION MESH TYPE
					if (entity.hasComponent<MeshComponent>()){
						if (_meshType.isUpdated()){
							auto &_mesh = entity.getComponent<MeshComponent>().getMesh();

							for (auto &_entity : selectedEntities){
								auto &_collisionComponent = _entity->getComponent<CollisionComponent>();
								auto _newMeshType = static_cast<CollisionMeshType>(_meshType.getValueId());

								_collisionComponent.setMeshType(_newMeshType, _mesh);
							}
						}
					}


					// LINEAR FACTOR
					auto &_linearFactorX = _collisionGUI.getChild<GUI::GUISlider>("linearFactorX");
					auto &_linearFactorY = _collisionGUI.getChild<GUI::GUISlider>("linearFactorY");
					auto &_linearFactorZ = _collisionGUI.getChild<GUI::GUISlider>("linearFactorZ");
					if (_linearFactorX.isUpdated() || _linearFactorY.isUpdated() || _linearFactorZ.isUpdated())
					{
						_body->setLinearFactor(btVector3(_linearFactorX.getFloatValue(),
							_linearFactorY.getFloatValue(), _linearFactorZ.getFloatValue()));
					}


					// ANGULAR FACTOR
					auto &_angularFactorX = _collisionGUI.getChild<GUI::GUISlider>("angularFactorX");
					auto &_angularFactorY = _collisionGUI.getChild<GUI::GUISlider>("angularFactorY");
					auto &_angularFactorZ = _collisionGUI.getChild<GUI::GUISlider>("angularFactorZ");
					if (_angularFactorX.isUpdated() || _angularFactorY.isUpdated() || _angularFactorZ.isUpdated())
					{
						_body->setAngularFactor(btVector3(_angularFactorX.getFloatValue(),
							_angularFactorY.getFloatValue(), _angularFactorZ.getFloatValue()));
					}


					// FRICTION
					auto &_friction = _collisionGUI.getChild<GUI::GUISlider>("friction");
					if (_friction.isUpdated()){
						_body->setFriction(_friction.getFloatValue());
					}


					// ROLLING FRICTION
					auto &_rollingFriction = _collisionGUI.getChild<GUI::GUISlider>("rollingFriction");
					if (_rollingFriction.isUpdated()){
						_body->setRollingFriction(_rollingFriction.getFloatValue());
					}


					// RESTITUTION
					auto &_restitution = _collisionGUI.getChild<GUI::GUISlider>("restitution");
					if (_restitution.isUpdated()){
						_body->setRestitution(_restitution.getFloatValue());
					}


					// LINEAR && ANGULAR DAMPINGS
					auto &_linearD = _collisionGUI.getChild<GUI::GUISlider>("linearDamping");
					auto &_angularD = _collisionGUI.getChild<GUI::GUISlider>("angularDamping");
					if (_linearD.isUpdated() || _angularD.isUpdated()){
						_body->setDamping(_linearD.getFloatValue(), _angularD.getFloatValue());
					}


					// LINEAR && ANGULAR SLEEPING THRESHOLDS
					auto &_linearST = _collisionGUI.getChild<GUI::GUISlider>("linearSleepingThreshold");
					auto &_angularST = _collisionGUI.getChild<GUI::GUISlider>("angularSleepingThreshold");
					if (_linearST.isUpdated() || _angularST.isUpdated()){
						_body->setSleepingThresholds(_linearST.getFloatValue(), _angularST.getFloatValue());
					}

				}


				if (entity.hasComponent<MeshComponent>())
				{
					auto &_renderer = entity.getComponent<MeshComponent>();
					auto &_mesh = _renderer.getMesh();

					//TODO
					auto _meshGUI = _windows.getWindow("mesh");
					{
						auto _path = _meshGUI.getChild
							<GUI::GUITextBox>("meshPath").getStringValue();
						_mesh.m_path = _path;


						_meshGUI.getChild<GUI::GUITextBox>("texturePath").setValue(_mesh.m_material);

						_meshGUI.getChild<GUI::GUIText>("vertexCount").setString(
							std::to_string(_mesh.m_vertexData.size()), GUI::ALIGN_LEFT);

						_meshGUI.getChild<GUI::GUIText>("polyCount").setString(
							std::to_string(_mesh.m_indices.size() / 3), GUI::ALIGN_LEFT);

					}

				}


				if (entity.hasComponent<MaterialComponent>()){
					auto &_color = entity.getComponent<MaterialComponent>();
					auto &_materialGUI = _windows.getWindow("material");

					auto &_colorR = _materialGUI.getChild<GUI::GUISlider>("colorR");
					if (_colorR.isUpdated()) _color.setColorR(_colorR.getFloatValue());

					auto &_colorG = _materialGUI.getChild<GUI::GUISlider>("colorG");
					if (_colorG.isUpdated()) _color.setColorG(_colorG.getFloatValue());

					auto &_colorB = _materialGUI.getChild<GUI::GUISlider>("colorB");
					if (_colorB.isUpdated()) _color.setColorB(_colorB.getFloatValue());

				}


				if (entity.hasComponent<LightComponent>()){
					auto &_light = entity.getComponent<LightComponent>();
					auto &_lightGUI = _windows.getWindow("light");

					auto &_directionX = _lightGUI.getChild<GUI::GUISlider>("directionX");
					auto &_directionY = _lightGUI.getChild<GUI::GUISlider>("directionY");
					auto &_directionZ = _lightGUI.getChild<GUI::GUISlider>("directionZ");
					if (_directionX.isUpdated() || _directionY.isUpdated() || _directionZ.isUpdated()){
						//TODO maybe
					}

					auto &_colorR = _lightGUI.getChild<GUI::GUISlider>("colorR");
					auto &_colorG = _lightGUI.getChild<GUI::GUISlider>("colorG");
					auto &_colorB = _lightGUI.getChild<GUI::GUISlider>("colorB");
					if (_colorR.isUpdated() || _colorG.isUpdated() || _colorB.isUpdated()){
						_light.m_color = glm::vec3(_colorR.getFloatValue(),
							_colorG.getFloatValue(), _colorB.getFloatValue());
					}

					auto &_intensity = _lightGUI.getChild<GUI::GUISlider>("intensity");
					if (_intensity.isUpdated()){
						_light.m_intensity = _intensity.getFloatValue();
					}

					auto &_attenuation = _lightGUI.getChild<GUI::GUISlider>("linearAttenuation");
					if (_attenuation.isUpdated()){
						_light.m_linearAttenuation = _attenuation.getFloatValue();
					}

					auto &_shininess = _lightGUI.getChild<GUI::GUISlider>("quadricAttenuation");
					if (_shininess.isUpdated()){
						_light.m_quadricAttenuation = _shininess.getFloatValue();
					}

					auto &_coneAngle = _lightGUI.getChild<GUI::GUISlider>("coneAngle");
					if (_coneAngle.isUpdated()){
						_light.setConeAngle(_coneAngle.getFloatValue());
					}

					auto &_lightType = _lightGUI.getChild<GUI::GUIDropdown>("lightType");
					if (_lightType.isUpdated()){

						LightType _type = static_cast<LightType>(_lightType.getValueId());
						_light.setType(_type);

						_coneAngle.setEnabled(_type == LIGHT_SPOTLIGHT);
						if (_type == LIGHT_SPOTLIGHT){
							if (_coneAngle.getFloatValue() < 0.0f)
								_light.setConeAngle(45.0f);
							_coneAngle.setValue(45.0f);
						} else _coneAngle.setValue(0.0f);
					}

				}

			}

		}
	}
}
