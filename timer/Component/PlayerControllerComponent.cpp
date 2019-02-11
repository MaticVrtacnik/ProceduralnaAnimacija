#include "PlayerControllerComponent.hpp"

#include "../Define/PrintDefines.hpp"
#include "../Collision/Collision.hpp"


namespace Engine{
	namespace Entities{
		namespace Components{
			using namespace Physics;

			PlayerControllerComponent::PlayerControllerComponent() :
				CharacterController()
			{
				//enableGravity();
			}

			PlayerControllerComponent::PlayerControllerComponent(Collision &collision) :
				CharacterController(collision)
			{
				//TODO add real filter groups
				PRINT("INITED CONTROLLER");

				auto _ghostObject = m_controllerInterface->getGhostObject();
				collision.getWorld().removeCollisionObject(_ghostObject);

				collision.getWorld().addCollisionObject(
					_ghostObject,
					btBroadphaseProxy::CharacterFilter,
					btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter
				);
			

				enableGravity();
			}

			PlayerControllerComponent::~PlayerControllerComponent(){

			}

		}
	}
}
