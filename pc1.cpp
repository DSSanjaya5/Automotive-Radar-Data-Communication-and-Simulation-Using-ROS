#include <bits/stdc++.h>
#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>
#include "hackathon/location.h"
#include "hackathon/radar_data.h"
using namespace std;

int main(int argc, char **argv)
{
  ros::init(argc, argv, "pc1");                  // Initialization of the ROS Node

  ros::NodeHandle n;                             // Declaration of the NodeHandle

  ros::Publisher RadarData_pub = n.advertise<hackathon::radar_data>("RadarData", 1000);        // Advertising the Publisher Node publishing over topic - "RadarData" and 1000 being the buffer size 
  
  ros::Rate loop_rate(1.5);                      // Initialising the publishing rate = 1.5 Hz

  // Declaration of Random Data Generators
  default_random_engine gen;
  uniform_real_distribution<double>  gen_azimuth(-180.0, 180.0);
  uniform_real_distribution<double>  gen_elevation(0.0, 180.0);
  uniform_real_distribution<double>  gen_radius(0.0, 100.0);

  // While ROS Node is running
  while (ros::ok())
  { 
    hackathon::location loc;                      // Instance of location.msg
    hackathon::radar_data data;                   // Instance of radar_data.msg
    
    for(int i=0; i<6; i++)                        // Loop for Number_Of_Locations = 6
    {
      loc.radius = 9;                             // Initialise Radius
      loc.azimuthAngle = gen_azimuth(gen);        // Initialise Azimuth Angle
      loc.elevationAngle = gen_elevation(gen);    // Initialise Elevation Angle
      data.location_list.push_back(loc);          // Store the values into data
      ROS_INFO_STREAM("R = "<<loc.radius<<", "<<"A = "<<loc.azimuthAngle<<", "<<"E = "<<loc.elevationAngle<<endl);
    }
    
    RadarData_pub.publish(data);                  // Publish the data
    ros::spinOnce();                              // Will call all the callbacks waiting to be called at that point in time.
    loop_rate.sleep();                            // Dynamically maintains the publishing rate 
  }
  return 0;
}
