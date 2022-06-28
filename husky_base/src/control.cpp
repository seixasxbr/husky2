#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Pose.h"
#include <nav_msgs/Odometry.h>

#include <vector>
#include <tf/tf.h>
#include "ros/ros.h"
#include "geometry_msgs/Pose.h"
#include <geometry_msgs/Twist.h>
#include <math.h> 
#include <std_msgs/Float64.h>

double roll, pitch, yaw;
void handle_poses(const nav_msgs::Odometry::ConstPtr& msg)
{
  ROS_INFO("Position-> x: [%f], y: [%f], z: [%f]", msg->pose.pose.position.x,msg->pose.pose.position.y, msg->pose.pose.position.z);
  tf::Quaternion q(
        msg->pose.pose.orientation.x,
        msg->pose.pose.orientation.y,
        msg->pose.pose.orientation.z,
        msg->pose.pose.orientation.w);
    tf::Matrix3x3 m(q);
    m.getRPY(roll, pitch, yaw);
    double secs = ros::Time::now().toSec();
    double Xr = 1;
    double Yr = 1;
    double PSIr = 0;
    double Ex = Xr - pose.pose.position.x;
    double Ey = Yr - pose.pose.position.y;
    double Epsi = PSIr - yaw;
    double e1 = (cos(yaw) * Ex) + (sin(yaw) * Ey);
    double e2 = (-sin(yaw) * Ex) + (cos(yaw) * Ey);
    double e3 = Epsi;
    double v = 
    ROS_INFO("yaw: [%f]", yaw);
}

int main(int argc, char **argv)
{

  ros::init(argc, argv, "listener");

  ros::NodeHandle n;

  ros::Subscriber sub = n.subscribe("/ground_truth/state", 1000, handle_poses);

  ros::spin();

  return 0;
}