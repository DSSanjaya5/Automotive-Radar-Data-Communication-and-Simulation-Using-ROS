#include <bits/stdc++.h>
#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>
#include "hackathon/location.h"
#include "hackathon/radar_data.h"
using namespace std;

int main(int argc, char **argv)
{
  ros::init(argc, argv, "pc1");      // Initialization if the ROS Node

  ros::NodeHandle n;                 // Declaration of the NodeHandle

  ros::Publisher RadarData_pub = n.advertise<hackathon::radar_data>("RadarData", 1000);        // Advertising the Publisher Node

  ros::Rate loop_rate(1.5);          // Setting the publishing rate = 1.5 Hz
  
  default_random_engine gen;
  uniform_real_distribution<double>  gen_azimuth(-180.0, 180.0);
  uniform_real_distribution<double>  gen_elevation(0.0, 180.0);
  //uniform_real_distribution<double>  gen_radius(0.0, 100.0);
  
  while (ros::ok())
  { 
    hackathon::location loc;
    hackathon::radar_data data;
    
    for(int i=0; i<6; i++)
    {
      loc.radius = 9;
      loc.azimuthAngle = gen_azimuth(gen);
      loc.elevationAngle = gen_elevation(gen);
      data.location_list.push_back(loc);
      ROS_INFO_STREAM("R = "<<loc.radius<<", "<<"A = "<<loc.azimuthAngle<<", "<<"E = "<<loc.elevationAngle<<endl);
    }
    
    RadarData_pub.publish(data);
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}
