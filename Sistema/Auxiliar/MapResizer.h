/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MapResizer.h
 * Author: x
 *
 * Created on 26 de Março de 2018, 17:14
 */

#ifndef MAPRESIZER_H
#define MAPRESIZER_H

#include "../PathPlanning/PathPlanningDefines.h"
#include <libplayerc++/playerc++.h>

class MapResizer
{
public:
    MapResizer();
    virtual ~MapResizer();

    bool Resize(int8_t* originalMap, uint32_t mapWidth, uint32_t mapHeight, uint32_t originalPixPerMeter = 1, uint32_t resizedPixPerMeter = 1);
    int8_t* GetResizedMap();
    int8_t* GetOriginalMap();
    uint32_t GetOriginalWidth();
    uint32_t GetOriginalHeigth();
    uint32_t GetOriginalPixPerMeter();
    uint32_t GetResizedWidth();
    uint32_t GetResizedHeigth();
    uint32_t GetResizedPixPerMeter();

    VertexPosition RealToResized(player_point_2d_t point);
    player_point_2d_t ResizedToReal(VertexPosition vP);
    void SetBegin(double beginX, double beginY);

private:
    int8_t* originalMap;
    uint32_t originalWidth;
    uint32_t originalHeigth;
    uint32_t originalPixPerMeter;

    int8_t* resizedMap;
    uint32_t resizedWidth;
    uint32_t resizedHeigth;
    uint32_t resizedPixPerMeter;

    uint32_t factor;
    
    double beginX;
    double beginY;
};

#endif /* MAPRESIZER_H */

