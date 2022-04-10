//
// Created by Pavlov Konstantin on 10.04.2022.
//

#ifndef NON_EUCLIDEAN_ENGINE_INPUT_H
#define NON_EUCLIDEAN_ENGINE_INPUT_H

#include <unordered_map>

class Input {
private:
    std::unordered_map<int,bool> keyMap;
public:
    Input() : keyMap(){}
    bool SetKey(int key,bool pressed){
        keyMap[key] = pressed;
    }
    bool IsKeyPressed(int key){
        return keyMap[key];
    }
    void ClearInput(){
        keyMap = {};
    }
};

#endif //NON_EUCLIDEAN_ENGINE_INPUT_H
