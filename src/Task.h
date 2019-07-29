//
// Created by heihei on 2019/7/28.
//

#ifndef VWBOT_CONTROLLER_TASK_H_
#define VWBOT_CONTROLLER_TASK_H_

#include "ball_state.h"
#include <geometry_msgs/PoseStamped.h>
#include "tf/transform_datatypes.h"
#include "Action.h"

#define MID_X 1.5
#define MID_Y 0.0
#define YELLOW_X 3.10
#define YELLOW_Y 0.55
#define RED_X 3.10
#define RED_Y 0.00
#define BLUE_X 3.10
#define BLUE_Y -0.55

namespace vwpp
{
    // enum TaskID
    // {
    //     GOTBALL,
    //     NOBALL;
    // };

    enum TaskState
    {
        GOT_START,
        GOT_FINSH,
        NO_START,
    };


    class Task
    {
    public:
        Task();

        void taskGotBall();  // Task1 When vwbot find the ball
        void taskNoBall();   // Task2 When vwbot didn't have ball

        TaskState getTaskState();  // Got the ball's state

    private:

        ros::NodeHandle nh;
        ros::Rate loop_rate;

        TaskState cur_task_state;

        void sub_from_ball_cb(const vwbot_controller::ball_state::ConstPtr &msg);  //sub ball's pose and color
        void sub_state_to_catchball_cb(const std_msgs::Bool::ConstPtr &msg);    //sub ball's state when catch the ball
        void sub_from_vwbot_cb(const geometry_msgs::PoseStamped::ConstPtr &msg);

        ros::Subscriber sub_ball_pose;  //sub ball's pose and color
        ros::Subscriber sub_ball_state;  // sub ball'state when catch the ball
        ros::Subscriber sub_vwbot; //sub vwbot's pose

        vwbot_controller::ball_state ball_pose; //ball's pose and color
        geometry_msgs::PoseStamped vwbot_pose;
        std_msgs::Bool ball_state; //ball's state when catch the ball

        //need to calculate the number of ball
    };






}
#endif //VWBOT_CONTROLLER_TASK_H_
