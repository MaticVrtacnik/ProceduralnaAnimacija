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


#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <memory>
#include <string>
#include <GL/glew.h>


namespace Engine{ namespace Textures{
	struct Texture;
	enum TextureType;
} }


namespace Engine{
	namespace Textures{

		struct Material{
		public:
			float m_displacementScale = 0.01f;
			float m_friction = 1.0f;
			float m_restitution = 0.1f;			
			std::string m_soundPath = "";

		public:
			std::string m_materialName = "";

		private:
			std::shared_ptr<Texture> m_albedo = nullptr;
			std::shared_ptr<Texture> m_normal = nullptr;
			std::shared_ptr<Texture> m_roughness = nullptr;
			std::shared_ptr<Texture> m_metallic = nullptr;
			std::shared_ptr<Texture> m_occlusion = nullptr;
			std::shared_ptr<Texture> m_height = nullptr;
			std::shared_ptr<Texture> m_opacity = nullptr;

		public:
			Material();
			
			Material(const Material &material);
			
			Material(Material &&material);

			Material(
				std::shared_ptr<Texture> albedo,
				std::shared_ptr<Texture> normal = nullptr,
				std::shared_ptr<Texture> roughness = nullptr,
				std::shared_ptr<Texture> metallic = nullptr,
				std::shared_ptr<Texture> occlusion = nullptr,
				std::shared_ptr<Texture> height = nullptr,
				std::shared_ptr<Texture> opacity = nullptr	
			);
			
			~Material();


			void setTexture(std::shared_ptr<Texture> texture);

			void setTexture(const std::string &path, TextureType type);

			void setTextureId(unsigned int textureId, TextureType type);


			Texture &getTexture(TextureType type) const;

			bool hasTexture(TextureType type) const;


			bool hasAlbedo() const;

			unsigned int getAlbedoTexture() const;

			bool hasNormal() const;

			unsigned int getNormalTexture() const;

			bool hasRoughness() const;

			unsigned int getRoughnessTexture() const;
			
			bool hasMetallic() const;

			unsigned int getMetallicTexture() const;
			
			bool hasOcclusion() const;

			unsigned int getOcclusionTexture() const;
			
			bool hasHeight() const;

			unsigned int getHeightTexture() const;

			bool hasOpacity() const;

			unsigned int getOpacityTexture() const;

		};


		bool loadMaterialConfig(Material &material, const std::string &materialName);
	}
}

#endif //MATERIAL_HPP