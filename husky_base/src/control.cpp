#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Pose.h"
#include <nav_msgs/Odometry.h>

void handle_poses(const nav_msgs::Odometry::ConstPtr& msg)
{
  ROS_INFO("Position-> x: [%f], y: [%f], z: [%f]", msg->pose.pose.position.x,msg->pose.pose.position.y, msg->pose.pose.position.z);
}

int main(int argc, char **argv)
{

  ros::init(argc, argv, "listener");

  ros::NodeHandle n;

  ros::Subscriber sub = n.subscribe("/ground_truth/state", 1000, handle_poses);

  ros::spin();

  return 0;
}