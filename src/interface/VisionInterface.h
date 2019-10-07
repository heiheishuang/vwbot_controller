//
// Created by heihei on 2019/10/5.
//

#ifndef VWBOT_CONTROLLER_VERSIONINTERFACE_H_
#define VWBOT_CONTROLLER_VERSIONINTERFACE_H_

#include "../utils/Color.h"
#include "vwbot_controller/PoseAndColor.h"
#include "vwbot_controller/PointServer.h"
#include "vwbot_controller/PointServerRequest.h"
#include "vwbot_controller/PointServerResponse.h"
#include <std_msgs/Float32.h>
#include <geometry_msgs/PoseStamped.h>
#include "vwbot_controller/ColorBool.h"
#include <std_msgs/Bool.h>
#include <ros/ros.h>
#include <boost/thread/mutex.hpp>

namespace vwpp
{

    class VisionInterface
    {
    public:
        virtual ~VisionInterface();

        void update();

        static VisionInterface* getInstance();

        //Public New in VisionInterface

        void send_delete(const bool &_delete);

        void send_flow_state(const bool &_flow);
        vwbot_controller::PoseAndColor getBallPose();
        dector::ColorBool getBallState();
        std_msgs::Float32 getRedAngle();
        std_msgs::Float32 getYellowAngle();
        std_msgs::Float32 getBlueAngle();
        bool getClientChangePoint(const geometry_msgs::PoseStamped& point);


    private:

        VisionInterface();

        static VisionInterface* instance;
        static boost::mutex mutex_instance;

        ros::NodeHandle nh;

        VisionInterface(const VisionInterface &);

        VisionInterface &operator=(const VisionInterface &);


        ros::Subscriber sub_ball_pose_color;  //sub ball's pose and color
        ros::Subscriber sub_ball_state;  // sub ball'state when catch the ball
        ros::ServiceClient client_change_point;
        ros::Subscriber sub_red;
        ros::Subscriber sub_yellow;
        ros::Subscriber sub_blue;

        vwbot_controller::PoseAndColor ball_pose_and_color; //ball's pose and color
        dector::ColorBool ball_state; //ball's state when catch the ball

        std_msgs::Float32 red_angle;
        std_msgs::Float32 yellow_angle;
        std_msgs::Float32 blue_angle;

        void sub_from_ball_cb(const vwbot_controller::PoseAndColor::ConstPtr &msg);  //sub ball's pose and color
        void sub_catchball_state_cb(const dector::ColorBool::ConstPtr &msg);    //sub ball's state when catch the ball

        void sub_red_angle_cb(const std_msgs::Float32::ConstPtr &msg );
        void sub_yellow_angle_cb(const std_msgs::Float32::ConstPtr &msg);
        void sub_blue_angle_cb(const std_msgs::Float32::ConstPtr &msg);
        ros::Publisher pub_flow;
        ros::Publisher pub_delete_point;


    };
}

#endif //VWBOT_CONTROLLER_VERSIONINTERFACE_H_
