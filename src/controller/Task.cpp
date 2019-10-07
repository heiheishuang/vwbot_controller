//
// Created by heihei on 2019/7/28.
//

#include "Task.h"

void vwpp::TaskHasBall::taskHasBall()
{

    ROS_INFO("Now in the taskHasBall!");

    VisionInterface::getInstance()->send_flow_state(true);

    vwbot_controller::PoseAndColor now_ball;
    geometry_msgs::PoseStamped now_vwbot_pose;
    geometry_msgs::PoseStamped target_pose;
    now_vwbot_pose = WorldInterface::getInstance()->getVwbotPose();
    now_ball = VisionInterface::getInstance()->getBallPose();

    tf::Matrix3x3 mat(tf::Quaternion(WorldInterface::getInstance()->getVwbotPose().pose.orientation.x,
                                     WorldInterface::getInstance()->getVwbotPose().pose.orientation.y,
                                     WorldInterface::getInstance()->getVwbotPose().pose.orientation.z,
                                     WorldInterface::getInstance()->getVwbotPose().pose.orientation.w));

    double yaw, pitch, roll;

    mat.getEulerYPR(yaw, pitch, roll);

    if (this->task_base->getPoseVariation() <= 0.035)
        WorldInterface::getInstance()->setCountPoseChange(WorldInterface::getInstance()->getCountPoseChange() + 1);
    else
        WorldInterface::getInstance()->setCountPoseChange(0);

    if (this->task_has_ball_state == 0)
    {

        ROS_WARN("taskHasBall action 0!");
        tf::Quaternion q;
        q.setRPY(0, 0, this->getRelativeYawToBall());
        target_pose.pose.position = now_ball.pose.pose.position;
        target_pose.pose.orientation.x = q[0];
        target_pose.pose.orientation.y = q[1];
        target_pose.pose.orientation.z = q[2];
        target_pose.pose.orientation.w = q[3];

        target_pose.header.frame_id = "map";
        target_pose.header.stamp = ros::Time::now();

        this->task_base->action_go_to_point->moveToPoint(target_pose);

        if (fabs(this->getAbsoluteYawToBall()) / 3.14 * 180 <= 15)
        {
            this->task_has_ball_state = 1;

            VwbotInterface::getInstance()->send_cancel();

        }

    }

    if (this->task_has_ball_state == 1)
    {
        ROS_ERROR("Now in action 1");

        std::cout << YELLOW << "####   DIS_YAW  %lf  #### " << fabs(this->getAbsoluteYawToBall()) / 3.14 * 180 << "\033[0m"
                  << std::endl;
        std::cout << YELLOW << "####   DIS_LENGTH  %lf  #### " << this->getDistanceToBall() << "\033[0m" << std::endl;

        this->task_base->action_go_to_point->calculateCmdVelByPoint(now_vwbot_pose, now_ball.pose.pose.position.x, now_ball.pose.pose.position.y);

        if (this->getDistanceToBall() <= 0.35 and fabs(this->getAbsoluteYawToBall()) / 3.14 * 180 <= 15)
        {
            this->task_has_ball_state = 2;
        }

    }
    if (this->task_has_ball_state == 2)
    {

        ROS_WARN("taskHasBall action 2!");
        //Cancel the navigation goal
        VwbotInterface::getInstance()->send_cancel();

        std_msgs::Float32 angle;
        if (VisionInterface::getInstance()->getBallPose().color == "red") angle = VisionInterface::getInstance()->getRedAngle();
        if (VisionInterface::getInstance()->getBallPose().color == "green") angle = VisionInterface::getInstance()->getYellowAngle();
        if (VisionInterface::getInstance()->getBallPose().color == "blue") angle = VisionInterface::getInstance()->getBlueAngle();

        geometry_msgs::Twist vel;

        std::cout << YELLOW << "####   DIS_YAW  %lf  #### " << fabs(this->getAbsoluteYawToBall()) / 3.14 * 180 << "\033[0m"
                  << std::endl;
        std::cout << YELLOW << "####   DIS_LENGTH  %lf  #### " << this->getDistanceToBall() << "\033[0m" << std::endl;

        ROS_ERROR("Current angle data: %lf", angle.data);

        this->task_base->action_go_to_point->calculateCmdVelByAngle(-(angle.data * M_PI / 180.));

        if (fabs(angle.data) > 500)
        {
            this->task_has_ball_state = 3;
        }

        aim_angle_in_action3 = yaw - this->angle_last / 180.0 * M_PI;

        this->angle_last = angle.data;

    }
    if (this->task_has_ball_state == 3)
    {

        static JudgeAchieveCounter judge_achieve_counter(ParamInterface::getInstance()->getWaitTheTaskHasBall3());

        if (judge_achieve_counter.isAchieve())
        {

            VisionInterface::getInstance()->send_delete(true);

            this->task_has_ball_state = 0;

            ROS_ERROR("Now action 3");
            ROS_ERROR("Now in action 3");
            ROS_ERROR("Now in Action 3 !!!!!!!");

            geometry_msgs::Twist vel;

            std_msgs::Float32 angle;
            if (VisionInterface::getInstance()->getBallPose().color == "red") angle = VisionInterface::getInstance()->getRedAngle();
            if (VisionInterface::getInstance()->getBallPose().color == "green") angle = VisionInterface::getInstance()->getYellowAngle();
            if (VisionInterface::getInstance()->getBallPose().color == "blue") angle = VisionInterface::getInstance()->getBlueAngle();


            std::cout << YELLOW << "####   DIS_YAW  %lf  #### "
                      << fabs(this->getAbsoluteYawToBall()) / 3.14 * 180 << "\033[0m"
                      << std::endl;
            std::cout << YELLOW << "####   DIS_LENGTH  %lf  #### " << this->getDistanceToBall() << "\033[0m"
                      << std::endl;

            this->task_base->action_go_to_point->setTarget(aim_angle_in_action3);
            this->task_base->action_go_to_point->calculateCmdVelByAngle(yaw);

            if (fabs(angle.data) < 500)
            {
                this->task_has_ball_state = 1;
                VwbotInterface::getInstance()->send_cancel();
            }

        }
        else
        {
            ROS_ERROR("Now action 3");
            ROS_ERROR("Now in action 3");
            ROS_ERROR("Now in Action 3 !!!!!!!");

            geometry_msgs::Twist vel;

            std::cout << YELLOW << "####   DIS_YAW  %lf  #### " << fabs(this->getAbsoluteYawToBall()) / 3.14 * 180 << "\033[0m"
                      << std::endl;
            std::cout << YELLOW << "####   DIS_LENGTH  %lf  #### " << this->getDistanceToBall() << "\033[0m"
                      << std::endl;

            aim_angle_in_action3 = aim_angle_in_action3 - yaw;

            if (aim_angle_in_action3 > 180) aim_angle_in_action3 = aim_angle_in_action3 - 360;
            if (aim_angle_in_action3 < -180) aim_angle_in_action3 = aim_angle_in_action3 + 360;

            this->task_base->action_go_to_point->setTarget(aim_angle_in_action3);
            this->task_base->action_go_to_point->calculateCmdVelByAngle(yaw);
            ROS_ERROR("Aim angle data: %lf", aim_angle_in_action3);

        }

    }
    this->task_base->setLastPose(WorldInterface::getInstance()->getVwbotPose());

    //Change the taskRunState

}

