//
// Created by Pavlov Konstantin on 20.03.2022.
//

#ifndef NON_EUCLIDEAN_ENGINE_SPHERE_H
#define NON_EUCLIDEAN_ENGINE_SPHERE_H

#include "entity.h"

class Sphere : public Entity
{
private:
    VertexBuffer vbo;
    IndexBuffer ibo;
    VertexAttribBuffer vao;
    float m_radius;
    glm::vec3 m_color;
public:
    void init(float radius, float crutch = 0, float depth = 0,glm::vec3 color = glm::vec3(0.0f,0.0f,0.0f));
    void draw(Shader& shader) override;
    void translate(const glm::vec3& translate) override;
    void scale(const float& scale) override;
    void rotate(const glm::quat& rotate) override;
    Matrix4d getTranslateMatrix() const override;
    glm::mat4x4 getRotateMatrix() const override;

    float getRadius() const;
    glm::vec3 getPosition() const;
};

#endif //NON_EUCLIDEAN_ENGINE_SPHERE_H
