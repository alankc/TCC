/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   PriorityQueue.h
 * Author: x
 *
 * Created on 5 de Março de 2018, 09:15
 */

#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include <map>
#include <queue>

#include "PathPlanningDefines.h"

typedef std::unordered_map<VertexPosition, Key> HashQueue;
typedef std::multimap<Key, VertexPosition, std::less<Key>> Queue;

class PriorityQueue
{
public:
    PriorityQueue();
    virtual ~PriorityQueue();
    void Insert(VertexPosition vP, Key k);
    void Update(VertexPosition vP, Key k);
    void Remove(VertexPosition vP);
    KeyQueue Pop();
    bool Contains(VertexPosition vP);
    KeyQueue Top();
    Key TopKey();
    bool Empty();
    void Clear();
private:
    HashQueue hashQueue;
    Queue priorityQueue;
};
#endif /* PRIORITYQUEUE_H */

