#include <bits/stdc++.h>
#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>
#include "hackathon/location.h"
#include "hackathon/radar_data_extension.h"
#include <fstream>
using namespace std;

int main(int argc, char **argv)
{
    // Initialization of the node and declaration of node handle
    ros::init(argc, argv, "pc1_dataset");
    ros::NodeHandle n;

    // Advertise the Publisher Node
    ros::Publisher RadarData_pub = n.advertise<hackathon::radar_data_extension>("RadarDataExtension", 2000);
    
    // Set the ROS Parameter
    ros::param::set("/Number_Of_Locations", 9);

    // Set the publishing rate = 1.5 Hz
    ros::Rate loop_rate(1.5);
    
    while (ros::ok())
    {
        // Declaration of instances of msg files
        for(int i=0; i<546; i++)
        {
            std::ostringstream oss;
            oss << std::setw(3) << std::setfill('0') << i;
            string file_name = oss.str();
            fstream file("/home/user/catkin_ws/src/hackathon/Radar_Dataset/000" + file_name + ".csv", ios::in);
            int lineNumber = 0;
            std::string rad, azi, ele, v_r, intensity;
            hackathon::location loc;
            hackathon::radar_data_extension data;
            getline(file, rad);
            getline(file, rad);
            //while(lineNumber<1943 && file.is_open())
            while(getline(file, rad, ',') && getline(file, azi, ',') && getline(file, ele, ',') && getline(file, v_r, ',') && getline(file, intensity))
            {
                ROS_INFO_STREAM("Radius = "<<(float)atof(rad.c_str())<<endl);
                loc.radius = (float)atof(rad.c_str());
                loc.azimuthAngle = (float)atof(azi.c_str());
                loc.elevationAngle = (float)atof(ele.c_str());
                loc.intensity = (float)atof(intensity.c_str());
                data.location_list.push_back(loc);
                ROS_INFO_STREAM("R = "<<loc.radius<<", "<<"A = "<<loc.azimuthAngle<<", "<<"E = "<<loc.elevationAngle<<"V_r = "<<v_r<<"Intensity = "<<loc.intensity<<endl);
            }

            ROS_INFO_STREAM("\nBatch complete\n");
            // Publish the data
            RadarData_pub.publish(data);

            file.close();

            // spinOnce() will call all the callbacks waiting to be called at that point in time
            ros::spinOnce();

            // Dynamically maintain the publishing rate
            loop_rate.sleep();
        }
    }
    ros::shutdown();
    return 0;
}

