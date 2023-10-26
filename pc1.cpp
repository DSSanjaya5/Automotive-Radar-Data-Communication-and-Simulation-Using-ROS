#include <bits/stdc++.h>
#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>
#include "hackathon/location.h"
#include "hackathon/radar_data.h"
using namespace std;
int num=0;

int main(int argc, char **argv)
{

  ros::init(argc, argv, "pc1");

  ros::NodeHandle n;

  ros::Publisher RadarData_pub = n.advertise<hackathon::radar_data>("RadarData", 1000);
  ros::param::set("/Number_Of_Locations", 6);

  ros::Rate loop_rate(1);
  
  default_random_engine gen;
     //normal_distribution<double>  gen_azimuth(-30.0, 30.0);
     //normal_distribution<double>  gen_elevation(-30.0, 30.0);
  uniform_real_distribution<double>  gen_azimuth(-30.0, 30.0);
  uniform_real_distribution<double>  gen_elevation(-30.0, 30.0);
  //uniform_real_distribution<double>  gen_radius(0.0, 100.0);
  
  while (ros::ok())
  {
    hackathon::location loc;
    hackathon::radar_data data;
    int num_of_loc;
    ros::param::get("/Number_Of_Locations", num_of_loc);
    for(int i=0; i<num_of_loc; i++)
    { 
      loc.radius = 2;
      loc.azimuthAngle = gen_azimuth(gen);
      loc.elevationAngle = gen_elevation(gen);
      data.location_list.push_back(loc);
      //data.id=num;
      ROS_INFO_STREAM("R = "<<loc.radius<<", "<<"A = "<<loc.azimuthAngle<<", "<<"E = "<<loc.elevationAngle<<endl);
    }
    
    RadarData_pub.publish(data);
    ros::spinOnce();
    loop_rate.sleep();
    //break;
  }

  return 0;
}
