//
// Created by heihei on 2019/7/29.
//

#include "Flow.h"


vwpp::FlowController::FlowController() :
        cur_flow_state(FLOW_START),
        cur_task_state(NO_BALL_START)
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
        if (cur_task_state == NO_BALL_START)
        {
            ROS_INFO("NOW cur_task_state is NO_BALL_START ");

            if (this->cur_task->getBallPose().pose.pose.position.x >= 0)
            {

                cur_task_state = HAS_BALL_START;

            }

            this->cur_task->taskNoBall();

        }
        else if (cur_task_state == HAS_BALL_START)
        {
            ROS_WARN("####  getTaskHasBallState = %d  ######", this->cur_task->getTaskHasBallState());
            ROS_WARN("####  getActionState = %d  ######", this->cur_task->getActionState());

            if (this->cur_task->getActionState() == GOT_GOAL and this->cur_task->getTaskHasBallState() == 2)
            {

                ROS_WARN("HasBall got goal ");
                this->cur_task->sendToTaskBall(0);
                cur_task_state = CATCH_BALL_START_CATCH;

            }
            else if (this->cur_task->getActionState() == FAILED_TO_GOAL)
            {

                ROS_WARN("HasBall failed to goal");
                cur_task_state = NO_BALL_START;

            }

            if (this->cur_task->getBallPose().pose.pose.position.x < 0)
            {

                ROS_INFO("From HasBall to NoBall!");
                cur_task_state = NO_BALL_START;

            }

            ROS_WARN("HasBall processing!");
            this->cur_task->taskHasBall();

        }
        else if (cur_task_state == CATCH_BALL_START_CATCH)
        {

            this->cur_task->taskCatchBall(true);

            static JudgeAchieveCounter judge_achieve_counter(10);

            if (judge_achieve_counter.isAchieve())
            {

                if (this->cur_task->getBallState().data != 0)
                {

                    cur_task_state = PUT_BALL_START;

                }
                else
                {

                    this->cur_task->taskCatchBall(false);
                    static JudgeAchieveCounter judge_achieve_counter2(10);
                    if (judge_achieve_counter2.isAchieve())
                    {
                        cur_task_state = NO_BALL_START;
                    }

                }

            }

        }
        else if (cur_task_state == CATCH_BALL_START_PUT)
        {

            this->cur_task->taskCatchBall(false);

            static JudgeAchieveCounter judge_achieve_counter(20);

            if (judge_achieve_counter.isAchieve())
            {

                cur_task_state = CHANGE_START;

            }

        }
        else if (cur_task_state == PUT_BALL_START)
        {

            if (this->cur_task->getActionState() == GOT_GOAL)
            {

                cur_task_state = CATCH_BALL_START_PUT;

            }
            else if (this->cur_task->getActionState() == FAILED_TO_GOAL)
            {

                cur_task_state = CATCH_BALL_START_PUT;
                //TODO
            }

            this->cur_task->taskPutBall();

        }
        else if (cur_task_state == CHANGE_START)
        {
            if (this->cur_task->getActionState() == GOT_GOAL)
            {
                cur_task_state = NO_BALL_START;
            }
            else if (this->cur_task->getActionState() == FAILED_TO_GOAL)
            {
                cur_task_state = NO_BALL_START;
            }

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





