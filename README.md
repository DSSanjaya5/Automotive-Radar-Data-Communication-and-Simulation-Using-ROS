# BITES-BOSCH Hackathon 2023 #
### Documentation is provided in the file - [Visioners_ROS.md](Visioners_ROS.md) ###

## Project Description: ##

### Problem Statement:Using ROS, Build a two-PC communication model that will acheive the following ###

* Define 2 PCs(Linux Machines) as follows: PC1 and PC2
* PC1 will send out data to the PC2 (either as ROS topic or Request/Response model)
* Data sent out by PC1 contains an array of structures called location_list of size NUMBER_OF_LOCATIONS with structure members:Radius,Azimuth angle,Elevation angle
* Verification of published data using Wireshark tool.
* PC1 must process data as per the designed algorithm and PC2 must be capable of receiving the data published by PC1 and PC2 ROS Node should convert data into x, y, z and use specific ROS message type to visualize the same in RVIZ
	 Once conversion is done,data must be published to RVIZ, where it should be visualized

***
### Content: ###

Every ROS prohject requires a catkin workspace needs to be created, which consists of package, here this package is hackathon folder consisting all thefiles required to run this project
 
The hackathon folder consists of following folders:

1) launch: Consists of a .launch file used to run all the nodes simultaneously

2) msg   : Consists of two .msg files used in pub-sub model,namely 
		location.msg
		radar_data.msg
	   the location.msg file consists of variables radius, azimuthAngle and elevationAngle of type float32 and  radar_data.msg file consists of array of data 	   type location
	   
3) src   : Consists of .cpp files namely
   * pc1.cpp : publisher of pub-sub model
   * pc2.cpp : subsciber of pub-sub model
   * pc1_server.cpp : server file of server client model
   * pc2_client.cpp : client file of server client model
		
5) srv   : Consists of .srv file required for server-client model		
 	        radar_data_srv.srv : consists of array of data type location
