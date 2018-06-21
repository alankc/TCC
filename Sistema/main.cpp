/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: x
 *
 * Created on 13 de Março de 2018, 14:34
 */

#ifndef BOOST_SIGNALS_NO_DEPRECATION_WARNING
#define BOOST_SIGNALS_NO_DEPRECATION_WARNING
#endif

#include <libplayerc++/playerc++.h>
#include "Auxiliar/MapResizer.h"
#include "PathPlanning/DStartLite.h"
#include "TasksPlanning/TaskPlanner.h"
#include "DataBase/DaoGeneral.h"
#include "DataBase/DaoTask.h"

#include <thread>
#include <mutex>
#include <limits>

bool notNear(player_point_2d_t current, player_point_2d_t dest, double dist = 1.00)
{
    if (std::sqrt(std::pow(current.px - dest.px, 2) + std::pow(current.py - dest.py, 2)) > dist)
        return true;

    return false;
}

void VerifyAdd(DaoTask& daoTask, TaskPlanner& taskPlanner)
{
    //while (true)
    {
        auto tasks = daoTask.GetPendingTasks();
        for (auto t : tasks)
        {
            daoTask.UpdateStatus(t.id, DaoTask::OPENNED);
            uint32_t i = 0;
            for (auto p : t.places)
            {
                daoTask.UpdateStatusPlace(t.id, i++, DaoTask::OPENNED);
            }
            taskPlanner.Add(t);
        }
        //sleep(10);
    }
}

void VerifyRemove(DaoTask& daoTask, TaskPlanner& taskPlanner)
{
    //while (true)
    {
        auto tasks = daoTask.GetTasksByStatus(DaoTask::REMOVE);
        for (auto t : tasks)
        {
            taskPlanner.Remove(t.id);
            daoTask.UpdateStatus(t.id, DaoTask::ERASED);
            uint32_t i = 0;
            for (auto p : t.places)
            {
                daoTask.UpdateStatusPlace(t.id, i++, DaoTask::ERASED);
            }
        }
        //sleep(10);
    }
}

void VerifyFailed(DaoTask& daoTask, TaskPlanner& taskPlanner)
{
    auto tasks = taskPlanner.GetFailedTasks();
    for (auto t : tasks)
    {
        daoTask.UpdateStatus(t.id, DaoTask::FAIL);
        uint32_t i = 0;
        for (auto p : t.places)
        {
            daoTask.UpdateStatusPlace(t.id, i++, DaoTask::FAIL);
        }
    }
    taskPlanner.ClearFailedTAsks();
}

