#include "DStartLite.h"

#include <iostream>

DStartLite::DStartLite()
{
    initial = true;
}

DStartLite::~DStartLite()
{
}

/*Other methods*/
void DStartLite::MountTheMap(int8_t* map, uint32_t mapWidth, uint32_t mapHeight, double diagonalCost, double nonDiagonalCost, int pixPerMeter, int newPixPerMeter)
{
    this->diagonalCost = diagonalCost;
    this->nonDiagonalCost = nonDiagonalCost;

    if (pixPerMeter > newPixPerMeter)
    {
        int factor = pixPerMeter / newPixPerMeter;
        int newMapWidth = mapWidth / factor;
        int newMapHeight = mapHeight / factor;

        for (int y = 0; y < newMapHeight; y++)
        {
            for (int x = 0; x < newMapWidth; x++)
            {
                int sum = 0;
                int sumMin = 0;
                for (int i = 0; i < factor; i++)
                {
                    int line = y * factor + i;
                    if (line >= mapHeight) break;
                    for (int j = 0; j < factor; j++)
                    {
                        int column = x * factor + j;
                        if (column >= mapWidth) break;

                        if (map[line * mapWidth + column] < 0)
                            sum++;
                        sumMin++;
                    }
                }
                VertexPosition tmp;
                tmp.x = x;
                tmp.y = y;

                if (sum >= sumMin)
                    this->map[tmp].free = true;
                else
                    this->map[tmp].free = false;
            }
        }

        this->mapWidth = newMapWidth;
        this->mapHeight = newMapHeight;
    }
    else
    {
        this->mapWidth = mapWidth;
        this->mapHeight = mapHeight;

        for (int y = 0; y < mapHeight; y++)
        {
            for (int x = 0; x < mapWidth; x++)
            {
                VertexPosition tmp;
                tmp.x = x;
                tmp.y = y;

                if (map[y * mapWidth + x] >= 0)
                    this->map[tmp].free = false;
                else
                    this->map[tmp].free = true;
            }
        }
    }
}

double DStartLite::TotalCost()
{
    if (initial)
    {
        initial = false;
        last = start;
        ComputeShortestPath();
    }

    if (start.data->g == INFINITY_CONST)
        return INFINITY_CONST;

    double sum = 0;
    Vertex tmp = start;
    Vertex newTmp;

    while (tmp != goal)
    {
        auto succ = Succ(tmp);
        double min = INFINITY_CONST;
        double costMin = INFINITY_CONST;
        for (auto i : succ)
        {
            Vertex sl;
            sl.position = i;
            sl.data = &map[i];

            double cost = ComputeCost(tmp, sl);
            double vlr = cost + sl.data->g;

            if (vlr < min)
            {
                min = vlr;
                costMin = cost;
                newTmp = sl;
            }
        }

        tmp = newTmp;
        sum += costMin;
    }

    return sum;
}
/**************************/

/*DStart principal methods*/
Key DStartLite::CalcKey(Vertex v)
{
    Key ret;
    double min = std::min(v.data->g, v.data->rhs);

    ret.first = min + ComputeH(start, v) + km;
    ret.second = min;

    return ret;
}

void DStartLite::Initialize(VertexPosition start, VertexPosition goal)
{
    U.Clear();
    km = 0;
    initial = true;

    for (int i = 0; i < mapHeight; i++)
    {
        for (int j = 0; j < mapWidth; j++)
        {
            VertexPosition tmp;
            tmp.x = j;
            tmp.y = i;
            map[tmp].g = INFINITY_CONST;
            map[tmp].rhs = INFINITY_CONST;
        }
    }

    this->start.position = start;
    this->start.data = &map[start];

    this->goal.position = goal;
    this->goal.data = &map[goal];
    this->goal.data->rhs = 0;

    U.Insert(goal, CalcKey(this->goal));
}

void DStartLite::UpdateVertex(Vertex v)
{
    if (v != goal)
    {
        double min = INFINITY_CONST;
        auto succ = Succ(v);
        for (auto i : succ)
        {
            Vertex sl;
            sl.position = i;
            sl.data = &map[i];

            double vlr = ComputeCost(v, sl) + sl.data->g;
            if (vlr < min)
                min = vlr;
        }
        v.data->rhs = min;
    }

    if (U.Contains(v.position))
    {
        U.Remove(v.position);
    }

    if (v.data->g != v.data->rhs)
    {
        U.Insert(v.position, CalcKey(v));
    }
}

