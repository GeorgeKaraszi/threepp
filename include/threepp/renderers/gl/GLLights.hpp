// https://github.com/mrdoob/three.js/blob/dev/src/renderers/webgl/WebGLLights.js

#ifndef THREEPP_GLLIGHTS_HPP
#define THREEPP_GLLIGHTS_HPP

#include "threepp/lights/lights.hpp"

#include "threepp/math/Vector2.hpp"
#include "threepp/math/Vector3.hpp"

#include <array>
#include <unordered_map>
#include <vector>

namespace threepp::gl {

    namespace {

        bool shadowCastingLightsFirst(const Light *lightA, const Light *lightB) {

            return (lightB->castShadow ? 1 : 0) > (lightA->castShadow ? 1 : 0);
        }

    }// namespace

    typedef std::unordered_map<std::string, NestedUniformValue> LightUniforms;

    struct UniformsCache {

        std::shared_ptr<LightUniforms> get(const Light &light) {

            if (lights.count(light.id)) {

                return lights.at(light.id);
            }

            auto type = light.type();
            std::shared_ptr<LightUniforms> uniforms;
            if (type == "DirectionalLight") {

                uniforms = std::make_shared<LightUniforms>(LightUniforms {
                        {"direction", Vector3()},
                        {"color", Color()}});

            } else if (type == "SpotLight") {

                uniforms = std::make_shared<LightUniforms>(LightUniforms {
                        {"position", Vector3()},
                        {"direction", Vector3()},
                        {"color", Color()},
                        {"distance", 0.f},
                        {"coneCos", 0.f},
                        {"penumbraCos", 0.f},
                        {"decay", 0.f}});

            } else if (type == "PointLight") {

                uniforms = std::make_shared<LightUniforms>(LightUniforms {
                        {"position", Vector3()},
                        {"color", Color()},
                        {"distance", 0.f},
                        {"decay", 0.f}});

            } else if (type == "HemisphereLight") {

                uniforms = std::make_shared<LightUniforms>(LightUniforms {
                        {"direction", Vector3()},
                        {"skyColor", Color()},
                        {"groundColor", Color()}});

            } else if (type == "RectAreaLight") {

                uniforms = std::make_shared<LightUniforms>(LightUniforms {
                        {"color", Color()},
                        {"position", Vector3()},
                        {"halfWidth", Vector3()},
                        {"halfHeight", Vector3()}});
            }

            lights[light.id] = uniforms;

            return lights.at(light.id);
        }

    private:
        std::unordered_map<unsigned int, std::shared_ptr<LightUniforms>> lights;
    };

    struct ShadowUniformsCache {

        LightUniforms &get(const Light &light) {

            if (lights.count(light.id)) {

                return lights.at(light.id);
            }

            auto type = light.type();
            LightUniforms uniforms;
            if (type == "DirectionalLight") {

                uniforms = {
                        {"shadowBias", 0.f},
                        {"shadowNormalBias", 0.f},
                        {"shadowRadius", 1.f},
                        {"shadowMapSize", Vector2()}};

            } else if (type == "SpotLight") {

                uniforms = {
                        {"shadowBias", 0.f},
                        {"shadowNormalBias", 0.f},
                        {"shadowRadius", 1.f},
                        {"shadowMapSize", Vector2()}};

            } else if (type == "PointLight") {

                uniforms = {
                        {"shadowBias", 0.f},
                        {"shadowNormalBias", 0.f},
                        {"shadowRadius", 1.f},
                        {"shadowMapSize", Vector2()},
                        {"shadowCameraNear", 1.f},
                        {"shadowCameraFar", 1000.f}};
            }

            lights[light.id] = uniforms;

            return lights.at(light.id);
        }

    private:
        std::unordered_map<unsigned int, LightUniforms> lights;
    };


    struct GLLights {

        struct LightState {

            struct Hash {

                int directionalLength = -1;
                int pointLength = -1;
                int spotLength = -1;

                int numDirectionalShadows = -1;
                int numPointShadows = -1;
                int numSpotShadows = -1;
            };

            unsigned int version = 0;

            Hash hash{};

            Color ambient{0, 0, 0};
            std::vector<Vector3> probe{9};
            std::vector<std::shared_ptr<LightUniforms>> directional;
            std::vector<LightUniforms> directionalShadow;
            std::vector<std::shared_ptr<Texture>> directionalShadowMap;
            std::vector<Matrix4> directionalShadowMatrix;
            std::vector<std::shared_ptr<LightUniforms>> spot;
            std::vector<LightUniforms> spotShadow;
            std::vector<std::shared_ptr<Texture>> spotShadowMap;
            std::vector<Matrix4> spotShadowMatrix;
            std::vector<std::shared_ptr<LightUniforms>> rectArea;
            std::vector<std::shared_ptr<LightUniforms>> point;
            std::vector<LightUniforms> pointShadow;
            std::vector<std::shared_ptr<Texture>> pointShadowMap;
            std::vector<Matrix4> pointShadowMatrix;
            std::vector<std::shared_ptr<LightUniforms>> hemi;
        };

        LightState state{};

        void setup(std::vector<Light *> &lights);

        void setupView(std::vector<Light *> &lights, Camera *camera);

    private:
        UniformsCache cache_;
        ShadowUniformsCache shadowCache_;

        unsigned int nextVersion = 0;
    };

}// namespace threepp::gl

#endif//THREEPP_GLLIGHTS_HPP
