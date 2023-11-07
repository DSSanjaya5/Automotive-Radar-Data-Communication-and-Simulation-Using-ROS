#include <bits/stdc++.h>
#include "ros/ros.h"
#include "beginner_tutorials/AddTwoInts.h"
#include "std_msgs/String.h"
#include <pcl_ros/point_cloud.h>
#include <pcl/point_types.h>
#include "hackathon/location.h"
#include "hackathon/radar_data_srv.h"
using namespace std;
    
// Declaration of the random generators (Data range based on MRR Sensor by BOSCH)
default_random_engine gen;
normal_distribution<double>  gen_azimuth(-42.0, 42.0);
normal_distribution<double>  gen_elevation(-6.0, 6.0);
normal_distribution<double>  gen_radius(0.0, 160.0);
int cyclicIncrementor = 0;


//	processData - Update .srv file with the generated random data


bool processData(hackathon::radar_data_srv::Request  &req, hackathon::radar_data_srv::Response  &res)
{
    hackathon::location loc;
    int num_of_loc;
    ros::param::get("/Number_Of_Locations", num_of_loc);
    for(int i=0; i<num_of_loc; i++)
    {   
        loc.radius = 0 + cyclicIncrementor;
        loc.azimuthAngle = gen_azimuth(gen);
        loc.elevationAngle = gen_elevation(gen);
        res.location_list.push_back(loc);
        ROS_INFO_STREAM("R = "<<loc.radius<<", "<<"A = "<<loc.azimuthAngle<<", "<<"E = "<<loc.elevationAngle<<endl);    
    }
    cyclicIncrementor++;
    return true;
}
   
int main(int argc, char **argv)
{
     // Initialization of the node and declaration of node handle
     ros::init(argc, argv, "add_two_ints_server");
     ros::NodeHandle n;
     
     // Advertise Service radar_data
     ros::ServiceServer service = n.advertiseService("radar_data_srv", processData);
     
     // Set the ROS Parameter
     ros::param::set("/Number_Of_Locations", 6);
     ROS_INFO("Processing Data......\n");
     
     // spin() will not return until the node has been shutdown
     ros::spin();
     
     ros::shutdown();
     return 0;
}
