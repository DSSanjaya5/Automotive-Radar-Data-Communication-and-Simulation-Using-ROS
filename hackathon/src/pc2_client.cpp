#include "ros/ros.h"
#include <cstdlib>
#include <bits/stdc++.h>
#include "std_msgs/String.h"
#include <pcl_ros/point_cloud.h>
#include <pcl/point_types.h>
#include "hackathon/location.h"
#include "hackathon/radar_data_srv.h"
#include "hackathon/coordinate_conversion.h"
using namespace std;
typedef pcl::PointCloud<pcl::PointXYZRGB> PointCloud;

// Publisher declaration to publish points
ros::Publisher points_pub;

    
int main(int argc, char **argv)
{  
    // Initialization of the node and declaration of node handle
    ros::init(argc, argv, "pc2_client");
    ros::NodeHandle n;
    
    // Create a client for radar_data service
    ros::ServiceClient client = n.serviceClient<hackathon::radar_data_srv>("radar_data_srv");
    
    // Advertise the Publisher Node
    points_pub = n.advertise<PointCloud>("RadarPointCloud", 1000);
    
    // Set the requesting rate = 1.5 Hz
    ros::Rate loop_rate(1.5);

    while(ros::ok())
    {
        hackathon::radar_data_srv srv;
        
        // If request is successful then, transform data from spherical coordinate system to cartesian coordinatate system
        // and append the data to a PointCloud points and publish it over RadarPointCloud topic 
        // else, display error
        
        if (client.call(srv))
        {
            PointCloud coordinates;
            coordinates.header.frame_id = "points";
            coordinates.height = 1;
            coordinates.width = srv.response.location_list.size();
            coordinates.points.resize(coordinates.height * coordinates.width);
            for(int i=0; i<srv.response.location_list.size(); i++)
            {
                CartesianCoordinate cartesianCoord = sphericalToCartesian(srv.response.location_list[i].radius, srv.response.location_list[i].azimuthAngle, srv.response.location_list[i].elevationAngle);
                 ROS_INFO_STREAM("x = " << cartesianCoord.x << ", " << "y = " << cartesianCoord.y << ", " << "z = " << cartesianCoord.z << endl);
  
                float r = 255.0 * (float) rand()/RAND_MAX;
                float g = 255.0 * (float) rand()/RAND_MAX;
                float b = 255.0 * (float) rand()/RAND_MAX;
                
                coordinates.points[i].x = cartesianCoord.x;
                coordinates.points[i].y = cartesianCoord.y;
                coordinates.points[i].z = cartesianCoord.z;
                coordinates.points[i].r = r;
                coordinates.points[i].g = g;
                coordinates.points[i].b = b;
                
                points_pub.publish(coordinates);
                
            }
        }
        else
        {
            ROS_ERROR("Failed to call service");
        }
        
        // Dynamically maintain the requesting rate
        loop_rate.sleep();
    }
    
    ros::shutdown();  
    return 0;    
}

