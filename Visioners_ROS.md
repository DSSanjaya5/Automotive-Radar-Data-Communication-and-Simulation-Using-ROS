<div align="center"> 
	<h1> BITES-BOSCH Hackathon 2023 </h1> 
	<h1> Team Visioners </h1> 
</div>
<details>
<summary> <h3> Table of Contents </h3></summary>

 + [About BITES-BOSCH Hackathon](#about-bites-bosch-hackathon)
 + [Problem Statement ROS](#problem-statement---ros)
   + nested list 2
+ markdown list 2
</details>

## About BITES-BOSCH Hackathon ##
BITES-Bosch hackathon is an excellent opportunity for the students of BITES Member Institutions to apply their knowledge, collaborate with like-minded individuals, and create practical solutions in the field of robotics and computer vision.

Hackathon Themes:

• Image Processing – Pixel Quest

• ROS – RoboHive: Unleashing the power of ROS

This hackathon is a collective event where the students come together to work on projects and challenges related to image analysis, computer vision and challenges related to robotics using the ROS framework. These events often foster a sense of community and excitement among participants, inspiring them to further explore the potential of ROS and image processing in robotics applications.

## Problem Statement - ROS ##
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

#### Wireshark ####
Wireshark is a network protocol analyzer, or an application that captures packets from a network connection, such as from your computer to the internet. For the installation of the Wireshark tool follow the steps provided [here](https://www.geeksforgeeks.org/how-to-install-and-use-wireshark-on-ubuntu-linux/).

## Publisher-Subscriber Model ##

![Topic-MultiplePublisherandMultipleSubscriber](https://github.com/DSSanjaya/Visioners_ROS/assets/83571032/ee86fc03-9d4d-4f51-b22e-b1f1091fc4ac)


## Server-Client Model ##

![Service-MultipleServiceClient](https://github.com/DSSanjaya/Visioners_ROS/assets/83571032/5bc4e887-e3f2-471d-91cb-a23613a57364)

## Communication Between two PCs ##
To run the Publisher-Suscriber model or Server-Client model one must set up the network between two PCs. We have used an ethernet cable as a medium of communication between two PCs. Thus enabling complete bi-directional connectivity between the two PCs. We must make sure that each PC advertises itself by a name/IP that the other PC can resolve. We only need one master. All nodes must be configured to use the same master, via ROS_MASTER_URI. Make sure to configure the IP address of the master PC using ROS_IP and ROS_MASTER_URI in the .bashrc file.
- Run the following commands in PC1 (Master)
  ```
  // Terminal 1 -
  roscore

  // Terminal 2 -
  export ROS_IP=10.0.0.1
  rosrun hackathon pc1
    
![Screenshot from 2023-10-16 17-05-37](https://github.com/DSSanjaya/Visioners_ROS/assets/83591388/fb6cfb1d-0a62-4223-b071-acf82102de07)

  
- Run the following commands in PC2
  ```
  export ROS_IP=10.0.0.2
  export ROS_MASTER_URI=http://10.0.0.1:11311
  rosrun hackathon pc2
    
    ```
![Screenshot from 2023-10-16 17-04-36](https://github.com/DSSanjaya/Visioners_ROS/assets/83591388/007d060a-44d2-4e74-ab06-269975553014)
