//
// Created by Pavlov Konstantin on 29.03.2022.
//

#ifndef NON_EUCLIDEAN_ENGINE_GEOMETRY_H
#define NON_EUCLIDEAN_ENGINE_GEOMETRY_H

#include "consts.h"
#include <Eigen/Dense>

using Matrix4d = Eigen::Matrix<double, 4, 4, Eigen::DontAlign>;
using Vector3d = Eigen::Vector3d;
using Vector4d = Eigen::Matrix<double, 4, 1, Eigen::DontAlign>;
using Vector2d = Eigen::Matrix<double, 2, 1, Eigen::DontAlign>;

constexpr auto M_TAU = 6.2831853071795864769252867665590057683943;

class VectorMath {
public:
    static Matrix4d perspective(double x, double y, double zNear, double zFar) {
        Matrix4d result;
        result << 2.0/x, 0.0, 0.0, 0.0,
                0.0, 2.0/y, 0.0, 0.0,
                0.0, 0.0, 0.0, 0.0,
                0.0, 0.0, -1.0, 0.0;
        return result;
    }

    static glm::mat4x4 euclideanTranslation(const glm::vec3 &v) {
        glm::mat4x4 result;
        result  = glm::mat4x4(1.0, 0.0, 0.0, 0.0,
                0.0, 1.0, 0.0, 0.0,
                0.0, 0.0, 1.0, 0.0,
                v.x, v.y, v.z, 1.0);
        return result;
    }

    // Moves the origin to the specified position
    static Matrix4d hyperbolicTranslation(const Vector4d &v) {
        Matrix4d result;
        double x = v.x(), y = v.y(), z = v.z(), w = v.w();
        double f = 1.0 / (w + 1.0);
        result << x*x*f + 1, x*y*f, x*z*f, x,
                x*y*f, y*y*f + 1, y*z*f, y,
                x*z*f, y*z*f, z*z*f + 1, z,
                x, y, z, w;
        return result;
    }

    static Matrix4d sphericalTranslation(const Vector4d &v) {
        Matrix4d result;
        double x = v.x(), y = v.y(), z = v.z(), w = v.w();
        double f = 1.0 / (w + 1.0);
        result << 1 - x*x*f, x*y*f, x*z*f, x,
                x*y*f, 1 - y*y*f, y*z*f, y,
                x*z*f, y*z*f, 1 - z*z*f, z,
                -x, -y, -z, w;
        return result;
    }

    // Does an ideal rotation about (0, 0, 1, 1)
    static Matrix4d horoRotation(double x, double y) {
        double sqrSum = 0.5*(x*x + y*y);
        Matrix4d result;
        result << 1, 0, x, x,
                0, 1, y, y,
                -x, -y, -sqrSum+1, -sqrSum,
                x, y, sqrSum, sqrSum+1;
        return result;
    }

    static Matrix4d rotation(const Vector3d &axis, double theta) {
        Matrix4d result = Matrix4d::Identity();
        double x = axis.x(), y = axis.y(), z = axis.z();
        double xx = x*x, yy = y*y, zz = z*z, yz = y*z, zx = z*x, xy = x*y;
        double c = cos(theta), s = sin(theta);

        result(0, 0) = xx + (1-xx)*c;
        result(1, 0) = xy*(1-c) + z*s;
        result(2, 0) = zx*(1-c) - y*s;

        result(0, 1) = xy*(1-c) - z*s;
        result(1, 1) = yy + (1-yy)*c;
        result(2, 1) = yz*(1-c) + x*s;

        result(0, 2) = zx*(1-c) + y*s;
        result(1, 2) = yz*(1-c) - x*s;
        result(2, 2) = zz + (1-zz)*c;

        return result;
    }

    static Matrix4d hyperbolicReflection(const Vector4d& normal) {
        return Matrix4d::Identity() - 2.0 * normal * Eigen::RowVector4d(normal(0), normal(1), normal(2), -normal(3));
    }

    static Matrix4d sphericalReflection(const Vector4d& normal) {
        return Matrix4d::Identity() - 2.0 * normal * Eigen::RowVector4d(normal(0), normal(1), normal(2), normal(3));
    }

    static Matrix4d hyperbolicTranspose(const Matrix4d& matrix) {
        Matrix4d result;

        result << matrix(0, 0), matrix(1, 0), matrix(2, 0), -matrix(3, 0),
                matrix(0, 1), matrix(1, 1), matrix(2, 1), -matrix(3, 1),
                matrix(0, 2), matrix(1, 2), matrix(2, 2), -matrix(3, 2),
                -matrix(0, 3), -matrix(1, 3), -matrix(2, 3), matrix(3, 3);

        return result;
    }

