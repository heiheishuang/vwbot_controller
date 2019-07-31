
#include <ros/ros.h>
#include "Flow.h"

int main(int argc, char** argv)
{
    ros::init(argc, argv, "vwbot_controller_node");
    ros::NodeHandle nh("~");

    vwpp::FlowController flow_controller;

    ros::Rate loop_rate(10);
    while (ros::ok())
    {
        flow_controller.flow();
        ROS_INFO("Now Star");
        if (flow_controller.getFlowState() == vwpp::FlowState::FLOW_FINSH)
        {
            break;
        }

        loop_rate.sleep();
    }

    return 0;
}