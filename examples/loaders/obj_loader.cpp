
#include <threepp/threepp.hpp>

using namespace threepp;

int main() {

    Canvas canvas{Canvas::Parameters().antialiasing(8)};

    auto scene = Scene::create();
    auto camera = PerspectiveCamera::create(75, canvas.getAspect(), 0.1f, 1000);
    camera->position.set(0, 100, 150);

    GLRenderer renderer(canvas);

    OBJLoader loader;
    auto obj = loader.load("data/models/obj/female02/female02.obj");

//    TextureLoader tl;
//    auto tex = tl.loadTexture("data/textures/uv_grid_opengl.jpg");
//    obj->traverse([tex](Object3D& child){
//
//        if (dynamic_cast<Mesh*>(&child)) {
//            Mesh*mesh = dynamic_cast<Mesh*>(&child);
//            for (auto m : mesh->materials()) {
//                if (std::dynamic_pointer_cast<MaterialWithMap>(m)) {
//                    std::dynamic_pointer_cast<MaterialWithMap>(m)->map = tex;
//                }
//            }
//        }
//    });
    scene->add(obj);

    auto light1 = PointLight::create(Color(0xffffff), 1.f);
    light1->position.set(25, 115, 25);
    scene->add(light1);

    auto light2 = PointLight::create(Color(0xffffff), 1.f);
    light2->position.set(-25, 115, 125);
    scene->add(light2);

    auto light3 = PointLight::create(Color(0xffffff), 1.f);
    light3->position.set(0, 25, -30);
    scene->add(light3);

    canvas.onWindowResize([&](WindowSize size) {
        camera->aspect = size.getAspect();
        camera->updateProjectionMatrix();
        renderer.setSize(size);
    });

    canvas.animate([&](float dt) {

        obj->rotation.y += 1 * dt;

        renderer.render(scene, camera);
    });
}