//
// Created by heihei on 2019/7/28.
//

#ifndef VWBOT_CONTROLLER_TASK_H_
#define VWBOT_CONTROLLER_TASK_H_

#include "PoseAndColor.h"
#include <geometry_msgs/PoseStamped.h>
#include <tf/transform_datatypes.h>
#include "Action.h"
#include "ColorBool.h"

#define MID_X 1.5
#define MID_Y 0.0

#define CHANGE_1_X 1.5
#define CHANGE_1_Y -1.0
#define CHANGE_2_X 1.5
#define CHANGE_2_Y 0.0
#define CHANGE_3_X 1.5
#define CHANGE_3_Y 1.0

#define FAST_ACTION_2 1
#define FAST_ACTION_3 1      //Using in the taskHasBall in order to improve the speed
#define WAIT_TIME 5 //Waiting for the hand to catch ball

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

    class Task
    {
    public:
        Task();
        virtual  ~Task();

        void taskHasBall();  // Task1 When vwbot find the ball
        void taskNoBall();   // Task2 When vwbot didn't have ball
        void taskPutBall();  // Task3 When vwbot in the right door
        void taskChange();   // Task4 When vwbot need to change the task
        void taskCatchBall(bool state); //Task5 When vwbot need to control the hand

        vwbot_controller::PoseAndColor getBallPose();
        geometry_msgs::PoseStamped getVwbotPose();
        dector::ColorBool getBallState();

        int getActionState();
        int getTaskChangeState();
        int getTaskHasBallState();
        int getTaskNoBallState();
        double getLengthBetweenBallAndVwbot();

        void sendToTaskHasBall(int state);
        void sendToTaskNoBall(int state);
        void sendToTaskChange(int state);
        void sendToColor(std::string color);

        void initBallOrientation();

    private:

        ros::NodeHandle nh;
        ros::Rate loop_rate;


        Action* cur_action;

        void sub_from_ball_cb(const vwbot_controller::PoseAndColor::ConstPtr &msg);  //sub ball's pose and color
        void sub_catchball_state_cb(const dector::ColorBool::ConstPtr &msg);    //sub ball's state when catch the ball
        void sub_from_vwbot_cb(const geometry_msgs::PoseStamped::ConstPtr &msg); //sub vwbot's pose

        ros::Subscriber sub_ball_pose_color;  //sub ball's pose and color
        ros::Subscriber sub_ball_state;  // sub ball'state when catch the ball
        ros::Subscriber sub_vwbot; //sub vwbot's pose

        vwbot_controller::PoseAndColor ball_pose; //ball's pose and color
        std::string now_color;
        geometry_msgs::PoseStamped vwbot_pose;
        geometry_msgs::PoseStamped ball_orientation;
        dector::ColorBool ball_state; //ball's state when catch the ball

        int task_has_ball_state;
        int task_no_ball_state;
        int task_change_point_state;

        //need to calculate the number of ball
    };






}
#endif //VWBOT_CONTROLLER_TASK_H_