void DStartLite::ComputeShortestPath()
{
    while ((U.TopKey() < CalcKey(start)) || (start.data->rhs != start.data->g))
    {
        KeyQueue kQ = U.Pop();

        Vertex u;
        u.position = kQ.vertexPosition;
        u.data = &map[kQ.vertexPosition];

        Key kOld = kQ.key;
        Key kNew = CalcKey(u);

        if (kOld < kNew)
        {
            U.Insert(u.position, kNew);
        }
        else if (u.data->g > u.data->rhs)
        {
            u.data->g = u.data->rhs;
            auto pred = Pred(u);
            for (auto i : pred)
            {
                Vertex s;
                s.position = i;
                s.data = &map[i];
                UpdateVertex(s);
            }
        }
        else
        {
            u.data->g = INFINITY_CONST;
            auto pred = Pred(u);
            pred.push_back(u.position);
            for (auto i : pred)
            {
                Vertex s;
                s.position = i;
                s.data = &map[i];
                UpdateVertex(s);
            }
        }
    }
}

bool DStartLite::GetNext(VertexPosition& nextPosition, std::vector<VertexUpdate> outdatedVertices)
{
    if (initial)
    {
        initial = false;
        last = start;
        ComputeShortestPath();
        //PrintMap(true);
    }

    if ((start == goal) || (start.data->g == INFINITY_CONST))
        return false;

    double min = INFINITY_CONST;
    auto succ = Succ(start);
    Vertex newStart;
    for (auto i : succ)
    {
        Vertex sl;
        sl.position = i;
        sl.data = &map[i];

        double vlr = ComputeCost(start, sl) + sl.data->g;
        if (vlr < min)
        {
            min = vlr;
            newStart = sl;
        }
    }

    start = newStart;
    nextPosition = start.position;

    if (!outdatedVertices.empty())
    {
        km = km + ComputeH(last, start);
        last = start;

        //Vertex v
        for (auto i : outdatedVertices)
        {
            CellMap::iterator it = map.find(i.position);
            if (it != map.end())
                it->second.free = i.free;
        }

        //Vertex u
        for (auto i : outdatedVertices)
        {
            CellMap::iterator it = map.find(i.position);
            if (it != map.end())
            {
                Vertex v;
                v.position = i.position;
                auto pred = Pred(v);
                for (auto j : pred)
                {
                    Vertex u;
                    u.position = j;
                    u.data = &map[j];
                    UpdateVertex(u);
                }
            }
        }
        ComputeShortestPath();
    }

    return true;
}

bool DStartLite::GetNextWayPoint(VertexPosition& nextPosition, std::vector<VertexUpdate> outdatedVertices)
{
    if (initial)
    {
        initial = false;
        last = start;
        ComputeShortestPath();
        //PrintMap(true);
    }

    if ((start == goal) || (start.data->g == INFINITY_CONST))
    {
        if (start.data->g == INFINITY_CONST)
        {
            std::cout << "Falha ao computar rota!,  Celula livre = " << map[start.position].free << std::endl;        
        }
        else
        {
            std::cout << "START = GOAL" << std::endl;
        }
        return false;
    }

    double min = INFINITY_CONST;
    int currVerticalState, currHorizontalState, nextVerticalState, nextHorizontalState;

    auto succ = Succ(start);
    Vertex newStart;
    for (auto i : succ)
    {
        Vertex sl;
        sl.position = i;
        sl.data = &map[i];

        double vlr = ComputeCost(start, sl) + sl.data->g;
        if (vlr < min)
        {
            min = vlr;
            newStart = sl;
        }
    }

    currVerticalState = nextVerticalState = (newStart.position.y - start.position.y);
    if (currVerticalState != 0)
        currVerticalState = nextVerticalState = currVerticalState / std::abs(currVerticalState);

    currHorizontalState = nextHorizontalState = (newStart.position.x - start.position.x) /*/ std::abs(newStart.position.x - start.position.x)*/;
    if (currHorizontalState != 0)
        currHorizontalState = nextHorizontalState = currHorizontalState / std::abs(currHorizontalState);

    Vertex currStart;
    while ((currVerticalState == nextVerticalState) && (currHorizontalState == nextHorizontalState))
    {
        currStart = newStart;
        auto succ = Succ(currStart);
        for (auto i : succ)
        {
            Vertex sl;
            sl.position = i;
            sl.data = &map[i];

            double vlr = ComputeCost(currStart, sl) + sl.data->g;
            if (vlr < min)
            {
                min = vlr;
                newStart = sl;
            }
        }

        if (newStart == goal)
        {
            currStart = newStart;
            break;
        }

        nextVerticalState = (newStart.position.y - currStart.position.y);
        if (nextVerticalState != 0)
            nextVerticalState = nextVerticalState / std::abs(nextVerticalState);

        nextHorizontalState = (newStart.position.x - currStart.position.x);
        if (nextHorizontalState != 0)
            nextHorizontalState = nextHorizontalState / std::abs(nextHorizontalState);
    }

    start = currStart;
    nextPosition = start.position;

    if (!outdatedVertices.empty())
    {
        km = km + ComputeH(last, start);
        last = start;

        //Vertex v
        for (auto i : outdatedVertices)
        {
            CellMap::iterator it = map.find(i.position);
            if (it != map.end())
                it->second.free = i.free;
        }

        //Vertex u
        for (auto i : outdatedVertices)
        {
            CellMap::iterator it = map.find(i.position);
            if (it != map.end())
            {
                Vertex v;
                v.position = i.position;
                auto pred = Pred(v);
                for (auto j : pred)
                {
                    Vertex u;
                    u.position = j;
                    u.data = &map[j];
                    UpdateVertex(u);
                }
            }
        }
        ComputeShortestPath();
    }

    return true;
}
/**************************/

