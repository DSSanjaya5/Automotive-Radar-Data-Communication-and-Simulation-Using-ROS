#include <bits/stdc++.h>
#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>
#include "hackathon/location.h"
#include "hackathon/radar_data_extension.h"
#include "hackathon/coordinate_conversion.h"
#include "hackathon/recover.h"
using namespace std;
#define RESET   "\033[0m"
#define GREEN   "\033[32m" 

int Max_Loc = 1000;
float Max_Temp, Min_Temp;
float expRate = 0;
string modulation;
bool systemReact = false;
float temp = 25.0;

void restrictExpRate()
{
    if(expRate>0.5)
        expRate = 0.5;
    else if(expRate<0.001)
        expRate = 0.001;
}

void increaseLoc()
{
    int num_of_loc;
    ros::param::get("/Number_Of_Locations", num_of_loc);
    int nlocs = (int)(num_of_loc*exp(0.1));
    if(nlocs>Max_Loc)
        nlocs = Max_Loc;
    ros::param::set("/Number_Of_Locations", nlocs);
}

void decreaseTemp(float& temp, float rate)
{
    if(temp<1)
        temp = abs(temp) * -exp(rate);
    else
        temp *= exp(-rate);
}

void increaseTemp(float& temp, float rate)
{
    if(temp<-1)
        temp = abs(temp) * -exp(-rate);
    else
        temp = abs(temp) * exp(rate);
}

void modulateTemperature(float& temp)
{
    if(modulation == "increase")
    {
        restrictExpRate();
        increaseTemp(temp, expRate);
        cout<<"\nMod Increasing...\n";
    }
    else if(modulation == "decrease")
    {
        restrictExpRate();
        decreaseTemp(temp, expRate);
        cout<<"\nMod Decreasing...\n";
    }
    else if(temp < (Max_Temp+Min_Temp)/2)
    {
        increaseTemp(temp, 0.1);
        cout<<"\nIncreasing...\n";
    }
    else if(temp > (Max_Temp+Min_Temp)/2)
    {
        decreaseTemp(temp, 0.1);
        cout<<"\nDecreasing...\n";
    }
}

void monitorTemperature(float& temp, int num_of_loc, hackathon::radar_data_extension& data, ros::Publisher& RadarData_pub, std_msgs::String& dtc, ros::Publisher& DTC_pub)
{
    vector<float> tempRange(2, 0);
    ros::param::get("/Temperature_Range", tempRange);
    ros::param::get("/Modulate_Temperature", modulation);
    ros::param::get("/ExpRate", expRate);
    Min_Temp = tempRange[0];
    Max_Temp = tempRange[1];
    cout<<"\n"<<modulation<<" Max_Temp = "<<Max_Temp<<" Min_Temp = "<<Min_Temp<<endl;

    if(temp > Max_Temp || temp < Min_Temp)
        systemReact = true;
    else if(temp < (Max_Temp-15) && temp>(Min_Temp+15))  
        systemReact = false;

    if(systemReact)
    {
        modulation = " ";
        ros::param::set("/Modulate_Temperature", modulation);
        ros::param::set("/Number_Of_Locations", Max_Loc/2);
        data.temperature = 0;
        // Publish the data
        RadarData_pub.publish(data);
        if(temp > (Max_Temp+Min_Temp)/2)
        {
            //decreaseTemp(temp, 0.05);
            temp = Max_Temp;
            dtc.data = "A0001";
            DTC_pub.publish(dtc);
        }
        else
        {
            //increaseTemp(temp, 0.05);
            temp = Min_Temp;
            dtc.data = "A0000";
            DTC_pub.publish(dtc);
        }
    }
    else
    {
        modulateTemperature(temp);
        if(num_of_loc < Max_Loc && (temp < (Max_Temp-15) && temp>(Min_Temp+15)))
            increaseLoc();
        data.temperature = temp;
        RadarData_pub.publish(data);
    }
}

