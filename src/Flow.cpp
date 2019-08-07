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

            ROS_INFO("Now cur_task_state is NO_BALL_START !");

            this->cur_task->taskNoBall();

            if (this->cur_task->getBallPose().pose.pose.position.x > 0)
            {

                ROS_INFO("Now cur_task_state is from NO_BALL_START to HAS_BALL_START!");
                this->cur_task->sendToTaskNoBall(0);
                cur_task_state = HAS_BALL_START;
                this->cur_task->initBallOrientation();

            }
            else if (this->cur_task->getActionState() == GOT_GOAL and this->cur_task->getTaskNoBallState() == 2)
            {
                ROS_INFO("Now taskNOBall Failed ! Now cur_task_state is from NO_BALL_START to CHANGE_START!");
                this->cur_task->sendToTaskNoBall(0);
                cur_task_state = CHANGE_START;
            //     Need to change the CHANGE_START number
            //     TODO
            }

        }
        else if (cur_task_state == HAS_BALL_START)
        {

            ROS_INFO("Now cur_task_state is HAS_BALL_START !");

            ROS_WARN("Now HAS_BALL_START is processing!");
            this->cur_task->taskHasBall();

            // if (this->cur_task->getActionState() == GOT_GOAL and this->cur_task->getTaskHasBallState() == 2)
            // {
            if (this->cur_task->getBallState().data != 0)
            {

                std::cout << RED << "Now the color is " << this->cur_task->getBallState().color << "\033[0m"
                          << std::endl;

                this->cur_task->sendToColor(this->cur_task->getBallState().color);

                ROS_ERROR("SPECIALLY ! Now cur_task_state is from HAS_BALL_START to CATCH_BALL_START_CATCH!");
                this->cur_task->sendToTaskHasBall(0);  //change the task_has_ball_state
                cur_task_state = CATCH_BALL_START_CATCH;

            }
            else if (this->cur_task->getLengthBetweenBallAndVwbot() <= DIS_LENGTH_WHEN_CATCH)
                // TODO
                //New in 8.6
                //else if (this->cur_task->getLengthBetweenBallAndVwbot() <= 0.05 and this->cur_task->getTaskHasBallState() == 2)
            {
                ROS_INFO("Now cur_task_state is from HAS_BALL_START to CATCH_BALL_START_CATCH! getLengthBetweenBallAndVwbot()<=DIS_LENGTH_WHEN_CATCH");
                this->cur_task->sendToTaskHasBall(0);
                cur_task_state = CATCH_BALL_START_CATCH;

            }

            else if (this->cur_task->getActionState() == GOT_GOAL and this->cur_task->getTaskHasBallState() == 2)
            {

                ROS_INFO("Now GOT_GOAL ! Now cur_task_state is from HAS_BALL_START to CATCH_BALL_START_CATCH!");
                this->cur_task->sendToTaskHasBall(0);
                cur_task_state = CATCH_BALL_START_CATCH;

            }
            else if (this->cur_task->getActionState() == FAILED_TO_GOAL)
            {

                ROS_INFO("Now cur_task_state is from HAS_BALL_START to NO_BALL_START!");
                this->cur_task->sendToTaskHasBall(0);
                cur_task_state = NO_BALL_START;

            }
            else if (this->cur_task->getTaskHasBallState() == 3 and
                    (this->cur_task->getLengthBetweenBallAndVwbot() >= 0.30 or this->cur_task->getYawBetweenBallAndVwbot() / 3.14 * 180 >= 15 ))
            {
                ROS_INFO( "Now cur_task_state is from HAS_BALL_START to NO_BALL_START");
                this->cur_task->sendToTaskHasBall(0);
                cur_task_state = NO_BALL_START;
            }

            ROS_ERROR("######## %lf  #####",this->cur_task->getBallPose().pose.pose.position.x);

            if (this->cur_task->getBallPose().pose.pose.position.x < 0)
            {

                ROS_INFO("Now cur_task_state is from HAS_BALL_START to NO_BALL_START!");
                this->cur_task->sendToTaskHasBall(0);
                cur_task_state = NO_BALL_START;

            }


        }
        else if (cur_task_state == CATCH_BALL_START_CATCH)
        {

            ROS_INFO("Now cur_task_state is CATCH_BALL_START_CATCH !");

            this->cur_task->taskCatchBall(true);

            static JudgeAchieveCounter judge_achieve_counter(WAIT_TIME);

            if (judge_achieve_counter.isAchieve())
            {

                if (this->cur_task->getBallState().data != 0)
                {

                    ROS_INFO("Now cur_task_state is from CATCH_BALL_START_CATCH to PUT_BALL_START!");
                    cur_task_state = PUT_BALL_START;

                }
                else
                {

                    ROS_INFO("Now cur_task_state is from CATCH_BALL_START_CATCH to CATCH_BALL_START_PUT!");
                    cur_task_state = CATCH_BALL_START_PUT;

                }

            }

        }
        else if (cur_task_state == CATCH_BALL_START_PUT)
        {

            ROS_INFO("Now cur_task_state is CATCH_BALL_START_PUT !");

            this->cur_task->taskCatchBall(false);

            static JudgeAchieveCounter judge_achieve_counter(WAIT_TIME);

            if (judge_achieve_counter.isAchieve())
            {

                ROS_INFO("Now cur_task_state is from CATCH_BALL_START_PUT to CHANGE_START!");

                cur_task_state = CHANGE_START;

            }

        }
        else if (cur_task_state == PUT_BALL_START)
        {

            ROS_INFO("Now cur_task_state is PUT_BALL_START !");
            this->cur_task->taskPutBall();


            if (this->cur_task->getActionState() == GOT_GOAL)
            {
                ROS_INFO("Now cur_task_state is from PUT_BALL_START to CATCH_BALL_START_PUT! Action is GOT_GOAL!");

                cur_task_state = CATCH_BALL_START_PUT;

            }
            else if (this->cur_task->getActionState() == FAILED_TO_GOAL)
            {

                ROS_INFO(
                        "Now cur_task_state is from PUT_BALL_START to CATCH_BALL_START_PUT! Action is FAILED_TO_GOAL!");
                cur_task_state = CATCH_BALL_START_PUT;
                //TODO
            }


        }
        else if (cur_task_state == CHANGE_START)
        {
            ROS_INFO("Now cur_task_state is CHANGE_START !");

            ROS_INFO("Now CHANGE_START is starting!");


            this->cur_task->taskChange();

            if (this->cur_task->getBallState().data != 0 and
                fabs(this->cur_task->getVwbotPose().pose.position.x - 3.10) >= 0.30)
            {

                ROS_ERROR("SPECIALLY ! Now cur_task_state is from HAS_BALL_START to CATCH_BALL_START_CATCH!");
                std::cout << RED << "Now the color is " << this->cur_task->getBallState().color << "\033[0m"
                          << std::endl;
                this->cur_task->sendToColor(this->cur_task->getBallState().color);
                cur_task_state = CATCH_BALL_START_CATCH;

            }
            else if (this->cur_task->getBallPose().pose.pose.position.x > 0 and
                    this->cur_task->getBallPose().pose.pose.position.x < 2.8)
            {

                ROS_INFO("Now cur_task_state is from CHANGE_START to HAS_BALL_START!");
                cur_task_state = HAS_BALL_START;
                this->cur_task->initBallOrientation();

            }
            else if (this->cur_task->getActionState() == GOT_GOAL)
            {
                ROS_INFO("Now cur_task_state is from CHANGE_START to NO_BALL_START!");

                int change_state;
                change_state = this->cur_task->getTaskChangeState() + 1;
                change_state = change_state % 3;
                this->cur_task->sendToTaskChange(change_state);   //Change the static point

                cur_task_state = NO_BALL_START;
            }
            else if (this->cur_task->getActionState() == FAILED_TO_GOAL)
            {
                ROS_INFO("Now cur_task_state is from CHANGE_START to NO_BALL_START!");

                int change_state;
                change_state = this->cur_task->getTaskChangeState() + 1;
                change_state = change_state % 3;
                this->cur_task->sendToTaskChange(change_state);   //Change the static point

                cur_task_state = NO_BALL_START;
            }

        }


    }
    else if (cur_flow_state == FLOW_FINSH)
    {
        //TODO
        //Need cur_flow_state = FLOW_FINSH
        //When all the ball is over
    }
}





