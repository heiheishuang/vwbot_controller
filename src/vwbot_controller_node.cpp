
#include <ros/ros.h>
#include "interface/WorldInterface.h"
#include "interface/ParamInterface.h"
#include "controller/Flow.h"

int main(int argc, char** argv)
{
    ros::init(argc, argv, "vwbot_controller_node");
    ros::NodeHandle nh("~");

    vwpp::WorldInterface::getInstance()->update();
    vwpp::ParamInterface::getInstance()->update();
    vwpp::VwbotInterface::getInstance()->update();

    vwpp::FlowController flow_controller;

    ros::Rate loop_rate(2);
    while (ros::ok())
    {

        vwpp::WorldInterface::getInstance()->update();
        vwpp::ParamInterface::getInstance()->update();
        vwpp::VwbotInterface::getInstance()->update();

        std::cout << GREEN << "Now start the flow!"<< "\033[0m" << std::endl;

        flow_controller.run();

        if (flow_controller.getFlowState() == vwpp::FlowState::FLOW_FINSH)
        {
            break;
        }

        loop_rate.sleep();


    }

    return 0;
}