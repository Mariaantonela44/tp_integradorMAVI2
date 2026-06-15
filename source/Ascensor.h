#ifndef ASCENSOR_H
#define ASCENSOR_H 

#include <box2d.h>
#include <raylib.h>

class Ascensor
{
private:
    b2Body* rail;
    b2Body* platform;

    b2PrismaticJoint* joint;

public:
    Ascensor(
        b2World* world,
        float x,
        float y
    );

    void Subir();
    void Bajar();
    void Detener();
    void Reiniciar();
    void Dibujar();
};
#endif