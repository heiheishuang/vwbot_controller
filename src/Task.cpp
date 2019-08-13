//
// Created by heihei on 2019/7/28.
//

#include "Task.h"

#include <utility>


vwpp::Task::Task() :
        nh("~"),
        loop_rate(20.0)
{
    //TODO
    ///ball isn't the name of the topic
    sub_ball_pose_color = nh.subscribe<vwbot_controller::PoseAndColor>
            ("/ball", 1, &Task::sub_from_ball_cb, this);
    //TODO
    //ball_state isn't the name of the topic
    sub_ball_state = nh.subscribe<dector::ColorBool>
            ("/ball_state", 10, &Task::sub_catchball_state_cb, this);
    //TODO
    ///vwbot_pose isn't the name of the topic
    sub_vwbot = nh.subscribe<geometry_msgs::PoseStamped>
            ("/robot_pose", 10, &Task::sub_from_vwbot_cb, this);

    sub_red = nh.subscribe<std_msgs::Float32>
            ("/red_angle", 10, &Task::sub_red_angular_cb, this);

    sub_yellow = nh.subscribe<std_msgs::Float32>
            ("/green_angle", 10, &Task::sub_yellow_angular_cb, this);

    sub_blue = nh.subscribe<std_msgs::Float32>
            ("/blue_angle", 10, &Task::sub_blue_angular_cb, this);

    client_change_point = nh.serviceClient<dector::PointServer>("/check_point");

    ROS_ERROR("subscribe created");


    ball_pose.pose.pose.position.x = -1;
    ball_pose.pose.pose.position.y = -1;
    ball_pose.pose.pose.position.z = -1;

    vwbot_pose.pose.position.x = 0.0;
    vwbot_pose.pose.position.y = 0.0;
    vwbot_pose.pose.orientation.w = 1;

    ball_orientation.pose.orientation.x = 0;
    ball_orientation.pose.orientation.y = 0;
    ball_orientation.pose.orientation.z = 0;
    ball_orientation.pose.orientation.w = 1;

    ball_state.data = false;  //Ball is not in the right place.

    this->cur_action = new Action;

    std_msgs::Bool hand_state;
    hand_state.data = false;
    this->cur_action->send_to_hand(hand_state);

    task_has_ball_state = 0;
    task_no_ball_state = 0;
    task_change_point_state = 0;
    //TODO
    task_ball_cancel = 0;

    now_color = "";

    angular_last = 0;
    aim_angle = 0;

    count_pose_change = 0;
    last_pose = vwbot_pose;


}


vwpp::Task::~Task()
{
    delete this->cur_action;
}


