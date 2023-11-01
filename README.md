# BITES-BOSCH Hackathon 2023 #
#### Documentation is provided in the file - [Visioners_ROS.md](Visioners_ROS.md) ####

***
### Content: ###

Every ROS project requires a catkin workspace, which consists of a ROS packages. For this ROS hackathon project we have creates a package - [hackathon](hackathon) (this folder contains all the files required to run this project)
 
The hackathon folder consists of following folders:

1. [src](/hackathon/src) : Consists of .cpp files, namely
   * [pc1.cpp](/hackathon/src/pc1.cpp) : publisher of pub-sub model
   * [pc2.cpp](/hackathon/src/pc2.cpp) : subsciber of pub-sub model
   * [pc1_server.cpp](/hackathon/src/pc1_server.cpp) : server file of server client model
   * [pc2_client.cpp](/hackathon/src/pc2_client.cpp) : client file of server client model

2. [msg](/hackathon/msg) : Consists of two .msg files, namely
   * [location.msg](/hackathon/msg/location.msg) : consists of variables radius, azimuthAngle and elevationAngle of type float32
   * [radar_data.msg](/hackathon/msg/radar_data.msg) : consists of array of data type location
		
3. [srv](/hackathon/srv) : Consists of .srv file required for server-client model		
   * [radar_data_srv.srv](/hackathon/srv/radar_data_srv.srv) : consists of array of data type location

4. [launch](/hackathon/launch) : Consists of a .launch file used to run all the nodes simultaneously
   * [ROS_hackathon.launch](/hackathon/launch/ROS_hackathon.launch)


***
## Project Description: ##

### Problem Statement:Using ROS, Build a two-PC communication model that will acheive the following ###

* Define 2 PCs(Linux Machines) as follows: PC1 and PC2
* PC1 will send out data to the PC2 (either as ROS topic or Request/Response model)
* Data sent out by PC1 contains an array of structures called location_list of size NUMBER_OF_LOCATIONS with structure members:Radius,Azimuth angle,Elevation angle
* Verification of published data using Wireshark tool.
* PC1 must process data as per the designed algorithm and PC2 must be capable of receiving the data published by PC1 and PC2 ROS Node should convert data into x, y, z and use specific ROS message type to visualize the same in RVIZ
* Once conversion is done,data must be published to RVIZ, where it should be visualized
