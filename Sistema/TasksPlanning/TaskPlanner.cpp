#include "TaskPlanner.h"
#include "../PathPlanning/DStartLite.h"
#include <thread>
#include <iostream>
#include <set>

TaskPlanner::TaskPlanner(int8_t* map, uint32_t mapWidth, uint32_t mapHeight)
{
    this->map = map;
    this->mapWidth = mapWidth;
    this->mapHeight = mapHeight;
}

TaskPlanner::~TaskPlanner()
{
}

void TaskPlanner::SetCurrentPosition(VertexPosition currentPosition)
{
    mtx.lock();
    this->currentPosition = currentPosition;
    positionChanged = true;
    mtx.unlock();
}

void TaskPlanner::Add(Task t)
{
    mtx.lock();
    if (t.id >= 0)
    {
        if (taskHash.find(t.id) == taskHash.end())
        {
            TaskData& tD = taskHash[t.id];
            tD.places = t.places;
            tD.priority = t.priority;
            tD.description = t.description;
            taskHashHasChanged = true;
        }
    }
    mtx.unlock();
}

void TaskPlanner::Remove(int id)
{
    mtx.lock();
    if (id >= 0)
    {
        auto it = taskHash.find(id);
        if (it != taskHash.end())
        {
            taskHash[it->first].toRemove = true;
            taskHashHasChanged = true;
        }
    }
    mtx.unlock();
}

bool TaskPlanner::GetCurrentTask(Task & t)
{
    mtx.lock();
    if (taskHash.size() > 0)
    {
        TaskData& newTask = taskHash[taskQueue.begin()->second];
        currentTask.id = taskQueue.begin()->second;
        currentTask.priority = newTask.priority;
        currentTask.cost = taskQueue.begin()->first;
        currentTask.places = newTask.places;
        currentTask.description = newTask.description;
        t = currentTask;
        mtx.unlock();
        return true;
    }
    mtx.unlock();
    return false;
}

bool TaskPlanner::TaskHasChanged()
{
    mtx.lock();
    if (taskHash.size() > 0)
    {
        if (currentTask.id != taskQueue.begin()->second)
        {
            mtx.unlock();
            return true;
        }
        mtx.unlock();
        return false;
    }
    mtx.unlock();
    return false;
}

bool TaskPlanner::GetCurrentPlace(TaskPlace & p)
{
    mtx.lock();
    if (taskHash.size() == 0)
    {
        taskQueue.clear();
        mtx.unlock();
        return false;
    }

    if (taskHashHasChanged || positionChanged)
    {
        SortTasks();
        taskHashHasChanged = false;
        positionChanged = false;
    }

    if (taskHash.size() == 0)
    {
        taskQueue.clear();
        mtx.unlock();
        return false;
    }

    TaskData* tD = &taskHash[taskQueue.begin()->second];
    p = tD->places.front();

    mtx.unlock();
    return true;
}

void TaskPlanner::PopCurrentPlace()
{
    mtx.lock();
    if (taskHash.size() > 0)
    {
        auto it = taskHash.find(taskQueue.begin()->second);
        if (it != taskHash.end())
        {
            TaskData* tD = &taskHash[it->first];
            currentPosition = tD->places.front().position;
            tD->places.erase(tD->places.begin());

            if ((tD->places.size() == 0) || (tD->toRemove))
            {
                taskHash.erase(taskQueue.begin()->second);
                taskQueue.erase(taskQueue.begin());
            }

            if (taskHash.size() == 0)
            {
                currentTask = Task();
            }
        }
        /*TaskData* tD = &taskHash[taskQueue.begin()->second];
        currentPosition = tD->places.front().position;
        tD->places.erase(tD->places.begin());

        if (tD->places.size() == 0)
        {
            taskHash.erase(taskQueue.begin()->second);
            taskQueue.erase(taskQueue.begin());
        }
        
        if (taskHash.size() == 0)
        {
            currentTask = Task();
        }*/
    }
    mtx.unlock();
}

