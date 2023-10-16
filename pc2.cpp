#include <bits/stdc++.h>
#include "ros/ros.h"
#include "std_msgs/String.h"
#include <pcl_ros/point_cloud.h>
#include <pcl/point_types.h>
#include "hackathon/location.h"
#include "hackathon/radar_data.h"
using namespace std;

typedef pcl::PointCloud<pcl::PointXYZ> PointCloud;                        // Create PointCloud typedef for pcl::PointXYZ
 
void processingCallback(const hackathon::radar_data r_data)
{
  PointCloud::Ptr coordinates (new PointCloud);                           // Declare a PointCloud pointer
  coordinates->header.frame_id = "points";                                // Set the frame id
  coordinates->height = coordinates->width = 1;                           // Set height and width

  // Transformation of Spherical Coordinates to Cartesian Coordinates
  for(int i=0; i<6; i++)
  {
    float x = r_data.location_list[i].radius*sin((180/M_PI)*(90-r_data.location_list[i].elevationAngle))*cos((180/M_PI)*r_data.location_list[i].azimuthAngle);
  
    float y = r_data.location_list[i].radius*sin((180/M_PI)*(90-r_data.location_list[i].elevationAngle))*sin((180/M_PI)*r_data.location_list[i].azimuthAngle);
  
    float z = r_data.location_list[i].radius*sin((180/M_PI)*(90-r_data.location_list[i].elevationAngle));
    
    ROS_INFO_STREAM("x = "<<x<<", "<<"y = "<<y<<", "<<"z = "<<z<<endl);
  
    coordinates->points.push_back(pcl::PointXYZ(x,y,z));                  // store the values of the cartesian coordinates 
  }
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "pc2");                                          // Initialization of ROS Node
  ros::NodeHandle n;                                                     // Declaration of the NodeHandle
  ros::Subscriber sub = n.subscribe("RadarData", 1000, processingCallback);  // Advertise

  ros::spin();

  return 0;
}
