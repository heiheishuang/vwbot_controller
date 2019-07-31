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
            sub_ball_pose_color = nh.subscribe<vwbot_controller::PoseAndColor>
                    ("/ball", 1, &Task::sub_from_ball_cb, this);
            //TODO
            //ball_state isn't the name of the topic
            sub_ball_state = nh.subscribe<std_msgs::Bool>
                    ("/ball_state", 10, &Task::sub_catchball_state_cb, this);
            //TODO
            ///vwbot_pose isn't the name of the topic
            sub_vwbot = nh.subscribe<geometry_msgs::PoseStamped>
                    ("/robot_pose",10,&Task::sub_from_vwbot_cb,this);

            ball_pose.pose.pose.position.x = -1;
            ball_pose.pose.pose.position.y = -1;
            ball_pose.pose.pose.position.z = -1;

            vwbot_pose.pose.position.x = 0.0;
            vwbot_pose.pose.position.y = 0.0;
            vwbot_pose.pose.orientation.w = 1;

            ball_state.data = false;  //Ball is not in the right place.

        }

void vwpp::Task::taskGotBall()
{

    vwbot_controller::PoseAndColor now_ball;
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

    task_action.action_move_base(now_vwbot);

    //Move to goal succeeded.
    if (task_action.getActionState() == GOT_GOAL )
    {
        std_msgs::Bool now_ball_state;

        geometry_msgs::PoseStamped door;

        // when the ball is in the right position
        now_ball_state.data = true;
        task_action.send_to_hand(now_ball_state);

        //TODO
        //Whether to add waiting time

        //sub ball's state
        //Judge whether to go to the door or not

        now_ball_state.data = this->ball_state.data;

        //push the ball to the goal
        if (ball_state.data != 0)
        {
            // Success in catching the ball
            if (now_ball.color == "yellow" )
            {
                door.header.stamp = ros::Time::now();
                door.header.frame_id = "map";
                door.pose.position.x = YELLOW_X;
                door.pose.position.y = YELLOW_Y;
                door.pose.orientation.w = 1;
                push_action.action_move_base(door);
            }
            else if (now_ball.color == "blue")
            {
                door.header.stamp = ros::Time::now();
                door.header.frame_id = "map";
                door.pose.position.x = BLUE_X;
                door.pose.position.y = BLUE_Y;
                door.pose.orientation.w = 1;
                push_action.action_move_base(door);
            }
            else if (now_ball.color == "red")
            {
                door.header.stamp = ros::Time::now();
                door.header.frame_id = "map";
                door.pose.position.x = RED_X;
                door.pose.position.y = RED_Y;
                door.pose.orientation.w = 1;
                push_action.action_move_base(door);
            }

            //When vwbot get to the door successfully, it needs to go to MID.
            if (push_action.getActionState() == GOT_GOAL)
            {
                // Put down the ball
                ball_state.data = false;
                task_action.send_to_hand(ball_state);

                //go to MID
                geometry_msgs::PoseStamped back_wards_goal;
                back_wards_goal.header.stamp = ros::Time::now();
                back_wards_goal.header.frame_id = "map";
                back_wards_goal.pose.position.y = MID_Y;
                back_wards_goal.pose.position.x = MID_X;
                back_wards_goal.pose.orientation.x = 0;
                back_wards_goal.pose.orientation.y = 0;
                back_wards_goal.pose.orientation.z = 0;
                back_wards_goal.pose.orientation.w = 1;
                task_action.action_move_base(back_wards_goal);

                //change the TaskState
                this->cur_task_state = NO_START;

            }
            else if (push_action.getActionState() == FAILED_TO_GOAL)
            {
                // TODO
                //Need to find a better way.

                ball_state.data = false;
                task_action.send_to_hand(ball_state);

                geometry_msgs::PoseStamped back_wards_goal;
                back_wards_goal.header.stamp = ros::Time::now();
                back_wards_goal.header.frame_id = "map";
                back_wards_goal.pose.position.y = MID_Y;
                back_wards_goal.pose.position.x = MID_X;
                back_wards_goal.pose.orientation.w = 1;
                task_action.action_move_base(back_wards_goal);

                this->cur_task_state = NO_START;
            }

        }
        else if (ball_state.data == 0)
        {
            //Failure in catching the ball

            this->ball_state.data = false;
            task_action.send_to_hand(ball_state);

            this->cur_task_state = NO_START;
        }


    }
    else if (task_action.getActionState() == FAILED_TO_GOAL)
    {
        //move to ball failed
        //need to find ball again
        this->ball_state.data = false;
        this->cur_task_state = NO_START;
    }


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

       if (fabs(now_pose.pose.position.x - 3.0) <= 0.1 or (fabs(now_pose.pose.position.x) <= 0.1))
           now_pose.pose.position.x = MID_X;
       if (fabs(now_pose.pose.position.y - 2.0) <= 0.1 or fabs(now_pose.pose.position.y + 2.0) <= 0.1)
           now_pose.pose.position.y = MID_Y;

       tf::Matrix3x3 mat(tf::Quaternion(now_pose.pose.orientation.x,now_pose.pose.orientation.y,
                                        now_pose.pose.orientation.z,now_pose.pose.orientation.w)) ;

       double yaw, pitch, roll;
       mat.getEulerYPR(yaw,pitch,roll);

       yaw = yaw / 3.14 * 180 + 10 ;
       yaw = yaw / 180 * 3.14 ;

       tf::Quaternion q;
       q.setRPY(roll, pitch, yaw);

       now_pose.pose.orientation.x = q[0];
       now_pose.pose.orientation.y = q[1];
       now_pose.pose.orientation.z = q[2];
       now_pose.pose.orientation.w = q[3];
       now_pose.header.frame_id = "map";
       now_pose.header.stamp = ros::Time::now();

        task_action.action_move_base(now_pose);

       if (ball_pose.pose.pose.position.x != -1 and ball_pose.pose.pose.position.y != -1
           and ball_pose.pose.pose.position.z != -1)

           cur_task_state = GOT_START;

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
}

void vwpp::Task::sub_from_ball_cb(const vwbot_controller::PoseAndColor::ConstPtr &msg)
{
    this->ball_pose = *msg;
}
