//
// Created by heihei on 2019/7/28.
//

#include "Task.h"


vwpp::Task::Task() :
        nh("~"),
        loop_rate(20.0),
        cur_task_state(NO_BALL_START)
{
    //TODO
    ///ball isn't the name of the topic
    sub_ball_pose_color = nh.subscribe<vwbot_controller::PoseAndColor>
            ("/ball", 1, &Task::sub_from_ball_cb, this);
    //TODO
    //ball_state isn't the name of the topic
    sub_ball_state = nh.subscribe<std_msgs::Bool>
            ("/ball_state", 10, &Task::sub_catchball_state_cb, this);
    //TODO
    ///vwbot_pose isn't the name of the topic
    sub_vwbot = nh.subscribe<geometry_msgs::PoseStamped>
            ("/robot_pose", 10, &Task::sub_from_vwbot_cb, this);
    ROS_ERROR("subscribe created");

    ball_pose.pose.pose.position.x = -1;
    ball_pose.pose.pose.position.y = -1;
    ball_pose.pose.pose.position.z = -1;

    vwbot_pose.pose.position.x = 0.0;
    vwbot_pose.pose.position.y = 0.0;
    vwbot_pose.pose.orientation.w = 1;

    goal_point.x = 0;
    goal_point.y = 0;
    goal_point.z = 0;

    ball_state.data = false;  //Ball is not in the right place.

    this->cur_action = new Action;

}

vwpp::Task::~Task()
{
    delete this->cur_action;
}

void vwpp::Task::taskHasBall()
{

    ROS_INFO("Now in the taskHasBall!");
    // vwbot_controller::PoseAndColor now_ball;
    // geometry_msgs::PoseStamped now_vwbot;
    //
    // now_ball = this->ball_pose;
    // now_vwbot = this->vwbot_pose;
    //
    // double length, dis_x, dis_y, dis_length;
    //
    // dis_x = ball_pose.pose.pose.position.x - vwbot_pose.pose.position.x;
    // dis_y = ball_pose.pose.pose.position.y - vwbot_pose.pose.position.y;
    //
    //
    // The aim is to arrive at the destination more accurately.

    // length = sqrt(dis_x * dis_x + dis_y * dis_y);
    //
    // if ( length > 0.80 )
    // {
    //     dis_length = length - 0.80;
    //     dis_x = dis_x / length * dis_length;
    //     dis_y = dis_y / length * dis_length;
    // }
    //
    // now_vwbot.pose.position.x = now_vwbot.pose.position.x + dis_x;
    // now_vwbot.pose.position.y = now_vwbot.pose.position.y + dis_y;
    // now_vwbot.header.stamp = ros::Time::now();

    // vwpp::Action task_action;
    // vwpp::Action push_action;
    //
    // task_action.action_move_base(now_vwbot);

    // this->cur_action->action_move_base(now_vwbot);

    //Move to goal succeeded.
    vwbot_controller::PoseAndColor now_ball;
    now_ball = this->ball_pose;

    this->cur_action->action_move_base(now_ball.pose);

    //TODO
    //Change the pose  orientation

    ROS_WARN("! this");
    if (this->cur_action->getActionState() == GOT_GOAL)
    {
        ROS_WARN("this->cur_action->getActionState() == GOT_GOAL");
        std_msgs::Bool catch_ball;
        // when the ball is in the right position
        catch_ball.data = true;
        this->cur_action->send_to_hand(catch_ball);
        ROS_INFO("Put to hand to catch the ball");


        //TODO
        //Whether to add waiting time

        //sub ball's state
        //Judge whether to go to the door or not

        //push the ball to the goal
        if (this->ball_state.data != 0)
        {
            // Success in catching the ball
            if (now_ball.color == "yellow")
            {
                this->goal_point.x = YELLOW_X;
                this->goal_point.y = YELLOW_Y;

                std::cout << YELLOW << "I WILL MOVE TO YELLOW DOOR!"<< "\033[0m" << std::endl;
                this->cur_task_state = PUT_BALL_START;
                // this->cur_action->action_move_base(door);

            }
            else if (now_ball.color == "blue")
            {
                this->goal_point.x = BLUE_X;
                this->goal_point.y = BLUE_Y;

                std::cout << BLUE << "I WILL MOVE TO BLUE DOOR!"<< "\033[0m" << std::endl;
                this->cur_task_state = PUT_BALL_START;
                // this->cur_action->action_move_base(door);

            }
            else if (now_ball.color == "red")
            {
                this->goal_point.x = RED_X;
                this->goal_point.y = RED_Y;

                std::cout << RED << "I WILL MOVE TO RED DOOR!"<< "\033[0m" << std::endl;
                this->cur_task_state = PUT_BALL_START;
                // this->cur_action->action_move_base(door);

            }

        }
        else if (ball_state.data == 0)
        {
            //Failure in catching the ball

            this->cur_action->send_to_hand(ball_state);

            this->cur_task_state = NO_BALL_START;
        }


    }
    //  not use
    //  TODO
    else if (this->cur_action->getActionState() == FAILED_TO_GOAL)
    {
        //move to ball failed
        //need to find ball again
        this->ball_state.data = false;
        this->cur_task_state = NO_BALL_START;
    }


}

