//
// Created by Pavlov Konstantin on 29.03.2022.
//

#ifndef NON_EUCLIDEAN_ENGINE_CUBE_H
#define NON_EUCLIDEAN_ENGINE_CUBE_H

#include "entity.h"

class Cube : public Entity
{
private:
    VertexBuffer vbo;
    IndexBuffer ibo;
    VertexAttribBuffer vao;
    float m_width;
    float m_height;
    float m_depth;
    glm::vec3 m_color;

public:
    void init(float width, float height, float depth,glm::vec3 color);
    void draw(Shader& shader) override;
    void translate(const glm::vec3& translate) override;
    void scale(const float& scale) override;
    void rotate(const glm::quat& rotate) override;
    Matrix4d getTranslateMatrix() const override;
    glm::mat4x4 getRotateMatrix() const override;

    float getWidth() const;
    float getHeight() const;
    float getDepth() const;
    glm::vec3 getPosition() const;
};

#endif //NON_EUCLIDEAN_ENGINE_CUBE_H
