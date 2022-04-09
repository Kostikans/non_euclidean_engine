//
// Created by Pavlov Konstantin on 20.03.2022.
//

#ifndef NON_EUCLIDEAN_ENGINE_INDEXBUFFER_H
#define NON_EUCLIDEAN_ENGINE_INDEXBUFFER_H

#include "../lib_includes.h"

class IndexBuffer
{
private:
    GLuint ID;
    GLsizei m_size;
public:
    IndexBuffer();
    IndexBuffer(const unsigned int* m_data, GLsizei size);
    void allocate(const void* m_data, GLsizei size);
    GLsizei size();
    void bind() const;
    void unbind() const;
};

#endif //NON_EUCLIDEAN_ENGINE_INDEXBUFFER_H