/*DStart auxiliary methods*/

double DStartLite::ComputeRHS(Vertex v)
{
    if (v == start)
        return 0;

    double min = INFINITY_CONST;
    auto pred = Pred(v);
    for (auto i : pred)
    {
        Vertex sl;
        sl.position = i;
        sl.data = &map[i];
        double vlr = ComputeCost(sl, v) + sl.data->g;
        if (vlr < min)
            min = vlr;
    }

    return min;
}

double DStartLite::ComputeH(Vertex v1, Vertex v2)
{
    //Diagonal distance
    //nD * (dx + dy) + (D - 2 * nD) * min(dx, dy)
    double dx = std::abs(v1.position.x - v2.position.x);
    double dy = std::abs(v1.position.y - v2.position.y);
    double min = std::min(dx, dy);
    return nonDiagonalCost * (dx + dy) + (diagonalCost - 2 * nonDiagonalCost) * min;
}

double DStartLite::ComputeCost(Vertex v1, Vertex v2)
{
    if (!v1.data->free || !v2.data->free)
        return INFINITY_CONST;

    if ((v1.position.x == v2.position.x) || (v1.position.y == v2.position.y))
        return nonDiagonalCost;

    return diagonalCost;
}

std::vector<VertexPosition> DStartLite::Succ(Vertex v)
{
    std::vector<VertexPosition> succ;

    int x = v.position.x - 1;
    int y = v.position.y - 1;
    int maxX = x + 3;
    int maxY = y + 3;

    for (; y < maxY; y++)
    {
        for (x = v.position.x - 1; x < maxX; x++)
        {
            if ((x == v.position.x) && (y == v.position.y)) continue;
            VertexPosition tmp;
            tmp.x = x;
            tmp.y = y;
            auto i = map.find(tmp);
            if ((i != map.end()) && (i->second.free))
                succ.push_back(tmp);
        }
    }

    return succ;
}

std::vector<VertexPosition> DStartLite::Pred(Vertex v)
{
    std::vector<VertexPosition> pred;

    int x = v.position.x - 1;
    int y = v.position.y - 1;
    int maxX = x + 3;
    int maxY = y + 3;

    for (; y < maxY; y++)
    {
        for (x = v.position.x - 1; x < maxX; x++)
        {
            if ((x == v.position.x) && (y == v.position.y)) continue;
            VertexPosition tmp;
            tmp.x = x;
            tmp.y = y;
            auto i = map.find(tmp);
            if ((i != map.end()) && (i->second.free))
                pred.push_back(tmp);
        }
    }

    return pred;
}

/**************************/

void DStartLite::PrintMap(bool withPath)
{
    std::vector<VertexPosition> path;

    if (withPath)
    {
        Vertex next = start;
        if (start.data->g != INFINITY_CONST)
        {
            while (next.position != goal.position)
            {
                double min = INFINITY_CONST;
                auto succ = Succ(next);
                for (auto i : succ)
                {
                    Vertex sl;
                    sl.position = i;
                    sl.data = &map[i];

                    double vlr = ComputeCost(start, sl) + sl.data->g;
                    if (vlr < min)
                    {
                        min = vlr;
                        next.position = sl.position;
                    }
                }
                path.push_back(next.position);
            }
        }
    }

    for (int i = 0; i < mapHeight; i++)
    {
        int line = (mapHeight - i - 1);
        if (line < 10)
            std::cout << "000";
        else if (line < 100)
            std::cout << "00";
        else if (line < 1000)
            std::cout << "0";
        std::cout << line << " ";

        for (int j = 0; j < mapWidth; j++)
        {
            VertexPosition tmp;
            tmp.x = j;
            tmp.y = (mapHeight - i - 1);

            if (map[tmp].free)
            {
                bool tst = false;
                for (auto k : path)
                {
                    if (k == tmp)
                    {
                        tst = true;
                        break;
                    }
                }

                if (tmp == start.position)
                {
                    std::cout << "S";
                }
                else if (tmp == goal.position)
                {
                    std::cout << "G";
                }
                else
                {
                    if (tst)
                        std::cout << "*";
                    else
                        std::cout << " ";
                }
            }
            else
            {
                std::cout << "X";
            }
        }
        std::cout << std::endl;
    }
}