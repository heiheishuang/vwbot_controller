//
// Created by heihei on 2019/7/28.
//

#include "Task.h"

vwpp::Task::Task():
        nh("~"),
        loop_rate(20.0),
        cur_task_state(NO_START)
        {
            //TODO
            ///ball isn't the name of the topic
            sub_ball_pose = nh.subscribe<vwbot_controller::ball_state>
                    ("/ball", 1, &Task::sub_from_ball_cb, this);
            //TODO
            //ball_state isn't the name of the topic
            sub_ball_state = nh.subscribe<std_msgs::Bool>("/ball_state",10,&Task::sub_state_to_catchball_cb,this);
            //TODO
            ///vwbot_pose isn't the name of the topic
            sub_vwbot = nh.subscribe<geometry_msgs::PoseStamped>("/vwbot_pose",10,&Task::sub_from_vwbot_cb,this);

            ball_pose.pose.pose.position.x = -1;
            ball_pose.pose.pose.position.y = -1;
            ball_pose.pose.pose.position.z = -1;

            vwbot_pose.pose.position.x = 0.0;
            vwbot_pose.pose.position.y = 0.0;
            vwbot_pose.pose.orientation.w = 1;

            ball_state.data = false;

        }

void vwpp::Task::taskGotBall()
{

    vwbot_controller::ball_state now_ball;
    geometry_msgs::PoseStamped now_vwbot;

    now_ball = this->ball_pose;
    now_vwbot = this->vwbot_pose;

    double length, dis_x, dis_y, dis_length;

    dis_x = ball_pose.pose.pose.position.x - vwbot_pose.pose.position.x;
    dis_y = ball_pose.pose.pose.position.y - vwbot_pose.pose.position.y;


    // The aim is to arrive at the destination more accurately.

    // length = sqrt(dis_x * dis_x + dis_y * dis_y);
    //
    // if ( length > 0.80 )
    // {
    //     dis_length = length - 0.80;
    //     dis_x = dis_x / length * dis_length;
    //     dis_y = dis_y / length * dis_length;
    // }

    now_vwbot.pose.position.x = now_vwbot.pose.position.x + dis_x;
    now_vwbot.pose.position.y = now_vwbot.pose.position.y + dis_y;
    now_vwbot.header.stamp = ros::Time::now();

    vwpp::Action task_action;
    vwpp::Action push_action;

    task_action.send_to_move_base(now_vwbot);
    if (task_action.getActionState() == GOT_GOAL )
    {
        std_msgs::Bool now_ball_state;

        geometry_msgs::PoseStamped door;
        // when the ball is in the right position

        now_ball_state.data = this->ball_state.data;
        task_action.send_to_hand(now_ball_state);

        //push the ball to the goal
        if (ball_state.data != 0)
        {
            if (now_ball.color == "yellow" )
            {
                door.header.stamp = ros::Time::now();
                door.header.frame_id = "base_link";
                door.pose.position.x = YELLOW_X;
                door.pose.position.y = YELLOW_Y;
                door.pose.orientation.w = 1;
                push_action.send_to_move_base(door);
            }
            else if (now_ball.color == "blue")
            {
                door.header.stamp = ros::Time::now();
                door.header.frame_id = "base_link";
                door.pose.position.x = BLUE_X;
                door.pose.position.y = BLUE_Y;
                door.pose.orientation.w = 1;
                push_action.send_to_move_base(door);
            }
            else if (now_ball.color == "red")
            {
                door.header.stamp = ros::Time::now();
                door.header.frame_id = "base_link";
                door.pose.position.x = RED_X;
                door.pose.position.y = RED_Y;
                door.pose.orientation.w = 1;
                push_action.send_to_move_base(door);
            }
            if (push_action.getActionState() == 1)
            {
                ball_state.data = true;
                task_action.send_to_hand(ball_state);
            }
        }
        this->cur_task_state = NO_START;

    }

    this->cur_task_state = NO_START;



}

void vwpp::Task::taskNoBall()
{
    vwpp::Action task_action;

    while (vwpp::Task::getTaskState() == NO_START)
    {

       geometry_msgs::PoseStamped now_pose;

       //TODO
       //Judging coordinates
       //sub now_pose
       now_pose = this->vwbot_pose;

       if (fabs(now_pose.pose.position.x - 3.0) <= 0.1 or fabs(now_pose.pose.position.x) <= 0.1)
           now_pose.pose.position.x = MID_X;
       if (fabs(now_pose.pose.position.y - 2.0) <= 0.1 or fabs(now_pose.pose.position.y + 2.0) <= 0.1)
           now_pose.pose.position.y = MID_Y;

       tf::Matrix3x3 mat(tf::Quaternion(now_pose.pose.orientation.x,now_pose.pose.orientation.y,
                                        now_pose.pose.orientation.z,now_pose.pose.orientation.w)) ;

       double yaw,pitch,roll;
       mat.getEulerYPR(yaw,pitch,roll);

       yaw = yaw / 3.14 * 180 + 10 ;

       tf::Quaternion q;
       q.setRPY(roll,pitch,yaw);

       now_pose.pose.orientation.x = q[0];
       now_pose.pose.orientation.y = q[1];
       now_pose.pose.orientation.z = q[2];
       now_pose.pose.orientation.w = q[3];
       now_pose.header.frame_id = "base_link";
       now_pose.header.stamp = ros::Time::now();

       task_action.send_to_move_base(now_pose);

       if (this->ball_pose.pose.pose.position.x != -1 and this->ball_pose.pose.pose.position.y != -1
           and this->ball_pose.pose.pose.position.z != -1)

           this->cur_task_state = GOT_START;

    }

}

vwpp::TaskState vwpp::Task::getTaskState()
{
    return cur_task_state;
}

void vwpp::Task::sub_from_ball_cb(const vwbot_controller::ball_state::ConstPtr &msg)
{
    this->ball_pose = *msg;
}

void vwpp::Task::sub_state_to_catchball_cb(const std_msgs::Bool::ConstPtr &msg)
{
    this->ball_state = *msg;
}

void vwpp::Task::sub_from_vwbot_cb(const geometry_msgs::PoseStamped::ConstPtr &msg)
{
    this->vwbot_pose = *msg;
}