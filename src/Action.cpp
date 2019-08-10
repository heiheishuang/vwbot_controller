//
// Created by heihei on 2019/7/28.
//

#include "Action.h"

#include <utility>


vwpp::Action::Action() :
        cur_action_state(NO_GOAL),
        nh("~"),
        loop_rate(10.0)
{

    pub_hand = nh.advertise<std_msgs::Bool>("/send_to_hand", 1);
    pub_flow = nh.advertise<std_msgs::Bool>("/send_flow_state",1);
    pub_cmd_vel = nh.advertise<geometry_msgs::Twist>("/cmd_vel_unstamped",1);
    pub_goal_id = nh.advertise<actionlib_msgs::GoalID>("/move_base/cancel",1);
    pub_delete_point = nh.advertise<std_msgs::Bool>("/delete_current_point",1);

}


int vwpp::Action::action_move_base(geometry_msgs::PoseStamped pose)
{
    ROS_INFO("Now in the action move_base!");

    ROS_INFO("I got the goal! %lf %lf %lf ", pose.pose.position.x, pose.pose.position.y, pose.pose.position.z);
    ROS_INFO("                %lf %lf %lf %lf ", pose.pose.orientation.x, pose.pose.orientation.y,
             pose.pose.orientation.z, pose.pose.orientation.w);

    MoveBaseClient ac("move_base", true);

    while (!ac.waitForServer(ros::Duration(5.0)))
    {
        ROS_INFO("Waiting~");
    }

    move_base_msgs::MoveBaseGoal goal;

    goal.target_pose = std::move(pose);

    ac.sendGoal(goal);

    cur_action_state = NO_GOAL;

    ac.waitForResult(ros::Duration(0.2));

    if (ac.getState() == actionlib::SimpleClientGoalState::LOST)
    {
        ROS_INFO("Move_base Failed!");
        this->cur_action_state = FAILED_TO_GOAL;
    }
    else if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
    {
        ROS_INFO("Move_base Succeeded! cur_action_state == COT_GOAL");
        this->cur_action_state = GOT_GOAL;
    }

}


void vwpp::Action::send_to_hand(const std_msgs::Bool &_ball)
{

    ROS_WARN("###   Now in the action send_to_hand!");
    this->pub_hand.publish(_ball);

}

void vwpp::Action::send_flow_state(const std_msgs::Bool &_flow)
{

    ROS_ERROR("### Now in the action send_to_flow_state");
    this->pub_flow.publish(_flow);

}
void vwpp::Action::send_cmd_vel(const geometry_msgs::Twist &_vel)
{

    ROS_ERROR("### Now in the action send_to_cmd_vel");
    ROS_ERROR("%lf %lf ",_vel.angular.z,_vel.linear.x);
    this->pub_cmd_vel.publish(_vel);

}
void vwpp::Action::send_cancel(const actionlib_msgs::GoalID &_goal_id)
{
    ROS_ERROR("##########Now Cancel");
    this->pub_goal_id.publish(_goal_id);
}

void vwpp::Action::send_delete(const std_msgs::Bool &_delete)
{
    ROS_ERROR("########Delete the point");
    this->pub_delete_point.publish(_delete);
}
vwpp::ActionState vwpp::Action::getActionState()
{
    return cur_action_state;
}
