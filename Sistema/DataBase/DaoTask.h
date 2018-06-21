/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DaoTask.h
 * Author: x
 *
 * Created on 11 de Abril de 2018, 13:40
 */

#ifndef DAOTASK_H
#define DAOTASK_H

#include "DaoGeneral.h"
#include "../TasksPlanning/TaskPlanningDefines.h"

class DaoTask
{
public:
    DaoTask(DaoGeneral* daoGeneral, uint32_t idMap);
    DaoTask(const DaoTask& orig);
    
    void SetIdMap(uint32_t idMap);

    bool InsertTask(std::string description, int priority, std::vector<uint32_t> places);
    bool UpdateStatus(int idTask, std::string status);
    bool UpdateStatusPlace(int idTask, uint32_t seqNumber, std::string status);
    bool Delete(int idTask);

    std::vector<Task> GetTasksByStatus(std::string status);
    std::vector<Task> GetPendingTasks();
    std::vector<int> GetRemovedTasks();
    virtual ~DaoTask();

    static const std::string REMOVE;
    static const std::string ERASED;
    static const std::string WAITING;
    static const std::string OPENNED;
    static const std::string PERFORMING;
    static const std::string DONE;
    static const std::string FAIL;

private:
    DaoGeneral* daoGenaral;
    uint32_t idMap;
};

#endif /* DAOTASK_H */