bool processRecoverRequest(hackathon::recover::Request  &req, hackathon::recover::Response  &res)
{
    if(req.recoverRADAR)
    {
        if(temp > (Max_Temp+Min_Temp)/2)
            decreaseTemp(temp, 0.01);
        else
            increaseTemp(temp, 0.01);
        systemReact = false;
        ROS_INFO_STREAM(GREEN<<"RADAR has been recovered"<<RESET<<endl);
        return true;
    }
    return false;
}

int main(int argc, char **argv)
{

    // Initialization of the node and declaration of node handle
    ros::init(argc, argv, "pc1_application");
    ros::NodeHandle n;

    // Advertise the Publisher Node
    ros::Publisher RadarData_pub = n.advertise<hackathon::radar_data_extension>("RadarDataExtension", 1000);
    ros::Publisher DTC_pub = n.advertise<std_msgs::String>("OBD", 100);

    // Advertise Service radar_data
     ros::ServiceServer service = n.advertiseService("recover", processRecoverRequest);

    vector<float> tempRange;
    tempRange.push_back(-40);         // Based on operating temperature of MRR and LRR Sensors by BOSCH
    tempRange.push_back(85);          // Min_Temp = -40 and Max_Temp = 85
    modulation = " ";                 // No Modulation initially
    expRate = 0;
    

    // Set the ROS Parameter
    ros::param::set("/Number_Of_Locations", Max_Loc);
    ros::param::set("/Temperature_Range", tempRange);
    ros::param::set("/Modulate_Temperature", modulation);
    ros::param::set("/ExpRate", expRate);

    // Set the publishing rate = 1.5 Hz
    ros::Rate loop_rate(1.5);

    // Declaration of the random generators (Data range based on MRR Sensor by BOSCH)
    default_random_engine gen;
    uniform_real_distribution<double>  gen_x(-5, 5);
    uniform_real_distribution<double>  gen_z(-5, 5);

    float y = 0;

    while (ros::ok())
    {
        // Get the ros parameter value
        int num_of_loc;
        ros::param::get("/Number_Of_Locations", num_of_loc);

        // Declaration of instances of msg files
        hackathon::location loc;
        hackathon::radar_data_extension data;
        std_msgs::String dtc;
        SphericalCoordinate sphericalCoord;

        for(int i=0; i<num_of_loc; i++)
        {
            float x = gen_x(gen);
            float z = gen_z(gen);
            if(x>=0 && x<=2 && z>=0 && z<=2)
            {
                sphericalCoord = cartesianToSpherical(x, y, z);
                loc.radius = sphericalCoord.radius;
                loc.azimuthAngle = sphericalCoord.azimuthAngle;
                loc.elevationAngle = sphericalCoord.elevationAngle;
            }
            else
            {
                sphericalCoord = cartesianToSpherical(x, 0, z);
                loc.radius = sphericalCoord.radius;
                loc.azimuthAngle = sphericalCoord.azimuthAngle;
                loc.elevationAngle = sphericalCoord.elevationAngle;
            }
            data.location_list.push_back(loc);
            ROS_INFO_STREAM("R = "<<loc.radius<<", "<<"A = "<<loc.azimuthAngle<<", "<<"E = "<<loc.elevationAngle<<endl);
        }

        data.temperature = temp;
        ROS_INFO_STREAM("Temperature = "<<temp<<endl);
        ROS_INFO_STREAM("Number of Locations : "<<num_of_loc<<endl);

        monitorTemperature(temp, num_of_loc, data, RadarData_pub, dtc, DTC_pub);

        y += 0.5;
        y = abs(y);
        if(y>10)
            y = 0.5;

        // spinOnce() will call all the callbacks waiting to be called at that point in time
        ros::spinOnce();

        // Dynamically maintain the publishing rate
        loop_rate.sleep();
    }

    ros::shutdown();
    return 0;
}
