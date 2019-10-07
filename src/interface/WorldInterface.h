//
// Created by heihei on 2019/10/2.
//

#ifndef VWBOT_CONTROLLER_WORLDINFORMATION_H_
#define VWBOT_CONTROLLER_WORLDINFORMATION_H_

#include <geometry_msgs/PoseStamped.h>
#include <ros/ros.h>
#include <boost/thread/mutex.hpp>

namespace vwpp
{

    class WorldInterface
    {
    public:

        virtual ~WorldInterface();

        void update();

        static WorldInterface* getInstance();

        //Task
        void setCountPoseChange(const int &_count);
        int getCountPoseChange();

        //MapInterface
        geometry_msgs::PoseStamped getVwbotPose();



    private:

        WorldInterface();

        static WorldInterface* instance;
        static boost::mutex mutex_instance;

        WorldInterface(const WorldInterface &, ros::Rate _loop_rate);

        WorldInterface &operator=(const WorldInterface &);

        ros::NodeHandle nh;
        ros::Rate loop_rate;

        int count_pose_change;

        ros::Subscriber sub_vwbot; //sub vwbot's pose

        geometry_msgs::PoseStamped vwbot_pose;

        void sub_from_vwbot_cb(const geometry_msgs::PoseStamped::ConstPtr &msg); //sub vwbot's pose

    };


}
#endif //VWBOT_CONTROLLER_WORLDINFORMATION_H_
