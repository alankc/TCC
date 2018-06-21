/*
 * The MIT License
 *
 * Copyright 2018 x.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/* 
 * File:   DStartLite.h
 * Author: x
 *
 * Created on 5 de Março de 2018, 12:13
 */

#ifndef DSTARTLITE_H
#define DSTARTLITE_H

#include "PathPlanningDefines.h"
#include "PriorityQueue.h"
#include <vector>
#include <unordered_map>

typedef std::unordered_map<VertexPosition, VertexData> CellMap;

class DStartLite
{
public:
    DStartLite();
    virtual ~DStartLite();

    void MountTheMap(int8_t* map, uint32_t mapWidth, uint32_t mapHeight, double diagonalCost = 1.41, double nonDiagonalCost = 1.0, int pixPerMeter = 1, int newPixPerMeter = 1);
    void Initialize(VertexPosition start, VertexPosition goal);
    bool GetNext(VertexPosition& nextPosition, std::vector<VertexUpdate> outdatedVertices);
    bool GetNextWayPoint(VertexPosition& nextPosition, std::vector<VertexUpdate> outdatedVertices);
    
    void PrintMap(bool withPath);

    double TotalCost();
private:
    CellMap map;
    uint32_t mapWidth;
    uint32_t mapHeight;
    double diagonalCost;
    double nonDiagonalCost;

    PriorityQueue U;

    Vertex start;
    Vertex goal;
    Vertex last;

    double km;

    bool initial;

    Key CalcKey(Vertex v);
    void UpdateVertex(Vertex v);
    void ComputeShortestPath();

    double ComputeRHS(Vertex v);
    double ComputeH(Vertex v1, Vertex v2);
    double ComputeCost(Vertex v1, Vertex v2);

    std::vector<VertexPosition> Succ(Vertex v);
    std::vector<VertexPosition> Pred(Vertex v);
    //std::vector<VertexPosition> GetPath();
};

#endif /* DSTARTLITE_H */

