#include <iostream>
#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>

// PCL includes
#include <pcl/point_types.h>
#include <pcl_ros/point_cloud.h>
#include <pcl/common/common.h>

#include "hackathon/radar_data.h"
#include "hackathon/location.h"
#include "hackathon/coordinate_conversion.h"

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
        sub_ = nh_.subscribe ("/velodyne_points", 1,  &SubscribePointCloud2Message::cloudCallback, this);
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
        
        hackathon::location loc;
        hackathon::radar_data data;
        SphericalCoordinate sphericalCoord;

for (std::size_t i = 0; i < cloud->points.size(); ++i)
    {
        pcl::PointXYZ point = cloud->points[i];

        // Access the coordinates of the point
        float x = point.x;
        float y = point.y;
        float z = point.z;

        sphericalCoord = cartesianToSpherical(x, y, z);

        loc.radius = sphericalCoord.radius;
        loc.azimuthAngle = sphericalCoord.azimuthAngle;
        loc.elevationAngle = sphericalCoord.elevationAngle;

        
        // Do something with the point's information (e.g., print them)
        std::cout << "Point " << i << ": Radius = " << loc.radius
                  << ", Azimuth = " << loc.azimuthAngle
                  << ", Elevation = " << loc.elevationAngle
                  << std::endl;
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
    ros::init (argc, argv, "pc1_gazebo", ros::init_options::AnonymousName);
    ros::NodeHandle n;
    pub_ = n.advertise<hackathon::radar_data>("RadarData", 6000);
    SubscribePointCloud2Message subscribePointCloud2Message;
    
    ros::spin();

    return (0);
}