#ifndef CLOTH_H
#define CLOTH_H


#include "particle.h"
#include "Constraint.h"
#include <iostream>
#include <ngl/Vec3.h>

#include <ngl/NGLInit.h>
#include <ngl/ShaderLib.h>
#include <vector>
#include <math.h>
#include <QOpenGLWindow>

class Cloth
{
private:
    int num_particles_width; // number of particles in "width" direction
    int num_particles_height; // number of particles in "height" direction

    Particle* getParticle(int x, int y) {return &particles[y*num_particles_width + x];}

    void makeConstraint(Particle *p1, Particle *p2)
    {
        constraints.push_back(Constraint(p1,p2));
    }

    ngl::Vec3 calcTriangleNormal(Particle *p1,Particle *p2,Particle *p3);

    void addWindForcesForTriangle(Particle *p1,Particle *p2,Particle *p3, const ngl::Vec3 direction);

    void drawTriangle(Particle *p1, Particle *p2, Particle *p3, const ngl::Vec3 color);


public:
    int CONSTRAINT_ITERATIONS = 15; // int used to decide how many iterations the constraints go through - later referenced in GUI
    std::vector<Particle> particles; // all particles that are part of this cloth
    std::vector<Constraint> constraints; // constraints between particles as part of this cloth
    struct Vertex
    {
           float x, y, z;  //vertex
           float nx, ny, nz; //normal
           float r, g, b; // colours
    };

    Cloth(float width, float height, int num_particles_width, int num_particles_height);
    Cloth();

    /* in getVertexData we get the normal vectors of the triangles defined by the positions of the points. */
    std::vector<Vertex> getVertexData(std::vector<Vertex> &vd);

    /*We are calling satisfyConstraint() for every constraint, and calling timeStep() for all particles */
    void timeStep();

    void addForce(const ngl::Vec3 direction);

    /* adding windForces */
    void windForce(const ngl::Vec3 direction);

    /* creating ball collision for the cloth */
    void ballCollision(const ngl::Vec3 center,const float radius );

    void doFrame();

};



#endif // CLOTH_H