void vwpp::TaskPutBall::goToPutBall()
{
    ROS_INFO("Now in the goToPutBall!");
    VisionInterface::getInstance()->send_flow_state(true);
    geometry_msgs::PoseStamped door;

    door.pose.orientation.w = 1;
    door.pose.orientation.x = 0;
    door.pose.orientation.y = 0;
    door.pose.orientation.z = 0;


    if (VisionInterface::getInstance()->getBallState().color == "green")
    {
        door.pose.position.x = ParamInterface::getInstance()->getPointYellowX();
        door.pose.position.y = ParamInterface::getInstance()->getPointYellowY();


        std::cout << YELLOW << "I WILL MOVE TO YELLOW DOOR!" << "\033[0m" << std::endl;

    }
    else if (VisionInterface::getInstance()->getBallState().color == "blue")
    {
        door.pose.position.x = ParamInterface::getInstance()->getPointBlueX();
        door.pose.position.y = ParamInterface::getInstance()->getPointBlueY();

        std::cout << BLUE << "I WILL MOVE TO BLUE DOOR!" << "\033[0m" << std::endl;

    }
    else if (VisionInterface::getInstance()->getBallState().color == "red")
    {
        door.pose.position.x = ParamInterface::getInstance()->getPointRedX();
        door.pose.position.y = ParamInterface::getInstance()->getPointRedY();

        std::cout << RED << "I WILL MOVE TO RED DOOR!" << "\033[0m" << std::endl;

    }
    door.header.frame_id = "map";
    door.header.stamp = ros::Time::now();

    aim_put_ball_point = door;

    this->task_base->setLastPose(WorldInterface::getInstance()->getVwbotPose());

    this->task_base->action_go_to_point->moveToPoint(door);

}

