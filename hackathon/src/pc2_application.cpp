#include <bits/stdc++.h>
#include "ros/ros.h"
#include "std_msgs/String.h"
#include <pcl_ros/point_cloud.h>
#include <pcl/point_types.h>
#include "hackathon/location.h"
#include "hackathon/radar_data_extension.h"
#include "hackathon/coordinate_conversion.h"
#include "hackathon/recover.h"
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define YELLOW  "\033[33m"
#define GREEN   "\033[32m" 
using namespace std;
typedef pcl::PointCloud<pcl::PointXYZI> PointCloud;

// Publisher declaration to publish points
ros::Publisher points_pub;

// Declare a client for recover service
ros::ServiceClient client;  

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
        coordinates.points[i].intensity = 255*(1-(r_data.location_list[i].radius/max_range));

        points_pub.publish(coordinates);

    }
    ROS_INFO_STREAM("\nTemperature = "<<r_data.temperature);
    ROS_INFO_STREAM("\nBatch complete\n");
}

void obdCallback(const std_msgs::String::ConstPtr& dtc)
{
    if(dtc->data == "A0001")
        ROS_INFO_STREAM(RED<<"RADAR temperature is above the operating temperature range !!"<< RESET<< endl);
    else if(dtc->data == "A0000")
        ROS_INFO_STREAM(RED<<"RADAR temperature is below the operating temperature range !!"<< RESET<< endl);

    int recoverAction;
    ros::param::get("/RecoverAction", recoverAction);
    if(recoverAction==1)
    {
        ros::param::set("/RecoverAction", 0);
        hackathon::recover srv;
        srv.request.recoverRADAR = true;
        if(client.call(srv))
        {
            ROS_INFO_STREAM(GREEN<<"RADAR has been recovered"<<RESET<<endl);
        }
    }
}

int main(int argc, char **argv)
{
    // Initialization of the node and declaration of node handle
    ros::init(argc, argv, "pc2_application");
    ros::NodeHandle n;

    ros::param::set("/RecoverAction", 0);

    // Subscribe to RadarDataExtension and OBD topics
    ros::Subscriber sub = n.subscribe("RadarDataExtension", 1000, processingCallback);
    ros::Subscriber DTC_sub = n.subscribe("OBD", 100, obdCallback);

    // Declare a client for recover service
    client = n.serviceClient<hackathon::recover>("recover");

    // Advertise the Publisher Node
    points_pub = n.advertise<PointCloud>("RadarPointCloud", 2000);

    // spin() will not return until the node has been shutdown
    ros::spin();

    ros::shutdown();
    return 0;
}