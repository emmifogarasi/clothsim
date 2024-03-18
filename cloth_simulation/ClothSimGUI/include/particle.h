#ifndef PARTICLE_H
#define PARTICLE_H


#include <ngl/Vec3.h>
#include <ngl/NGLInit.h>




#define DAMPING 0.01 // how much to damp the cloth simulation each frame
#define TIME_STEPSIZE2 0.5*0.5 // how large time step each particle takes each frame


/* The particle class represents a particle of mass that can move around in 3D space*/
class Particle
{
private:
    float mass; // the mass of the particle

    ngl::Vec3 m_pos; // the current position of the particle
    ngl::Vec3 old_pos; // the position of the particle in the previous time step
    ngl::Vec3 m_acceleration; // a vector for acceleration
    ngl::Vec3 accumulated_normal; // an accumulated normal

public:
    bool movable = true; // used to pin the cloth on the ceiling

    Particle(ngl::Vec3 pos);
    Particle();

    void addForce(ngl::Vec3 f);

    void timeStep();

    ngl::Vec3& getPos();

    void resetAcceleration();

    void offsetPos(const ngl::Vec3 v);

    void makeUnmovable(); // to make parts of the cloth unmovable

    void addToNormal(ngl::Vec3 normal);

    ngl::Vec3& getNormal();

    void resetNormal();

};


#endif //PARTICLE_H
