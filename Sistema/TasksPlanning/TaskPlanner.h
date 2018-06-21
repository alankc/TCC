#ifndef TASKPLANNER_H
#define TASKPLANNER_H

#include <queue>
#include <map>
#include <mutex>
#include "TaskPlanningDefines.h"

typedef std::unordered_map<int, TaskData> TaskHash;
typedef std::vector<std::pair<double, int>> TaskQueue;
typedef std::vector<Task> FailedTasks;

class TaskPlanner
{
public:

    TaskPlanner(int8_t* map, uint32_t mapWidth, uint32_t mapHeight);
    virtual ~TaskPlanner();

    void SetCurrentPosition(VertexPosition currentPosition);

    void Add(Task t);
    void Remove(int id);
    Task Pop();

    bool GetCurrentTask(Task& t);
    bool GetCurrentPlace(TaskPlace& p);
    void PopCurrentPlace();
    bool TaskHasChanged();
    
    FailedTasks GetFailedTasks();
    void ClearFailedTAsks();
private:
    int8_t* map;
    uint32_t mapWidth;
    uint32_t mapHeight;

    TaskHash taskHash;
    TaskQueue taskQueue;
    FailedTasks failedTasks;
    bool taskHashHasChanged;
    std::mutex mtx; 

    Task currentTask;
    VertexPosition currentPosition;
    bool positionChanged;

    void SortTasks();
    void ComputeTotalCost(TaskData& t, VertexPosition initialPosition);
    void ComputeDistance(int8_t* map, uint32_t mapWidth, uint32_t mapHeight, VertexPosition start, VertexPosition goal, double& cost);
};

#endif /* TASKPLANNER_H */

