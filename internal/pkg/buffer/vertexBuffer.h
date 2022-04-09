//
// Created by Pavlov Konstantin on 20.03.2022.
//

#ifndef NON_EUCLIDEAN_ENGINE_VERTEXBUFFER_H
#define NON_EUCLIDEAN_ENGINE_VERTEXBUFFER_H

#include "../lib_includes.h"

class VertexBuffer
{
private:
    GLuint ID;

public:
    VertexBuffer();
    VertexBuffer(const float *m_data ,GLsizei size);
    void allocate(const void* m_data, GLsizei size);
    void bind() const;
    void unbind() const;
};


#endif //NON_EUCLIDEAN_ENGINE_VERTEXBUFFER_H
