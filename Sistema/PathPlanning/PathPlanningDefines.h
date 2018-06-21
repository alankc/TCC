#ifndef DEFINES_H
#define DEFINES_H

#define BOOST_SIGNALS_NO_DEPRECATION_WARNING

#include <limits>
#include <unordered_map>

const double MAX_WIDTH_MAP = 10000;
const double INFINITY_CONST = std::numeric_limits<double>::infinity();

struct VertexPosition
{
    int x;
    int y;

    VertexPosition() : x(0), y(0)
    {
    }

    bool operator==(const VertexPosition &vP) const
    {
        return ((x == vP.x) && (y == vP.y));
    }

    bool operator!=(const VertexPosition &vP) const
    {
        return ((x != vP.x) || (y != vP.y));
    }
};

namespace std
{

    template <>
    struct hash<VertexPosition>
    {

        std::size_t operator()(const VertexPosition& vP) const
        {
            return vP.x + vP.y * MAX_WIDTH_MAP;
        }
    };

}

struct VertexData
{
    double g;
    double rhs;
    bool free;

    VertexData() : g(INFINITY_CONST), rhs(INFINITY_CONST), free(true)
    {
    }
};

struct Vertex
{
    VertexPosition position;
    VertexData* data;

    bool operator==(const Vertex &v) const
    {
        return (position == v.position);
    }

    bool operator!=(const Vertex &v) const
    {
        return (position != v.position);
    }
};

struct Key
{
    double first;
    double second;

    Key() : first(0), second(0)
    {
    }

    bool operator==(const Key &k) const
    {
        return ((first == k.first) && (second == k.second));
    }

    bool operator!=(const Key &k) const
    {
        return ((first != k.first) || (second != k.second));
    }

    bool operator>(const Key &k) const
    {
        if (first > k.first) return true;
        if (first < k.first) return false;
        return second > k.second;
    }

    bool operator<(const Key &k) const
    {
        if (first < k.first) return true;
        if (first > k.first) return false;
        return second < k.second;
    }

    bool operator<=(const Key &k) const
    {
        if (first <= k.first) return true;
        if (first > k.first) return false;
        return second <= k.second;
    }
};

struct KeyQueue
{
    Key key;
    VertexPosition vertexPosition;

    bool operator==(const KeyQueue &kQ) const
    {
        return key == kQ.key;
    }

    bool operator!=(const KeyQueue &kQ) const
    {
        return key != kQ.key;
    }

    bool operator>(const KeyQueue &kQ) const
    {
        return key > kQ.key;
    }

    bool operator<(const KeyQueue &kQ) const
    {
        return key < kQ.key;
    }
};

struct VertexUpdate
{
    bool free;
    VertexPosition position;

    VertexUpdate() : free(true)
    {
    }

    bool operator==(const Vertex &v) const
    {
        return position == v.position;
    }

    bool operator!=(const Vertex &v) const
    {
        return position != v.position;
    }
};

#endif /* DEFINES_H */

