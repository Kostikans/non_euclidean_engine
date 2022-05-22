//
// Created by Pavlov Konstantin on 21.05.2022.
//

#ifndef NON_EUCLIDEAN_ENGINE_HOROSPHERE_H
#define NON_EUCLIDEAN_ENGINE_HOROSPHERE_H

#include <array>
#include "entity.h"

class Horosphere : public Entity {
private:

public:
    void init(const char *texturePath = nullptr) {
        Builder builder = Builder{};
        int numSteps = 400;
        double size = 20;
        double textureSize = 5;

        Eigen::Array<GLuint, Eigen::Dynamic, Eigen::Dynamic> vertices(numSteps + 1, numSteps + 1);

        for (int i = 0; i <= numSteps; ++i) {
            for (int j = 0; j <= numSteps; ++j) {
                double xPos = (static_cast<double>(i) / numSteps - 0.5) * size;
                double yPos = (static_cast<double>(j) / numSteps - 0.5) * size;
                Matrix4d rotation = VectorMath::horoRotation(xPos, yPos);
                vertices(i, j) = builder.addVertex(
                        rotation * Vector4d(0, 0, 0, 1),
                        Vector4d(0, 0, 1, -1),
                        Vector2d(xPos * textureSize, yPos * textureSize));
            }
        }

        for (int i = 0; i < numSteps; ++i) {
            for (int j = 0; j < numSteps; ++j) {
                builder.addTriangle(vertices(i, j), vertices(i + 1, j), vertices(i, j + 1));
                builder.addTriangle(vertices(i, j + 1), vertices(i + 1, j), vertices(i + 1, j + 1));
            }
        }

        builder.addBackFaces();

        vbo.allocate(builder.getVertices().data(), builder.getVertices().size() * sizeof(VertexData));
        ibo.allocate(builder.getElements().data(), builder.getElements().size() * sizeof(GLuint));

        if (texturePath != nullptr) {
            texture = new Texture(texturePath);
        }
    };

    Matrix4d getTranslateMatrix() const override {
        Matrix4d translate = VectorMath::hyperbolicTranslation(
                Vector4d(m_position.x, m_position.y, m_position.z, 1.0f));

        return translate;
    };

    void setModelMatrix(const Matrix4d &in) {
        model = in;
    };

    glm::mat4x4 getRotateMatrix() const override {
        glm::quat rotate = glm::normalize(m_rotate);

        return glm::toMat4(rotate);
    };

    glm::vec3 getPosition() const {
        return m_position;
    };
};

#endif //NON_EUCLIDEAN_ENGINE_HOROSPHERE_H