void TaskPlanner::SortTasks()
{
#ifdef STATISTICS 
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
#endif
    failedTasks.clear();
    std::set<int> taskNotAdded;
    std::set<int> toRemove;
    for (auto i : taskHash)
    {
        if (i.second.toRemove)
        {
            toRemove.insert(i.first);
        }
        else
        {
            taskNotAdded.insert(i.first);
        }
    }

    for (auto i : toRemove)
    {
        auto it = taskHash.find(i);
        if (it != taskHash.end())
            taskHash.erase(it);
    }

    VertexPosition last = currentPosition;

    taskQueue.clear();
    while (!taskNotAdded.empty())
    {
        double minCost = INFINITY_CONST;
        int minId;
#ifdef STATISTICS 
        std::cout << "-------" << std::endl;
#endif
        for (auto i : taskNotAdded)
        {
            TaskData& t = taskHash[i];
            ComputeTotalCost(t, last);
#ifdef STATISTICS 
            std::cout << i << "\t" << t.priority << "\t" << t.cost * t.priority << "\t" << t.cost << std::endl;
#endif
            if (t.cost <= minCost)
            {
                minCost = t.cost;
                minId = i;
            }
        }

        if (minCost != INFINITY_CONST)
        {
            last = taskHash[minId].places.back().position;
            taskQueue.push_back(std::make_pair(minCost, minId));
        }
        else
        {
            Task tmp;
            tmp.id = minId;
            tmp.places = taskHash[minId].places;
            tmp.priority = taskHash[minId].priority;
            tmp.cost = INFINITY_CONST;
            failedTasks.push_back(tmp);
        }
        taskNotAdded.erase(minId);
    }
#ifdef STATISTICS 
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "\n\nTime (Milliseconds)= " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << std::endl;
    std::cout << "Time (Microseconds)= " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "\n" << std::endl;
#endif
}

void TaskPlanner::ComputeTotalCost(TaskData& t, VertexPosition initialPosition)
{
    //When I have to compute all
    if ((t.distances.size() == 0) && (t.places.size() > 1))
    {
        std::thread th[t.places.size()];
        double distances[t.places.size()];

        VertexPosition last = initialPosition;
        for (int i = 0; i < t.places.size(); i++)
        {
            th[i] = std::thread(&TaskPlanner::ComputeDistance, this, map, mapWidth, mapHeight, last, t.places[i].position, std::ref(distances[i]));
            last = t.places[i].position;
        }

        t.distances.clear();
        for (int i = 0; i < t.places.size(); i++)
        {
            th[i].join();
            t.distances.push_back(distances[i]);
        }
    }
    else //When i have to update just the first step
    {
        while (t.distances.size() > t.places.size()) //When the position was reached
            t.distances.erase(t.distances.begin());
        ComputeDistance(map, mapWidth, mapHeight, initialPosition, t.places[0].position, std::ref(t.distances[0]));
    }

    t.cost = 0;
    for (int i = 0; i < t.distances.size(); i++)
        t.cost += t.distances[i] / (double) t.priority;
}

void TaskPlanner::ComputeDistance(int8_t* map, uint32_t mapWidth, uint32_t mapHeight, VertexPosition start, VertexPosition goal, double& cost)
{
    DStartLite d;
    d.MountTheMap(map, mapWidth, mapHeight);
    d.Initialize(start, goal);
    cost = d.TotalCost();
//    std::cout << "Start: (" << start.x << ", " << start.y << ")" << std::endl;
//    std::cout << "Goal: (" << goal.x << ", " << goal.y << ")" << std::endl;
//    std::cout << "Cost: " << cost << std::endl;
}

FailedTasks TaskPlanner::GetFailedTasks()
{
    return failedTasks;
}

void TaskPlanner::ClearFailedTAsks()
{
    failedTasks.clear();
}