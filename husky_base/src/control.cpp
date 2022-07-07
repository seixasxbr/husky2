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

double v, w;
double Xr, Yr, PSIr;
double roll, pitch, yaw;
double x, y;
void handle_poses(const nav_msgs::Odometry::ConstPtr& msg)
{
  // ROS_INFO("Position-> x: [%f], y: [%f], z: [%f]", msg->pose.pose.position.x,msg->pose.pose.position.y, msg->pose.pose.position.z);
  tf::Quaternion q(
        msg->pose.pose.orientation.x,
        msg->pose.pose.orientation.y,
        msg->pose.pose.orientation.z,
        msg->pose.pose.orientation.w);
    tf::Matrix3x3 m(q);
    m.getRPY(roll, pitch, yaw);
    double secs = ros::Time::now().toSec();
    // double Xr = -1;
    // double Yr = 1;
    // double PSIr = 0;
    x = msg->pose.pose.position.x;
    y = msg->pose.pose.position.y;
    Xr = 3 * sin(0.3 * secs);
    Yr = 3 * cos(0.3 * secs);
    if(Yr == 0){
      Yr = 0.00001;
    }
    PSIr = atan(-Xr / Yr);
    double Ex = Xr - msg->pose.pose.position.x;
    double Ey = Yr - msg->pose.pose.position.y;
    double Epsi = PSIr - yaw;
    double e1 = (cos(yaw) * Ex) + (sin(yaw) * Ey);
    double e2 = (-sin(yaw) * Ex) + (cos(yaw) * Ey);
    double e3 = Epsi;
    double Vr = 0.9;
    double k1 = 2;
    double k2 = 3;
    double k3 = 1;
    double Wr = 0.6;
    v = Vr * cos(e3) + k1 * e1;
    w = Wr + Vr * k2 * e2 + k3 * sin(e3);
    // ROS_INFO("yaw: [%f]", yaw);
    // ROS_INFO("v: [%f]", v);
    // ROS_INFO("w: [%f]", w);
    // ROS_INFO("e1: [%f]", e1);
    // ROS_INFO("e2: [%f]", e2);
    // ROS_INFO("e3: [%f]", e3);
    ROS_INFO("psiR: [%f]", PSIr);
    ROS_INFO("Xr: [%f]", Xr);
    ROS_INFO("Yr: [%f]", Yr);
    // ROS_INFO("yaw: [%f]", yaw);
}

int main(int argc, char **argv)
{

  ros::init(argc, argv, "listener");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("/ground_truth/state", 1000, handle_poses);
  ros::Publisher Xr_pub = n.advertise<std_msgs::Float64>("/Xr", 1000);
  ros::Publisher Yr_pub = n.advertise<std_msgs::Float64>("/Yr", 1000);
  ros::Publisher X_pub = n.advertise<std_msgs::Float64>("/X", 1000);
  ros::Publisher Y_pub = n.advertise<std_msgs::Float64>("/Y", 1000);
  ros::Publisher PSIr_pub = n.advertise<std_msgs::Float64>("/PSIr", 1000);
  ros::Publisher PSI_pub = n.advertise<std_msgs::Float64>("/PSI", 1000);
  ros::Publisher chatter_pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 1000);
  // ros::Rate loop_rate(250);  
  ros::Rate loop_rate(500);  
  int count = 0;
  while (ros::ok())
  {
    /**
     * This is a message object. You stuff it with data, and then publish it.
     */
    double t1 = ros::Time::now().toSec();

    geometry_msgs::Twist msg;
    msg.linear.x = v;
    msg.angular.z = w;
    chatter_pub.publish(msg);

    std_msgs::Float64 xx;
    xx.data = x;
    X_pub.publish(xx);

    std_msgs::Float64 yy;
    yy.data = y;
    Y_pub.publish(yy);

    std_msgs::Float64 xrr;
    xrr.data = Xr;
    Xr_pub.publish(xrr);

    std_msgs::Float64 yrr;
    yrr.data = Yr;
    Yr_pub.publish(yrr);
    
    std_msgs::Float64 psirr;
    psirr.data = PSIr;
    PSIr_pub.publish(psirr);

    std_msgs::Float64 psii;
    psii.data = yaw;
    PSI_pub.publish(psii);

    ros::spinOnce();
    loop_rate.sleep();
    ++count;
    t1 = ros::Time::now().toSec() - t1;
    // ROS_INFO_STREAM("delta_t: " << t1);
  }
  ros::spin();
  return 0;
}