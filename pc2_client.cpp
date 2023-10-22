#include "ros/ros.h"
#include "hackathon/radar_data_srv.h"
#include <cstdlib>
#include <bits/stdc++.h>
#include "std_msgs/String.h"
#include <pcl_ros/point_cloud.h>
#include <pcl/point_types.h>
#include "hackathon/location.h"
#include "hackathon/radar_data_srv.h"
using namespace std;

typedef pcl::PointCloud<pcl::PointXYZRGB> PointCloud;
ros::Publisher points_pub;
    
int main(int argc, char **argv)
{
   
      ros::init(argc, argv, "pc2_client");
   
     ros::NodeHandle n;
     ros::ServiceClient client = n.serviceClient<hackathon::radar_data_srv>("radar_data_srv");
     points_pub = n.advertise<PointCloud>("RadarPointCloud", 1000);
     ros::Rate loop_rate(1.5);
     while(ros::ok())
     {
     hackathon::radar_data_srv srv;
     if (client.call(srv))
     {
        PointCloud coordinates;
  	coordinates.header.frame_id = "points";
        coordinates.height = 1;
        coordinates.width = srv.response.location_list.size();
        coordinates.points.resize(coordinates.height * coordinates.width);
  	for(int i=0; i<srv.response.location_list.size(); i++)
  	{
      float x = srv.response.location_list[i].radius*sin((180/M_PI)*(90-srv.response.location_list[i].elevationAngle))*cos((180/M_PI)*srv.response.location_list[i].azimuthAngle);
  
      float y = srv.response.location_list[i].radius*sin((180/M_PI)*(90-srv.response.location_list[i].elevationAngle))*sin((180/M_PI)*srv.response.location_list[i].azimuthAngle);
  
      float z = srv.response.location_list[i].radius*cos((180/M_PI)*(90-srv.response.location_list[i].elevationAngle));
    
      ROS_INFO_STREAM("x = "<<x<<", "<<"y = "<<y<<", "<<"z = "<<z<<endl);
  
      float r = 255.0 * (float) rand()/RAND_MAX;
      float g = 255.0 * (float) rand()/RAND_MAX;
      float b = 255.0 * (float) rand()/RAND_MAX;
      coordinates.points[i].x = x;
      coordinates.points[i].y = y;
      coordinates.points[i].z = z;
      coordinates.points[i].r = r;
      coordinates.points[i].g = g;
      coordinates.points[i].b = b;
      //coordinates.points.push_back(pcl::PointXYZ(x,y,z));
      points_pub.publish(coordinates);
      
  }
     }
     else
     {
       ROS_ERROR("Failed to call service");
       return 1;
     }
     loop_rate.sleep();
 }  
     return 0;
     
   }

