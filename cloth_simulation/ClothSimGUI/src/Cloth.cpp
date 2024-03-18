

#include <ngl/NGLStream.h>

#include "particle.h"
#include "Cloth.h"
#include "NGLScene.h"
#include <GL/gl.h>
#include <QtDebug>
#include <math.h>
#include <ngl/Vec3.h>
#include <cmath>
#include <vector>



Cloth::Cloth(float width, float height, int num_particles_width, int num_particles_height) : num_particles_width(num_particles_width), num_particles_height(num_particles_height)
{
    particles.resize(num_particles_width*num_particles_height); //I am essentially using this vector as an array with room for num_particles_width*num_particles_height particles

    // creating particles in a grid of particles from (0,0,0) to (width,-height,0)
    for(int x=0; x<num_particles_width; x++)
    {
        for(int y=0; y<num_particles_height; y++)
        {
            ngl::Vec3 pos = ngl::Vec3(width * (x/(float)num_particles_width - 0.5),
                            -height * (y/(float)num_particles_height),
                            0);
            particles[y*num_particles_width+x]= Particle(pos); // insert particle in column x at y'th row
            // qDebug()<<"PARTICLES: "<<particles[y*num_particles_width+x].pos.m_x<<'\n';
        }

    }

    // Connecting immediate neighbor particles with constraints
    for(int x=0; x<num_particles_width; x++)
    {
        for(int y=0; y<num_particles_height; y++)
        {
            if (x<num_particles_width-1) makeConstraint(getParticle(x,y),getParticle(x+1,y));
            if (y<num_particles_height-1) makeConstraint(getParticle(x,y),getParticle(x,y+1));
            if (x<num_particles_width-1 && y<num_particles_height-1) makeConstraint(getParticle(x,y),getParticle(x+1,y+1));
            if (x<num_particles_width-1 && y<num_particles_height-1) makeConstraint(getParticle(x+1,y),getParticle(x,y+1));
        }
    }


    // Connecting secondary neighbors with constraints (distance 2 and sqrt(4) in the grid)
    for(int x=0; x<num_particles_width; x++)
    {
        for(int y=0; y<num_particles_height; y++)
        {
            if (x<num_particles_width-2) makeConstraint(getParticle(x,y),getParticle(x+2,y));
            if (y<num_particles_height-2) makeConstraint(getParticle(x,y),getParticle(x,y+2));
            if (x<num_particles_width-2 && y<num_particles_height-2) makeConstraint(getParticle(x,y),getParticle(x+2,y+2));
            if (x<num_particles_width-2 && y<num_particles_height-2) makeConstraint(getParticle(x+2,y),getParticle(x,y+2));			}
    }


    // making the upper left most three and right most three particles unmovable
    for(int i=0;i<3; i++)
    {
        getParticle(0+i ,0)->offsetPos(ngl::Vec3(width/200,0.0,0.0)); //moving the particles a bit towards the centre so when the cloth hangs it looks more realistic
        getParticle(0+i ,0)->makeUnmovable();

        getParticle(0+i ,0)->offsetPos(ngl::Vec3(width/-200,0.0,0.0)); //moving the particles a bit towards the centre so when the cloth hangs it looks more realistic
        getParticle(num_particles_width-1-i ,0)->makeUnmovable();
    }

}

Cloth::Cloth()
{

}

ngl::Vec3 Cloth::calcTriangleNormal(Particle *p1,Particle *p2,Particle *p3)
{
    ngl::Vec3 pos1 = p1->getPos();
    ngl::Vec3 pos2 = p2->getPos();
    ngl::Vec3 pos3 = p3->getPos();

    ngl::Vec3 v1 = pos2-pos1;
    ngl::Vec3 v2 = pos3-pos1;

    return v1.cross(v2);
}

/* calculating what angle the wind hits the normal of the triangles by using dot product */
void Cloth::addWindForcesForTriangle(Particle *p1,Particle *p2,Particle *p3, const ngl::Vec3 direction)
{
    ngl::Vec3 normal = calcTriangleNormal(p1,p2,p3);
    ngl::Vec3 d = normal;
    d.normalize();
    ngl::Vec3 force = normal*(d.dot(direction));
    p1->addForce(force);
    p2->addForce(force);
    p3->addForce(force);
}

