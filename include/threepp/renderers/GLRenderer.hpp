// https://github.com/mrdoob/three.js/blob/r129/src/renderers/WebGLRenderer.js

#ifndef THREEPP_GLRENDERER_HPP
#define THREEPP_GLRENDERER_HPP

#include "threepp/cameras/Camera.hpp"

#include "threepp/math/Plane.hpp"
#include "threepp/math/Vector2.hpp"
#include "threepp/math/Vector4.hpp"
#include <threepp/math/Color.hpp>
#include <threepp/math/Frustum.hpp>

#include "threepp/Canvas.hpp"
#include "threepp/constants.hpp"

#include "threepp/objects/Points.hpp"

#include "threepp/renderers/gl/GLAttributes.hpp"
#include "threepp/renderers/gl/GLBackground.hpp"
#include "threepp/renderers/gl/GLBufferRenderer.hpp"
#include "threepp/renderers/gl/GLCapabilities.hpp"
#include "threepp/renderers/gl/GLClipping.hpp"
#include "threepp/renderers/gl/GLGeometries.hpp"
#include "threepp/renderers/gl/GLInfo.hpp"
#include "threepp/renderers/gl/GLMaterials.hpp"
#include "threepp/renderers/gl/GLObjects.hpp"
#include "threepp/renderers/gl/GLProgram.hpp"
#include "threepp/renderers/gl/GLPrograms.hpp"
#include "threepp/renderers/gl/GLRenderLists.hpp"
#include "threepp/renderers/gl/GLRenderStates.hpp"
#include "threepp/renderers/gl/GLShadowMap.hpp"
#include "threepp/renderers/gl/GLState.hpp"
#include "threepp/renderers/gl/GLTextures.hpp"
#include "threepp/renderers/gl/GLUniforms.hpp"

#include <memory>
#include <vector>

namespace threepp {

    class GLRenderer {

    public:
        struct Parameters {

            bool alpha;
            bool depth;
            bool stencil;
            bool antialias;
            bool premultipliedAlpha;
            bool preserveDrawingBuffer;
        };

        // clearing

        bool autoClear = true;
        bool autoClearColor = true;
        bool autoClearDepth = true;
        bool autoClearStencil = true;

        // scene graph

        bool sortObjects = true;

        // user-defined clipping

        std::vector<Plane> clippingPlanes;
        bool localClippingEnabled = false;

        // physically based shading

        float gammaFactor = 2.0f;// for backwards compatibility
        int outputEncoding = LinearEncoding;

        // physical lights

        bool physicallyCorrectLights = false;

        // tone mapping

        int toneMapping = NoToneMapping;
        float toneMappingExposure = 1.0f;

        bool checkShaderErrors = false;

        gl::GLState state;
        gl::GLShadowMap shadowMap;

        explicit GLRenderer(Canvas &canvas, const Parameters &parameters = Parameters());

        [[nodiscard]] int getTargetPixelRatio() const;

        void setPixelRatio(int value);

        [[nodiscard]] WindowSize getSize() const;

        void setSize(WindowSize size);

        void getDrawingBufferSize(Vector2 &target) const;

        void setDrawingBufferSize(int width, int height, int pixelRatio);

        void getCurrentViewport(Vector4 &target) const;

        void getViewport(Vector4 &target) const;

        void setViewport(const Vector4 &v);

        void setViewport(int x, int y, int width, int height);

        void getScissor(Vector4 &target);

        void setScissor(const Vector4 &v);

        void setScissor(int x, int y, int width, int height);

        [[nodiscard]] bool getScissorTest() const;

        void setScissorTest(bool boolean);

        // Clearing

        void getClearColor(Color &target) const;

        void setClearColor(const Color &color, float alpha = 1);

        [[nodiscard]] float getClearAlpha() const;

        void setClearAlpha(float clearAlpha);

        void clear(bool color = true, bool depth = true, bool stencil = true);

        void clearColor();
        void clearDepth();
        void clearStencil();

        void dispose();

        void deallocateMaterial(Material &material);

        void releaseMaterialProgramReferences(Material &material);

        void renderBufferDirect(
                const std::shared_ptr<Camera> &camera,
                const std::shared_ptr<Scene> &scene,
                const std::shared_ptr<BufferGeometry> &geometry,
                const std::shared_ptr<Material> &material,
                const std::shared_ptr<Object3D> &object,
                std::optional<GeometryGroup> group);

        void render(const std::shared_ptr<Scene> &scene, const std::shared_ptr<Camera> &camera);

