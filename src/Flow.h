//
// Created by heihei on 2019/7/29.
//

#ifndef VWBOT_CONTROLLER_FLOW_H_
#define VWBOT_CONTROLLER_FLOW_H_

#include "Task.h"
namespace vwpp
{
    enum FlowState
    {
        FLOW_START,
        FlOW_PROCESS,
        FLOW_FINSH
    };

    class FlowController
    {
    public:

        FlowController();

        FlowState getFlowState();
        // TaskState getTaskState();

        void flow();

        // Task now_task;

    private:
        FlowState cur_flow_state;
        Task *cur_task;
       // TaskState cur_task_state;

    };
}

#endif //VWBOT_CONTROLLER_FLOW_H_