    static Vector4d hyperbolicNormal(const Vector4d& v0, const Vector4d& v1, const Vector4d& v2) {
        double x =   v0[1]*v1[2]*v2[3] + v0[2]*v1[3]*v2[1] + v0[3]*v1[1]*v2[2] - v0[1]*v1[3]*v2[2] - v0[2]*v1[1]*v2[3] - v0[3]*v1[2]*v2[1];
        double y = -(v0[0]*v1[2]*v2[3] + v0[2]*v1[3]*v2[0] + v0[3]*v1[0]*v2[2] - v0[0]*v1[3]*v2[2] - v0[2]*v1[0]*v2[3] - v0[3]*v1[2]*v2[0]);
        double z =   v0[0]*v1[1]*v2[3] + v0[1]*v1[3]*v2[0] + v0[3]*v1[0]*v2[1] - v0[0]*v1[3]*v2[1] - v0[1]*v1[0]*v2[3] - v0[3]*v1[1]*v2[0];
        double w = -(v0[0]*v1[1]*v2[2] + v0[1]*v1[2]*v2[0] + v0[2]*v1[0]*v2[1] - v0[0]*v1[2]*v2[1] - v0[1]*v1[0]*v2[2] - v0[2]*v1[1]*v2[0]);
        return Vector4d(x, y, z, -w);
    }

    static Vector4d sphericalNormal(const Vector4d& v0, const Vector4d& v1, const Vector4d& v2) {
        double x =   v0[1]*v1[2]*v2[3] + v0[2]*v1[3]*v2[1] + v0[3]*v1[1]*v2[2] - v0[1]*v1[3]*v2[2] - v0[2]*v1[1]*v2[3] - v0[3]*v1[2]*v2[1];
        double y = -(v0[0]*v1[2]*v2[3] + v0[2]*v1[3]*v2[0] + v0[3]*v1[0]*v2[2] - v0[0]*v1[3]*v2[2] - v0[2]*v1[0]*v2[3] - v0[3]*v1[2]*v2[0]);
        double z =   v0[0]*v1[1]*v2[3] + v0[1]*v1[3]*v2[0] + v0[3]*v1[0]*v2[1] - v0[0]*v1[3]*v2[1] - v0[1]*v1[0]*v2[3] - v0[3]*v1[1]*v2[0];
        double w = -(v0[0]*v1[1]*v2[2] + v0[1]*v1[2]*v2[0] + v0[2]*v1[0]*v2[1] - v0[0]*v1[2]*v2[1] - v0[1]*v1[0]*v2[2] - v0[2]*v1[1]*v2[0]);
        return Vector4d(x, y, z, w);
    }

    static double hyperbolicDotProduct(const Vector4d& v0, const Vector4d& v1) {
        return v0(0)*v1(0) + v0(1)*v1(1) + v0(2)*v1(2) - v0(3)*v1(3);
    }

    static double hyperbolicSqrNorm(const Vector4d& v) {
        return hyperbolicDotProduct(v, v);
    }

    // Moves the origin in the specified direction with a distance proportional
    // to the magnitude of the argument (The fourth component is assumed to be 0)
    static Matrix4d hyperbolicDisplacement(const Vector4d &displacement) {
        double norm = displacement.norm();
        double scaleFactor = norm < 1e-30 ? 1.0 : sinh(norm) / norm;
        Vector4d translateVector(displacement.x() * scaleFactor, displacement.y() * scaleFactor, displacement.z() * scaleFactor, cosh(norm));
        return hyperbolicTranslation(translateVector);
    }

    static Matrix4d sphericalDisplacement(const Vector4d &displacement) {
        double norm = displacement.norm();
        double scaleFactor = norm < 1e-30 ? 1.0 : sin(norm) / norm;
        Vector4d translateVector(displacement.x() * scaleFactor, displacement.y() * scaleFactor, displacement.z() * scaleFactor, cos(norm));
        return sphericalTranslation(translateVector);
    }

    static Matrix4d hyperbolicQrUnitary(const Matrix4d& matrix) {
        Matrix4d result = matrix;
        result.col(0) /= sqrt(hyperbolicSqrNorm(result.col(0)));
        result.col(1) -= result.col(0) * hyperbolicDotProduct(result.col(0), result.col(1));
        result.col(2) -= result.col(0) * hyperbolicDotProduct(result.col(0), result.col(2));
        result.col(3) -= result.col(0) * hyperbolicDotProduct(result.col(0), result.col(3));

        result.col(1) /= sqrt(hyperbolicSqrNorm(result.col(1)));
        result.col(2) -= result.col(1) * hyperbolicDotProduct(result.col(1), result.col(2));
        result.col(3) -= result.col(1) * hyperbolicDotProduct(result.col(1), result.col(3));

        result.col(2) /= sqrt(hyperbolicSqrNorm(result.col(2)));
        result.col(3) -= result.col(2) * hyperbolicDotProduct(result.col(2), result.col(3));

        result.col(3) /= sqrt(-hyperbolicSqrNorm(result.col(3)));
        return result;
    }

