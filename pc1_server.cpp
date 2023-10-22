#include <bits/stdc++.h>
#include "ros/ros.h"
#include "beginner_tutorials/AddTwoInts.h"
#include "std_msgs/String.h"
#include <pcl_ros/point_cloud.h>
#include <pcl/point_types.h>
#include "hackathon/location.h"
#include "hackathon/radar_data_srv.h"
using namespace std;
    
int num =0;
default_random_engine gen;
     //normal_distribution<double>  gen_azimuth(-30.0, 30.0);
     //normal_distribution<double>  gen_elevation(-30.0, 30.0);
    uniform_real_distribution<double>  gen_azimuth(-30.0, 30.0);
    uniform_real_distribution<double>  gen_elevation(-30.0, 30.0);


bool processData(hackathon::radar_data_srv::Request  &req, hackathon::radar_data_srv::Response  &res)
{
    num++;
    hackathon::location loc;
    int num_of_loc;
    ros::param::get("/Number_Of_Locations", num_of_loc);
    for(int i=0; i<num_of_loc; i++)
    { 
      loc.radius = 2;
      loc.azimuthAngle = gen_azimuth(gen);
      loc.elevationAngle = gen_elevation(gen);
      res.location_list.push_back(loc);
      res.id=num;
      ROS_INFO_STREAM("R = "<<loc.radius<<", "<<"A = "<<loc.azimuthAngle<<", "<<"E = "<<loc.elevationAngle<<endl);
           
    }
      return true;
   }
   
int main(int argc, char **argv)
{
     ros::init(argc, argv, "add_two_ints_server");
     ros::NodeHandle n;
     ros::ServiceServer service = n.advertiseService("radar_data_srv", processData);
     ros::param::set("/Number_Of_Locations", 6);
     ROS_INFO("Processing Data......\n");
     ros::spin();
     return 0;
   }
