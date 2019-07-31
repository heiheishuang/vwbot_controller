//
// Created by heihei on 2019/7/28.
//

#include "Action.h"

#include <utility>

vwpp::Action::Action():
    cur_action_state(NO_GOAL),
    nh("~"),
    loop_rate(20.0)

    {
        pub_hand = nh.advertise<std_msgs::Bool>("/send_to_hand",1);

    }

int vwpp::Action::action_move_base(geometry_msgs::PoseStamped pose)
{
    ROS_INFO("Now in the action move_base!");

    MoveBaseClient ac("move_base", true);

    while (!ac.waitForServer(ros::Duration(5.0)))
    {
        ROS_INFO("Waiting~");
    }

    move_base_msgs::MoveBaseGoal goal;

    goal.target_pose = std::move(pose);

    ac.sendGoal(goal);

    ac.waitForResult();

    if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
    {
        ROS_INFO("SUCCEEDED!");
        // Action move_base is all right
        //TODO
        cur_action_state = GOT_GOAL;
        return 0;
    }
    else
    {
        ROS_WARN("FAILED!");
        cur_action_state = FAILED_TO_GOAL;
        return 0;
    }

}

void vwpp::Action::send_to_hand(const std_msgs::Bool &_ball)
{

    ROS_INFO("Now in the action send_to_hand!");
    this->pub_hand.publish(_ball);

}


vwpp::ActionState vwpp::Action::getActionState()
{
    return cur_action_state;
}
