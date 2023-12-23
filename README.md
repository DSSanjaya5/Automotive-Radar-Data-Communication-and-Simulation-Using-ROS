<div align="center"> 
	<h1> BITES-BOSCH Hackathon 2023 </h1> 
	<h1> Team Visioners </h1> 
</div>
<details open>
<summary> <h3> Table of Contents </h3></summary>

 + [About BITES-BOSCH Hackathon](#about-bites-bosch-hackathon)
 + [Problem Statement ROS](#problem-statement---ros)
 + [Introduction](#introduction)
 + [Installation of Softwares](#installation-of-softwares)
   	+ [Ubuntu 20.04.6 LTS](#ubuntu-20046-lts)
   	+ [ROS Noetic & RVIZ](#ros-noetic-ninjemys-and-rviz)
   	+ [Wireshark](#wireshark)
 + [Network Setup](#network-setup)
 + [Hackathon implementation](#hackathon-implementation)		
 	+ [Algorithm](#algorithm)
 	+ [Publisher-Subscriber Model](#publisher-subscriber-model)
 	+ [Server-Client Model](#server-client-model)
 	+ [Diffference between Publisher-Subscriber Model and Server-Client Model](#diffference-between-publisher-subscriber-model-and-server-client-model)
 	+ [Data Generation](#data-generation)
 	+ [Data Visualization](#data-visualization)
 	+ [Data Conversions](#data-conversions)
 	+ [WireShark Analysis](#wireshark-analysis)
  + [Temperature Application](#temperature-application) 
  + [Hackathon Extension](#hackathon-extension)
  
</details>

***
## About BITES-BOSCH Hackathon ##
<p align="justify">
BITES-Bosch hackathon is an excellent opportunity for the students of BITES Member Institutions to apply their knowledge, collaborate with like-minded individuals, and create practical solutions in the field of robotics and computer vision. </p>

Hackathon Themes:

• Image Processing – Pixel Quest

• ROS – RoboHive: Unleashing the power of ROS
<p align="justify">
This hackathon is a collective event where the students come together to work on projects and challenges related to image analysis, computer vision and challenges related to robotics using the ROS framework. These events often foster a sense of community and excitement among participants, inspiring them to further explore the potential of ROS and image processing in robotics applications. </p>

***
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
***
## Introduction ##
<p align="justify">
The Robot Operating System (ROS) is a set of software libraries and tools that help you build robot applications. From drivers to state-of-the-art algorithms, and with powerful developer tools, ROS has what you need for your next robotics project. And it's all open source. ROS is neither a framework of libraries nor an operating system, thus it is known as a *Middleware* or a *Meta Operating System*. </p>

***
## Installation of Softwares ##

#### Ubuntu 20.04.6 LTS ####
In this hackathon, we use ROS Noetic and it requires Ubuntu 20.04.6 LTS. To download the Ubuntu 20.04.6 LTS iso file [click here](https://releases.ubuntu.com/focal/).

#### ROS Noetic Ninjemys and RVIZ ####
For the installation of the ROS Noetic Ninjemys follow the steps provided [here](http://wiki.ros.org/noetic/Installation/Ubuntu). RVIZ (ROS-Visualization) is a standard visualization tool included as part of the ROS ecosystem. So there is no need to install RVIZ separately.

#### Wireshark ####
Wireshark is a network protocol analyzer, or an application that captures packets from a network connection, such as from your computer to the internet. For the installation of the Wireshark tool follow the steps provided [here](https://www.geeksforgeeks.org/how-to-install-and-use-wireshark-on-ubuntu-linux/).

***
## Network Setup ##

<p align="justify">
The two PCs are interconnected via an Ethernet cable. PC1 and PC2 have been assigned IP addresses of 10.0.0.1 and 10.0.0.2, respectively, and have Netmask values of 255.255.255.0 each. </p>

| PC1                            | PC2                            |
| ----------------------------------- | ----------------------------------- |
| <img src="https://github.com/DSSanjaya/Visioners_ROS/assets/148639131/0598790e-eeb5-429d-a4ea-71e9c1e49a84.png" width=100% height=100%> | <img src="https://github.com/DSSanjaya/Visioners_ROS/assets/148639131/1150a7bd-8498-4e74-bbd8-8dd529448bea.png" width=100% height=100%> |

#### .bashrc file ####

<p align="justify">
The .bashrc file serves as a script file that is executed upon user login. It contains a collection of configurations specifically designed for the terminal session.
Add the lines mentioned below at the end of .bashrc file in PC1 and PC2.</p>

```
export ROS_MASTER_URI=http://10.0.0.1:11311           #For PC2, keep this line same
export ROS_IP=10.0.0.1                                #For PC2: export ROS_IP=10.0.0.2
export ROSLAUNCH_SSH_UNKNOWN=1
export QT_PLUGIN_PATH=/usr/lib/x86_64-linux-gnu/qt5/plugins/xcbglintegrations
source /opt/ros/noetic/setup.bash
source ~/catkin_ws/devel/setup.bash
 ```
***
## Hackathon Implementation ##
### Algorithm ###

#### Data Generation ####
The sensor data generation can be done in PC1 by generating points using random number generating functions which are part of standard Library of C++. Some of the random distribution functions are:
* Uniform_int_distribution
* Uniform_real_distribution
* normal_distribution
* poission_distribution
  
The  ***central limit theorem***  states that the sum (or average) of a large number of independent and identically distributed random variables, regardless of their original distribution, will be approximately _normally_ distributed. This makes the  ***normal_distribution***  a natural choice for modeling the distribution of the RADAR points, which is often the case in real-world scenarios.

### Data Conversions ###
The data sent by radar sensor is in spherical coordinate system (consisting of radius, azimuthal angle and elevation angle), which must be transformed into cartesian coordinate system (consisting of x, y and z coordinates) in PC2, this is done using following formulae

* x = radius * sin( (PI/180) * (90-elevationAngle)) * cos( (PI/180) * azimuthAngle)
* y = radius * sin( (PI/180) * (90-elevationAngle)) * sin( (PI/180) * azimuthAngle) 
* z = radius * cos( (PI/180) * (90-elevationAngle))

### Data Visualization ###
To visualize the data sent by PC1(i.e sensor node) is transformed to cartesian coordinate system in PC2 and then this converted data is published on PointCloud topic - RadarPointCloud. This PointCloud is visualized using the Rviz tool.

Here visulaization of two type of data is done i.e
1) Data according to range of mid range radars
2) Points in a Square representing different planes
 
![square](https://github.com/DSSanjaya/Visioners_ROS/assets/83597430/33eedd26-1651-4113-b776-2823a4e725cd)

***
## Publisher-Subscriber Model ##

<p align="justify">
PC1 acts as the publisher in this scenario, while PC2 serves as the subscriber. PC1 is responsible for publishing data by utilizing two .msg files, which are defined as follows:</p>

```
# location.msg
float32 radius
float32 azimuthAngle
float32 elevationAngle
 ```

```
# radar_data.msg
location[] location_list
 ```

```
# radar_data_extension.msg
location[] location_list
float32 temperature
 ```

```
# radar_data_srv.srv

---
location[] location_list
 ```

<img src="https://github.com/DSSanjaya/Visioners_ROS/assets/83571032/ee86fc03-9d4d-4f51-b22e-b1f1091fc4ac.gif" width="60%" height="10%"/>

### Flowchart ###

| Publisher                            | Subscriber                           |
| ----------------------------------- | ----------------------------------- |
| <img src="https://github.com/DSSanjaya/Visioners_ROS/assets/148639131/5a296354-11d6-4d23-a5b7-d46e646c332b.png" width=100% height=100%> | <img src="https://github.com/DSSanjaya/Visioners_ROS/assets/148639131/d999c2bc-5390-4ea9-a993-adbf4f69100d.png" width=100% height=100%>|


***
## Server-Client Model ##

<img src="https://github.com/DSSanjaya/Visioners_ROS/assets/83571032/5bc4e887-e3f2-471d-91cb-a23613a57364.gif" width="60%" height="10%"/>

### Flowchart ###

| Publisher-Subscriber Model                            | Server-Client Model                           |
| ----------------------------------- | ----------------------------------- |
| <img src="https://github.com/DSSanjaya/Visioners_ROS/assets/148639131/acb8e196-1fba-4650-bece-bc5e164192bc.png" width=100% height=100%> | <img src="https://github.com/DSSanjaya/Visioners_ROS/assets/148639131/5a55032a-0c4e-45c3-a778-2c4c90eddd32.png" width=100% height=100%>|

### Demonstration ###

https://github.com/DSSanjaya/Visioners_ROS/assets/148639131/b53fdeeb-c6f9-412b-b13a-9823370c814b
  
***
## Diffference between Publisher-Subscriber Model and Server-Client Model ##

| Publisher-Subscriber                | Server-Client                           |
| ----------------------------------- | ----------------------------------- |
| Communication is one-to-many (broadcasting). | Communication is one-to-one.|
| Asynchronous and event-driven. | Synchronous and request-response oriented. |
| Suitable for continuous data streams, such as sensor data. | Suitable for discrete service requests where a client needs a specific task to be performed.|
| Useful when multiple nodes need to process the same type of information independently. | Useful for obtaining information or triggering actions with a clear start and end. |
***
## WireShark Analysis ##

Analysis of Transmitted Packets:
 * Source (PC1) IP – 10.0.0.1
 * Destination (PC2) IP – 10.0.0.2
 * For better understanding we have changed the parameter Number_Of_Locations to 1.
 * The data size in bytes is 20. In which,
     *	Header size = 8 bytes (IP of PC1 & PC2 4 bytes each)
     *	Transmitted data size = 12 bytes (3x4 bytes – float32 radius, azimuth angle and elevation angle)
 * The transmitted data was radius = 2 and the other two values were randomly generated. 
 * This 4-byte floating-point number will be arranged in little-endian format. A little-endian 00:00:00:40, in decimal is 2.

### Image ###

 * The orange box, yellow box and red box represents radius, azimuth angle and elevation angle.
 * The data displayed in Wireshark will be in hex, so to convert it into floating values we have used the following command:
  ```
   perl -e 'print unpack "f", pack "H*", "940e4b3f";'
  ``` 

<img src="https://github.com/DSSanjaya/Visioners_ROS/assets/148639131/fa414d69-579d-4cc6-944e-64ed35eef5aa.png" width="60%" height="10%"/>


* The data is converted and has been verified with values –
   * Radius = 2
   * Azimuth Angle = -0.68913
   * Elevation Angle = 27.66571 

Reference: [Interpreting floating point numbers from hex values](https://ask.wireshark.org/question/29733/interpreting-floating-point-numbers-from-hex-values/).

## Temperature Application ##
* Radar gives out its temperature as one of the parameters and according to the datasheet, BOSCH's radar sensors have operating temperature range from -40°C to 85°C.
* Normally most of the radars have Number_Of_Locations around 1000, including the radar sensors by BOSCH.
* Initially the radar is set to provide *Max_Loc* = 1000 locations under normal temperature.
* To simulate this scenario of modulating the radar temperature, we use a parameter server called /Modulate_Temperature which varies the temperature exponentially.
* To increase or decrease the temperature use the following commands :
  ```
   /Modulate_Temperature increase
   /Modulate_Temperature decrease
  ```
* Exponential rate can be set using the command ``` /ExpRate <value> ``` and the value can be from 0.01 to 0.5 and if the input goes beyond the expected value we saturate the exponential rate by setting it to min or max based on the input. 
* As the radar temperature goes beyond the optimal temperature range, transmission of the number of locations reduces by half and cuts off transmission of other parameters like speed.
* Radar sends DTC (Diagnostic Trouble Code) A1000 (Overcooling) or A1001 (Overheating) to the ECU(Electronic Control Unit) ie. PC2.
* To overcome this situation of overheating or overcooling of radar temperature a recovery action has to be performed at PC2 using the following command.
    ```
    /RecoverAction 1
    ```
* By this recover action the temperature starts to decrease/increase exponentially based on DTC and the Number_Of_Locations sent by the radar remains the same until the hysteresis temperature (ie. Max_Temp - 15 or Min_Temp + 15) is reached. 
* After crossing the hysteresis temperature the Number_Of_Locations starts to increase exponentially till it reaches *Max_Loc*.

***