void vwpp::TaskNoBall::taskNOBall()
{

    this->task_base->setTaskRunState(TASK_START);
    VisionInterface::getInstance()->send_flow_state(false);
    ROS_INFO("Now in the taskNoball!");
    ROS_INFO("I'm finding the ball , waiting~ ");

    geometry_msgs::PoseStamped cur_pose;

    cur_pose = WorldInterface::getInstance()->getVwbotPose();

    if (fabs(cur_pose.pose.position.x - 3.0) <= 0.1)
        cur_pose.pose.position.x = cur_pose.pose.position.x - 0.5;
    if (fabs(cur_pose.pose.position.x - 0) <= 0.1)
        cur_pose.pose.position.x = cur_pose.pose.position.x + 0.5;
    if (fabs(cur_pose.pose.position.y - 2.0) <= 0.1)
        cur_pose.pose.position.y = cur_pose.pose.position.y - 0.5;
    if (fabs(cur_pose.pose.position.y + 2.0) <= 0.1)
        cur_pose.pose.position.y = cur_pose.pose.position.y + 0.5;

    if (VwbotInterface::getInstance()->getMoveBaseState() != GOT_GOAL and this->task_no_ball_state == 0)
    {

        cur_pose.header.frame_id = "map";
        cur_pose.header.stamp = ros::Time::now();
        cur_pose.pose.orientation.x = 0;
        cur_pose.pose.orientation.y = 0;
        cur_pose.pose.orientation.z = 1;
        cur_pose.pose.orientation.w = 0;

        this->task_base->action_go_to_point->moveToPoint(cur_pose);
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

        tf::Matrix3x3 mat(tf::Quaternion(WorldInterface::getInstance()->getVwbotPose().pose.orientation.x,
                                         WorldInterface::getInstance()->getVwbotPose().pose.orientation.y,
                                         WorldInterface::getInstance()->getVwbotPose().pose.orientation.z,
                                         WorldInterface::getInstance()->getVwbotPose().pose.orientation.w));

        double yaw, pitch, roll;

        mat.getEulerYPR(yaw, pitch, roll);

        if (fabs(yaw / 3.14 * 180) - 180 < 15)
            task_no_ball_state = 2;

        this->task_base->action_go_to_point->moveToPoint(cur_pose);

        ROS_ERROR("###################NO BALL 2");

    }
    this->task_base->setLastPose(WorldInterface::getInstance()->getVwbotPose());

    if (this->task_no_ball_state == 2 and VwbotInterface::getInstance()->getMoveBaseState() == GOT_GOAL)
    {
        this->task_base->setTaskRunState(TASK_FINSH);
    }

}

