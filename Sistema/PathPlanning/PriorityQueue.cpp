#include "PriorityQueue.h"

PriorityQueue::PriorityQueue()
{
}

PriorityQueue::~PriorityQueue()
{
}

void PriorityQueue::Insert(VertexPosition vP, Key k)
{
    HashQueue::iterator it = hashQueue.find(vP);
    if (it == hashQueue.end())
    {
        priorityQueue.insert(std::make_pair(k, vP));
        hashQueue[vP] = k;
    }
}

void PriorityQueue::Update(VertexPosition vP, Key k)
{
    HashQueue::iterator it = hashQueue.find(vP);
    if (it != hashQueue.end())
        it->second = k;
}

void PriorityQueue::Remove(VertexPosition vP)
{
    auto it = hashQueue.find(vP);
    if (it != hashQueue.end())
    {
        hashQueue.erase(it);
        for (auto i : priorityQueue)
        {
            if (i.second == vP)
            {
                priorityQueue.erase(i.first);
                break;
            }
        }
    }
}

KeyQueue PriorityQueue::Pop()
{
    KeyQueue kQ;

    if (hashQueue.empty() || priorityQueue.empty())
    {
        kQ.key.first = INFINITY_CONST;
        kQ.key.second = INFINITY_CONST;
        kQ.vertexPosition.x = -1;
        kQ.vertexPosition.y = -1;
    }
    else
    {
        auto it = priorityQueue.cbegin();

        kQ.key = hashQueue[it->second];
        kQ.vertexPosition = it->second;

        priorityQueue.erase(it);
        hashQueue.erase(kQ.vertexPosition);
    }

    return kQ;
}

bool PriorityQueue::Contains(VertexPosition vP)
{
    auto it = hashQueue.find(vP);
    return (it != hashQueue.end());
}

KeyQueue PriorityQueue::Top()
{
    KeyQueue kQ;

    if (hashQueue.empty() || priorityQueue.empty())
    {
        kQ.key.first = INFINITY_CONST;
        kQ.key.second = INFINITY_CONST;
        kQ.vertexPosition.x = -1;
        kQ.vertexPosition.y = -1;
        hashQueue.clear();
        priorityQueue.clear();
    }
    else
    {
        auto it = priorityQueue.cbegin();

        kQ.key = hashQueue[it->second];
        kQ.vertexPosition = it->second;
    }

    return kQ;
}

Key PriorityQueue::TopKey()
{
    Key k;

    if (hashQueue.empty() || priorityQueue.empty())
    {
        k.first = INFINITY_CONST;
        k.second = INFINITY_CONST;
        hashQueue.clear();
        priorityQueue.clear();
    }
    else
    {
        auto it = priorityQueue.cbegin();
        k = hashQueue[it->second];
    }

    return k;
}

bool PriorityQueue::Empty()
{
    return hashQueue.empty() && priorityQueue.empty();
}

void PriorityQueue::Clear()
{
    hashQueue.clear();
    priorityQueue.clear();
}