        void projectObject(
                const std::shared_ptr<Object3D> &object,
                const std::shared_ptr<Camera> &camera,
                int groupOrder, bool sortObjects);

        void renderTransmissiveObjects(
                std::vector<std::shared_ptr<gl::RenderItem>> &opaqueObjects,
                std::vector<std::shared_ptr<gl::RenderItem>> &transmissiveObjects,
                const std::shared_ptr<Scene> &scene, const std::shared_ptr<Camera> &camera);

        void renderObjects(
                std::vector<std::shared_ptr<gl::RenderItem>> &renderList,
                const std::shared_ptr<Scene> &scene, const std::shared_ptr<Camera> &camera);

        void renderObject(
                const std::shared_ptr<Object3D> &object,
                const std::shared_ptr<Scene> &scene,
                const std::shared_ptr<Camera> &camera,
                const std::shared_ptr<BufferGeometry> &geometry,
                const std::shared_ptr<Material> &material,
                std::optional<GeometryGroup> group);

        std::shared_ptr<gl::GLProgram> getProgram(
                const std::shared_ptr<Material> &material,
                const std::shared_ptr<Scene> &scene,
                const std::shared_ptr<Object3D> &object);

        void updateCommonMaterialProperties(
                const std::shared_ptr<Material> &material,
                gl::ProgramParameters &parameters);

        std::shared_ptr<gl::GLProgram> setProgram(
                const std::shared_ptr<Camera> &camera,
                const std::shared_ptr<Scene> &scene,
                const std::shared_ptr<Material> &material,
                const std::shared_ptr<Object3D> &object);

        void markUniformsLightsNeedsUpdate(UniformMap &uniforms, bool value);

        bool materialNeedsLights(const std::shared_ptr<Material> &material);

        [[nodiscard]] int getActiveCubeFace() const {

            return _currentActiveCubeFace;
        }

        [[nodiscard]] int getActiveMipmapLevel() const {

            return _currentActiveMipmapLevel;
        }

        std::shared_ptr<GLRenderTarget> &getRenderTarget() {

            return _currentRenderTarget;
        }

        void setRenderTarget(const std::shared_ptr<GLRenderTarget> &renderTarget, int activeCubeFace = 0, int activeMipmapLevel = 0);

    private:
        struct OnMaterialDispose : EventListener {

            explicit OnMaterialDispose(GLRenderer &scope);

            void onEvent(Event &event) override;

        private:
            GLRenderer &scope_;
        };

        OnMaterialDispose onMaterialDispose;

        Canvas &canvas_;

        std::shared_ptr<gl::GLRenderList> currentRenderList;
        std::shared_ptr<gl::GLRenderState> currentRenderState;

        std::vector<std::shared_ptr<gl::GLRenderList>> renderListStack;
        std::vector<std::shared_ptr<gl::GLRenderState>> renderStateStack;

        int _currentActiveCubeFace = 0;
        int _currentActiveMipmapLevel = 0;
        std::shared_ptr<GLRenderTarget> _currentRenderTarget = nullptr;
        unsigned int _currentMaterialId = -1;

        std::shared_ptr<Camera> _currentCamera = nullptr;
        Vector4 _currentViewport;
        Vector4 _currentScissor;
        std::optional<bool> _currentScissorTest;

        //

        WindowSize _size;

        int _pixelRatio = 1;

        Vector4 _viewport;
        Vector4 _scissor;
        bool _scissorTest = false;

        std::vector<unsigned int> _currentDrawBuffers;

        // frustum

        Frustum _frustum{};

        // clipping

        bool _clippingEnabled = false;
        bool _localClippingEnabled = false;

        // camera matrices cache

        Matrix4 _projScreenMatrix{};

        Vector3 _vector3{};

        gl::GLInfo info;

        gl::GLBackground background;
        gl::GLProperties properties;
        gl::GLGeometries geometries;
        gl::GLBindingStates bindingStates;
        gl::GLAttributes attributes;
        gl::GLClipping clipping;
        gl::GLTextures textures;
        gl::GLMaterials materials;
        gl::GLRenderStates renderStates;
        gl::GLRenderLists renderLists;
        gl::GLObjects objects;
        gl::GLPrograms programCache;

        std::unique_ptr<gl::GLBufferRenderer> bufferRenderer;
        std::unique_ptr<gl::GLIndexedBufferRenderer> indexedBufferRenderer;

        friend struct gl::ProgramParameters;
        friend struct gl::GLShadowMap;
    };

}// namespace threepp

#endif//THREEPP_GLRENDERER_HPP
