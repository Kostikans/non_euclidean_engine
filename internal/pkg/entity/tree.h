//
// Created by Pavlov Konstantin on 05.04.2022.
//

#ifndef NON_EUCLIDEAN_ENGINE_TREE_H
#define NON_EUCLIDEAN_ENGINE_TREE_H

#include "prism.h"

class Tree : public Entity {
private:
    std::vector<glm::mat4x4> recursiveTransformations;
    std::vector<Prism *> prisms;
    double sideLength;
public:
    Tree() : sideLength(acosh(3)) {
        glm::mat4x4 translation = VectorMath::hyperbolicDisplacementGlm(glm::vec4(0, 0, sideLength, 0));

        recursiveTransformations.push_back(translation);
        recursiveTransformations.push_back(glm::rotate(translation, float(M_TAU / 4.0), glm::vec3(1, 0, 0)));
        recursiveTransformations.push_back(glm::rotate(translation, float(M_TAU / 4.0), glm::vec3(0, 1, 0)));
        recursiveTransformations.push_back(glm::rotate(translation, float(M_TAU / 4.0), glm::vec3(-1, 0, 0)));
        recursiveTransformations.push_back(glm::rotate(translation, float(M_TAU / 4.0), glm::vec3(0, -1, 0)));
    }

    void init(glm::mat4x4 transform, int layers) {
        buildBranch(transform, layers);
        buildBranch(glm::rotate(transform, float(M_TAU / 4.0), glm::vec3(1, 0, 0)), layers);
        buildBranch(glm::rotate(transform, float(M_TAU / 4.0), glm::vec3(0, 1, 0)), layers);
        buildBranch(glm::rotate(transform, float(M_TAU / 4.0), glm::vec3(-1, 0, 0)), layers);
        buildBranch(glm::rotate(transform, float(M_TAU / 4.0), glm::vec3(0, -1, 0)), layers);
        buildBranch(glm::rotate(transform, float(M_TAU / 2.0), glm::vec3(1, 0, 0)), layers);
    };

    void buildBranch(glm::mat4x4 transform, int layers) {
        if (layers == 0) {
            return;
        }

        Prism *prism = new Prism();
        prism->init(Matrix4d{}, 8, 0.1, sideLength, 6, glm::vec3(120, 120, 120));
        prisms.emplace_back(prism);

        if (layers != 0) {
            for (int i = 0; i < recursiveTransformations.size(); ++i) {
                buildBranch(transform * recursiveTransformations[i], layers - 1);
            }
        }
    }

    void draw(Shader *shader) override {
        for (int i = 0; i < prisms.size(); i++) {
            prisms[i]->setModelMatrix(model);
            prisms[i]->draw(shader);
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

#endif //NON_EUCLIDEAN_ENGINE_TREE_H
