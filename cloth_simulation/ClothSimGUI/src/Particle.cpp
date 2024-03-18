
#include "particle.h"
#include <ngl/NGLInit.h>

#include <ngl/Vec3.h>

#define DAMPING 0.01 // how much to damp the cloth simulation each frame
#define TIME_STEPSIZE2 0.5*0.5 // how large time step each particle takes each frame

 Particle::Particle(ngl::Vec3 pos)
 {
     this->m_pos = pos;
     this->old_pos = pos;
     this->m_acceleration = ngl::Vec3 (0,0,0);
     this->mass = 1;
 }


 Particle::Particle()
 {

 }


void Particle::addForce(ngl::Vec3 f)
{
    m_acceleration += f/mass;   //f=m*a - used for acceleration
}


void Particle::timeStep()
{
    if(movable)
    {
        ngl::Vec3 temp = m_pos;
        m_pos = m_pos + (m_pos-old_pos)*(1.0-DAMPING) + m_acceleration*TIME_STEPSIZE2;
        old_pos = temp;
        m_acceleration = ngl::Vec3(0,0,0);
    }
}

ngl::Vec3& Particle::getPos()
{
    return m_pos;
}

void Particle::resetAcceleration()
{
    m_acceleration = ngl::Vec3(0,0,0);
}

void Particle::offsetPos(const ngl::Vec3 v)
{
   if(movable) m_pos += v;
}

void Particle::makeUnmovable()
{
     movable = false;
}

void Particle::addToNormal(ngl::Vec3 normal)
{
    ngl::Vec3 n = normal;
    n.normalize();
    accumulated_normal = accumulated_normal + n;
}

ngl::Vec3& Particle::getNormal()
{
    return accumulated_normal;
}

void Particle::resetNormal()
{
    accumulated_normal = ngl::Vec3(0,0,0);
}




