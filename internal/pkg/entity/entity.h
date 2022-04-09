#ifndef NON_EUCLIDEAN_ENGINE_ENTITY_H
#define NON_EUCLIDEAN_ENGINE_ENTITY_H

#include "../buffer/vertexBuffer.h"
#include "../buffer/indexBuffer.h"
#include "../buffer/vertexAttribBuffer.h"
#include "../shader/shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include "../core/geometry.h"

struct VertexData
{
    VertexData() = default;;
    VertexData(glm::vec3 m_pos, glm::vec2 m_texCoord, glm::vec3 m_norm)
    {
        position = glm::vec4(m_pos,1.0f);
        texCoord = m_texCoord;
        normal = m_norm;
    }
    VertexData(glm::vec4 m_pos, glm::vec2 m_texCoord, glm::vec3 m_norm)
    {
        position = m_pos;
        texCoord = m_texCoord;
        normal = m_norm;
    }
    explicit VertexData(glm::vec4 m_pos)
    {
        position = m_pos;
    }
    glm::vec4 position{};
    glm::vec2 texCoord{};
    glm::vec3 normal{};
};


class Entity {
public:
    virtual void draw(Shader &shader) = 0;

    virtual void translate(const glm::vec3 &translate) = 0;

    virtual void scale(const float &scale) = 0;

    virtual void rotate(const glm::quat &rotate) = 0;

    [[nodiscard]] virtual Matrix4d getTranslateMatrix() const = 0;

    [[nodiscard]] virtual glm::mat4x4 getRotateMatrix() const = 0;

protected:
    glm::vec3 m_position = glm::vec3(0,0,0);
    Matrix4d model = Matrix4d::Identity();

    glm::quat m_rotate;
    float m_scale;
};

#endif //NON_EUCLIDEAN_ENGINE_ENTITY_H