void vwpp::Task::taskPutBall()
{

    ROS_INFO("Now in the taskPutBall!");
    geometry_msgs::PoseStamped door;

    door.pose.orientation.w = 1;
    door.pose.orientation.x = 0;
    door.pose.orientation.y = 0;
    door.pose.orientation.z = 0;
    door.pose.position = this->goal_point;
    door.header.frame_id = "map";
    door.header.stamp = ros::Time::now();

    this->cur_action->action_move_base(door);

    //When vwbot get to the door successfully, it needs to go to MID.
    if (this->cur_action->getActionState() == GOT_GOAL and this->cur_task_state == PUT_BALL_START)
    {

        // Put down the ball
        ball_state.data = false;
        this->cur_action->send_to_hand(ball_state);

        //go to MID
        this->goal_point.z = 0;
        this->goal_point.x = MID_X;
        this->goal_point.y = MID_Y;

        this->cur_task_state = CHANGE_START;

    }
    else if (this->cur_action->getActionState() == FAILED_TO_GOAL)
    {
        // TODO
        //Need to find a better way.

        ball_state.data = false;
        this->cur_action->send_to_hand(ball_state);

        this->cur_task_state = NO_BALL_START;
    }


}

void vwpp::Task::taskNoBall()
{

    //TODO
    //Plan 2 taskNoBall can use the velocity
    ROS_INFO("Now in the taskNoball!");

    // vwpp::Action task_action;

    if (this->ball_pose.pose.pose.position.x != -1 and this->ball_pose.pose.pose.position.y != -1
        and this->ball_pose.pose.pose.position.z != -1)
    {

        cur_task_state = HAS_BALL_START;
        ROS_INFO("I find the ball and I will in the taskHasBall!");

    }
    else
    {
        cur_task_state = NO_BALL_START;
        ROS_INFO("Still need to find the ball! ");
    }

    if (vwpp::Task::getTaskState() == NO_BALL_START)
    {
        ROS_INFO("I'm finding the ball , waiting~ ");
        geometry_msgs::PoseStamped cur_pose;

        //TODO
        //Judging coordinates
        //sub cur_pose
        cur_pose = this->vwbot_pose;

        if (fabs(cur_pose.pose.position.x - 3.0) <= 0.1 or (fabs(cur_pose.pose.position.x) <= 0.1))
            cur_pose.pose.position.x = MID_X;
        if (fabs(cur_pose.pose.position.y - 2.0) <= 0.1 or fabs(cur_pose.pose.position.y + 2.0) <= 0.1)
            cur_pose.pose.position.y = MID_Y;

        printf("Orientation %lf %lf \n", cur_pose.pose.orientation.z, cur_pose.pose.orientation.w);

        tf::Matrix3x3 mat(tf::Quaternion(cur_pose.pose.orientation.x, cur_pose.pose.orientation.y,
                                         cur_pose.pose.orientation.z, cur_pose.pose.orientation.w));

        double yaw, pitch, roll;
        mat.getEulerYPR(yaw, pitch, roll);

        yaw = yaw / 3.14 * 180 + 45;
        yaw = yaw / 180 * 3.14;

        tf::Quaternion q;
        q.setRPY(roll, pitch, yaw);

        cur_pose.pose.orientation.x = q[0];
        cur_pose.pose.orientation.y = q[1];
        cur_pose.pose.orientation.z = q[2];
        cur_pose.pose.orientation.w = q[3];
        cur_pose.header.frame_id = "map";
        cur_pose.header.stamp = ros::Time::now();

        printf("Orientation %lf %lf \n", cur_pose.pose.orientation.z, cur_pose.pose.orientation.w);

        this->cur_action->action_move_base(cur_pose); //pub the aim goal
        // task_action.action_move_base(cur_pose);


    }

}

void vwpp::Task::taskChange()
{
    ROS_INFO("Now in the changeTask!");

    geometry_msgs::PoseStamped goal;
    goal.header.frame_id = "map";
    goal.header.stamp = ros::Time::now();
    goal.pose.position = this->goal_point;
    goal.pose.orientation.w = 1;
    goal.pose.orientation.x = 0;
    goal.pose.orientation.y = 0;
    goal.pose.orientation.z = 0;

    // changeTask 1 from taskPutBall to taskNoBall
    this->cur_action->action_move_base(goal);

    if (this->cur_action->getActionState() == GOT_GOAL and this->cur_task_state == CHANGE_START)
    {
        //change the TaskState
        this->cur_task_state = NO_BALL_START;

    }
    else if (this->cur_action->getActionState() == FAILED_TO_GOAL)
    {

        this->cur_task_state = NO_BALL_START;

    }
}

vwpp::TaskState vwpp::Task::getTaskState()
{
    return cur_task_state;
}


void vwpp::Task::sub_catchball_state_cb(const std_msgs::Bool::ConstPtr &msg)
{
    this->ball_state = *msg;
}


void vwpp::Task::sub_from_vwbot_cb(const geometry_msgs::PoseStamped::ConstPtr &msg)
{
    this->vwbot_pose = *msg;
    ROS_INFO("########## %lf %lf", msg->pose.orientation.z, msg->pose.orientation.w);

}


void vwpp::Task::sub_from_ball_cb(const vwbot_controller::PoseAndColor::ConstPtr &msg)
{
    this->ball_pose = *msg;
}
