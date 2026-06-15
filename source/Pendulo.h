#ifndef PENDULO_H
#define PENDULO_H 

#include <box2d.h>
#include <raylib.h>

class Pendulo
{
private:
    b2Body* anchor;
    b2Body* arm;
    b2RevoluteJoint* joint;

public:
    Pendulo(b2World* world,float x,float y);
    void GirarIzquierda();
    void GirarDerecha();
    void Detener();
    void MoverDerecha();
    void MoverIzquierda();
    void Dibujar();
    float GetX();
    float GetY();
    void seguirMouse();
};
#endif