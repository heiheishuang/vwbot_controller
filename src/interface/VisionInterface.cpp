//
// Created by heihei on 2019/10/5.
//

#include "VisionInterface.h"

vwpp::VisionInterface::VisionInterface():
        nh("~")
{

    // Information From WorldInterface
    sub_ball_pose_color = nh.subscribe<vwbot_controller::PoseAndColor>
            ("/ball", 1, &VisionInterface::sub_from_ball_cb, this);

    sub_ball_state = nh.subscribe<dector::ColorBool>
            ("/ball_state", 10, &VisionInterface::sub_catchball_state_cb, this);

    sub_red = nh.subscribe<std_msgs::Float32>
            ("/red_angle", 10, &VisionInterface::sub_red_angle_cb, this);

    sub_yellow = nh.subscribe<std_msgs::Float32>
            ("/green_angle", 10, &VisionInterface::sub_yellow_angle_cb, this);

    sub_blue = nh.subscribe<std_msgs::Float32>
            ("/blue_angle", 10, &VisionInterface::sub_blue_angle_cb, this);

    client_change_point = nh.serviceClient<dector::PointServer>("/check_point");

    // Publish To WorldInterface


    pub_flow = nh.advertise<std_msgs::Bool>("/send_flow_state",1);

    pub_delete_point = nh.advertise<std_msgs::Bool>("/delete_current_point",1);

}

vwpp::VisionInterface::~VisionInterface()
{
    delete instance;
}

vwpp::VisionInterface* vwpp::VisionInterface::instance = nullptr;
boost::mutex vwpp::VisionInterface::mutex_instance;  //TODO

vwpp::VisionInterface* vwpp::VisionInterface::getInstance()
{
    if (instance == nullptr)
    {
        boost::unique_lock<boost::mutex> uq_lock_instance(mutex_instance);
        if (instance == nullptr)
        {
            instance = new VisionInterface();
        }
    }

    return instance;
}

vwpp::VisionInterface::VisionInterface(const VisionInterface &)
{

}

vwpp::VisionInterface &vwpp::VisionInterface::operator=(const vwpp::VisionInterface &)
{

}

void vwpp::VisionInterface::update()
{
    ros::spinOnce();
}

void vwpp::VisionInterface::sub_catchball_state_cb(const dector::ColorBool::ConstPtr &msg)    //ball_state
{

    this->ball_state = *msg;

}
void vwpp::VisionInterface::sub_from_ball_cb(const vwbot_controller::PoseAndColor::ConstPtr &msg)  //ball_pose_and_color
{
    this->ball_pose_and_color = *msg;

    std::cout << YELLOW << "#### ball_pose  " << this->ball_pose_and_color.pose.pose.position.x << "  " <<
              this->ball_pose_and_color.pose.pose.position.y << "   " << this->ball_pose_and_color.color << " ####  \033[0m" << std::endl;

}

void vwpp::VisionInterface::sub_red_angle_cb(const std_msgs::Float32::ConstPtr &msg)  //red_angle
{
    this->red_angle.data = -msg->data;
}

void vwpp::VisionInterface::sub_blue_angle_cb(const std_msgs::Float32::ConstPtr &msg)  //blue_angle
{
    this->blue_angle.data = -msg->data;
}

void vwpp::VisionInterface::sub_yellow_angle_cb(const std_msgs::Float32::ConstPtr &msg)  //yellow_angle
{
    this->yellow_angle.data = -msg->data;
}


void vwpp::VisionInterface::send_flow_state(const bool &_flow)
{
    std_msgs::Bool flow_state;
    flow_state.data = _flow;
    this->pub_flow.publish(flow_state);
}


void vwpp::VisionInterface::send_delete(const bool &_delete)
{
    ROS_ERROR("########Delete the point");
    std_msgs::Bool state;
    state.data = true;
    this->pub_delete_point.publish(state);
}

vwbot_controller::PoseAndColor vwpp::VisionInterface::getBallPose()
{
    return this->ball_pose_and_color;
}


dector::ColorBool vwpp::VisionInterface::getBallState()
{
    return ball_state;
}
std_msgs::Float32 vwpp::VisionInterface::getRedAngle()
{
    return this->red_angle;
}


std_msgs::Float32 vwpp::VisionInterface::getYellowAngle()
{
    return this->yellow_angle;
}


std_msgs::Float32 vwpp::VisionInterface::getBlueAngle()
{
    return this->blue_angle;
}

bool vwpp::VisionInterface::getClientChangePoint(const geometry_msgs::PoseStamped& point)
{
    dector::PointServer srv;
    srv.request.point = point.pose.position;
    return this->client_change_point.call(srv);
}
