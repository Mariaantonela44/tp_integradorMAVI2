#ifndef ZONACAJA_H
#define ZONACAJA_H    

#include "raylib.h"

class ZonaCajas
{
private:
    Rectangle area;

public:
    void Crear(float x, float y, float ancho, float alto);
    void Draw();
    bool Contiene(float x, float y);
};
#endif