#include <iostream>
#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>

// PCL includes
#include <pcl/point_types.h>
#include <pcl_ros/point_cloud.h>
#include <pcl/common/common.h>

#include "velodyne_gazebo_plugins/radar_data.h"
#include "velodyne_gazebo_plugins/location.h"

using namespace std;

ros::Publisher pub_;
class SubscribePointCloud2Message
{
public:
    ros::Publisher points_pub;
    SubscribePointCloud2Message()
    {
        // velodyne_points  -> VLP-16
        // velodyne_points2 -> HDL-32E
        sub_ = nh_.subscribe ("/velodyne_points2", 1,  &SubscribePointCloud2Message::cloudCallback, this);
    }

private:
    // Callback
    void cloudCallback(const pcl::PCLPointCloud2::ConstPtr& callback)
    {
        pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ>);
        pcl::fromPCLPointCloud2(*callback, *cloud);

        std::cout << "Real Time Point Cloud Data:" << std::endl;
        std::cerr << *cloud << std::endl;
        std::cout << "Point Cloud data has: " << cloud->points.size () << " data points." << std::endl;
        std::cout << "----------------------------" << std::endl;
        
        velodyne_gazebo_plugins::location loc;
        velodyne_gazebo_plugins::radar_data data;

for (std::size_t i = 0; i < cloud->points.size(); ++i)
    {
        pcl::PointXYZ point = cloud->points[i];

        // Access the coordinates of the point
        float x = point.x;
        float y = point.y;
        float z = point.z;

        // Calculate radius
        float radius = sqrt(x*x + y*y + z*z);;

        // Calculate azimuth (angle in the xy-plane)
        float azimuth = (180/M_PI) * atan(y/x);

        // Calculate elevation (angle from the xy-plane)
        float elevation = 90 - ((180/M_PI) * atan(sqrt(x*x + y*y)/z));
        

        // Access the intensity value
        //float intensity = point.intensity;

        // Do something with the point's information (e.g., print them)
        std::cout << "Point " << i << ": Radius = " << radius
                  << ", Azimuth = " << azimuth
                  << ", Elevation = " << elevation
                 << std::endl;

        loc.radius = radius;
        loc.azimuthAngle = azimuth;
        loc.elevationAngle = elevation;
        data.location_list.push_back(loc);

    }
      pub_.publish(data);
      cout<<" !! DATA PUBLISHED !!\n";
    }

private:
    ros::NodeHandle nh_;
    ros::Subscriber sub_;
};

int main(int argc, char** argv)
{
    ros::init (argc, argv, "Subscribe_PointCloud2_Message", ros::init_options::AnonymousName);
    ros::NodeHandle n;
    pub_ = n.advertise<velodyne_gazebo_plugins::radar_data>("RadarData", 2000);
    SubscribePointCloud2Message subscribePointCloud2Message;
    
    ros::spin();

    return (0);
}