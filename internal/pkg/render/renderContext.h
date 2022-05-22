//
// Created by Pavlov Konstantin on 22.05.2022.
//

#ifndef NON_EUCLIDEAN_ENGINE_RENDERCONTEXT_H
#define NON_EUCLIDEAN_ENGINE_RENDERCONTEXT_H

#include "../shader/shader.h"
#include "../lib_includes.h"
#include "../entity/entity.h"

const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;

class RenderContext {
public:
    explicit RenderContext(Shader *shader) : shader(shader) {}

    void setDimensions(int width, int height) {
        this->width = width;
        this->height = height;
    }

    int getWidth() {
        return width;
    }

    int getHeight() {
        return height;
    }

    void setProjection(const Matrix4d &projection) {
        this->projection = projection;
    }

    void setView(const Matrix4d &View) {
        this->view = View;
    }

    const Matrix4d &getView() const {
        return view;
    }

    const Matrix4d &getProjection() const {
        return projection;
    }

    Shader* getShader() const {
        return shader;
    }

    void use() {
        shader->use();
    }

    void render(Entity *entity) {
        entity->draw(shader);
    }

    void setUniforms() {
        shader->setMat4Eugen("P", projection.cast<float>());
        shader->setMat4Eugen("V", view.cast<float>());
    }

private:
    Matrix4d projection = Matrix4d::Identity();
    Matrix4d view = Matrix4d::Identity();
    Shader *shader = nullptr;

    int width = SCR_WIDTH;
    int height = SCR_HEIGHT;
};


#endif //NON_EUCLIDEAN_ENGINE_RENDERCONTEXT_H
