// https://github.com/mrdoob/three.js/blob/r129/src/materials/MeshBasicMaterial.js

#ifndef THREEPP_MESHBASICMATERIAL_HPP
#define THREEPP_MESHBASICMATERIAL_HPP

#include "threepp/materials/Material.hpp"
#include "threepp/math/Color.hpp"
#include "threepp/textures/Texture.hpp"

#include <optional>

namespace threepp {

    class MeshBasicMaterial : public Material, MaterialWithWireframe {

    public:

        Color color = Color(0xffffff);

        std::optional<Texture> map = std::nullopt;

        std::optional<Texture> lightMap = std::nullopt;
        float lightMapIntensity = 1.0;

        std::optional<Texture> aoMap = std::nullopt;
        float aoMapIntensity = 1.0;

        std::optional<Texture> specularMap = std::nullopt;

        std::optional<Texture> alphaMap = std::nullopt;

        std::optional<Texture> envMap = std::nullopt;
        int combine = MultiplyOperation;
        float reflectivity = 1;
        float refractionRatio = 0.98f;

        [[nodiscard]] std::string getWireframeLinecap() const override {
            return wireframeLinecap_;
        }

        [[nodiscard]] std::string getWireframeLinejoin() const override {
            return wireframeLinejoin_;
        }

        [[nodiscard]] bool getWireframe() const override {
            return wireframe_;
        }

        void setWireframe(bool wireframe) override {
            wireframe_ = wireframe;
        }

        [[nodiscard]] float getWireframeLinewidth() const override {
            return wireframeLinewidth_;
        }
        void setWireframeLinewidth(float width) override {
            wireframeLinewidth_ = width;
        }

        static std::shared_ptr<MeshBasicMaterial> create() {
            return std::shared_ptr<MeshBasicMaterial>(new MeshBasicMaterial());
        }

    protected:

        MeshBasicMaterial() = default;

    private:

        std::string wireframeLinecap_ = "round";
        std::string wireframeLinejoin_ = "round";

        bool wireframe_ = false;
        float wireframeLinewidth_ = 1;

    };

}// namespace threepp

#endif//THREEPP_MESHBASICMATERIAL_HPP
