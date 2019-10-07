//
// Created by heihei on 2019/7/28.
//

#include "Action.h"

vwpp::ActionGoToPoint::ActionGoToPoint() :
        target(0.0)
{

}

vwpp::ActionGoToPoint::~ActionGoToPoint()
= default;

void vwpp::ActionGoToPoint::setTarget(const double &_target)
{
    this->target = _target;
}

void vwpp::ActionGoToPoint::calculateCmdVelByAngle(const double &_angle) //By angle
{
    geometry_msgs::Twist cmd_vel;
    static vwpp::PIDController pid_controller_toward_angular(ParamInterface::getInstance()->getParamPidAction1P(),
                                                             ParamInterface::getInstance()->getParamPidAction1I(),
                                                             ParamInterface::getInstance()->getParamPidAction1D(), true, 1.5);

    pid_controller_toward_angular.setTarget(target);

    pid_controller_toward_angular.update(_angle);
    cmd_vel.linear.x = ParamInterface::getInstance()->getParamPidAction1Vel();
    cmd_vel.linear.y = 0.0;
    cmd_vel.angular.x = 0.0;
    cmd_vel.angular.y = 0.0;
    cmd_vel.angular.z = pid_controller_toward_angular.output();

    VwbotInterface::getInstance()->send_cmd_vel(cmd_vel);

}

void vwpp::ActionGoToPoint::calculateCmdVelByPoint(const geometry_msgs::PoseStamped &_vwbot,
                                                                   const double &_ball_x, const double&_ball_y)
{

    double dis_yaw, dis_x, dis_y;
    dis_x = _ball_x - _vwbot.pose.position.x;
    dis_y = _ball_y - _vwbot.pose.position.y;
    dis_yaw = atan2(dis_y, dis_x);   //vwbot_pose and ball_pose

    tf::Matrix3x3 mat(tf::Quaternion(_vwbot.pose.orientation.x, _vwbot.pose.orientation.y,
                                     _vwbot.pose.orientation.z, _vwbot.pose.orientation.w));

    double yaw, pitch, roll;

    mat.getEulerYPR(yaw, pitch, roll);

    double update_angle;

    update_angle = (yaw - dis_yaw) / M_PI * 180;
    if (update_angle > 180) update_angle = update_angle - 360;
    if (update_angle < -180) update_angle = update_angle + 360;


    geometry_msgs::Twist cmd_vel;
    static vwpp::PIDController pid_controller_toward_angular(ParamInterface::getInstance()->getParamPidAction1P(),
                                                             ParamInterface::getInstance()->getParamPidAction1I(),
                                                             ParamInterface::getInstance()->getParamPidAction1D(), true, 1.5);

    pid_controller_toward_angular.setTarget(target);

    pid_controller_toward_angular.update(update_angle / 180 * M_PI);

    cmd_vel.linear.x = ParamInterface::getInstance()->getParamPidAction1Vel();
    cmd_vel.linear.y = 0.0;
    cmd_vel.angular.x = 0.0;
    cmd_vel.angular.y = 0.0;
    cmd_vel.angular.z = pid_controller_toward_angular.output();

    VwbotInterface::getInstance()->send_cmd_vel(cmd_vel);

}

void vwpp::ActionGoToPoint::moveToPoint(const geometry_msgs::PoseStamped &_vwbot)
{
    VwbotInterface::getInstance()->send_move_base(_vwbot);
}