    static Matrix4d sphericalQrUnitary(const Matrix4d& matrix) {
        Matrix4d result = matrix;
        result.col(0).normalize();
        result.col(1) -= result.col(0) * result.col(0).dot(result.col(1));
        result.col(2) -= result.col(0) * result.col(0).dot(result.col(2));
        result.col(3) -= result.col(0) * result.col(0).dot(result.col(3));

        result.col(1).normalize();
        result.col(2) -= result.col(1) * result.col(1).dot(result.col(2));
        result.col(3) -= result.col(1) * result.col(1).dot(result.col(3));

        result.col(2).normalize();
        result.col(3) -= result.col(2) * result.col(2).dot(result.col(3));

        result.col(3).normalize();
        return result;
    }

    static Matrix4d hyperbolicSvdUnitary(const Matrix4d& matrix);

    static Matrix4d sphericalSvdUnitary(const Matrix4d& matrix);

    static double dotProductGlm(const glm::vec4& v0, const glm::vec4& v1) {
        return v0.x*v1.x + v0.y*v1.y + v0.z*v1.z - v0.w*v1.w;
    }

    static double dotProduct(const Vector4d & v0, const Vector4d& v1) {
        return v0.x()*v1.x() + v0.y()*v1.y() + v0.z()*v1.z() - v0.w()*v1.w();
    }

    // Moves the origin to the specified position
    static glm::mat4x4 hyperbolicTranslationGlm(const glm::vec4 &v) {
        glm::mat4x4 result;
        double x = v.x, y = v.y, z = v.z, w = v.w;
        double f = 1.0 / (w + 1.0);
        result = glm::mat4x4( x*x*f + 1, x*y*f, x*z*f, x,
                x*y*f, y*y*f + 1, y*z*f, y,
                x*z*f, y*z*f, z*z*f + 1, z,
                x, y, z, w);
        return result;
    }

    static glm::mat4x4 hyperbolicDisplacementGlm(const glm::vec4 &displacement) {
        double norm = glm::length(displacement);
        double scaleFactor = norm < 1e-30 ? 1.0 : sinh(norm) / norm;
        glm::vec4 translateVector(displacement.x * scaleFactor, displacement.y * scaleFactor, displacement.z * scaleFactor, cosh(norm));
        return hyperbolicTranslationGlm(translateVector);
    }

    static glm::vec4 hyperbolicNormalGlm(const glm::vec4& v0, const glm::vec4& v1, const glm::vec4& v2) {
        double x =   v0[1]*v1[2]*v2[3] + v0[2]*v1[3]*v2[1] + v0[3]*v1[1]*v2[2] - v0[1]*v1[3]*v2[2] - v0[2]*v1[1]*v2[3] - v0[3]*v1[2]*v2[1];
        double y = -(v0[0]*v1[2]*v2[3] + v0[2]*v1[3]*v2[0] + v0[3]*v1[0]*v2[2] - v0[0]*v1[3]*v2[2] - v0[2]*v1[0]*v2[3] - v0[3]*v1[2]*v2[0]);
        double z =   v0[0]*v1[1]*v2[3] + v0[1]*v1[3]*v2[0] + v0[3]*v1[0]*v2[1] - v0[0]*v1[3]*v2[1] - v0[1]*v1[0]*v2[3] - v0[3]*v1[1]*v2[0];
        double w = -(v0[0]*v1[1]*v2[2] + v0[1]*v1[2]*v2[0] + v0[2]*v1[0]*v2[1] - v0[0]*v1[2]*v2[1] - v0[1]*v1[0]*v2[2] - v0[2]*v1[1]*v2[0]);
        return glm::vec4(x, y, z, -w);
    }

    static Matrix4d orthogonalizeGramSchmidt(const Matrix4d& matrix) {
        Matrix4d result = matrix;
        result.col(0) /= sqrt(hyperbolicSqrNorm(result.col(0)));
        result.col(1) -= result.col(0) * hyperbolicDotProduct(result.col(0), result.col(1));
        result.col(2) -= result.col(0) * hyperbolicDotProduct(result.col(0), result.col(2));
        result.col(3) -= result.col(0) * hyperbolicDotProduct(result.col(0), result.col(3));

        result.col(1) /= sqrt(hyperbolicSqrNorm(result.col(1)));
        result.col(2) -= result.col(1) * hyperbolicDotProduct(result.col(1), result.col(2));
        result.col(3) -= result.col(1) * hyperbolicDotProduct(result.col(1), result.col(3));

        result.col(2) /= sqrt(hyperbolicSqrNorm(result.col(2)));
        result.col(3) -= result.col(2) * hyperbolicDotProduct(result.col(2), result.col(3));

        result.col(3) /= sqrt(-hyperbolicSqrNorm(result.col(3)));
        return result;
    }
};

#endif //NON_EUCLIDEAN_ENGINE_GEOMETRY_H
