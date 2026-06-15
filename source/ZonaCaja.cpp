#include "ZonaCaja.h"

void ZonaCajas::Crear(float x, float y, float ancho, float alto){
    area = Rectangle{ x, y, ancho, alto };
}

void ZonaCajas::Draw(){
    DrawRectangleRec(area, Color{ 120, 120, 120, 180 });
    DrawRectangleLinesEx(area, 3, BLACK);
}
bool ZonaCajas::Contiene(float x, float y){
    return CheckCollisionPointRec(Vector2{ x, y }, area);
}