void vwpp::TaskChangeWatchTower::taskChangeWatchTower()
{
    ROS_INFO("Now in the changeTask!");

    VisionInterface::getInstance()->send_flow_state(false);
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
        goal.pose.position.x = ParamInterface::getInstance()->getChange1X();
        goal.pose.position.y = ParamInterface::getInstance()->getChange1Y();

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
        goal.pose.position.x = ParamInterface::getInstance()->getChange2X();
        goal.pose.position.y = ParamInterface::getInstance()->getChange2Y();

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
        goal.pose.position.x = ParamInterface::getInstance()->getChange3X();
        goal.pose.position.y = ParamInterface::getInstance()->getChange3Y();

    }

    if (VisionInterface::getInstance()->getClientChangePoint(goal))
    {

        ROS_ERROR("The goal is right");
        this->task_base->action_go_to_point->moveToPoint(goal);
    }
    else
    {
        ROS_ERROR("change the change goal");
        goal.pose.position.x = goal.pose.position.x - 0.15;
        this->task_base->action_go_to_point->moveToPoint(goal);

        ROS_ERROR("%lf %lf ", goal.pose.position.x, goal.pose.position.y);
    }

    this->task_base->setLastPose(WorldInterface::getInstance()->getVwbotPose());


    //Change the taskRunSate

}

//*****************************
//TaskBase
vwpp::TaskBase::TaskBase()
{
    this->last_pose = WorldInterface::getInstance()->getVwbotPose();
    this->task_run_state = TASK_START;
}

vwpp::TaskBase::~TaskBase()
= default;

