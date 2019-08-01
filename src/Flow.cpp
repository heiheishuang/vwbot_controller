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

vwpp::TaskState vwpp::FlowController::getTaskState()
{
    return  cur_task_state;
}

void vwpp::FlowController::flow()
{
    Task now_task;

    if (cur_flow_state == FLOW_START)
    {
        if (cur_task_state == NO_START)
        {
            ROS_INFO("Now cur_flow_state is FLOW_STARE , cur_task_state is NO_START , task is taskNoBall! ");
            now_task.taskNoBall();
            cur_task_state = now_task.getTaskState();
        }
        else if (cur_task_state == GOT_START)
        {
            ROS_INFO("Now I GOT THE BALL!");
            now_task.taskGotBall();
            cur_task_state = now_task.getTaskState();
        }

        cur_flow_state = FlOW_PROCESS;
        ROS_INFO("Now cur_flow_state is FLOW_PROCESS");
    }
    else if (cur_flow_state == FlOW_PROCESS)
    {
        if (cur_task_state == NO_START)
        {
            ROS_INFO("Now cur_flow_state if Flow_PROCESS, cur_task_state is NO_START , task is taskNoBall! ");
            now_task.taskNoBall();
            cur_task_state = now_task.getTaskState();
        }
        else if (cur_task_state == GOT_START)
        {
            ROS_INFO("Now cur_flow_state if Flow_PROCESS, cur_task_state is GOT_START , task is taskGotBall! ");
            now_task.taskGotBall();
            cur_task_state = now_task.getTaskState();
        }
    }
    else if (cur_flow_state == FLOW_FINSH)
    {
        //TODO
        //Need cur_flow_state = FLOW_FINSH
        //When all the ball is over
    }
}





