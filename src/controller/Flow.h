//
// Created by heihei on 2019/7/29.
//

#ifndef VWBOT_CONTROLLER_FLOW_H_
#define VWBOT_CONTROLLER_FLOW_H_



#include "vwbot_controller/TutorialsConfig.h"
#include "../interface/ParamInterface.h"
#include "Task.h"
#include "../utils/Color.h"

namespace vwpp
{
    enum FlowState
    {
        FLOW_START,
        FLOW_FINSH
    };

    enum TaskState
    {
        HAS_BALL_START,
        NO_BALL,
        PUT_BALL_START,
        CHANGE_START,
        CATCH_BALL_START_CATCH,
        CATCH_BALL_START_PUT,
    };

    class FlowController
    {
    public:

        FlowController();

        virtual ~FlowController();

        FlowState getFlowState();

        void run();

    private:

        FlowState cur_flow_state;
        TaskState cur_task_state;

        TaskHasBall* task_has_ball;
        TaskNoBall* task_no_ball;
        TaskChangeWatchTower* task_change_watch_tower;
        TaskPutBall* task_put_ball;


    };
}

#endif //VWBOT_CONTROLLER_FLOW_H_