void vwpp::Task::taskHasBall()
{

    ROS_INFO("Now in the taskHasBall!");

    std_msgs::Bool flow_state;
    flow_state.data = true;
    this->cur_action->send_flow_state(flow_state);

    vwbot_controller::PoseAndColor now_ball;
    geometry_msgs::PoseStamped now_vwbot_pose;
    geometry_msgs::PoseStamped target_pose;
    now_vwbot_pose = this->vwbot_pose;
    now_ball = this->ball_pose;

    //now_color
    now_color = this->ball_pose.color;


    double dis_yaw, dis_x, dis_y, dis_length;
    dis_x = now_ball.pose.pose.position.x - now_vwbot_pose.pose.position.x;
    dis_y = now_ball.pose.pose.position.y - now_vwbot_pose.pose.position.y;
    dis_yaw = atan2(dis_y, dis_x);

    dis_length = sqrt(dis_x * dis_x + dis_y * dis_y);

    tf::Matrix3x3 mat(tf::Quaternion(now_vwbot_pose.pose.orientation.x, now_vwbot_pose.pose.orientation.y,
                                     now_vwbot_pose.pose.orientation.z, now_vwbot_pose.pose.orientation.w));

    double yaw, pitch, roll;

    mat.getEulerYPR(yaw, pitch, roll);

    //New in 8.11 add the last pose;
    //*****************************
    if (getChange() <= 0.03)
        this->count_pose_change = this->count_pose_change + 1;
    else
        this->count_pose_change = 0;


    if (this->task_has_ball_state == 0)
    {

        ROS_WARN("taskHasBall action 1!");
        tf::Quaternion q;
        q.setRPY(0, 0, dis_yaw);
        target_pose.pose.position = vwbot_pose.pose.position;
        target_pose.pose.orientation.x = q[0];
        target_pose.pose.orientation.y = q[1];
        target_pose.pose.orientation.z = q[2];
        target_pose.pose.orientation.w = q[3];

        target_pose.header.frame_id = "map";
        target_pose.header.stamp = ros::Time::now();

        ball_orientation.pose.orientation = target_pose.pose.orientation;

        this->cur_action->action_move_base(target_pose);

        std::cout << YELLOW << "####   DIS_YAW  %lf  #### " << fabs(dis_yaw - yaw) / 3.14 * 180 << "\033[0m"
                  << std::endl;

        if (fabs(dis_yaw - yaw) / 3.14 * 180 <= 15)
        {
            this->task_has_ball_state = 1;
            this->task_ball_cancel = 1;
        }

    }

    if (this->task_has_ball_state == 1)
    {
        // length less than 30cm has problem
        // TODO
        ROS_WARN("taskHasBall action 2!");
        target_pose.header.frame_id = "map";
        target_pose.header.stamp = ros::Time::now();

        target_pose.pose = now_ball.pose.pose;
        target_pose.pose.position.x =
                (dis_length - 0.20) / dis_length * dis_x * FAST_ACTION_2 + now_vwbot_pose.pose.position.x;
        target_pose.pose.position.y =
                (dis_length - 0.20) / dis_length * dis_y * FAST_ACTION_2 + now_vwbot_pose.pose.position.y;

        //Less than 30cm using the goal at 30cm
        // target_pose.pose.position = ball_orientation.pose.position;
        tf::Quaternion q;
        q.setRPY(0, 0, dis_yaw);
        target_pose.pose.orientation.x = q[0];
        target_pose.pose.orientation.y = q[1];
        target_pose.pose.orientation.z = q[2];
        target_pose.pose.orientation.w = q[3];

        ball_orientation.pose.orientation = target_pose.pose.orientation;



        this->cur_action->action_move_base(target_pose);

        std::cout << YELLOW << "####   DIS_YAW  %lf  #### " << fabs(dis_yaw - yaw) / 3.14 * 180 << "\033[0m"
                  << std::endl;
        std::cout << YELLOW << "####   DIS_LENGTH  %lf  #### " << dis_length << "\033[0m" << std::endl;

        if (dis_length <= 0.25 and fabs(dis_yaw - yaw) / 3.14 * 180 <= 15)
        {
            this->task_has_ball_state = 2;
        }

    }

    if (this->task_has_ball_state == 2)
    {

        if (this->getLengthBetweenBallAndVwbot() >= 0.30 or this->getYawBetweenBallAndVwbot() / 3.14 * 180 >= 15)
        {
            // ROS_INFO( "Now cur_task_state is from HAS_BALL_START to NO_BALL_START");
            // this->sendToTaskHasBall(0);
            this->task_has_ball_state = 0;
        }

        //Cancel the navigation goal
        if (this->task_ball_cancel == 1)
        {
            actionlib_msgs::GoalID goal_id;
            this->cur_action->send_cancel(goal_id);
            this->task_ball_cancel = 0;


        }

        ROS_WARN("taskHasBall action 3!");
        //
        // target_pose.pose.position.x = dis_x * FAST_ACTION_3 + now_vwbot_pose.pose.position.x;
        // target_pose.pose.position.y = dis_y * FAST_ACTION_3 + now_vwbot_pose.pose.position.y;
        //
        // target_pose.header.frame_id = "map";
        // target_pose.header.stamp = ros::Time::now();
        //
        // target_pose.pose.orientation = ball_orientation.pose.orientation;
        // this->cur_action->action_move_base(target_pose);

        std_msgs::Float32 angle;
        if (this->ball_pose.color == "red") angle = this->red_angle;
        if (this->ball_pose.color == "green") angle = this->yellow_angle;
        if (this->ball_pose.color == "blue") angle = this->blue_angle;

        geometry_msgs::Twist vel;

        std::cout << YELLOW << "####   DIS_YAW  %lf  #### " << fabs(dis_yaw - yaw) / 3.14 * 180 << "\033[0m"
                  << std::endl;
        std::cout << YELLOW << "####   DIS_LENGTH  %lf  #### " << dis_length << "\033[0m" << std::endl;


        static vwpp::PIDController pid_controller_toward_angular(PID_ANGULAR_P, PID_ANGULAR_I, PID_ANGULAR_D);

        pid_controller_toward_angular.setTarget(0.0);
        ROS_ERROR("Current angle data: %lf", angle.data);
        pid_controller_toward_angular.update(-(angle.data * M_PI / 180.));

        vel.linear.x = PID_VEL;
        vel.linear.y = 0.0;
        vel.angular.x = 0.0;
        vel.angular.y = 0.0;
        vel.angular.z = pid_controller_toward_angular.output();
        // vel.angular.z = 0.1;

        ROS_ERROR("Now in Action 3 and angle is %lf ", angle.data);

        this->cur_action->send_cmd_vel(vel);
        if (angle.data > 500)
        {
            this->task_has_ball_state = 3;
        }

        aim_angle = yaw + angle.data;

    }
    if (this->task_has_ball_state == 3)
    {

        static JudgeAchieveCounter judge_achieve_counter(WAIT_TIME);

        if (judge_achieve_counter.isAchieve())
        {

            this->deletePoint();
            this->task_has_ball_state = 0;

            ROS_ERROR("Now action 4");
            ROS_ERROR("Now in action 4");
            ROS_ERROR("Now in Action 4 !!!!!!!");

            geometry_msgs::Twist vel;

            std_msgs::Float32 angular;
            if (this->ball_pose.color == "red") angular = this->red_angle;
            if (this->ball_pose.color == "green") angular = this->yellow_angle;
            if (this->ball_pose.color == "blue") angular = this->blue_angle;


            std::cout << YELLOW << "####   DIS_YAW  %lf  #### " << fabs(dis_yaw - yaw) / 3.14 * 180 << "\033[0m"
                      << std::endl;
            std::cout << YELLOW << "####   DIS_LENGTH  %lf  #### " << dis_length << "\033[0m" << std::endl;


            static vwpp::PIDController pid_controller_toward_angular(PID_ANGULAR_P, PID_ANGULAR_I, PID_ANGULAR_D);

            pid_controller_toward_angular.setTarget(aim_angle);
            ROS_ERROR("Aim angle data: %lf", aim_angle);

            vel.linear.x = PID_VEL;
            vel.linear.y = 0.0;
            vel.angular.x = 0.0;
            vel.angular.y = 0.0;
            vel.angular.z = pid_controller_toward_angular.output();
            // vel.angular.z = 0.1;
            this->cur_action->send_cmd_vel(vel);

            if (angular.data < 500)
            {
                this->task_has_ball_state = 1;
                this->task_ball_cancel = 1;
            }

        }
        else
        {
            ROS_ERROR("Now action 4");
            ROS_ERROR("Now in action 4");
            ROS_ERROR("Now in Action 4 !!!!!!!");

            geometry_msgs::Twist vel;

            std_msgs::Float32 angular;
            if (this->ball_pose.color == "red") angular = this->red_angle;
            if (this->ball_pose.color == "green") angular = this->yellow_angle;
            if (this->ball_pose.color == "blue") angular = this->blue_angle;


            std::cout << YELLOW << "####   DIS_YAW  %lf  #### " << fabs(dis_yaw - yaw) / 3.14 * 180 << "\033[0m"
                      << std::endl;
            std::cout << YELLOW << "####   DIS_LENGTH  %lf  #### " << dis_length << "\033[0m" << std::endl;


            static vwpp::PIDController pid_controller_toward_angular(PID_ANGULAR_P, PID_ANGULAR_I, PID_ANGULAR_D);

            pid_controller_toward_angular.setTarget(aim_angle);
            ROS_ERROR("Aim angle data: %lf", aim_angle);

            vel.linear.x = PID_VEL;
            vel.linear.y = 0.0;
            vel.angular.x = 0.0;
            vel.angular.y = 0.0;
            vel.angular.z = pid_controller_toward_angular.output();
            // vel.angular.z = 0.1;
            this->cur_action->send_cmd_vel(vel);

            if (angular.data < 500)
            {
                this->task_has_ball_state = 1;
                this->task_ball_cancel = 1;
            }
        }



    }
    this->last_pose = this->vwbot_pose;

}


