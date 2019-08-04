
#include <ros/ros.h>
#include "Flow.h"

int main(int argc, char** argv)
{
    ros::init(argc, argv, "vwbot_controller_node");
    ros::NodeHandle nh("~");

    vwpp::FlowController flow_controller;

    ros::Rate loop_rate(2);
    while (ros::ok())
    {
        std::cout << GREEN << "Now start the flow!"<< "\033[0m" << std::endl;

        ros::spinOnce();

        flow_controller.run();


        if (flow_controller.getFlowState() == vwpp::FlowState::FLOW_FINSH)
        {
            break;
        }

        loop_rate.sleep();
    }

    return 0;
}