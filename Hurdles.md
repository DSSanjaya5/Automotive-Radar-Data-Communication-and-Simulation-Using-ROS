# Hurdles Faced #

During the hackathon our team faced numerous technical issues. The issues and their solutions are as follows :

## 1] Couldn’t find python module ‘rosdep2.rospack’ ##
To solve this run the following commands,

`sudo apt install python-rosdep`
`sudo rosdep init`
`rosdep update`

## 2] ConnectionRefusedError: Cannot Un-pack non-iterable ConnectionRefused object ##
This is solved by sourcing the paths and setting the environment variables in env.sh of both the PCs

```
source /opt/ros/noetic/setup.bash
source ~/catkin_ws/devel/setup.bash
export ROS_IP=10.0.0.2
export ROS_MASTER_URI=http://10.0.0.1:11311
export ROSLAUNCH_SSH_UNKNOWN=1
export DISPLAY=:0  #add this only in PC2
```

## 3] ssh: connect to host <host> port 22: Connection refused  ##
To solve this run `sudo apt-get install ssh` on both PCs. Then run the following: 

PC1 terminal: `ssh 10.0.0.2` 

PC2 terminal: `ssh 10.0.0.1`

## 4] R

## 5] OGRE Exception(3:RenderingAPI Exception) Unbale to create a suitable GLX Context rviz::RenderSystem:-error creating render window: OGRE Exception ##
To solve this run the following commands:
'sudo apt-get install libxinerama-dev'
`sudo apt-get update`
`sudo apt-get install ros-noetic-pcl-conversions` 
`sudo apt-get install ros-noetic-pcl-ros`