void vwpp::Task::taskPutBall()
{

    ROS_INFO("Now in the taskPutBall!");
    std_msgs::Bool flow_state;
    flow_state.data = true;
    this->cur_action->send_flow_state(flow_state);
    geometry_msgs::PoseStamped door;

    door.pose.orientation.w = 1;
    door.pose.orientation.x = 0;
    door.pose.orientation.y = 0;
    door.pose.orientation.z = 0;

    if (this->now_color == "green")
    {
        door.pose.position.x = YELLOW_X;
        door.pose.position.y = YELLOW_Y;

        std::cout << YELLOW << "I WILL MOVE TO YELLOW DOOR!" << "\033[0m" << std::endl;

    }
    else if (this->now_color == "blue")
    {
        door.pose.position.x = BLUE_X;
        door.pose.position.y = BLUE_Y;

        std::cout << BLUE << "I WILL MOVE TO BLUE DOOR!" << "\033[0m" << std::endl;

    }
    else if (this->now_color == "red")
    {
        door.pose.position.x = RED_X;
        door.pose.position.y = RED_Y;

        std::cout << RED << "I WILL MOVE TO RED DOOR!" << "\033[0m" << std::endl;

    }
    door.header.frame_id = "map";
    door.header.stamp = ros::Time::now();

    this->cur_action->action_move_base(door);

}


