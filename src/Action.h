//
// Created by heihei on 2019/7/28.
//

#ifndef VWBOT_CONTROLLER_ACTION_H_
#define VWBOT_CONTROLLER_ACTION_H_

#include <actionlib/client/simple_action_client.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <geometry_msgs/PoseStamped.h>
#include <std_msgs/Bool.h>
#include <ros/ros.h>

#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

namespace vwpp
{

    enum ActionState
    {
        NO_GOAL,
        GOT_GOAL,
        FAILED_TO_GOAL
    };

    class Action
    {
    public:

        Action();

        int action_move_base(geometry_msgs::PoseStamped _pose_stamped);

        void send_to_hand(const std_msgs::Bool &_ball);

        ActionState getActionState();

    private:

        ActionState cur_action_state;

        ros::NodeHandle nh;
        ros::Rate loop_rate;
        ros::Publisher pub_hand;

    };
}



#endif //VWBOT_CONTROLLER_ACTION_H_
