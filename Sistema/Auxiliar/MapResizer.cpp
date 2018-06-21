#include "MapResizer.h"

MapResizer::MapResizer()
{
}

MapResizer::~MapResizer()
{

}

bool MapResizer::Resize(int8_t* originalMap, uint32_t mapWidth, uint32_t mapHeight, uint32_t originalPixPerMeter, uint32_t resizedPixPerMeter)
{
    if (resizedPixPerMeter > originalPixPerMeter)
        return false;

    this->factor = originalPixPerMeter / resizedPixPerMeter;

    this->originalPixPerMeter = originalPixPerMeter;
    this->resizedPixPerMeter = resizedPixPerMeter;

    this->originalWidth = mapWidth;
    this->originalHeigth = mapHeight;

    this->resizedWidth = mapWidth / factor;
    this->resizedHeigth = mapHeight / factor;

    this->originalMap = originalMap;
    this->resizedMap = new int8_t[resizedWidth * resizedHeigth];

    for (int y = 0; y < resizedHeigth; y++)
    {
        for (int x = 0; x < resizedWidth; x++)
        {
            int sum = 0;
            int sumMin = 0;
            for (int i = 0; i < factor; i++)
            {
                int line = y * factor + i;
                if (line >= originalHeigth) break;
                for (int j = 0; j < factor; j++)
                {
                    int column = x * factor + j;
                    if (column >= originalWidth) break;

                    if (originalMap[line * originalWidth + column] < 0)
                        sum++;
                    sumMin++;
                }
            }

            if (sum >= sumMin)
                resizedMap[y * resizedWidth + x] = -1;
            else
                resizedMap[y * resizedWidth + x] = 1;
        }
    }
    return true;
}

int8_t* MapResizer::GetResizedMap()
{
    return resizedMap;
}

int8_t* MapResizer::GetOriginalMap()
{
    return originalMap;
}

uint32_t MapResizer::GetOriginalWidth()
{
    return originalWidth;
}

uint32_t MapResizer::GetOriginalHeigth()
{
    return originalHeigth;
}

uint32_t MapResizer::GetOriginalPixPerMeter()
{
    return originalPixPerMeter;
}

uint32_t MapResizer::GetResizedWidth()
{
    return resizedWidth;
}

uint32_t MapResizer::GetResizedHeigth()
{
    return resizedHeigth;
}

uint32_t MapResizer::GetResizedPixPerMeter()
{
    return resizedPixPerMeter;
}

void MapResizer::SetBegin(double beginX, double beginY)
{
    this->beginX = beginX;
    this->beginY = beginY;
}

VertexPosition MapResizer::RealToResized(player_point_2d_t point)
{
    VertexPosition ret;
    ret.x = (((double) point.px - beginX)* (double) originalPixPerMeter) / (double) factor;
    ret.y = (((double) point.py - beginY)* (double) originalPixPerMeter) / (double) factor;
    return ret;
}

player_point_2d_t MapResizer::ResizedToReal(VertexPosition vP)
{
    player_point_2d_t ret;
    ret.px = ((double) vP.x + 0.5) * (double) factor / (double) originalPixPerMeter + beginX;
    ret.py = ((double) vP.y + 0.5) * (double) factor / (double) originalPixPerMeter + beginY;
    return ret;
}