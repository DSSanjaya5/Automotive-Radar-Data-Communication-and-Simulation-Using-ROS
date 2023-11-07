#include <bits/stdc++.h>
#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>
#include "hackathon/location.h"
#include "hackathon/radar_data.h"
#include <fstream>
using namespace std;

int main(int argc, char **argv)
{
    // Initialization of the node and declaration of node handle
    ros::init(argc, argv, "pc1_square");
    ros::NodeHandle n;

    // Advertise the Publisher Node
    ros::Publisher RadarData_pub = n.advertise<hackathon::radar_data>("RadarData", 1000);
    
    // Set the ROS Parameter
    ros::param::set("/Number_Of_Locations", 9);

    // Set the publishing rate = 1.5 Hz
    ros::Rate loop_rate(1.5);
    
    while (ros::ok())
    {
        // Declaration of instances of msg files
        fstream file("/home/user/catkin_ws/src/hackathon/src/spherical_coordinates.csv", ios::in);
        for(int i=0; i<10; i++)
        {
            int lineNumber = 0;
            std::string rad, azi, ele;
            hackathon::location loc;
            hackathon::radar_data data;

            while(lineNumber<9 && file.is_open())
            {
                if(getline(file, rad, ',') && getline(file, azi, ',') && getline(file, ele))
                {
                    ROS_INFO_STREAM("Radius == "<<(float)atof(rad.c_str())<<endl);
                    loc.radius = (float)atof(rad.c_str());
                    loc.azimuthAngle = (float)atof(azi.c_str());
                    loc.elevationAngle = (float)atof(ele.c_str());
                    data.location_list.push_back(loc);
                    ROS_INFO_STREAM("R = "<<loc.radius<<", "<<"A = "<<loc.azimuthAngle<<", "<<"E = "<<loc.elevationAngle<<endl);
                }
                lineNumber++;
            }

            ROS_INFO_STREAM("\nBatch complete\n");
            // Publish the data
            RadarData_pub.publish(data);

            // spinOnce() will call all the callbacks waiting to be called at that point in time
            ros::spinOnce();

            // Dynamically maintain the publishing rate
            loop_rate.sleep();
        }
        file.close();
    }
    ros::shutdown();
    return 0;
}

