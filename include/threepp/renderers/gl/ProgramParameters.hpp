
#ifndef THREEPP_PROGRAMPARAMETERS_HPP
#define THREEPP_PROGRAMPARAMETERS_HPP

#include "GLClipping.hpp"
#include "GLLights.hpp"
#include "threepp/core/Uniform.hpp"

#include <optional>
#include <string>
#include <unordered_map>

namespace threepp {

    class GLRenderer;

    namespace gl {

        struct ProgramParameters {

            std::optional<std::string> shaderID;
            std::string shaderName;

            std::unordered_map<std::string, std::string> defines;

            std::string vertexShader;
            std::string fragmentShader;

            bool isRawShaderMaterial{};

            std::string precision = "highp";

            bool instancing{};
            bool instancingColor{};

            bool supportsVertexTextures;
            int outputEncoding{};
            bool map{};
            int mapEncoding{};
            bool matcap{};
            int matcapEncoding{};
            bool envMap{};
            int envMapMode{};
            int envMapEncoding{};
            bool envMapCubeUV{};
            bool lightMap{};
            int lightMapEncoding{};
            bool aoMap{};
            bool emissiveMap{};
            int emissiveMapEncoding{};
            bool bumpMap{};
            bool normalMap{};
            bool objectSpaceNormalMap{};
            bool tangentSpaceNormalMap{};
            bool clearcoatMap;
            bool clearcoatRoughnessMap;
            bool clearcoatNormalMap;
            bool displacementMap{};
            bool roughnessMap;
            bool metalnessMap;
            bool specularMap{};
            bool alphaMap{};

            bool gradientMap{};

            std::optional<Color> sheen;

            bool transmission;
            bool transmissionMap;
            bool thicknessMap;

            std::optional<int> combine;

            bool vertexTangents{};
            bool vertexColors{};
            bool vertexAlphas{};
            bool vertexUvs;
            bool uvsVertexOnly;

            bool fog;
            bool useFog{};
            bool fogExp2{};

            bool flatShading{};

            bool sizeAttenuation{};
            bool logarithmicDepthBuffer{};

            bool skinning;
            int maxBones;
            bool useVertexTexture{};

            bool morphTargets{};
            bool morphNormals{};

            int numDirLights;
            int numPointLights;
            int numSpotLights;
            int numRectAreaLights;
            int numHemiLights;

            int numDirLightShadows;
            int numPointLightShadows;
            int numSpotLightShadows;

            int numClippingPlanes;
            int numClipIntersection{};

            bool dithering{};

            bool shadowMapEnabled{};
            int shadowMapType{};

            int toneMapping{};
            bool physicallyCorrectLights{};

            bool premultipliedAlpha{};

            float alphaTest{};
            bool doubleSided{};
            bool flipSided{};

            int depthPacking{};

            std::optional<std::string> index0AttributeName;

            std::shared_ptr<UniformMap> uniforms;

            ProgramParameters(
                    const GLRenderer &renderer,
                    const GLLights::LightState &lights,
                    int numShadows,
                    const std::shared_ptr<Object3D> &object,
                    const std::shared_ptr<Scene> &scene,
                    Material *material,
                    const std::unordered_map<std::string, std::string> &shaderIDs);

            [[nodiscard]] std::string hash() const;
        };

    }// namespace gl

}// namespace threepp

#endif//THREEPP_PROGRAMPARAMETERS_HPP
