#ifndef RENDERER_SYSTEM_HPP
#define RENDERER_SYSTEM_HPP

#include <glm/glm.hpp>
#include <gl/glew.h>

#include "System.hpp"
#include "../Collision/Collision.hpp"
#include "../TransformVariables.hpp"


namespace Engine{
	namespace Entities{
		namespace Systems{	

			class RendererSystem : public System{
			public:
				static const int c_resolution = 128;
				std::vector<unsigned int> m_radianceTextures, m_normalsTextures, m_depthTextures;
				std::vector<unsigned int>	m_irradianceTextures, m_varianceTextures;
				std::vector<unsigned int>	m_radianceCubemaps;

				unsigned int m_radianceTextureArray, m_normalsTextureArray, m_depthTextureArray;
				unsigned int m_irradianteCubemapArray, m_varianceCubemapArray;
				unsigned int m_radianceCubemapArray;

			protected:
				Entity m_sunEntity;
				std::vector<Entity> m_environmentalProbes;
				
			protected:
				void renderOccludedLights();

				void renderEntitiesTessellated();

				void renderWaterEntities();

				void renderShadows();

				void renderPaintDepth();


				unsigned int getNearestProbe(const glm::vec3 &location);

				void generateProbeTextures();

				void createProbeTextureArrays();

			public:
				RendererSystem();

				~RendererSystem();


				void init() override;

				void preUpdate(float fps) override;

				void update(float fps) override;

				void postUpdate(float fps) override;

				void onEntityAdded(Entity &entity) override;

				void onEntityRemoved(Entity &entity) override;


					
				void renderCollisionBody(btRigidBody *body, const glm::vec4 color = glm::vec4(1.0f));
		
				void renderCollisionShape(btCollisionShape *shape, const Location &location, 
					const Rotation &rotation, const Scale &scale, const glm::vec4 color = glm::vec4(1.0f));

				//name...cube, sphere, etc.
				void renderPrimitive(const std::string &name,
					const glm::mat4 &modelMatrix, const glm::vec4 color = glm::vec4(1.0f));

				void renderPrimitive(const std::string &name, const glm::vec3 &location,
					const glm::quat &rotation, const glm::vec3 &scale, const glm::vec4 color = glm::vec4(1.0f));


				//TODO
				struct LineData{};	
				void drawLine(const glm::vec3 &start, const glm::vec3 &end){

				}

			};
		}
	}
}

#endif //RENDERER_SYSTEM_HPP