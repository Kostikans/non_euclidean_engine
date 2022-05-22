//
// Created by Pavlov Konstantin on 22.05.2022.
//

#ifndef NON_EUCLIDEAN_ENGINE_SCENE_H
#define NON_EUCLIDEAN_ENGINE_SCENE_H

#include <unordered_set>
#include <memory>
#include "../entity/entity.h"
#include "../camera/camera.h"
#include "../input/input.h"
#include "../render/renderContext.h"

class Scene {
public:
    void addEntity(Entity *entity) {
        entities.insert(entity);
    }

    void setCamera(Camera *camera) {
        this->camera = camera;
    }

    void render(RenderContext *context) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        context->use();
        float ratio = (double) context->getWidth() / (double) context->getHeight();
        double zoom = camera->getCameraZoom();

        Matrix4d projection = VectorMath::perspective(ratio * zoom, zoom, 0.01, 10);

        context->setProjection(projection);
        context->setView(camera->GetViewMatrix());
        context->setUniforms();

        for (Entity *entity: entities) {
            context->render(entity);
        }
    }

private:
    std::unordered_set<Entity *> entities;
    Camera *camera = nullptr;
};


#endif //NON_EUCLIDEAN_ENGINE_SCENE_H
