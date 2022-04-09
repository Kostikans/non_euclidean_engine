//
// Created by Pavlov Konstantin on 29.03.2022.
//

#include "cube.h"
#include  <glm/gtx/io.hpp>


void Cube::draw(Shader& shader)
{
    shader.use();
    vao.bind();

    AttribLayout layout;
    layout.push<float>(4);
    layout.push<float>(2);
    layout.push<float>(3);
    vao.pushLayout(layout, vbo);

    shader.setVec3("color",m_color);
    shader.setMat4("R", getRotateMatrix());
    shader.setMat4Eugen("T", getTranslateMatrix().cast<float>());

    ibo.bind();
    glDrawElements(GL_TRIANGLES, ibo.size(), GL_UNSIGNED_INT, 0);
    ibo.unbind();
}

void Cube::translate(const glm::vec3& translate)
{
    m_position += translate;
}

void Cube::scale(const float& scale)
{
    m_scale *= scale;
}

void Cube::rotate(const glm::quat& rotate)
{
    m_rotate *= rotate;
}

void Cube::init(float width, float height, float depth,glm::vec3 color)
{
    m_width = width;
    m_height = height;
    m_depth = depth;

    std::vector<VertexData> vertexes;

    vertexes.push_back(VertexData(glm::vec3(-width, height, depth), glm::vec2(0.0f, height), glm::vec3(0.0f, 0.0f, 1.0f)));
    vertexes.push_back(VertexData(glm::vec3(-width, -height, depth), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
    vertexes.push_back(VertexData(glm::vec3(width, height, depth), glm::vec2(width, height), glm::vec3(0.0f, 0.0f, 1.0f)));
    vertexes.push_back(VertexData(glm::vec3(width, -height, depth), glm::vec2(width, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));

    vertexes.push_back(VertexData(glm::vec3(width, height, depth), glm::vec2(0.0f, height), glm::vec3(1.0f, 0.0f, 0.0f)));
    vertexes.push_back(VertexData(glm::vec3(width, -height, depth), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
    vertexes.push_back(VertexData(glm::vec3(width, height, -depth), glm::vec2(depth, height), glm::vec3(1.0f, 0.0f, 0.0f)));
    vertexes.push_back(VertexData(glm::vec3(width, -height, -depth), glm::vec2(depth, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)));

    vertexes.push_back(VertexData(glm::vec3(width, height, depth), glm::vec2(0.0f, width), glm::vec3(0.0f, 1.0f, 0.0f)));
    vertexes.push_back(VertexData(glm::vec3(width, height, -depth), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    vertexes.push_back(VertexData(glm::vec3(-width, height, depth), glm::vec2(width, width), glm::vec3(0.0f, 1.0f, 0.0f)));
    vertexes.push_back(VertexData(glm::vec3(-width, height, -depth), glm::vec2(width, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));

    vertexes.push_back(VertexData(glm::vec3(width, height, -depth), glm::vec2(0.0f, height), glm::vec3(0.0f, 0.0f, -1.0f)));
    vertexes.push_back(VertexData(glm::vec3(width, -height, -depth), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
    vertexes.push_back(VertexData(glm::vec3(-width, height, -depth), glm::vec2(width, height), glm::vec3(0.0f, 0.0f,-1.0f)));
    vertexes.push_back(VertexData(glm::vec3(-width, -height, -depth), glm::vec2(width, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));

    vertexes.push_back(VertexData(glm::vec3(-width, height, depth), glm::vec2(depth, height), glm::vec3(-1.0f, 0.0f, 0.0f)));
    vertexes.push_back(VertexData(glm::vec3(-width, height, -depth), glm::vec2(0.0f, height), glm::vec3(-1.0f, 0.0f, 0.0f)));
    vertexes.push_back(VertexData(glm::vec3(-width, -height, depth), glm::vec2(depth, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f)));
    vertexes.push_back(VertexData(glm::vec3(-width, -height, -depth), glm::vec2(0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f)));

    vertexes.push_back(VertexData(glm::vec3(-width, -height, depth), glm::vec2(0.0f, height), glm::vec3(0.0f, -1.0f, 0.0f)));
    vertexes.push_back(VertexData(glm::vec3(-width, -height, -depth), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    vertexes.push_back(VertexData(glm::vec3(width, -height, depth), glm::vec2(width, height), glm::vec3(0.0f, -1.0f, 0.0f)));
    vertexes.push_back(VertexData(glm::vec3(width, -height, -depth), glm::vec2(width, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

    std::vector<GLuint> indices;
    for (int i = 0; i < 24; i += 4) {
        indices.push_back(i + 0);
        indices.push_back(i + 1);
        indices.push_back(i + 2);
        indices.push_back(i + 2);
        indices.push_back(i + 1);
        indices.push_back(i + 3);
    }
    vbo.allocate(vertexes.data(), vertexes.size() * sizeof(VertexData));
    ibo.allocate(indices.data(), indices.size() * sizeof(GLuint));

    m_color = color;
}

float Cube::getWidth() const
{
    return m_width;
}

float Cube::getHeight() const
{
    return m_height;
}

float Cube::getDepth() const
{
    return m_depth;
}

glm::vec3 Cube::getPosition() const
{
    return m_position;
}

Matrix4d Cube::getTranslateMatrix() const {
    Matrix4d translate = VectorMath::hyperbolicTranslation(Vector4d(m_position.x,m_position.y,m_position.z,1.0f));

    return translate;
};

glm::mat4x4 Cube::getRotateMatrix() const {
    glm::quat rotate = glm::normalize(m_rotate);

    return glm::toMat4(rotate);
}


