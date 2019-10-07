//
// Created by heihei on 2019/10/5.
//

#ifndef VWBOT_CONTROLLER_VWBOTINTERFACE_H_
#define VWBOT_CONTROLLER_VWBOTINTERFACE_H_

#include <actionlib/client/simple_action_client.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/Bool.h>
#include <ros/ros.h>

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

namespace vwpp
{

    enum ActionState
    {
        NO_GOAL,
        GOT_GOAL,
        FAILED_TO_GOAL
    };

    class VwbotInterface
    {
    public:

        virtual ~VwbotInterface();

        void update();

        static VwbotInterface* getInstance();

        void send_cancel();

        void send_cmd_vel(const geometry_msgs::Twist &_vel);

        void send_move_base(const geometry_msgs::PoseStamped &_pose_stamped);

        void send_to_hand(const bool &_ball);

        ActionState getMoveBaseState();

    private:

        VwbotInterface();

        static VwbotInterface* instance;
        static boost::mutex mutex_instance;

        ros::NodeHandle nh;

        VwbotInterface(const VwbotInterface &);

        VwbotInterface &operator=(const VwbotInterface &);

        ActionState move_base_state;

        ros::Publisher pub_hand;
        ros::Publisher pub_cmd_vel;
        ros::Publisher pub_goal_id;
    };
}
#endif //VWBOT_CONTROLLER_VWBOTINTERFACE_H_
