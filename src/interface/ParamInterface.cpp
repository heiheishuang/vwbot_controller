//
// Created by heihei on 2019/10/4.
//

#include "ParamInterface.h"

vwpp::ParamInterface::~ParamInterface()
{
    delete instance;
}

vwpp::ParamInterface::ParamInterface()
{
    nh = ros::NodeHandle("~");
    callback_type = boost::bind(&vwpp::ParamInterface::reconfig_cb, this, _1, _2);
    server.setCallback(callback_type);
}
vwpp::ParamInterface* vwpp::ParamInterface::instance = nullptr;
boost::mutex vwpp::ParamInterface::mutex_instance;

vwpp::ParamInterface* vwpp::ParamInterface::getInstance()
{
    if (instance == nullptr)
    {
        boost::unique_lock<boost::mutex> uq_lock_instance(mutex_instance);
        if (instance == nullptr)
        {
            instance = new ParamInterface();
        }
    }

    return instance;
}

vwpp::ParamInterface::ParamInterface(const vwpp::ParamInterface &)
{

}


vwpp::ParamInterface &vwpp::ParamInterface::operator=(const vwpp::ParamInterface &)
{

}

double vwpp::ParamInterface::getParamPidAction1P() const
{
    return param_pid_action1_p;
}


double vwpp::ParamInterface::getParamPidAction1I() const
{
    return param_pid_action1_i;
}


double vwpp::ParamInterface::getParamPidAction1D() const
{
    return param_pid_action1_d;
}


double vwpp::ParamInterface::getParamPidAction1Vel() const
{
    return param_pid_action1_vel;
}


double vwpp::ParamInterface::getChange1X() const
{
    return change_1_x;
}


double vwpp::ParamInterface::getChange1Y() const
{
    return change_1_y;
}


double vwpp::ParamInterface::getChange2X() const
{
    return change_2_x;
}


double vwpp::ParamInterface::getChange2Y() const
{
    return change_2_y;
}


double vwpp::ParamInterface::getChange3X() const
{
    return change_3_x;
}


double vwpp::ParamInterface::getChange3Y() const
{
    return change_3_y;
}


double vwpp::ParamInterface::getPointYellowX() const
{
    return point_yellow_x;
}


double vwpp::ParamInterface::getPointYellowY() const
{
    return point_yellow_y;
}


double vwpp::ParamInterface::getPointRedX() const
{
    return point_red_x;
}


double vwpp::ParamInterface::getPointRedY() const
{
    return point_red_y;
}


double vwpp::ParamInterface::getPointBlueX() const
{
    return point_blue_x;
}


double vwpp::ParamInterface::getPointBlueY() const
{
    return point_blue_y;
}


double vwpp::ParamInterface::getDisLengthWhenCatch() const
{
    return dis_length_when_catch;
}


double vwpp::ParamInterface::getWaitTheTaskHasBall3() const
{
    return wait_the_task_has_ball3;
}

double vwpp::ParamInterface::getAbsoluteYawToBall() const
{
    return absolute_yaw_to_ball;
}


double vwpp::ParamInterface::getPoseVariation() const
{
    return pose_variation;
}

void vwpp::ParamInterface::update()
{
    ros::spinOnce();
}

void vwpp::ParamInterface::reconfig_cb(vwbot_controller::TutorialsConfig &_config, uint32_t _level)
{
    this->param_pid_action1_p = _config.param_pid_action1_p;
    this->param_pid_action1_i = _config.param_pid_action1_i;
    this->param_pid_action1_d = _config.param_pid_action1_d;
    this->param_pid_action1_vel = _config.param_pid_action1_vel;

    this->change_1_x = _config.change_1_x;
    this->change_1_y = _config.change_1_y;
    this->change_2_x = _config.change_2_x;
    this->change_2_y = _config.change_2_y;
    this->change_3_x = _config.change_3_x;
    this->change_3_y = _config.change_3_y;

    this->dis_length_when_catch = _config.dis_length_when_catch;
    this->wait_the_task_has_ball3 = _config.wait_the_task_has_ball3;

    this->point_blue_x = _config.point_blue_x;
    this->point_blue_y = _config.point_blue_y;
    this->point_red_x = _config.point_red_x;
    this->point_red_y = _config.point_red_y;
    this->point_blue_x = _config.point_blue_x;
    this->point_blue_y = _config.point_blue_y;

    this->absolute_yaw_to_ball = _config.absolute_yaw_to_ball;
    this->pose_variation = _config.pose_variation;



}




