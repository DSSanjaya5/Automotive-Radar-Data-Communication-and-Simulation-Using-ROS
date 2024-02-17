<div align="center"> 
	<img src="https://github.com/DSSanjaya/Visioners_ROS/blob/main/docs/BITES_BOSCH_Logo.jpg" width=25% height=25%>
	<h1> Winners of BGSW - BITES Hackathon 2024 </h1> 
	<h1> Team Visioners </h1>  
</div>
<details open> <summary> <h3> Table of Contents </h3></summary>
	
 + [About BGSW-BITES Hackathon](#about-bites-bosch-hackathon)
 + [Problem Statement ROS](#problem-statement---ros)
 + [Installation of Softwares](#installation-of-softwares)
 + [Network Setup](#network-setup)
 + [Hackathon Implementation](#hackathon-implementation)
	+ [Algorithm](#algorithm)
	+ [Publisher-Subscriber Model](#publisher-subscriber-model)
	+ [Server-Client Model](#server-client-model)
 	+ [WireShark Analysis](#wireshark-analysis)
  + [Temperature Application](#temperature-application) 
  + [Integration of the framework with Gazebo](#integration-of-the-framework-with-gazebo)
  + [About Us](#about-us)
  
</details>

***
## About BITES-BOSCH Hackathon ##
<p align="justify">
BITES-Bosch hackathon is an excellent opportunity for the students of BITES Member Institutions to apply their knowledge, collaborate with like-minded individuals, and create practical solutions in the field of robotics and computer vision. </p>  
Hackathon Themes:

• Image Processing – Pixel Quest  
• ROS – RoboHive: Unleashing the power of ROS

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
|<p align="center"> <img src="https://github.com/DSSanjaya/Visioners_ROS/assets/148639131/0598790e-eeb5-429d-a4ea-71e9c1e49a84.png" width=70% height=70%> </p>|<p align="center"> <img src="https://github.com/DSSanjaya/Visioners_ROS/assets/148639131/1150a7bd-8498-4e74-bbd8-8dd529448bea.png" width=70% height=70%> </p>|

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
* uniform_real_distribution
* normal_distribution
* poission_distribution

<p align="justify">
The <i><b>central limit theorem</b></i> states that the sum (or average) of a large number of independent and identically distributed random variables, regardless of their original distribution, will be approximately _normally_ distributed. This makes the <i><b>normal_distribution</b></i> a natural choice for modeling the distribution of the RADAR points, which is often the case in real-world scenarios. </p>

#### Data Transformation ####

The data sent by radar sensor is in spherical coordinate system (consisting of radius, azimuthal angle and elevation angle), which must be transformed into cartesian coordinate system (consisting of x, y and z coordinates) in PC2, this is done using following formulae:

* x = radius * sin( (PI/180) * (90-elevationAngle)) * cos( (PI/180) * azimuthAngle)
* y = radius * sin( (PI/180) * (90-elevationAngle)) * sin( (PI/180) * azimuthAngle) 
* z = radius * cos( (PI/180) * (90-elevationAngle))

#### Data Visualization ####
To visualize the data sent by PC1(i.e sensor node) is transformed to cartesian coordinate system in PC2 and then this converted data is published on PointCloud topic - RadarPointCloud. This PointCloud is visualized using the Rviz tool.

#### Parameter Server ####
As per the hackathon terms, we use the Number_Of_Locations as a parameter server. The value of this parameter server can be changed using the following command:
```
rosparam set /Number_Of_Locations <value>
```
With this parameter server we can change the Number_Of_Locations during runtime.

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

### Flowchart ###

| Publisher Node              | Subscriber Node                     |
| ----------------------------------- | ----------------------------------- |
| <img src="https://github.com/DSSanjaya/Visioners_ROS/blob/main/docs/Publisher.png" width=100% height=100%> | <img src="https://github.com/DSSanjaya/Visioners_ROS/blob/main/docs/Subscriber.png" width=100% height=100%> |

### Demonstration ###

https://github.com/DSSanjaya/Visioners_ROS/assets/148639131/1b623628-bf2f-4b98-a8ab-ee9676f7798c

***
## Server-Client Model ##

<p align="justify">
PC1 acts as the server in this scenario, while PC2 serves as the client.</p>

```
# radar_data_srv.srv

---
location[] location_list
 ```

### Flowchart ###

| Server Node                 | Client Node                     |
| ----------------------------------- | ----------------------------------- |
| <img src="https://github.com/DSSanjaya/Visioners_ROS/blob/main/docs/Server.png" width=100% height=100%> | <img src="https://github.com/DSSanjaya/Visioners_ROS/blob/main/docs/Client.png" width=100% height=100%> |

### Demonstration ###

https://github.com/DSSanjaya/Visioners_ROS/assets/148639131/49470acc-beef-4399-987c-9b9efb2df1ff
  
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
* The orange box, yellow box and red box represents radius, azimuth angle and elevation angle respectively.

<p align="center">
<img src="https://github.com/DSSanjaya/Visioners_ROS/assets/148639131/9c931802-2bf7-4173-aeb4-bf89a09d739e.png" width="60%" height="10%"/>
</p> 

 * The data displayed in Wireshark will be in hex, so to convert it into floating values we have used the following command:
  ```
   perl -e 'print unpack "f", pack "H*", "5f53dd41";'
  ``` 
<p align="center">
<img src="https://github.com/DSSanjaya/Visioners_ROS/assets/148639131/fa414d69-579d-4cc6-944e-64ed35eef5aa.png" width="60%" height="10%"/>
</p>

* The data is converted and has been verified with values –
   * Radius = 2
   * Azimuth Angle = -0.68913
   * Elevation Angle = 27.66571 

Reference: [Interpreting floating point numbers from hex values](https://ask.wireshark.org/question/29733/interpreting-floating-point-numbers-from-hex-values/).

***
## Testing with Astyx HiRes2019 Dataset ##
To test the designed framework of communication between two PCs using ROS, we have used the [Astyx HiRes2019 Dataset](https://github.com/under-the-radar/radar_dataset_astyx). The Astyx HiRes2019 dataset has 546 entries (from 0 to 545) and around 1,000 radar points per frame. Each entry has lidar, radar, camera, calibration data. Its radar data includes x, y, z, radial velocity and intensity. We have converted the cartesian coordinates to spherical coordinates and have saved the data in .csv format. [Click here](https://github.com/DSSanjaya/Visioners_ROS/tree/main/hackathon/Radar_Dataset) to see the radar dataset. Camera visualiztion of the dataset can be seen [here](https://videos.ctfassets.net/95kuvdv8zn1v/1v9GXArq9zPaNQtJy7A0go/231c3bbe4165c56682fb5db59eca9843/Complex_Bus_2x.mp4).

https://github.com/DSSanjaya/Visioners_ROS/assets/148639131/556058d0-a354-4d10-a184-715cdc2b70a8

***
## Temperature Application ##

* Radar gives out its temperature as one of the parameters and according to the datasheet, BOSCH's radar sensors have operating temperature range from -40°C to 85°C.
* Normally most of the radars have Number_Of_Locations around 1000, including the radar sensors by BOSCH.
* Initially the radar is set to provide *Max_Loc* = 1000 locations under normal temperature.
* To simulate this scenario of modulating the radar temperature, we use a parameter server called /Modulate_Temperature which varies the temperature exponentially.
* To increase or decrease the temperature use the following commands :
  ```
   rosparam set /Modulate_Temperature increase
   rosparam set /Modulate_Temperature decrease
  ```
* Exponential rate can be set using the command ``` /ExpRate <value> ``` and the value can be from 0.01 to 0.5 and if the input goes beyond the expected value we saturate the exponential rate by setting it to min or max based on the input. 
* As the radar temperature goes beyond the optimal temperature range, transmission of the number of locations reduces by half and cuts off transmission of other parameters like speed
* Radar sends DTC (Diagnostic Trouble Code) A1000 (Overcooling) or A1001 (Overheating) to the ECU(Electronic Control Unit) ie. PC2.
* To overcome this situation of overheating or overcooling of radar temperature a recovery action has to be performed at PC2 using the following command.
  ```
    rosparam set /RecoverAction 1
  ```
* By this recover action the temperature starts to decrease/increase exponentially based on DTC and the Number_Of_Locations sent by the radar remains the same until the hysteresis temperature (ie. Max_Temp - 15 or Min_Temp + 15) is reached. 
* After crossing the hysteresis temperature the Number_Of_Locations starts to increase exponentially till it reaches *Max_Loc*.

### Flowchart ###

| PC1 Node                            | PC2 Node                            |
| ----------------------------------- | ----------------------------------- |
| <img src="https://github.com/DSSanjaya/Visioners_ROS/blob/main/docs/PC1.png" width=100% height=100%> | <img src="https://github.com/DSSanjaya/Visioners_ROS/blob/main/docs/PC2.png" width=100% height=100%>|

### Demonstration ###

https://github.com/DSSanjaya/Visioners_ROS/assets/148639131/6059bbd7-e371-427a-978b-c316b409bd99

***


## Integration of the framework with Gazebo ##
The designed framework has already been tested on the Astyx HiRes2019 dataset. To have custom data that is not generated using random generators or a dataset, we have made use of a Gazebo simulator to simulate a custom environment with a robot having a sensor. As a 3D radar sensor model wasn't available, we made use of the [Velodyne LiDAR sensor](https://bitbucket.org/DataspeedInc/velodyne_simulator/src/master/) to perceive the environment. Now this sensor publishes the data points over the topic  ```/velodyne_points ``` . So the PC1 node of our framework subscribes to this topic, obtains the spherical coordinate points and publishes them over another topic  ```/RadarPoints ```. Now, PC2 which is subscribing to this topic, receives and transforms the data into cartesian coordinates and publishes the data over a topic of type PointCloud. This PointCloud can be visualized in the Rviz tool.

### Demonstration ###

https://github.com/DSSanjaya/Visioners_ROS/assets/148639131/e240a32e-aa18-44cd-a90b-ffdf7c51cfad

***

## About Us ##

<a href="https://www.linkedin.com/in/dssanjaya/"><img align="center" src="https://img.shields.io/badge/LinkedIn-blue?logo=linkedin&logoColor=white&style=for-the-badge" alt="icon | LinkedIn" width="100px"/></a>
* [D S Sanjaya](https://www.linkedin.com/in/dssanjaya/)
* [Prashanth H](https://www.linkedin.com/in/prashanth-h-07746126a/)  
* [Abhay Joshi](https://www.linkedin.com/in/abhay-joshi-6337a8228/)  
* [Nitish Kulkarni](https://www.linkedin.com/in/nitish-kulkarni-bb0427190/)
  
***
