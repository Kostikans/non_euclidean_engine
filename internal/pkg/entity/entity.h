#ifndef NON_EUCLIDEAN_ENGINE_ENTITY_H
#define NON_EUCLIDEAN_ENGINE_ENTITY_H

#include "../buffer/vertexBuffer.h"
#include "../buffer/indexBuffer.h"
#include "../buffer/vertexAttribBuffer.h"
#include "../texture/texture.h"
#include "../shader/shader.h"
#include "builder.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include "../core/geometry.h"

class Entity {
public:
    virtual void draw(Shader *shader) {
        vao.bind();

        AttribLayout layout;
        layout.push<float>(4);
        layout.push<float>(2);
        layout.push<float>(4);
        vao.pushLayout(layout, vbo);

        if (texture != nullptr) {
            texture->bind();
            shader->setInt("texture_sampler", 0);
        }

        shader->setMat4("R", getRotateMatrix());
        shader->setMat4Eugen("T",  Matrix4d::Identity().cast<float>());

        ibo.bind();
        glDrawElements(GL_TRIANGLES, ibo.size(), GL_UNSIGNED_INT, 0);
        ibo.unbind();
    };

    virtual void translate(const glm::vec3 &translate) {
        m_position += translate;
    };

    virtual void scale(const float &scale) {
        m_scale *= scale;
    };

    virtual void rotate(const glm::quat &rotate) {
        m_rotate *= rotate;
    };

    [[nodiscard]] virtual Matrix4d getTranslateMatrix() const {
        Matrix4d translate = VectorMath::hyperbolicTranslation(
                Vector4d(m_position.x, m_position.y, m_position.z, 1.0f));

        return translate;
    };

    [[nodiscard]] virtual glm::mat4x4 getRotateMatrix() const {
        glm::quat rotate = glm::normalize(m_rotate);

        return glm::toMat4(rotate);
    };

protected:
    VertexBuffer vbo;
    IndexBuffer ibo;
    VertexAttribBuffer vao;
    Texture *texture;

    glm::vec3 m_position = glm::vec3(0, 0, 0);
    Matrix4d model = Matrix4d::Identity();

    glm::quat m_rotate;
    float m_scale;
};

#endif //NON_EUCLIDEAN_ENGINE_ENTITY_H
