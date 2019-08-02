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
        FLOW_FINSH
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
        Task* cur_task;

    };
}

#endif //VWBOT_CONTROLLER_FLOW_H_
