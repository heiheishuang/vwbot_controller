//
// Created by heihei on 2019/7/29.
//

#include "Flow.h"

vwpp::FlowController::FlowController():
        cur_flow_state(FLOW_START)
        // cur_task_state(NO_START)
{
    this->cur_task = new Task;
}

vwpp::FlowState vwpp::FlowController::getFlowState()
{
    return cur_flow_state;
}

// vwpp::TaskState vwpp::FlowController::getTaskState()
// {
//     return  cur_task_state;
// }

void vwpp::FlowController::flow()
{

    if (cur_flow_state == FLOW_START)
    {
        if (this->cur_task->getTaskState() == NO_START)
        {
            ROS_INFO("Now cur_flow_state is FLOW_STARE , cur_task_state is NO_START , task is taskNoBall! ");
            this->cur_task->taskNoBall();
            // cur_task_state = now_task.getTaskState();
        }
        else if (this->cur_task->getTaskState() == GOT_START)
        {
            ROS_INFO("Now I GOT THE BALL!");
            this->cur_task->taskGotBall();
        }

    }
    else if (cur_flow_state == FLOW_FINSH)
    {
        //TODO
        //Need cur_flow_state = FLOW_FINSH
        //When all the ball is over
    }
}