int main(int argc, char** argv)
{
    DaoGeneral tst("tcp://127.0.0.1:3306", "robot_database", "root", "");
    DaoTask dbT(&tst, 1);

    std::cout << "Connecting to robot" << std::endl;
    PlayerCc::PlayerClient r("localhost");

    //This two lines make the player send data just when it's requested
    r.SetDataMode(PLAYER_DATAMODE_PULL);
    r.SetReplaceRule(true, PLAYER_MSGTYPE_DATA, -1, -1);

    PlayerCc::MapProxy m(&r, 0);
    PlayerCc::Position2dProxy pControl(&r, 2);
    PlayerCc::Position2dProxy pAMCL(&r, 1);
    PlayerCc::Position2dProxy pRobot(&r, 0);
    
    pRobot.SetMotorEnable(true);
    
    r.Read();

    std::cout << "Reading Map" << std::endl;
    m.RequestMap();
    int8_t* aMap = new int8_t[m.GetHeight() * m.GetWidth()];
    m.GetMap(aMap);

    std::cout << "Map Resizing" << std::endl;
    MapResizer mR;
    mR.Resize(aMap, m.GetWidth(), m.GetHeight(), 35, 10);
    mR.SetBegin(-16.66, -30.51);

    std::cout << "Mount DStartLite" << std::endl;
    DStartLite pP;
    pP.MountTheMap(mR.GetResizedMap(), mR.GetResizedWidth(), mR.GetResizedHeigth());

    std::cout << "Init TaskPlanner" << std::endl;
    TaskPlanner tP(mR.GetResizedMap(), mR.GetResizedWidth(), mR.GetResizedHeigth());

    player_point_2d_t point;
    r.Read();
    point.px = pControl.GetXPos();
    point.py = pControl.GetYPos();

    tP.SetCurrentPosition(mR.RealToResized(point));

    //std::thread t_add(VerifyAdd, std::ref(dbT), std::ref(tP));
    //std::thread t_remove(VerifyRemove, std::ref(dbT), std::ref(tP));

    VertexPosition current;
    VertexPosition next;
    TaskPlace place;
    uint32_t count = 0;
    while (true)
    {
        //std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        r.Read();
        std::cout << "loop " << count++ << std::endl;

        VerifyFailed(dbT, tP);
        VerifyAdd(dbT, tP);
        VerifyRemove(dbT, tP);

        Task t;
        while (tP.GetCurrentPlace(place))
        {
            VerifyFailed(dbT, tP);
            next = place.position;
            if (tP.TaskHasChanged())
            {
                //When exists more than one task into the list
                if (t.id != -1)
                    dbT.UpdateStatus(t.id, DaoTask::DONE);

                if (tP.GetCurrentTask(t))
                {
                    dbT.UpdateStatus(t.id, DaoTask::PERFORMING);
                    std::cout << "Task        : " << t.id << std::endl;
                    std::cout << "Priority    : " << t.priority << std::endl;
                    std::cout << "Description : " << t.description << std::endl;
                }
                else
                    break;
            }

            std::cout << "Going to: " << place.description << std::endl;
            dbT.UpdateStatusPlace(t.id, place.seqNumber, DaoTask::PERFORMING);

            r.Read();

            point.px = pControl.GetXPos();
            point.py = pControl.GetYPos();
            current = mR.RealToResized(point);

            pP.Initialize(current, next);

            //Nova versão
            VertexPosition tmpCurr, tmpPrev;
            tmpCurr.x = tmpCurr.y = 0;
            tmpPrev = tmpCurr;
            std::vector<VertexUpdate> up;
            while (pP.GetNextWayPoint(tmpCurr, up))
            {
                player_point_2d_t pointTmpCurr, pointTmpPrev;
                pointTmpCurr = mR.ResizedToReal(tmpCurr);
                pointTmpPrev = mR.ResizedToReal(tmpPrev);

               // if (!notNear(pointTmpCurr, pointTmpPrev, 0.25)) continue;

                tmpPrev = tmpCurr;

                pControl.GoTo(pointTmpCurr.px, pointTmpCurr.py, 0);

                r.Read();
                point.px = pControl.GetXPos();
                point.py = pControl.GetYPos();
                std::cout << pointTmpCurr << std::endl;
                while (notNear(point, pointTmpCurr))
                {
                    r.Read();
                    point.px = pControl.GetXPos();
                    point.py = pControl.GetYPos();
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                }
            }
            //std::cin.get();
            //r.Read();
            //std::cout << "Distance: " << std::sqrt(std::pow(mR.ResizedToReal(place.position).px - pSimulation.GetXPos(), 2) + std::pow(mR.ResizedToReal(place.position).py - pSimulation.GetYPos(), 2)) << "\n" << std::endl;
            //Fim nova versão

            //Versão Original
            /*            VertexPosition tmp;
                        std::vector<VertexUpdate> up;
                        while (pP.GetNext(tmp, up))
                        {
                            player_point_2d_t pointTmp;
                            pointTmp = mR.ResizedToReal(tmp);
                            p.GoTo(pointTmp.px, pointTmp.py, 0);

                            r.Read();
                            point.px = p.GetXPos();
                            point.py = p.GetYPos();
                            while (notNear(point, pointTmp))
                            {
                                r.Read();
                                point.px = p.GetXPos();
                                point.py = p.GetYPos();
                            }
                        }
             */
            tP.PopCurrentPlace();
            dbT.UpdateStatusPlace(t.id, place.seqNumber, DaoTask::DONE);

            //When exists just one task into the list
            if (!tP.GetCurrentPlace(place))
                dbT.UpdateStatus(t.id, DaoTask::DONE);

            VerifyAdd(dbT, tP);
            VerifyRemove(dbT, tP);
            
            std::cout << "Press enter to continue!" << std::flush;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        }
        sleep(1);

        //std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        //std::cout << "Time (seconds)= " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << std::endl;
        //std::cout << "Time (minutes)= " << std::chrono::duration_cast<std::chrono::minutes>(end - begin).count() << std::endl;
    }
    return EXIT_SUCCESS;
}