/* this is used to create smooth shading - the vertex normals are added to the triangle normals and thus we get a smoother look */
void Cloth::drawTriangle(Particle *p1, Particle *p2, Particle *p3, const ngl::Vec3 color)
{

}

std::vector<Cloth::Vertex> Cloth::getVertexData(std::vector<Cloth::Vertex> &vd)
{
    vd.clear(); //clear the array

    ngl::Vec3 pos;
    ngl::Vec3 n;
    Vertex v;


    // reset normals (which where written to last frame)
    std::vector<Particle>::iterator particle;
    for(particle = particles.begin(); particle != particles.end(); particle++)
    {
       (*particle).resetNormal();
    }

    //create smooth per particle normals by adding up all the (hard) triangle normals that each particle is part of
    for(int x = 0; x<num_particles_width-1; x++)
    {
        for(int y=0; y<num_particles_height-1; y++)
        {
            ngl::Vec3 normal = calcTriangleNormal(getParticle(x+1,y),getParticle(x,y),getParticle(x,y+1));
            getParticle(x+1,y)->addToNormal(normal);
            getParticle(x,y)->addToNormal(normal);
            getParticle(x,y+1)->addToNormal(normal);

            normal = calcTriangleNormal(getParticle(x+1,y+1),getParticle(x+1,y),getParticle(x,y+1));
            getParticle(x+1,y+1)->addToNormal(normal);
            getParticle(x+1,y)->addToNormal(normal);
            getParticle(x,y+1)->addToNormal(normal);
        }
    }

    for(particle = particles.begin(); particle != particles.end(); particle++)
    {
        pos = (*particle).getPos();

        v.x = pos.m_x;
        v.y = pos.m_y;
        v.z = pos.m_z;

        n = particle->getNormal();
        n.normalize();
        v.nx = n.m_x;
        v.ny = n.m_y;
        v.nz = n.m_z;

        vd.push_back(v);
     //   qDebug()<<"v x: "<<v.x<<'\n';
     //   qDebug()<<"pos x: "<<particle->movable<<'\n';
    }

     return vd;
}


void Cloth::timeStep()
{
    std::vector<Constraint>::iterator constraint;
    for(int i=0; i<CONSTRAINT_ITERATIONS; i++) // iterate over all constraints several times
    {
        for(constraint = constraints.begin(); constraint != constraints.end(); constraint++ )
        {
            (*constraint).satisfyConstraint(); // satisfy constraint.
        }
    }

    std::vector<Particle>::iterator particle;
    for(particle = particles.begin(); particle != particles.end(); particle++)
    {
        (*particle).timeStep(); // calculate the position of each particle at the next time step.
    }
}

/* used to add gravity to all particles*/
void Cloth::addForce(const ngl::Vec3 direction)
{
    std::vector<Particle>::iterator particle;
    for(particle = particles.begin(); particle != particles.end(); particle++)
    {
        (*particle).addForce(direction); // add the forces to each particle
    }

}


void Cloth::windForce(const ngl::Vec3 direction)
{
    for(int x = 0; x<num_particles_width-1; x++)
    {
        for(int y=0; y<num_particles_height-1; y++)
        {
            addWindForcesForTriangle(getParticle(x+1,y),getParticle(x,y),getParticle(x,y+1),direction);
            addWindForcesForTriangle(getParticle(x+1,y+1),getParticle(x+1,y),getParticle(x,y+1),direction);
        }
    }
}

/* calculating the collision of the ball */
void Cloth::ballCollision(const ngl::Vec3 center,const float radius )
    {
        std::vector<Particle>::iterator particle;
        for(particle = particles.begin(); particle != particles.end(); particle++)
        {
            ngl::Vec3 v = (*particle).getPos()-center;
            float l = v.length();
            if ( v.length() < radius) // if the particle is inside the ball
            {
                v.normalize();
                (*particle).offsetPos(v*(radius-l)); // project the particle to the surface of the ball
            }
        }
    }


void Cloth::doFrame()
{

}
