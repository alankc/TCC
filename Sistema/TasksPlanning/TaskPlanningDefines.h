#ifndef TASKPLANNINGDEFINES_H
#define TASKPLANNINGDEFINES_H

#include "../PathPlanning/PathPlanningDefines.h"
#include <vector>

struct TaskPlace
{  
    uint32_t seqNumber;
    std::string description;
    VertexPosition position;
};

struct Task
{
    int id;
    std::vector<TaskPlace> places;
    int priority;
    double cost;
    std::string description;
    
    Task() : id(-1), priority(-1), cost(-1)
    {
    }

    bool operator==(const Task &t) const
    {
        return id == t.id;
    }
};

struct TaskData
{
    std::vector<TaskPlace> places;
    std::vector<double> distances;

    int priority;
    double cost;

    std::string description;
    
    bool toRemove;
    
    TaskData() : priority(-1), cost(-1), toRemove(false)
    {
    }
};

#endif /* TASKPLANNINGDEFINES_H */

