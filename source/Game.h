#ifndef GAME_H
#define GAME_H 

#include "Mundo.h"
#include "Ascensor.h"
#include "Caja.h"
#include "Pendulo.h"
#include "raylib.h"
#include "Plataforma.h"
#include "Player.h"
#include "ZonaCaja.h"
#include <vector>


class Game
{
private:
    Mundo mundo;
    Ascensor* ascensor;
    Caja* caja;
    Pendulo* pendulo;
    Texture2D fondo;
    Texture2D texturaPlataforma;
    Texture2D TexturaCaja;
    Plataforma plataformaIzquierda;
    Plataforma plataformaDerecha;
    Plataforma plataformaSuperior;
    Plataforma limiteIzquierdo;
    Plataforma limiteDerecho;
    Plataforma limiteSuperior;
    std::vector<Caja*> cajas;
    Texture2D idle[6];
    Texture2D jump[3];
    Texture2D run[8];
    Player* player;
    ZonaCajas zonaCajas;
    Music musicaFondo;
    Sound sonidoSalto;
    Sound sonidoVictoria;
    Sound sonidoGameOver;
    int cajasSoltadas;
    int frameActual;
    float tiempoAnimacion;
    bool gano;
    bool perdio;
    bool juegoTerminado;
    bool sonidoFinalReproducido;

    //FUNCIONES
    void CargarRecursos();
    void CrearObjetos();
    void LiberarRecursos();
    void ProcesarControles();
    void Update();
    void Draw();
    void ReiniciarJuego();

public:

    Game();
    ~Game();
    void Run();

};
#endif