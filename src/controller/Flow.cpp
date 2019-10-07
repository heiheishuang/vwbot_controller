//
// Created by heihei on 2019/7/29.
//

#include "Flow.h"


vwpp::FlowController::FlowController() :
        cur_flow_state(FLOW_START),
        cur_task_state(NO_BALL)
{
    this->task_change_watch_tower = new TaskChangeWatchTower;
    this->task_put_ball = new TaskPutBall;
    this->task_no_ball = new TaskNoBall;
    this->task_has_ball = new TaskHasBall;
}


vwpp::FlowController::~FlowController()
{
    delete this->task_has_ball;
    delete this->task_no_ball;
    delete this->task_put_ball;
    delete this->task_change_watch_tower;
}


vwpp::FlowState vwpp::FlowController::getFlowState()
{
    return cur_flow_state;
}


void vwpp::FlowController::run()
{

    if (cur_flow_state == FLOW_START)
    {

        ROS_INFO("Vwbot Pose x = %lf y = %lf", WorldInterface::getInstance()->getVwbotPose().pose.position.x,
                 WorldInterface::getInstance()->getVwbotPose().pose.position.x);
        ROS_INFO("CountPoseChange is %d ", WorldInterface::getInstance()->getCountPoseChange());

        if (cur_task_state == HAS_BALL_START)
        {

            std::cout << CYAN << "Now cur_task_state is HAS_BALL_START !" << "\033[0m"
                      << std::endl;
            this->task_has_ball->taskHasBall();


            //Special ball
            if (VisionInterface::getInstance()->getBallState().data != 0 and WorldInterface::getInstance()->getVwbotPose().pose.position.x <= 2.8)
            {

                std::cout << RED << "Now the color is " << VisionInterface::getInstance()->getBallState().color << "\033[0m"
                          << std::endl;

                ROS_ERROR("SPECIALLY ! Now cur_task_state is from HAS_BALL_START to CATCH_BALL_START_CATCH!");
                this->task_has_ball->setTaskHasBallState(0);  //change the task_has_ball_state
                cur_task_state = CATCH_BALL_START_CATCH;

            }

            if (this->task_has_ball->getDistanceToBall() <= ParamInterface::getInstance()->getDisLengthWhenCatch()
                and this->task_has_ball->getTaskHasBallState() == 2)
            {

                std::cout << GREEN << "!!!!!   getLengthBetweenBallAndVwbot()<=DIS_LENGTH_WHEN_CATCH " << "\033[0m"
                          << std::endl;
                std::cout << GREEN << " DIS_LENGTH_WHEN_CATCH = " << this->task_has_ball->getDistanceToBall()
                          << "\033[0m" << std::endl;

                this->task_has_ball->setTaskHasBallState(0);

                cur_task_state = CATCH_BALL_START_CATCH;
                VisionInterface::getInstance()->send_delete(true);

            }

            if (VisionInterface::getInstance()->getBallPose().pose.pose.position.x < 0)
            {

                ROS_INFO("Now cur_task_state is from HAS_BALL_START to NO_BALL_START!");
                this->task_has_ball->setTaskHasBallState(0);
                cur_task_state = NO_BALL;

            }

            if (WorldInterface::getInstance()->getCountPoseChange() >= 10)
            {

                ROS_ERROR("Delete the point");
                VisionInterface::getInstance()->send_delete(true);
                this->task_has_ball->setTaskHasBallState(0);
                cur_task_state = NO_BALL;
            }

        }
        if (cur_task_state == NO_BALL)
        {

            std::cout << CYAN << "Now cur_task_state is NO_BALL !" << "\033[0m" << std::endl;
            this->task_no_ball->taskNOBall();

            if (VisionInterface::getInstance()->getBallState().data != 0 and WorldInterface::getInstance()->getVwbotPose().pose.position.x <= 2.8)
            {

                std::cout << RED << "Now the color is " << VisionInterface::getInstance()->getBallState().color << "\033[0m"
                          << std::endl;

                ROS_ERROR("SPECIALLY ! Now cur_task_state is from HAS_BALL_START to CATCH_BALL_START_CATCH!");
                this->task_no_ball->setTaskNoBallState(0);
                cur_task_state = CATCH_BALL_START_CATCH;

            }

            if (VisionInterface::getInstance()->getBallPose().pose.pose.position.x > 0)
            {

                ROS_INFO("Now cur_task_state is from NO_BALL_START to HAS_BALL_START!");
                this->task_no_ball->setTaskNoBallState(0);
                cur_task_state = HAS_BALL_START;

            }

            if (this->task_no_ball->getTaskRunState() == TASK_FINSH )
            {
                ROS_INFO("Now taskNOBall Failed ! Now cur_task_state is from NO_BALL_START to CHANGE_START!");
                this->task_no_ball->setTaskNoBallState(0);
                cur_task_state = CHANGE_START;
            }

        }
        else if (cur_task_state == CATCH_BALL_START_CATCH)
        {

            std::cout << CYAN << "Now cur_task_state is CATCH_BALL_START_CATCH !" << "\033[0m"
                      << std::endl;

            VwbotInterface::getInstance()->send_to_hand(true);

            static JudgeAchieveCounter judge_achieve_counter(1);

            if (judge_achieve_counter.isAchieve())
            {

                if (VisionInterface::getInstance()->getBallState().data != 0)
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

            std::cout << CYAN << "Now cur_task_state is CATCH_BALL_START_PUT !" << "\033[0m"
                      << std::endl;

            VwbotInterface::getInstance()->send_to_hand(false);

            static JudgeAchieveCounter judge_achieve_counter(1);

            if (judge_achieve_counter.isAchieve())
            {

                ROS_INFO("Now cur_task_state is from CATCH_BALL_START_PUT to CHANGE_START!");

                cur_task_state = CHANGE_START;

            }

        }
        else if (cur_task_state == PUT_BALL_START)
        {

            std::cout << CYAN << "Now cur_task_state is PUT_BALL_START !" << "\033[0m"
                      << std::endl;

            if (VwbotInterface::getInstance()->getMoveBaseState() == GOT_GOAL)
            {
                ROS_INFO("Now cur_task_state is from PUT_BALL_START to CATCH_BALL_START_PUT! Action is GOT_GOAL!");

                cur_task_state = CATCH_BALL_START_PUT;
            }
            else if (VwbotInterface::getInstance()->getMoveBaseState() == FAILED_TO_GOAL)
            {

                ROS_INFO(
                        "Now cur_task_state is from PUT_BALL_START to CATCH_BALL_START_PUT! Action is FAILED_TO_GOAL!");
                cur_task_state = CATCH_BALL_START_PUT;
            }


            if (WorldInterface::getInstance()->getVwbotPose().pose.position.x > 3.1 and
                fabs(WorldInterface::getInstance()->getVwbotPose().pose.position.y -
                     this->task_put_ball->getAimPutBallPoint().pose.position.y) <= 0.05)
            {
                VwbotInterface::getInstance()->send_cancel();
                cur_task_state = CATCH_BALL_START_PUT;
            }

            this->task_put_ball->goToPutBall();

        }
        else if (cur_task_state == CHANGE_START)
        {

            std::cout << CYAN << "Now cur_task_state is  CHANGE_START!" << "\033[0m"
                      << std::endl;

            this->task_change_watch_tower->taskChangeWatchTower();

            if (VisionInterface::getInstance()->getBallPose().pose.pose.position.x > 0 and
                VisionInterface::getInstance()->getBallPose().pose.pose.position.x < 2.8)
            {

                ROS_INFO("Now cur_task_state is from CHANGE_START to HAS_BALL_START!");
                cur_task_state = HAS_BALL_START;
            }
            else if (VwbotInterface::getInstance()->getMoveBaseState() == GOT_GOAL)
            {
                ROS_INFO("Now cur_task_state is from CHANGE_START to NO_BALL!");

                int change_state;
                change_state = this->task_change_watch_tower->getTaskChangePointState() + 1;
                change_state = change_state % 3;
                this->task_change_watch_tower->setTaskChangePointState(change_state);   //Change the static point

                cur_task_state = NO_BALL;
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





