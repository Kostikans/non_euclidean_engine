//
// Created by Pavlov Konstantin on 22.05.2022.
//

#ifndef NON_EUCLIDEAN_ENGINE_TEXTURE_H
#define NON_EUCLIDEAN_ENGINE_TEXTURE_H

#include "../lib_includes.h"

class Texture {
private:
    GLuint ID;
public:
    Texture();

    Texture(const char *texturePath);

    void allocate(const char *texturePath);

    void bind();

    void unbind();
};


#endif //NON_EUCLIDEAN_ENGINE_TEXTURE_H
