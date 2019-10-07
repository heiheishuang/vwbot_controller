//
// Created by heihei on 2019/10/2.
//

#include "WorldInterface.h"

vwpp::WorldInterface::WorldInterface():
        nh("~"),
        loop_rate(20.0),
        count_pose_change(0)
{
    sub_vwbot = nh.subscribe<geometry_msgs::PoseStamped>
            ("/robot_pose", 10, &WorldInterface::sub_from_vwbot_cb, this);

}

vwpp::WorldInterface::~WorldInterface()
{
    delete instance;
}

vwpp::WorldInterface* vwpp::WorldInterface::instance = nullptr;
boost::mutex vwpp::WorldInterface::mutex_instance;  //TODO

vwpp::WorldInterface* vwpp::WorldInterface::getInstance()
{
    if (instance == nullptr)
    {
        boost::unique_lock<boost::mutex> uq_lock_instance(mutex_instance);
        if (instance == nullptr)
        {
            instance = new WorldInterface();
        }
    }

    return instance;
}

vwpp::WorldInterface::WorldInterface(const vwpp::WorldInterface &, ros::Rate _loop_rate):
        loop_rate(std::move(_loop_rate))
{

}


vwpp::WorldInterface &vwpp::WorldInterface::operator=(const vwpp::WorldInterface &)
{

}



void vwpp::WorldInterface::update()
{
    ros::spinOnce();
}


void vwpp::WorldInterface::sub_from_vwbot_cb(const geometry_msgs::PoseStamped::ConstPtr &msg)   //vwbot_pose
{

    this->vwbot_pose = *msg;

}



geometry_msgs::PoseStamped vwpp::WorldInterface::getVwbotPose()
{
    return this->vwbot_pose;
}



void vwpp::WorldInterface::setCountPoseChange(const int &_count)
{
    this->count_pose_change = _count;
}

int vwpp::WorldInterface::getCountPoseChange()
{
    return this->count_pose_change;
}


