//
// Created by heihei on 2019/10/4.
//

#ifndef VWBOT_CONTROLLER_PARAMINTERFACE_H_
#define VWBOT_CONTROLLER_PARAMINTERFACE_H_

#include <ros/ros.h>
#include <dynamic_reconfigure/server.h>
#include <utility>
#include "vwbot_controller/TutorialsConfig.h"

namespace vwpp
{
    class ParamInterface
    {
    public:

        virtual ~ParamInterface();

        static ParamInterface* getInstance();

        void update();

        double getParamPidAction1P() const;

        double getParamPidAction1I() const;

        double getParamPidAction1D() const;

        double getParamPidAction1Vel() const;

        double getChange1X() const;

        double getChange1Y() const;

        double getChange2X() const;

        double getChange2Y() const;

        double getChange3X() const;

        double getChange3Y() const;

        double getPointYellowX() const;

        double getPointYellowY() const;

        double getPointRedX() const;

        double getPointRedY() const;

        double getPointBlueX() const;

        double getPointBlueY() const;

        double getDisLengthWhenCatch() const;

        double getWaitTheTaskHasBall3() const;

        double getAbsoluteYawToBall() const;

        double getPoseVariation() const;

    private:
        ParamInterface();

        ParamInterface(const ParamInterface &);

        ParamInterface &operator=(const ParamInterface &);

        static ParamInterface* instance;
        static boost::mutex mutex_instance;

        ros::NodeHandle nh;

        dynamic_reconfigure::Server<vwbot_controller::TutorialsConfig> server;
        dynamic_reconfigure::Server<vwbot_controller::TutorialsConfig>::CallbackType callback_type;

        void reconfig_cb(vwbot_controller::TutorialsConfig &_config, uint32_t _level);

        double param_pid_action1_p;
        double param_pid_action1_i;
        double param_pid_action1_d;
        double param_pid_action1_vel;

        double change_1_x;
        double change_1_y;
        double change_2_x;
        double change_2_y;
        double change_3_x;
        double change_3_y;

        double point_yellow_x;
        double point_yellow_y;
        double point_red_x;
        double point_red_y;
        double point_blue_x;
        double point_blue_y;

        double dis_length_when_catch;
        double wait_the_task_has_ball3;

        double absolute_yaw_to_ball;
        double pose_variation;


    };

}
#endif //VWBOT_CONTROLLER_PARAMINTERFACE_H_
