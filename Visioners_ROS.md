# BITES-BOSCH ROS Hackathon #
This is the documentation of the ROS hackathon by Team Visioners, KLE Technological University, Hubli.

## Problem Statement ##
Using ROS, Build a two-PC communication model that will acheive the following 

We define 2 PCs(Linux Machines) as follows: PC1 and PC2 
 - PC1 will send out data to the PC2 (either as ROS topic or Request/Response model) 
 - Data sent out by PC1 contains following data:
 - An array of structures called location_list of size _NUMBER_OF_LOCATIONS_ with structure as shown below
	```
    Single_location:
		Radius
		Azimuth angle
		Elevation angle
    ```

 - The data from PC1 must be sent cyclically( period can be defined by the teams, suggest to keep the period within 1 second)
 - Verify the published data using Wireshark tool.
 - PC1 must process data as per the designed algorithm
 - PC2 must be capable of receiving the data published by PC1 (Teams are allowed to use publisher-subscriber or server-client methods)
 - PC2 ROS Node should convert data into x, y, z and use specific ROS message type to visualize the same in RVIZ 
	* Hint : Teams are free to look up open source and understand.
   * Double-Hint : *Pcl-classes*
 - Once conversion is done, another topic of the type as described above must be published and using RVIZ, it must be visualized

## Introduction ##
The Robot Operating System (ROS) is a set of software libraries and tools that help you build robot applications. From drivers to state-of-the-art algorithms, and with powerful developer tools, ROS has what you need for your next robotics project. And it's all open source. ROS is neither a framework of libraries nor an operating system, thus it is known as a *Middleware* or a *Meta Operating System*.

## Installation of Softwares ##

#### Ubuntu 20.04.6 LTS ####
In this hackathon, we use ROS Noetic and it requires Ubuntu 20.04.6 LTS. To download the Ubuntu 20.04.6 LTS iso file [click here](https://releases.ubuntu.com/focal/)

#### ROS Noetic Ninjemys and RVIZ ####
For the installation of the ROS Noetic Ninjemys follow the steps provided [here](http://wiki.ros.org/noetic/Installation/Ubuntu). RVIZ (ROS-Visualization) is a standard visualization tool included as part of the ROS ecosystem. So there is no need to install RVIZ separately.

#### 
