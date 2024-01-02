#ifndef COORDINATE_CONVERSION_H
#define COORDINATE_CONVERSION_H
#include <ros/ros.h>

struct SphericalCoordinate {
    float radius;
    float azimuthAngle;
    float elevationAngle;
};

struct CartesianCoordinate {
    float x;
    float y;
    float z;
};

CartesianCoordinate sphericalToCartesian(float r, float a, float e);

SphericalCoordinate cartesianToSpherical(float x, float y, float z);

#endif // SPHERICAL_TO_CARTESIAN_H
