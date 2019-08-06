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

    task_has_ball_state = 0;

    now_color = "";

}


vwpp::Task::~Task()
{
    delete this->cur_action;
}


void vwpp::Task::taskHasBall()
{

    ROS_INFO("Now in the taskHasBall!");

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
    dis_yaw = atan2(dis_y , dis_x);

    dis_length = sqrt(dis_x * dis_x +dis_y * dis_y);

    tf::Matrix3x3 mat(tf::Quaternion(now_vwbot_pose.pose.orientation.x, now_vwbot_pose.pose.orientation.y,
                                     now_vwbot_pose.pose.orientation.z, now_vwbot_pose.pose.orientation.w));

    double yaw, pitch, roll;

    mat.getEulerYPR(yaw, pitch, roll);

    std::cout << YELLOW << "####   DIS_YAW  %lf  #### " <<  fabs(dis_yaw - yaw) << "\033[0m" << std::endl;

    if (this->cur_action->getActionState() != GOT_GOAL and this->task_has_ball_state == 0)
    {    //TODO
        //change the pose of the vwbot to ball

        ROS_WARN("taskHasBall action 1!");
        target_pose.pose = now_ball.pose.pose;
        target_pose.pose.position.x = (dis_length - 0.30 ) / dis_length * dis_x + now_vwbot_pose.pose.position.x;
        target_pose.pose.position.y = (dis_length - 0.30 ) / dis_length * dis_y + now_vwbot_pose.pose.position.y;

        // NEW in 8.4
        // TODO
        tf::Quaternion q;
        q.setRPY(0, 0, dis_yaw);
        target_pose.pose.orientation.x = q[0];
        target_pose.pose.orientation.y = q[1];
        target_pose.pose.orientation.z = q[2];
        target_pose.pose.orientation.w = q[3];

        target_pose.header.frame_id = "map";
        target_pose.header.stamp = ros::Time::now();


        ball_orientation.pose.orientation = target_pose.pose.orientation;

        this->cur_action->action_move_base(target_pose);

    }
    else
    {
        // length less than 30cm has problem
        // TODO
        ROS_WARN("taskHasBall action 2!");
        target_pose.header.frame_id = "map";
        target_pose.header.stamp = ros::Time::now();

        target_pose.pose = now_ball.pose.pose;
        target_pose.pose.position.x = (dis_length + 0.03) / dis_length * dis_x + now_vwbot_pose.pose.position.x;
        target_pose.pose.position.y = (dis_length + 0.03) / dis_length * dis_y + now_vwbot_pose.pose.position.y;

        //Less than 30cm using the goal at 30cm
        // target_pose.pose.position = ball_orientation.pose.position;
        target_pose.pose.orientation = ball_orientation.pose.orientation;

        // NEW in 8.5
        // TODO
        // tf::Quaternion q;
        // q.setRPY(0, 0, yaw);
        // target_pose.pose.orientation.x = q[0];
        // target_pose.pose.orientation.y = q[1];
        // target_pose.pose.orientation.z = q[2];
        // target_pose.pose.orientation.w = q[3];


        // target_pose.pose.orientation.x = 0;
        // target_pose.pose.orientation.y = 0;
        // target_pose.pose.orientation.z = 0;
        // target_pose.pose.orientation.w = 1;
        // NEW in 8.5

        this->cur_action->action_move_base(target_pose);
        this->task_has_ball_state = 2;
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

    printf("Orientation %lf %lf \n", cur_pose.pose.orientation.z, cur_pose.pose.orientation.w);

    tf::Matrix3x3 mat(tf::Quaternion(cur_pose.pose.orientation.x, cur_pose.pose.orientation.y,
                                     cur_pose.pose.orientation.z, cur_pose.pose.orientation.w));

    double yaw, pitch, roll;
    mat.getEulerYPR(yaw, pitch, roll);

    yaw = yaw / 3.14 * 180 + 20;
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


void vwpp::Task::taskChange()
{
    ROS_INFO("Now in the changeTask!");

    geometry_msgs::PoseStamped goal;
    goal.header.frame_id = "map";
    goal.header.stamp = ros::Time::now();
    goal.pose.orientation.w = 0;
    goal.pose.orientation.x = 0;
    goal.pose.orientation.y = 0;
    goal.pose.orientation.z = 1;
    goal.pose.position.x = MID_X;
    goal.pose.position.y = MID_Y;

    // changeTask 1 from taskPutBall to taskNoBall
    this->cur_action->action_move_base(goal);

}


void vwpp::Task::taskCatchBall(bool state)
{

    std_msgs::Bool hand_state;
    hand_state.data = state;
    this->cur_action->send_to_hand(hand_state);

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
    return  task_has_ball_state;
}

int vwpp::Task::getActionState()
{
    return this->cur_action->getActionState();
}

double vwpp::Task::getLengthBetweenBallAndVwbot()
{
    double dis_yaw, dis_x, dis_y, dis_length;
    dis_x = this->ball_pose.pose.pose.position.x - this->vwbot_pose.pose.position.x;
    dis_y = this->ball_pose.pose.pose.position.y - this->vwbot_pose.pose.position.y;
    dis_length = sqrt(dis_x * dis_x + dis_y * dis_y);
    return dis_length;
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
    ROS_WARN("#### ball_pose   %lf %lf %lf", this->ball_pose.pose.pose.position.x,
            this->ball_pose.pose.pose.position.y, this->ball_pose.pose.pose.position.z);

}

void vwpp::Task::sendToTaskBall(int state)
{

    this->task_has_ball_state = state;

}
void vwpp::Task::sendToColor(std::string color)
{
    this->now_color = std::move(color);
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