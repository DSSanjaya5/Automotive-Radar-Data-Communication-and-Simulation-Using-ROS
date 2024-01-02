#include <cmath>
#include "hackathon/coordinate_conversion.h"


// Function to convert spherical coordinates to Cartesian coordinates
CartesianCoordinate sphericalToCartesian(float r, float a, float e) 
{
    CartesianCoordinate cartesianCoord;
    
    cartesianCoord.x = r * sin((M_PI / 180) * (90 - e)) * cos((M_PI / 180) * a);
    cartesianCoord.y = r * sin((M_PI / 180) * (90 - e)) * sin((M_PI / 180) * a);
    cartesianCoord.z = r * cos((M_PI / 180) * (90 - e));

    return cartesianCoord;
}

SphericalCoordinate cartesianToSpherical(float x, float y, float z) 
{
    SphericalCoordinate sphericalCoord;

    sphericalCoord.radius = sqrt(x*x + y*y + z*z);
    sphericalCoord.azimuthAngle = (180/M_PI) * atan(y/x);
    sphericalCoord.elevationAngle = 90 - ((180/M_PI) * atan(sqrt(x*x + y*y)/z));

    return sphericalCoord;
}