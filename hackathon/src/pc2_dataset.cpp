#include <bits/stdc++.h>
#include "ros/ros.h"
#include "std_msgs/String.h"
#include <pcl_ros/point_cloud.h>
#include <pcl/point_types.h>
#include "hackathon/location.h"
#include "hackathon/radar_data_extension.h"
#include "hackathon/coordinate_conversion.h"
using namespace std;
typedef pcl::PointCloud<pcl::PointXYZI> PointCloud;

// Publisher declaration to publish points
ros::Publisher points_pub;


/*
	processingCallback - Transform data from spherical coordinate system to cartesian coordinate system
	Append the data to a PointCloud points and publish it over RadarPointCloud topic
*/

void processingCallback(const hackathon::radar_data_extension r_data)
{
    PointCloud coordinates;
    coordinates.header.frame_id = "points";
    coordinates.height = 1;
    coordinates.width = r_data.location_list.size();
    coordinates.points.resize(coordinates.height * coordinates.width);
    float max_range=25.0;

    for(int i=0; i<r_data.location_list.size(); i++)
    {
        CartesianCoordinate cartesianCoord = sphericalToCartesian(r_data.location_list[i].radius, r_data.location_list[i].azimuthAngle, r_data.location_list[i].elevationAngle);

        ROS_INFO_STREAM("x = " << cartesianCoord.x << ", " << "y = " << cartesianCoord.y << ", " << "z = " << cartesianCoord.z << endl);

        coordinates.points[i].x = cartesianCoord.x;
        coordinates.points[i].y = cartesianCoord.y;
        coordinates.points[i].z = cartesianCoord.z;
        coordinates.points[i].intensity = r_data.location_list[i].intensity;

        points_pub.publish(coordinates);
    }
    ROS_INFO_STREAM("\nBatch complete\n");
}

int main(int argc, char **argv)
{

    /*string cmd = "rosrun rviz rviz -d /home/user/catkin_ws/src/hackathon/rviz/radar.rviz";
    int result = system(cmd.c_str());
    if(result == -1)
    	ROS_INFO_STREAM("\nRviz launch failed.\n");*/

    // Initialization of the node and declaration of node handle
    ros::init(argc, argv, "pc2_dataset");
    ros::NodeHandle n;

    // Subscribe to RadarData topic
    ros::Subscriber sub = n.subscribe("RadarDataExtension", 1000, processingCallback);

    // Advertise the Publisher Node
    points_pub = n.advertise<PointCloud>("RadarPointCloud", 1000);

    // spin() will not return until the node has been shutdown
    ros::spin();

    ros::shutdown();
    return 0;
}