// https://github.com/mrdoob/three.js/blob/r129/src/helpers/PointLightHelper.js

#ifndef THREEPP_POINTLIGHTHELPER_HPP
#define THREEPP_POINTLIGHTHELPER_HPP

#include "threepp/geometries/SphereGeometry.hpp"
#include "threepp/lights/PointLight.hpp"
#include "threepp/objects/Mesh.hpp"

namespace threepp {

    class PointLightHelper : public Mesh {

    public:
        void update() {

            this->matrix.copy(this->light->matrixWorld);

            if (this->color) {

                std::dynamic_pointer_cast<MaterialWithColor>(this->material())->color.setHex(*this->color);

            } else {

                std::dynamic_pointer_cast<MaterialWithColor>(this->material())->color.copy(this->light->color);
            }
        }

        static std::shared_ptr<PointLightHelper> create(const std::shared_ptr<PointLight> &light, float sphereSize, std::optional<unsigned int> color = std::nullopt) {

            return std::shared_ptr<PointLightHelper>(new PointLightHelper(light, sphereSize, color));
        }

    protected:
        std::optional<unsigned int> color;
        std::shared_ptr<PointLight> light;

        PointLightHelper(std::shared_ptr<PointLight> light, float sphereSize, std::optional<unsigned int> color)
            : Mesh(nullptr, nullptr), light(std::move(light)), color(color) {

            geometry_ = SphereGeometry::create(sphereSize, 4, 2);

            auto material = MeshBasicMaterial::create();
            material->wireframe = true;
            material->fog = false;
            material->toneMapped = false;
            this->materials_[0] = std::move(material);

            this->light->updateMatrixWorld();

            this->matrixAutoUpdate = false;

            update();
        }
    };

}// namespace threepp

#endif//THREEPP_POINTLIGHTHELPER_HPP
