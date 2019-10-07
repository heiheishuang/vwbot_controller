//
// Created by heihei on 2019/7/28.
//

#ifndef VWBOT_CONTROLLER_ACTION_H_
#define VWBOT_CONTROLLER_ACTION_H_

#include <geometry_msgs/PoseStamped.h>
#include <tf/transform_datatypes.h>
#include <geometry_msgs/Twist.h>
#include <ros/ros.h>

#include "../interface/ParamInterface.h"
#include "PIDController.h"
#include "../utils/Color.h"
#include "../interface/VisionInterface.h"
#include "../interface/VwbotInterface.h"

namespace vwpp
{

    class ActionGoToPoint
    {
    public:

        ActionGoToPoint();
        virtual ~ActionGoToPoint();

        void calculateCmdVelByAngle(const double &_angle);
        void calculateCmdVelByPoint(const geometry_msgs::PoseStamped &_vwbot,
                                                    const double &_ball_x, const double &_ball_y);
        void moveToPoint(const geometry_msgs::PoseStamped &_vwbot);

        void setTarget(const double &_target);

    private:

        double target;

    };

}



#endif //VWBOT_CONTROLLER_ACTION_H_
