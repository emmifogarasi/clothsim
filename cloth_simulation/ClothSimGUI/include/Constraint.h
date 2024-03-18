#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include "particle.h"
#include "ngl/Vec3.h"
#include <vector>




class Constraint
{
private:
    float rest_distance; // the length between particle p1 and p2 in rest configuration

public:
    Particle *p1, *p2; // the two particles that are connected through the constraints

    Constraint(Particle *p1, Particle *p2);

    void satisfyConstraint();

};


#endif // CONSTRAINT_H
