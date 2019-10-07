//
// Created by heihei on 2019/7/28.
//

#ifndef VWBOT_CONTROLLER_TASK_H_
#define VWBOT_CONTROLLER_TASK_H_

#include "../interface/WorldInterface.h"
#include "../utils/JudgeAchieveCounter.h"
#include "../interface/VwbotInterface.h"
#include "../interface/VisionInterface.h"
#include "Action.h"
#include "../utils/Color.h"
namespace vwpp
{

    enum TaskRunState
    {
        TASK_START,
        TASK_FINSH
    };

    class TaskBase
    {
    public:
        TaskBase();
        virtual ~TaskBase();
        double getPoseVariation();
        void setLastPose(const geometry_msgs::PoseStamped &_last_pose);

        TaskRunState getTaskRunState() const;
        void setTaskRunState(TaskRunState _task_run_state);

        ActionGoToPoint* action_go_to_point;

    private:
        geometry_msgs::PoseStamped last_pose;
        TaskRunState task_run_state;

    };

    class TaskHasBall
    {
    public:
        TaskHasBall();
        virtual ~TaskHasBall();
        void taskHasBall();
        int getTaskHasBallState() const;
        void setTaskHasBallState(int _task_has_ball_state);

        double getRelativeYawToBall();
        double getAbsoluteYawToBall();
        double getDistanceToBall();

    private:
        TaskBase* task_base;
        int task_has_ball_state;
        double angle_last;
        double aim_angle_in_action3;
    };

    class TaskPutBall
    {
    public:
        TaskPutBall();
        virtual ~TaskPutBall();
        void goToPutBall();
        const geometry_msgs::PoseStamped &getAimPutBallPoint() const;

    private:
        TaskBase* task_base;
        geometry_msgs::PoseStamped aim_put_ball_point;

    };

    class TaskNoBall
    {
    public:
        TaskNoBall();
        virtual ~TaskNoBall();
        void taskNOBall();
        int getTaskNoBallState() const;
        void setTaskNoBallState(int _task_no_ball_state);

        TaskRunState getTaskRunState();

    private:
        TaskBase* task_base;
        int task_no_ball_state;
    };

    class TaskChangeWatchTower
    {
    public:
        TaskChangeWatchTower();
        virtual ~TaskChangeWatchTower();
        void taskChangeWatchTower();
        int getTaskChangePointState() const;
        void setTaskChangePointState(int _task_change_point_state);


    private:
        TaskBase* task_base;
        int task_change_point_state;

    };

}
#endif //VWBOT_CONTROLLER_TASK_H_
