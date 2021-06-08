#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>

#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Bool.h"
#include "geometry_msgs/PoseArray.h"
#include "geometry_msgs/PoseStamped.h"

#ifndef M_PI
// M_PI is not part of the C++ standard. Rather it is part of the POSIX standard. As such,
// it is not directly available on Visual C++ (although _USE_MATH_DEFINES does exist).
#define M_PI 3.14159265358979323846
#endif


class LandingPad_Tracker {
    private:
        // ros::NodeHandle nh;
        ros::Subscriber sub_whycon_poses;
        ros::Subscriber sub_whycon_istracking;
        ros::Publisher  pub_whycon_target;
        std::string sub_whycon_poses_topic;
        std::string sub_whycon_istracking_topic;
        std::string pub_whycon_target_topic;

    public:
        LandingPad_Tracker(ros::NodeHandle *nh) {
            sub_whycon_istracking_topic = std::string("/whycon/is_tracking");
            sub_whycon_poses_topic = std::string("/whycon/poses");
            pub_whycon_target_topic = std::string("/whycon/target");
            sub_whycon_istracking = nh->subscribe<std_msgs::Bool>(sub_whycon_istracking_topic.c_str(), 1, &LandingPad_Tracker::CB_whycon_istracking, this);
            sub_whycon_poses = nh->subscribe<geometry_msgs::PoseArray>(sub_whycon_poses_topic.c_str(), 1, &LandingPad_Tracker::CB_whycon_poses, this);
            pub_whycon_target = nh->advertise<geometry_msgs::PoseStamped>(pub_whycon_target_topic.c_str(), 1);
        }

    std_msgs::Bool is_tracking;
    void CB_whycon_istracking(const std_msgs::Bool::ConstPtr& msg) {
        is_tracking.data = msg->data;

        if (!is_tracking.data) {
            ros::Time now = ros::Time::now();
            // Building the Header part of PoseStamped message
            posestamped.header.seq = 0;
            posestamped.header.stamp = now;
            posestamped.header.frame_id = "whycon";
            // Building the Point part of PoseStamped message
            posestamped.pose.position.x = 0.0;
            posestamped.pose.position.y = 0.0;
            posestamped.pose.position.z = 0.0;
            // Building the Quaternion part of PoseStamped message
            posestamped.pose.orientation.x = 0.0;
            posestamped.pose.orientation.y = 0.0;
            posestamped.pose.orientation.z = 0.0;
            posestamped.pose.orientation.w = 1.0;
        }
        // publish the message to ROS
        pub_whycon_target.publish(posestamped);
    }
    
    geometry_msgs::PoseStamped  posestamped;
    void CB_whycon_poses(const geometry_msgs::PoseArray::ConstPtr& msg) {
        // Building the Header part of PoseStamped message
        posestamped.header=msg->header;

        for(uint i = 0; i < msg->poses.size(); i++) {
            // Building the Point part of PoseStamped message
            posestamped.pose.position.x = msg->poses[i].position.x;
            posestamped.pose.position.y = msg->poses[i].position.y;
            posestamped.pose.position.z = msg->poses[i].position.z;
            // Building the Quaternion part of PoseStamped message
            posestamped.pose.orientation.x = msg->poses[i].orientation.x;
            posestamped.pose.orientation.y = msg->poses[i].orientation.y;
            posestamped.pose.orientation.z = msg->poses[i].orientation.z;
            posestamped.pose.orientation.w = msg->poses[i].orientation.w;
        }
    }
}; // LandingPad_Tracker

int main(int argc, char **argv) {
    ros::init(argc, argv, "landing_pad");
    ros::NodeHandle nh;
    ros::Rate loop_rate(10);
    LandingPad_Tracker lp_tracker = LandingPad_Tracker(&nh);
    ros::spin();
    // return 0;
}
