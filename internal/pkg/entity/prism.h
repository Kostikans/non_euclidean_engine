//
// Created by Pavlov Konstantin on 05.04.2022.
//

#ifndef NON_EUCLIDEAN_ENGINE_PRISM_H
#define NON_EUCLIDEAN_ENGINE_PRISM_H

#include <array>
#include "entity.h"

class Prism : public Entity
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
    void init(glm::mat4x4 transform, int sides, double radius, double length, int steps,glm::vec3 color){
        std::vector<VertexData> vertexes;
        std::vector<GLuint> indices;
        // Add vertices
            double sinhRadius = sinh(radius);
            double coshRadius = cosh(radius);

            std::vector<std::vector<std::array<GLuint, 2>>> prism;
            for (int side = 0; side < sides; ++side) {
                prism.emplace_back();
                auto& prismSide = prism.back();

                double angle0 = side * M_TAU / sides;
                double angle1 = ((side + 1) % sides) * M_TAU / sides;
                std::array<glm::vec4, 2> basePos {
                        glm::vec4(cos(angle0) * sinh(radius), sin(angle0) * sinh(radius), 0, cosh(radius)),
                        glm::vec4(cos(angle1) * sinh(radius), sin(angle1) * sinh(radius), 0, cosh(radius))
                };
                glm::vec4 baseNormal = VectorMath::hyperbolicNormalGlm(basePos[0], basePos[1], VectorMath::hyperbolicTranslationGlm(basePos[0]) * glm::vec4(0, 0, 1, sqrt(2)));
                baseNormal /= sqrt(VectorMath::dotProduct(baseNormal,baseNormal));

                for (int step = 0; step <= steps; ++step) {
                    prismSide.emplace_back();
                    auto& prismSection = prismSide.back();
                    glm::mat4x4 stepTransform = transform * VectorMath::hyperbolicDisplacementGlm(glm::vec4(0, 0, length * step / steps, 0));
                    for (int k=0; k<2; k++) {
                        vertexes.emplace_back(VertexData(stepTransform * basePos[k], glm::vec2((double)step / steps, (double)(side + k) / sides), stepTransform * baseNormal));
                        prismSection[k] = vertexes.size() - 1;
                    }
                }
            }

            // Add faces
            for (int side = 0; side < sides; ++side) {
                for (int step = 0; step < steps; ++step) {
                    indices.emplace_back(prism[side][step][0]);
                    indices.emplace_back(prism[side][step + 1][1]);
                    indices.emplace_back(prism[side][step][1]);
                    indices.emplace_back(prism[side][step][0]);
                    indices.emplace_back(prism[side][step + 1][0]);
                    indices.emplace_back(prism[side][step + 1][1]);
                }
            }

        vbo.allocate(vertexes.data(), vertexes.size() * sizeof(VertexData));
        ibo.allocate(indices.data(), indices.size() * sizeof(GLuint));

        m_color = color;
    };

    void draw(Shader& shader) override{
        shader.use();
        vao.bind();

        AttribLayout layout;
        layout.push<float>(4);
        layout.push<float>(2);
        layout.push<float>(3);
        vao.pushLayout(layout, vbo);

        shader.setVec3("color",m_color);
        shader.setMat4("R", getRotateMatrix());
        shader.setMat4Eugen("T", model.cast<float>());

        ibo.bind();
        glDrawElements(GL_TRIANGLES, ibo.size(), GL_UNSIGNED_INT, 0);
        ibo.unbind();
    };

    void translate(const glm::vec3& translate) override{
        m_position += translate;
    };

    void scale(const float& scale) override {
        m_scale *= scale;
    };

    void rotate(const glm::quat& rotate) override {
        m_rotate *= rotate;
    };

    Matrix4d getTranslateMatrix() const override {
        Matrix4d translate = VectorMath::hyperbolicTranslation(Vector4d(m_position.x,m_position.y,m_position.z,1.0f));

        return translate;
    };

    void setModelMatrix(const Matrix4d &in){
        model = in;
    };

    glm::mat4x4 getRotateMatrix() const override{
        glm::quat rotate = glm::normalize(m_rotate);

        return glm::toMat4(rotate);
    };

    glm::vec3 getPosition() const{
        return m_position;
    };
};

#endif //NON_EUCLIDEAN_ENGINE_PRISM_H
