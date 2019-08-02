//
// Created by heihei on 2019/7/29.
//

#include "Flow.h"


vwpp::FlowController::FlowController() :
        cur_flow_state(FLOW_START)
{
    this->cur_task = new Task();
}


vwpp::FlowController::~FlowController()
{
    delete this->cur_task;
}


vwpp::FlowState vwpp::FlowController::getFlowState()
{
    return cur_flow_state;
}



void vwpp::FlowController::run()
{

    if (cur_flow_state == FLOW_START)
    {
        if (this->cur_task->getTaskState() == NO_BALL_START)
        {
            ROS_INFO("Now cur_flow_state is FLOW_STARE , cur_task_state is NO_BALL_START , task is taskNoBall! ");
            this->cur_task->taskNoBall();

        }
        else if (this->cur_task->getTaskState() == HAS_BALL_START)
        {
            ROS_INFO("Now I GOT THE BALL!");
            this->cur_task->taskHasBall();

        }
        else if (this->cur_task->getTaskState() == PUT_BALL_START)
        {
            ROS_INFO("NOW I will put the ball!");
            this->cur_task->taskPutBall();
        }
        else if (this->cur_task->getTaskState() == CHANGE_START)
        {
            ROS_INFO("Now I will change the task from taskPutBall to taskNoball ");
            this->cur_task->taskChange();
        }

    }
    else if (cur_flow_state == FLOW_FINSH)
    {
        //TODO
        //Need cur_flow_state = FLOW_FINSH
        //When all the ball is over
    }
}





