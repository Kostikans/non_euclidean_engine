//
// Created by Pavlov Konstantin on 20.03.2022.
//

#ifndef NON_EUCLIDEAN_ENGINE_VERTEXATTRIBBUFFER_H
#define NON_EUCLIDEAN_ENGINE_VERTEXATTRIBBUFFER_H

#include "../lib_includes.h"

#include "attribLayout.h"
#include "vertexBuffer.h"

class VertexAttribBuffer
{
private:
    GLuint ID;

public:
    VertexAttribBuffer();
    void pushLayout(const AttribLayout &layout, const VertexBuffer &vbo);
    void bind();
    void unbind();
};

#endif //NON_EUCLIDEAN_ENGINE_VERTEXATTRIBBUFFER_H
