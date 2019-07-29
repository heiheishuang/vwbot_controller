//
// Created by heihei on 2019/7/29.
//

#include "Flow.h"

vwpp::FlowController::FlowController():
        cur_flow_state(FLOW_START),
        cur_task_state(NO_START)
{

}

vwpp::FlowState vwpp::FlowController::getFlowState()
{
    return cur_flow_state;
}

int vwpp::FlowController::flow()
{
    Task now_task;

    if (cur_flow_state == FLOW_START)
    {
        if (cur_task_state == NO_START)
        {
            now_task.taskNoBall();
            cur_task_state = now_task.getTaskState();
        }

        cur_flow_state = FlOW_PROCESS;
    }
    else if (cur_flow_state == FlOW_PROCESS)
    {
        if (cur_task_state == NO_START)
        {
            now_task.taskNoBall();
            cur_task_state = now_task.getTaskState();
        }
        else if (cur_task_state == GOT_START)
        {
            now_task.taskGotBall();
            cur_task_state = now_task.getTaskState();
        }
    }

}





