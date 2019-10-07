//
// Created by heihei on 2019/10/5.
//

#include "VwbotInterface.h"

vwpp::VwbotInterface::VwbotInterface():
        nh("~")

{

    // Information From VwbotInterface

    move_base_state = NO_GOAL;

    // Publish To VwbotInterface

    pub_hand = nh.advertise<std_msgs::Bool>("/send_to_hand", 1);

    pub_cmd_vel = nh.advertise<geometry_msgs::Twist>("/cmd_vel_unstamped",1);

    pub_goal_id = nh.advertise<actionlib_msgs::GoalID>("/move_base/cancel",1);


}

vwpp::VwbotInterface::~VwbotInterface()
{
    delete instance;
}

vwpp::VwbotInterface* vwpp::VwbotInterface::instance = nullptr;
boost::mutex vwpp::VwbotInterface::mutex_instance;  //TODO

vwpp::VwbotInterface* vwpp::VwbotInterface::getInstance()
{
    if (instance == nullptr)
    {
        boost::unique_lock<boost::mutex> uq_lock_instance(mutex_instance);
        if (instance == nullptr)
        {
            instance = new VwbotInterface();
        }
    }

    return instance;
}

vwpp::VwbotInterface::VwbotInterface(const VwbotInterface &):
        move_base_state(NO_GOAL)
{

}

vwpp::VwbotInterface &vwpp::VwbotInterface::operator=(const vwpp::VwbotInterface &)
{

}

void vwpp::VwbotInterface::update()
{
    ros::spinOnce();
}

void vwpp::VwbotInterface::send_move_base(const geometry_msgs::PoseStamped &_pose_stamped)
{
    ROS_INFO("Now in the action move_base!");

    ROS_INFO("I got the goal! %lf %lf %lf ", _pose_stamped.pose.position.x, _pose_stamped.pose.position.y, _pose_stamped.pose.position.z);
    ROS_INFO("                %lf %lf %lf %lf ", _pose_stamped.pose.orientation.x, _pose_stamped.pose.orientation.y,
             _pose_stamped.pose.orientation.z, _pose_stamped.pose.orientation.w);

    MoveBaseClient ac("move_base", true);

    while (!ac.waitForServer(ros::Duration(5.0)))
    {
        ROS_INFO("Waiting~");
    }

    move_base_msgs::MoveBaseGoal goal;

    goal.target_pose = _pose_stamped;

    ac.sendGoal(goal);

    ac.waitForResult(ros::Duration(0.2));

    this->move_base_state = NO_GOAL;

    if (ac.getState() == actionlib::SimpleClientGoalState::LOST)
    {
        ROS_INFO("Move_base Failed!");
        this->move_base_state = FAILED_TO_GOAL;
    }
    else if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
    {
        ROS_INFO("Move_base Succeeded! cur_action_state == COT_GOAL");
        this->move_base_state = GOT_GOAL;
    }

}

vwpp::ActionState vwpp::VwbotInterface::getMoveBaseState()
{
    return  this->move_base_state;
}

void vwpp::VwbotInterface::send_cmd_vel(const geometry_msgs::Twist &_vel)
{

    ROS_ERROR("### Now in the action send_to_cmd_vel");
    ROS_ERROR("%lf %lf ",_vel.angular.z,_vel.linear.x);
    this->pub_cmd_vel.publish(_vel);

}

void vwpp::VwbotInterface::send_cancel()
{
    actionlib_msgs::GoalID _cancel_goal_id;
    ROS_ERROR("##########Now Cancel the navigation goal");
    this->pub_goal_id.publish(_cancel_goal_id);
}

void vwpp::VwbotInterface::send_to_hand(const bool &_ball)
{

    ROS_WARN("###   Now in the action send_to_hand!");
    std_msgs::Bool hand;
    hand.data = _ball;
    this->pub_hand.publish(hand);

}