void vwpp::Task::taskNoBall()
{

    std_msgs::Bool flow_state;
    flow_state.data = false;
    this->cur_action->send_flow_state(flow_state);
    //TODO
    //Plan 2 taskNoBall can use the velocity
    ROS_INFO("Now in the taskNoball!");

    // vwpp::Action task_action;

    ROS_INFO("I'm finding the ball , waiting~ ");
    geometry_msgs::PoseStamped cur_pose;

    //TODO
    //Judging coordinates
    //sub cur_pose
    cur_pose = this->vwbot_pose;

    if (fabs(cur_pose.pose.position.x - 3.0) <= 0.1)
        cur_pose.pose.position.x = cur_pose.pose.position.x - 0.5;
    if (fabs(cur_pose.pose.position.x - 0) <= 0.1)
        cur_pose.pose.position.x = cur_pose.pose.position.x + 0.5;
    if (fabs(cur_pose.pose.position.y - 2.0) <= 0.1)
        cur_pose.pose.position.y = cur_pose.pose.position.y - 0.5;
    if (fabs(cur_pose.pose.position.y + 2.0) <= 0.1)
        cur_pose.pose.position.y = cur_pose.pose.position.y + 0.5;

    //###################
    // printf("Orientation %lf %lf \n", cur_pose.pose.orientation.z, cur_pose.pose.orientation.w);
    //

    //
    //
    // yaw = yaw / 3.14 * 180 + 20;
    // yaw = yaw / 180 * 3.14;
    //
    // tf::Quaternion q;
    // q.setRPY(roll, pitch, yaw);
    //
    // cur_pose.pose.orientation.x = q[0];
    // cur_pose.pose.orientation.y = q[1];
    // cur_pose.pose.orientation.z = q[2];
    // cur_pose.pose.orientation.w = q[3];
    // cur_pose.header.frame_id = "map";
    // cur_pose.header.stamp = ros::Time::now();
    //
    // printf("Orientation %lf %lf \n", cur_pose.pose.orientation.z, cur_pose.pose.orientation.w);
    //
    // this->cur_action->action_move_base(cur_pose); //pub the aim goal
    //####################

    if (this->cur_action->getActionState() != GOT_GOAL and this->task_no_ball_state == 0)
    {
        cur_pose.header.frame_id = "map";
        cur_pose.header.stamp = ros::Time::now();
        cur_pose.pose.orientation.x = 0;
        cur_pose.pose.orientation.y = 0;
        cur_pose.pose.orientation.z = 1;
        cur_pose.pose.orientation.w = 0;
        this->cur_action->action_move_base(cur_pose);
        ROS_ERROR("###################NO BALL 1");
    }
    else
    {
        cur_pose.header.frame_id = "map";
        cur_pose.header.stamp = ros::Time::now();
        cur_pose.pose.orientation.x = 0;
        cur_pose.pose.orientation.y = 0;
        cur_pose.pose.orientation.z = 0;
        cur_pose.pose.orientation.w = 1;

        tf::Matrix3x3 mat(tf::Quaternion(this->vwbot_pose.pose.orientation.x, this->vwbot_pose.pose.orientation.y,
                                         this->vwbot_pose.pose.orientation.z, this->vwbot_pose.pose.orientation.w));

        double yaw, pitch, roll;

        mat.getEulerYPR(yaw, pitch, roll);

        if (fabs(yaw / 3.14 * 180) - 180 < 15)
            task_no_ball_state = 2;
        this->cur_action->action_move_base(cur_pose);
        ROS_ERROR("###################NO BALL 2");

    }


}


