#include <algorithm>
#include "GI/uvmapper.h"

UVMapper::UVMapper( glm::ivec2 size) :
    m_size(size),
    m_scale(0.f),
    m_border(glm::vec2(1.)/glm::vec2(size))
{}

UVMapper::~UVMapper()
{}

void UVMapper::computeLightmapPack(std::vector<Object*> &obj)
{
    m_obj = obj;

    std::vector<Quadrilateral> m_quad;

    //UV Planar projection
    for(unsigned o=0; o<m_obj.size(); o++)
    {
        glm::vec3 *vertex = m_obj[o]->getVertex();
        glm::vec2 *uv = m_obj[o]->getTexCoordsLightmap();

        for(unsigned i=0; i<m_obj[o]->getNbFaces(); i++)
        {
            glm::vec3 pn = glm::abs( m_obj[o]->getNormalsFaces()[i]);

            if( pn.x > pn.y && pn.x > pn.z) //Plane X
            {
                for(int j=0; j<4; j++)
                {
                    uv[i*4+j].x = vertex[i*4+j].y;
                    uv[i*4+j].y = vertex[i*4+j].z;
                }
            }
            else if( pn.y > pn.x && pn.y > pn.z) //Plane Y
            {
                for(int j=0; j<4; j++)
                {
                    uv[i*4+j].x = vertex[i*4+j].x;
                    uv[i*4+j].y = vertex[i*4+j].z;
                }
            }
            else //Plane Z
            {
                for(int j=0; j<4; j++)
                {
                    uv[i*4+j].x = vertex[i*4+j].y;
                    uv[i*4+j].y = vertex[i*4+j].x;
                }
            }

            //Normalize to 0 .. X & store the edges
            glm::vec2 min=uv[i*4],max=uv[i*4];
            for(int j=1; j<4; j++)
            {
                min = glm::min( min, uv[i*4+j]);
                max = glm::max( max, uv[i*4+j]);
            }

            Quadrilateral q;
            q.edge = max-min;
            q.p1 = &uv[i*4+0]; q.p2 = &uv[i*4+1]; q.p3 = &uv[i*4+2], q.p4 = &uv[i*4+3];
            m_quad.push_back( q );

            //Because TRIGONOMETRIC ORDER... =)
            uv[i*4+0] = glm::vec2(0.f);
            uv[i*4+1] = glm::vec2(q.edge.x,0.f);
            uv[i*4+2] = q.edge;
            uv[i*4+3] = glm::vec2(0.f,q.edge.y);
        }
    }

    //Compute areaMax
    m_scale = 0.f;
    for(unsigned i=0; i<m_quad.size(); i++)
        m_scale += m_quad[i].getArea();
    m_scale = sqrt(m_scale)*1.35;

    //Normalize to fit to texture
    for(unsigned i = 0; i < m_quad.size(); ++i)
        m_quad[i].edge /= m_scale;
    for(unsigned i=0; i<m_obj.size(); i++)
    for(unsigned j=0; j<m_obj[i]->getNbVertex(); j++)
        m_obj[i]->getTexCoordsLightmap()[j] /= m_scale;

    //Sort by area
    std::sort(m_quad.begin(), m_quad.end());


    //Recursive packing
    Node root;
    for(unsigned i = 0; i < m_quad.size(); ++i)
    {
        if( insert(&root,m_quad[i]) == NULL)
        {
            std::cout << m_quad[i].edge.x << " | " << m_quad[i].edge.y << std::endl;
            std::cout << "UV MAP PROBLEM" << std::endl;
        }
    }
}




Node* UVMapper::insert(Node *n, Quadrilateral &q)
{
    //If this node have children, we try to recursively insert into them
    if(n->child[0] && n->child[1])
    {
        Node *c = insert(n->child[0],q);
        return c ? c : insert(n->child[1],q);
    }
    else
    {
        //Can this rectangle be packed into this node ?
        if( q.edge.x > n->rect.z || q.edge.y > n->rect.w)
        {
            return NULL;
        }
        //Does this rectangle have exactly the same size as this node ?
        if( q.edge.x == n->rect.z && q.edge.y == n->rect.w)
        {
            glm::vec2 offset(n->rect.x, n->rect.y);
            *(q.p1) += offset;
            *(q.p2) += offset;
            *(q.p3) += offset;
            *(q.p4) += offset;
            return n;
        }

        n->child[0] = new Node();
        n->child[1] = new Node();
        //Decide which way to split
        float dw = n->rect.z - q.edge.x;
        float dh = n->rect.w - q.edge.y;

        if( dw < dh)
        {
            n->child[0]->rect = glm::vec4(n->rect.x+q.edge.x, n->rect.y, n->rect.z-q.edge.x, q.edge.y);
            n->child[1]->rect = glm::vec4(n->rect.x, n->rect.y+q.edge.y, n->rect.z, n->rect.w-q.edge.y);

            n->child[0]->rect += glm::vec4(m_border.x,0.f,-m_border.x,0.f);
            n->child[1]->rect += glm::vec4(0.f,m_border.y,0.f,-m_border.y);


        }
        else
        {
            n->child[0]->rect = glm::vec4(n->rect.x, n->rect.y+q.edge.y, q.edge.x, n->rect.w-q.edge.y);
            n->child[1]->rect = glm::vec4(n->rect.x+q.edge.x, n->rect.y, n->rect.z-q.edge.x, n->rect.w);

            n->child[0]->rect += glm::vec4(0.f,m_border.y,0.f,-m_border.y);
            n->child[1]->rect += glm::vec4(m_border.x,0.f,-m_border.x,0.f);
         }

        //Update the uv of the rectangle
        glm::vec2 offset(n->rect.x, n->rect.y);
        q.offset = offset;
        *(q.p1) += offset;
        *(q.p2) += offset;
        *(q.p3) += offset;
        *(q.p4) += offset;

        return n;
    }
}
