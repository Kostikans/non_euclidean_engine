//
// Created by Pavlov Konstantin on 27.03.2022.
//

//
// Created by Pavlov Konstantin on 20.03.2022.
//

#include "sphere.h"
#include  <glm/gtx/io.hpp>


void Sphere::draw(Shader &shader) {
    shader.use();
    vao.bind();

    AttribLayout layout;
    layout.push<float>(3);
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

void Sphere::translate(const glm::vec3 &translate) {
    m_position += translate;
}

void Sphere::scale(const float &scale) {
    m_scale *= scale;
}

void Sphere::rotate(const glm::quat &rotate) {
    m_rotate *= rotate;
}

void Sphere::init(float radius, float crutch, float depth,glm::vec3 color) {
    m_radius = radius;
    std::vector<VertexData> vertexes;
    float sectorCount = 10;
    float stackCount = 10;
    float x, y, z, xy;
    float nx, ny, nz, lengthInv = 1.0f / radius;
    float s, t;

    float sectorStep = 2 * M_PI / sectorCount;
    float stackStep = M_PI / stackCount;
    float sectorAngle, stackAngle;

    for (int i = 0; i <= stackCount; ++i) {
        stackAngle = M_PI / 2 - i * stackStep;
        xy = radius * cosf(stackAngle);
        z = radius * sinf(stackAngle);

        for (int j = 0; j <= sectorCount; ++j) {
            sectorAngle = j * sectorStep;

            x = xy * cosf(sectorAngle);
            y = xy * sinf(sectorAngle);

            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;

            s = (float) j / sectorCount;
            t = (float) i / stackCount;

            vertexes.push_back(VertexData(glm::vec3(x, y, z), glm::vec2(s, t), glm::vec3(nx, ny, nz)));
        }
    }
    std::vector<GLuint> indices;
    int k1, k2;
    for (int i = 0; i < stackCount; ++i) {
        k1 = i * (sectorCount + 1);
        k2 = k1 + sectorCount + 1;

        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
            if (i != 0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }
            if (i != (stackCount - 1)) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }

    vbo.allocate(vertexes.data(), vertexes.size() * sizeof(VertexData));
    ibo.allocate(indices.data(), indices.size() * sizeof(GLuint));
    m_color = color;
}

float Sphere::getRadius() const {
    return m_radius;
}

glm::vec3 Sphere::getPosition() const {
    return m_position;
}

Matrix4d Sphere::getTranslateMatrix() const {
    Matrix4d translate = VectorMath::hyperbolicTranslation(Vector4d(m_position.x,m_position.y,m_position.z,1.0f));

    return translate;
};

glm::mat4x4 Sphere::getRotateMatrix() const {
    glm::quat rotate = glm::normalize(m_rotate);

    return glm::toMat4(rotate);
}


