//
// Created by heihei on 2019/7/28.
//

#ifndef VWBOT_CONTROLLER_TASK_H_
#define VWBOT_CONTROLLER_TASK_H_

#include "PoseAndColor.h"
#include <geometry_msgs/PoseStamped.h>
#include "tf/transform_datatypes.h"
#include "Action.h"

#define MID_X 0.5
#define MID_Y 0.0
#define YELLOW_X 3.10
#define YELLOW_Y 0.55
#define RED_X 3.10
#define RED_Y 0.00
#define BLUE_X 3.10
#define BLUE_Y -0.55

#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"

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

        void sub_from_ball_cb(const vwbot_controller::PoseAndColor::ConstPtr &msg);  //sub ball's pose and color
        void sub_catchball_state_cb(const std_msgs::Bool::ConstPtr &msg);    //sub ball's state when catch the ball
        void sub_from_vwbot_cb(const geometry_msgs::PoseStamped::ConstPtr &msg); //sub vwbot's pose

        ros::Subscriber sub_ball_pose_color;  //sub ball's pose and color
        ros::Subscriber sub_ball_state;  // sub ball'state when catch the ball
        ros::Subscriber sub_vwbot; //sub vwbot's pose

        vwbot_controller::PoseAndColor ball_pose; //ball's pose and color
        geometry_msgs::PoseStamped vwbot_pose;
        std_msgs::Bool ball_state; //ball's state when catch the ball

        //need to calculate the number of ball
    };






}
#endif //VWBOT_CONTROLLER_TASK_H_
