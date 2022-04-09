//
// Created by Pavlov Konstantin on 20.03.2022.
//

#ifndef NON_EUCLIDEAN_ENGINE_ATTRIBLAYOUT_H
#define NON_EUCLIDEAN_ENGINE_ATTRIBLAYOUT_H

#include "../lib_includes.h"
#include <vector>

struct VertexBufferElement
{
    unsigned int type;
    unsigned int count;
    unsigned char normalized;
    static unsigned int GetSizeofType(unsigned int type)
    {
        switch (type)
        {
            case GL_FLOAT: return 4;
            case GL_UNSIGNED_INT: return 4;
            case GL_UNSIGNED_BYTE: return 1;
        }
        return 0;
    }
};
class AttribLayout
{
private:
    std::vector<VertexBufferElement> m_Elements;
    unsigned int m_Stride;

public:
    AttribLayout()
            :m_Stride(0)
    {}
    template<typename T>
    void push(unsigned int count)
    {
        //static_assert(false);
    }
    template<>
    void push<float>(unsigned int count)
    {
        m_Elements.push_back({ GL_FLOAT,count,GL_FALSE });
        m_Stride += count * VertexBufferElement::GetSizeofType(GL_FLOAT);
    }
    template<>
    void push<unsigned int>(unsigned int count)
    {
        m_Elements.push_back({ GL_UNSIGNED_INT,count,GL_FALSE });
        m_Stride += count * VertexBufferElement::GetSizeofType(GL_UNSIGNED_INT);
    }
    template<>
    void push<unsigned char>(unsigned int count)
    {
        m_Elements.push_back({ GL_UNSIGNED_BYTE,count,GL_TRUE });
        m_Stride += count * VertexBufferElement::GetSizeofType(GL_UNSIGNED_BYTE);
    }

    inline const std::vector<VertexBufferElement> GetElements() const { return m_Elements; }
    inline unsigned int GetStride() const { return m_Stride; }

};

#endif //NON_EUCLIDEAN_ENGINE_ATTRIBLAYOUT_H