double vwpp::TaskBase::getPoseVariation()
{
    geometry_msgs::PoseStamped vwbot_pose, ball_pose;
    vwbot_pose = WorldInterface::getInstance()->getVwbotPose();

    double change_position, change_orientation, change_yaw;
    change_position = sqrt((vwbot_pose.pose.position.x - this->last_pose.pose.position.x) *
                           (vwbot_pose.pose.position.x - this->last_pose.pose.position.x)
                           + (vwbot_pose.pose.position.y - this->last_pose.pose.position.y) *
                             (vwbot_pose.pose.position.y - this->last_pose.pose.position.y));
    change_orientation = 10 * sqrt((vwbot_pose.pose.orientation.z - this->last_pose.pose.orientation.z) *
                                   (vwbot_pose.pose.orientation.z - this->last_pose.pose.orientation.z)
                                   + (vwbot_pose.pose.orientation.w - this->last_pose.pose.orientation.w) *
                                     (vwbot_pose.pose.orientation.w - this->last_pose.pose.orientation.w));

    tf::Matrix3x3 mat(tf::Quaternion(vwbot_pose.pose.orientation.x, vwbot_pose.pose.orientation.y,
                                     vwbot_pose.pose.orientation.z, vwbot_pose.pose.orientation.w));

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


void vwpp::TaskBase::setLastPose(const geometry_msgs::PoseStamped &_last_pose)
{
    last_pose = _last_pose;
}


vwpp::TaskRunState vwpp::TaskBase::getTaskRunState() const
{
    return task_run_state;
}


void vwpp::TaskBase::setTaskRunState(vwpp::TaskRunState _task_run_state)
{
    task_run_state = _task_run_state;
}


double vwpp::TaskHasBall::getRelativeYawToBall()
{
    double dis_yaw, dis_x, dis_y;
    dis_x = VisionInterface::getInstance()->getBallPose().pose.pose.position.x - WorldInterface::getInstance()->getVwbotPose().pose.position.x;
    dis_y = VisionInterface::getInstance()->getBallPose().pose.pose.position.x - WorldInterface::getInstance()->getVwbotPose().pose.position.y;
    dis_yaw = atan2(dis_y, dis_x);   //vwbot_pose and ball_pose
    return dis_yaw;
}
double vwpp::TaskHasBall::getDistanceToBall()
{
    double dis_x, dis_y, dis_length;
    dis_x = VisionInterface::getInstance()->getBallPose().pose.pose.position.x - WorldInterface::getInstance()->getVwbotPose().pose.position.x;
    dis_y = VisionInterface::getInstance()->getBallPose().pose.pose.position.y - WorldInterface::getInstance()->getVwbotPose().pose.position.x;
    dis_length = sqrt(dis_x * dis_x + dis_y * dis_y);

    return dis_length;
}
double vwpp::TaskHasBall::getAbsoluteYawToBall()
{
    double dis_yaw, dis_x, dis_y;
    dis_x = VisionInterface::getInstance()->getBallPose().pose.pose.position.x - WorldInterface::getInstance()->getVwbotPose().pose.position.x;
    dis_y = VisionInterface::getInstance()->getBallPose().pose.pose.position.y - WorldInterface::getInstance()->getVwbotPose().pose.position.x;
    dis_yaw = atan2(dis_y, dis_x);   //vwbot_pose and ball_pose

    tf::Matrix3x3 mat(tf::Quaternion(WorldInterface::getInstance()->getVwbotPose().pose.orientation.x, WorldInterface::getInstance()->getVwbotPose().pose.orientation.y,
                                     WorldInterface::getInstance()->getVwbotPose().pose.orientation.z, WorldInterface::getInstance()->getVwbotPose().pose.orientation.w));

    double yaw, pitch, roll;

    mat.getEulerYPR(yaw, pitch, roll);

    return  dis_yaw - yaw;
}


//TaskHasBall

vwpp::TaskHasBall::TaskHasBall():
        task_has_ball_state(0),
        angle_last(0),
        aim_angle_in_action3(0)
{
    this->task_base = new TaskBase;
}

vwpp::TaskHasBall::~TaskHasBall()
{
    delete this->task_base;
}

int vwpp::TaskHasBall::getTaskHasBallState() const
{
    return task_has_ball_state;
}


void vwpp::TaskHasBall::setTaskHasBallState(int _task_has_ball_state)
{
    task_has_ball_state = _task_has_ball_state;
}



//TaskPutBall

vwpp::TaskPutBall::TaskPutBall()
{
    this->task_base = new TaskBase;
    this->aim_put_ball_point.pose.position.x = 3.1;
    this->aim_put_ball_point.pose.position.y = 0.0;
    this->aim_put_ball_point.pose.position.z = 0.0;
}

vwpp::TaskPutBall::~TaskPutBall()
{
    delete this->task_base;
}


const geometry_msgs::PoseStamped &vwpp::TaskPutBall::getAimPutBallPoint() const
{
    return aim_put_ball_point;
}


//TaskNoBall

vwpp::TaskNoBall::TaskNoBall():
    task_no_ball_state(0)
{
    this->task_base = new TaskBase;
}

vwpp::TaskNoBall::~TaskNoBall()
{
    delete this->task_base;
}

void vwpp::TaskNoBall::setTaskNoBallState(int _task_no_ball_state)
{
    task_no_ball_state = _task_no_ball_state;
}

int vwpp::TaskNoBall::getTaskNoBallState() const
{
    return task_no_ball_state;
}

vwpp::TaskRunState vwpp::TaskNoBall::getTaskRunState()
{
    return  this->task_base->getTaskRunState();
}

//TaskChangeWatchTower
vwpp::TaskChangeWatchTower::TaskChangeWatchTower():
    task_change_point_state(0)
{
    this->task_base = new TaskBase;
}

vwpp::TaskChangeWatchTower::~TaskChangeWatchTower()
{
    delete this->task_base;
}

int vwpp::TaskChangeWatchTower::getTaskChangePointState() const
{
    return task_change_point_state;
}

void vwpp::TaskChangeWatchTower::setTaskChangePointState(int _task_change_point_state)
{
    task_change_point_state = _task_change_point_state;
}
