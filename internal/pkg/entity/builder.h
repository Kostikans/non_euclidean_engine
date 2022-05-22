//
// Created by Pavlov Konstantin on 21.05.2022.
//

#ifndef NON_EUCLIDEAN_ENGINE_BUILDER_H
#define NON_EUCLIDEAN_ENGINE_BUILDER_H

struct VertexData {
    VertexData() = default;;

    VertexData(std::array<float, 3> m_pos, std::array<float, 2> m_texCoord, std::array<float, 4> m_norm) {
        position = std::array<float, 4>{m_pos[0], m_pos[1], m_pos[2], 1.0f};
        texCoord = m_texCoord;
        normal = m_norm;
    }

    VertexData(std::array<float, 4> m_pos, std::array<float, 2> m_texCoord, std::array<float, 4> m_norm) {
        position = m_pos;
        texCoord = m_texCoord;
        normal = m_norm;
    }

    VertexData(Vector4d m_pos, Vector2d m_texCoord, Vector4d m_normal) {
        position = std::array<float, 4>{static_cast<float>(m_pos[0]), static_cast<float>(m_pos[1]),static_cast<float>(m_pos[2]), static_cast<float>(m_pos[3])};

        texCoord = std::array<float, 2>{static_cast<float>(m_texCoord[0]), static_cast<float>(m_texCoord[1])};
        normal = std::array<float, 4>{static_cast<float>(m_normal[0]), static_cast<float>(m_normal[1]), static_cast<float>(m_normal[2]), static_cast<float>(m_normal[3])};
    }

    explicit VertexData(std::array<float, 4> &m_pos) {
        position = m_pos;
    }

    std::array<float, 4> position{};
    std::array<float, 2> texCoord{};
    std::array<float, 4> normal{};
};

class Builder {
public:
    Builder() : currentVertex(0) {}

    GLuint addVertex(Vector4d pos, Vector4d normal, Vector2d texCoord) {
        vertices.emplace_back(
                std::array<float, 4>{static_cast<float>(pos[0]), static_cast<float>(pos[1]), static_cast<float>(pos[2]),
                                     static_cast<float>(pos[3])},
                std::array<float, 2>{static_cast<float>(texCoord[0]), static_cast<float>(texCoord[1])},
                std::array<float, 4>{static_cast<float>(normal[0]), static_cast<float>(normal[1]),
                                     static_cast<float>(normal[2]), static_cast<float>(normal[3])});

        return currentVertex++;
    }

    GLuint addVertex(std::array<float, 4> pos, std::array<float, 2> texCoord, std::array<float, 4> normal) {
        vertices.emplace_back(pos, texCoord, normal);

        return currentVertex++;
    }

    void addTriangle(GLuint vertex0, GLuint vertex1, GLuint vertex2) {
        elements.push_back(vertex0);
        elements.push_back(vertex1);
        elements.push_back(vertex2);
    }

    void addBackFaces() {
        int numExistingVertices = vertices.size();
        for (int i = 0; i < numExistingVertices; ++i) {
            const VertexData &existingVertex = vertices[i];
            const std::array<float, 4> &n = existingVertex.normal;
            std::array<float, 4> normal = std::array<float, 4>{-n[0], -n[1], -n[2], -n[3]};
            vertices.emplace_back(existingVertex.position, existingVertex.texCoord, normal);
        }

        int numExistingElements = elements.size();
        for (int i = 0; i < numExistingElements; i += 3) {
            addTriangle(elements[i] + numExistingVertices, elements[i + 2] + numExistingVertices,
                        elements[i + 1] + numExistingVertices);
        }
    }

    [[nodiscard]] std::vector<VertexData> getVertices() const {
        return vertices;
    }

    [[nodiscard]] std::vector<GLuint> getElements() const {
        return elements;
    }

private:
    GLuint currentVertex;
    std::vector<VertexData> vertices;
    std::vector<GLuint> elements;
};

#endif //NON_EUCLIDEAN_ENGINE_BUILDER_H
