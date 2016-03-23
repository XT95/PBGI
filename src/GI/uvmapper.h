#ifndef UVMAPPER_H
#define UVMAPPER_H

#include <iostream>
#include "Engine/object.h"



typedef struct _quadrilateral
{
    glm::vec2 edge,offset;
    glm::vec2 *p1,*p2,*p3,*p4;

    float getArea() const
    {
        return edge.x*edge.y;
    }

    bool operator < (_quadrilateral const &a) const
    {
        return getArea() > a.getArea();
    }
}Quadrilateral;


typedef struct _node
{
    _node *child[2];
    glm::vec4 rect;

    _node()
    {
        child[0] = NULL;
        child[1] = NULL;
        rect = glm::vec4(0.f,0.f,1.f,1.f);
    }
    ~_node()
    {
        delete child[0];
        delete child[1];
    }
}Node;

class UVMapper
{
public:
    UVMapper( glm::ivec2 size );
    ~UVMapper();

    void computeLightmapPack(std::vector<Object*> &obj);
    float getScale() { return m_scale; }
private:
    Node *insert(Node *n, Quadrilateral &r);
    glm::ivec2 m_size;
    float m_scale;
    glm::vec2 m_border;
    std::vector<Object*> m_obj;
};

#endif // UVMAPPER_H