void vwpp::Task::taskChange()
{
    ROS_INFO("Now in the changeTask!");
    std_msgs::Bool flow_state;
    flow_state.data = false;
    this->cur_action->send_flow_state(flow_state);

    geometry_msgs::PoseStamped goal;


    if (this->task_change_point_state == 1)
    {

        ROS_ERROR("Now change point is 1");
        goal.header.frame_id = "map";
        goal.header.stamp = ros::Time::now();
        goal.pose.orientation.w = 0;
        goal.pose.orientation.x = 0;
        goal.pose.orientation.y = 0;
        goal.pose.orientation.z = 1;
        goal.pose.position.x = CHANGE_1_X;
        goal.pose.position.y = CHANGE_1_Y;

    }
    else if (this->task_change_point_state == 2)
    {
        ROS_ERROR("Now change point is 2");
        goal.header.frame_id = "map";
        goal.header.stamp = ros::Time::now();
        goal.pose.orientation.w = 0;
        goal.pose.orientation.x = 0;
        goal.pose.orientation.y = 0;
        goal.pose.orientation.z = 1;
        goal.pose.position.x = CHANGE_2_X;
        goal.pose.position.y = CHANGE_2_Y;

    }
    else
    {
        ROS_ERROR("Now change point is 3");
        goal.header.frame_id = "map";
        goal.header.stamp = ros::Time::now();
        goal.pose.orientation.w = 0;
        goal.pose.orientation.x = 0;
        goal.pose.orientation.y = 0;
        goal.pose.orientation.z = 1;
        goal.pose.position.x = CHANGE_3_X;
        goal.pose.position.y = CHANGE_3_Y;

    }

    dector::PointServer srv;

    srv.request.point = goal.pose.position;
    // changeTask 1 from taskPutBall to,

    if (client_change_point.call(srv))
    {

        ROS_ERROR("The goal is right");

        this->cur_action->action_move_base(goal);
    }
    else
    {
        ROS_ERROR("change the change goal");
        goal.pose.position.x = goal.pose.position.x - 0.15;

        this->cur_action->action_move_base(goal);
        ROS_ERROR("%lf %lf ", goal.pose.position.x, goal.pose.position.y);
    }

}


void vwpp::Task::taskCatchBall(bool state)
{

    std_msgs::Bool flow_state;
    flow_state.data = true;
    this->cur_action->send_flow_state(flow_state);
    std_msgs::Bool hand_state;
    hand_state.data = state;
    this->cur_action->send_to_hand(hand_state);

}


void vwpp::Task::deletePoint()
{
    std_msgs::Bool delete_point;
    delete_point.data = true;
    this->cur_action->send_delete(delete_point);
    this->count_pose_change = 0;
}


vwbot_controller::PoseAndColor vwpp::Task::getBallPose()
{
    return this->ball_pose;
}


dector::ColorBool vwpp::Task::getBallState()
{
    return ball_state;
}


int vwpp::Task::getTaskHasBallState()
{
    return task_has_ball_state;
}


int vwpp::Task::getTaskNoBallState()
{
    return task_no_ball_state;
}


int vwpp::Task::getTaskChangeState()
{
    return this->task_change_point_state;
}


int vwpp::Task::getActionState()
{
    return this->cur_action->getActionState();
}


geometry_msgs::PoseStamped vwpp::Task::getVwbotPose()
{
    return this->vwbot_pose;
}


double vwpp::Task::getYawBetweenBallAndVwbot()
{
    double dis_yaw, dis_x, dis_y;

    vwbot_controller::PoseAndColor now_ball;
    geometry_msgs::PoseStamped now_vwbot_pose;
    now_vwbot_pose = this->vwbot_pose;
    now_ball = this->ball_pose;

    tf::Matrix3x3 mat(tf::Quaternion(now_vwbot_pose.pose.orientation.x, now_vwbot_pose.pose.orientation.y,
                                     now_vwbot_pose.pose.orientation.z, now_vwbot_pose.pose.orientation.w));

    double yaw, pitch, roll;
    mat.getEulerYPR(yaw, pitch, roll);

    dis_x = now_ball.pose.pose.position.x - now_vwbot_pose.pose.position.x;
    dis_y = now_ball.pose.pose.position.y - now_vwbot_pose.pose.position.y;
    dis_yaw = atan2(dis_y, dis_x);
    return fabs(dis_yaw - yaw);

}


double vwpp::Task::getLengthBetweenBallAndVwbot()
{
    double dis_yaw, dis_x, dis_y, dis_length;
    dis_x = this->ball_pose.pose.pose.position.x - this->vwbot_pose.pose.position.x;
    dis_y = this->ball_pose.pose.pose.position.y - this->vwbot_pose.pose.position.y;
    dis_length = sqrt(dis_x * dis_x + dis_y * dis_y);
    return dis_length;
}


