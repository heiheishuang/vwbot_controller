
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
        ROS_INFO("Now Start");

        ros::spinOnce();

        flow_controller.flow();

        std::cout << "Now cur_flow_state is " << flow_controller.getFlowState() << '\n';
        std::cout << "Now cur_task_state is " << flow_controller.getTaskState() << '\n';

        if (flow_controller.getFlowState() == vwpp::FlowState::FLOW_FINSH)
        {
            break;
        }

        loop_rate.sleep();
    }

    return 0;
}