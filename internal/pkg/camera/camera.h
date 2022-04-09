#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_operation.hpp>
#include "../core/geometry.h"
#include <glm/gtx/string_cast.hpp>
#include  <glm/gtx/io.hpp>

#include <vector>

const float ZOOM        =  1.0f;

const glm::vec3 basePoint = glm::vec3(0.0f,0.0,0.0f);

class Camera
{
public:
    Matrix4d pos;
    Vector4d velocity;

    bool slow;
    float zoom;
    float near,far;
    bool forwardPress,backwardPress,upPress,downPress,leftPress,rightPress,clockWise,counterClockWise;

    Camera() : pos(Matrix4d::Identity()), velocity(0, 0, 0, 0), zoom(ZOOM), slow(false){

    }

    double getCameraZoom() {
        return zoom;
    }

    void ProcessMouseScroll(float yoffset)
    {
        zoom -= (float)yoffset;
        if (zoom < 1.0f)
            zoom = 1.0f;
        if (zoom > 45.0f)
            zoom = 45.0f;
    }

    void ProcessKeyboard( float dt)
    {
        Vector4d goalVel(0, 0, 0, 0);
        double speed = 1.0;
        if (forwardPress) {
            goalVel(2) -= speed;
        }
        if (backwardPress) {
            goalVel(2) += speed;
        }
        if (leftPress) {
            goalVel(0) -= speed;
        }
        if (rightPress) {
            goalVel(0) += speed;
        }

        if (downPress) {
            goalVel(1) -= 1;
        }
        if (upPress) {
            goalVel(1) += 1;
        }

        double norm = goalVel.norm();
        if (norm > 1) {
            goalVel /= norm;
        }
        goalVel *= slow ? 0.2 : 2;

        double maxChange = (slow ? 0.8 : 4) * dt;
        Vector4d velDiff = goalVel - velocity;
        double velDiffNorm = velDiff.norm();
        if (velDiffNorm < maxChange) {
            velocity = goalVel;
        } else {
            velocity += velDiff / velDiffNorm * maxChange;
        }

        forwardPress = backwardPress = leftPress = rightPress = clockWise = counterClockWise = upPress = downPress = false;
    }

    void ProcessMouse(double xoffset, double yoffset,double dt) {
        glm::vec2 mouseLook(xoffset,yoffset);
        Matrix4d rotation = Matrix4d::Identity();
        rotation *= VectorMath::rotation(Vector3d(1, 0, 0), -mouseLook.y * 0.002 * zoom);
        rotation *= VectorMath::rotation(Vector3d(0, 1, 0), -mouseLook.x * 0.002 * zoom);

        double zRotation = 0;
        if (clockWise) {
            zRotation -= 1;
        }
        if (counterClockWise) {
            zRotation += 1;
        }
        rotation *= VectorMath::rotation(Vector3d(0, 0, 1), zRotation * dt);

        pos *= rotation;
        velocity = VectorMath::hyperbolicTranspose(rotation) * velocity;

        updateCamera(dt);
    }

    void updateCamera(float dt) {
        ProcessKeyboard(dt);
        setPositionFromVelocity(dt);
        renormalize();
    }

    Matrix4d GetPos() {
        return pos;
    }

    Matrix4d GetViewMatrix() {
        return VectorMath::hyperbolicTranspose(pos);
    }

private:
    void setPositionFromVelocity(float dt ) {
        pos *= VectorMath::hyperbolicDisplacement(velocity * dt);
    }

    void renormalize() {
        pos = VectorMath::orthogonalizeGramSchmidt(pos);
    }
};
#endif