#include <bits/stdc++.h>
#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>
#include "hackathon/location.h"
#include "hackathon/radar_data_extension.h"
using namespace std;

int main(int argc, char **argv)
{	
    // Initialization of the node and declaration of node handle
    ros::init(argc, argv, "pc1");				
    ros::NodeHandle n;
  
    // Advertise the Publisher Node
    ros::Publisher RadarData_pub = n.advertise<hackathon::radar_data_extension>("RadarDataExtension", 1000);
  
    // Set the ROS Parameter
    ros::param::set("/Number_Of_Locations", 6);

    // Set the publishing rate = 1.5 Hz
    ros::Rate loop_rate(1.5);
  
    // Declaration of the random generators (Data range based on MRR Sensor by BOSCH)
    default_random_engine gen;
    normal_distribution<double>  gen_speed(-100.0, 100.0);
    normal_distribution<double>  gen_temp(20.0, 50.0);
    normal_distribution<double>  gen_azimuth(-1.0, 1.0);
    normal_distribution<double>  gen_elevation(-1.0, 1.0);
    normal_distribution<double>  gen_radius(0.0, 2.0);
  
    // Variable to update radius cyclically
    int cyclicIncrementor = 0;
  
    while (ros::ok())
    { 
        // Declaration of instances of msg files
        hackathon::location loc;
        hackathon::radar_data_extension data;
    
        // Get the ros parameter value
        int num_of_loc;
        ros::param::get("/Number_Of_Locations", num_of_loc);
    	
    	float rad = gen_radius(gen);
    	data.temperature = gen_temp(gen);
	    data.speed = gen_speed(gen);
        // Update the locations data
        for(int i=0; i<num_of_loc; i++)
        {   
            
            loc.radius = rad;
            loc.azimuthAngle = gen_azimuth(gen);
            loc.elevationAngle = gen_elevation(gen);
            data.location_list.push_back(loc);
            ROS_INFO_STREAM("R = "<<loc.radius<<", "<<"A = "<<loc.azimuthAngle<<", "<<"E = "<<loc.elevationAngle<<endl);
        }
        ROS_INFO_STREAM("Temperature - "<<data.temperature<<"Speed - "<<data.speed);
        cyclicIncrementor++;
        if(cyclicIncrementor>30)
        	cyclicIncrementor = 0;
    
        // Publish the data
        RadarData_pub.publish(data);
    
        // spinOnce() will call all the callbacks waiting to be called at that point in time
        ros::spinOnce();
    
        // Dynamically maintain the publishing rate
        loop_rate.sleep();
    }
  
    ros::shutdown();
    return 0;
}