int vwpp::Task::getCountPoseChange()
{
    return this->count_pose_change;
}


void vwpp::Task::sub_catchball_state_cb(const dector::ColorBool::ConstPtr &msg)
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

    std::cout << YELLOW << "#### ball_pose  " << this->ball_pose.pose.pose.position.x << "  " <<
              this->ball_pose.pose.pose.position.y << "   " << this->ball_pose.color << " ####  \033[0m" << std::endl;

}


void vwpp::Task::sendToTaskHasBall(int state)
{

    this->task_has_ball_state = state;

}


void vwpp::Task::sendToTaskNoBall(int state)
{
    this->task_no_ball_state = state;
}


void vwpp::Task::sendToTaskChange(int state)
{
    this->task_change_point_state = state;
}


void vwpp::Task::sendToColor(std::string color)
{
    this->now_color = std::move(color);
}


void vwpp::Task::sendToAngularLast(double angular)
{
    this->angular_last = angular;
}


void vwpp::Task::initBallOrientation()
{

    vwbot_controller::PoseAndColor now_ball;
    geometry_msgs::PoseStamped now_vwbot_pose;
    geometry_msgs::PoseStamped target_pose;

    now_vwbot_pose = this->vwbot_pose;
    now_ball = this->ball_pose;

    double dis_yaw, dis_x, dis_y;
    dis_x = now_ball.pose.pose.position.x - now_vwbot_pose.pose.position.x;
    dis_y = now_ball.pose.pose.position.y - now_vwbot_pose.pose.position.y;
    dis_yaw = atan(dis_y / dis_x);

    tf::Quaternion q;
    q.setRPY(0, 0, dis_yaw);
    target_pose.pose.orientation.x = q[0];
    target_pose.pose.orientation.y = q[1];
    target_pose.pose.orientation.z = q[2];
    target_pose.pose.orientation.w = q[3];

    target_pose.pose.position = now_ball.pose.pose.position;

    this->ball_orientation = target_pose;


}


void vwpp::Task::sub_red_angular_cb(const std_msgs::Float32::ConstPtr &msg)
{
    this->red_angle.data = -msg->data;
}


void vwpp::Task::sub_blue_angular_cb(const std_msgs::Float32::ConstPtr &msg)
{
    this->blue_angle.data = -msg->data;
}


void vwpp::Task::sub_yellow_angular_cb(const std_msgs::Float32::ConstPtr &msg)
{
    this->yellow_angle.data = -msg->data;
}


double vwpp::Task::getChange()
{
    double change_position, change_orientation, change_yaw;
    change_position = sqrt((this->vwbot_pose.pose.position.x - this->last_pose.pose.position.x) *
                           (this->vwbot_pose.pose.position.x - this->last_pose.pose.position.x)
                           + (this->vwbot_pose.pose.position.y - this->last_pose.pose.position.y) *
                             (this->vwbot_pose.pose.position.y - this->last_pose.pose.position.y));
    change_orientation = 10 * sqrt((this->vwbot_pose.pose.orientation.z - this->last_pose.pose.orientation.z) *
                                   (this->vwbot_pose.pose.orientation.z - this->last_pose.pose.orientation.z)
                                   + (this->vwbot_pose.pose.orientation.w - this->last_pose.pose.orientation.w) *
                                     (this->vwbot_pose.pose.orientation.w - this->last_pose.pose.orientation.w));

    tf::Matrix3x3 mat(tf::Quaternion(this->vwbot_pose.pose.orientation.x, this->vwbot_pose.pose.orientation.y,
                                     this->vwbot_pose.pose.orientation.z, this->vwbot_pose.pose.orientation.w));

    double yaw, pitch, roll;
    mat.getEulerYPR(yaw, pitch, roll);

    tf::Matrix3x3 mat_last(tf::Quaternion(this->last_pose.pose.orientation.x, this->last_pose.pose.orientation.y,
                                          this->last_pose.pose.orientation.z, this->last_pose.pose.orientation.w));

    double last_yaw, last_pitch, last_roll;
    mat_last.getEulerYPR(last_yaw, last_pitch, last_roll);

    change_yaw = last_yaw - yaw;

    if (change_yaw > 180) change_yaw = change_yaw - 360;
    if (change_yaw < -180) change_yaw = change_yaw + 360;

    ROS_ERROR("%lf    %lf   %lf", change_orientation, change_position, change_yaw);
    return sqrt(change_yaw * change_yaw * 100 + change_position * change_position);


}
