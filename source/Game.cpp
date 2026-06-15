#include "Game.h"
#include "raylib.h"

Game::Game(){
    InitWindow(1600, 900, "TP integrador");

    SetTargetFPS(60);
    InitAudioDevice();

    player = nullptr;
    ascensor = nullptr;
    caja = nullptr;
    pendulo = nullptr;
    CargarRecursos();
    CrearObjetos();
    juegoTerminado = false;
    sonidoFinalReproducido = false;
}

Game::~Game(){
    delete ascensor;
    delete caja;
    delete pendulo;
    delete player;
    for (int i = 0; i < cajas.size(); i++){
        delete cajas[i];
    }

    LiberarRecursos();
    
    CloseWindow();
}

void Game::CargarRecursos()
{
    fondo = LoadTexture("assets/fondo1.png");
    texturaPlataforma= LoadTexture("assets/plataforma.png");
	TexturaCaja = LoadTexture("assets/caja.png");
    musicaFondo = LoadMusicStream("assets/musica.mp3");
    sonidoSalto = LoadSound("assets/salto.wav");
    sonidoVictoria = LoadSound("assets/victoria.wav");
    sonidoGameOver = LoadSound("assets/game-over.wav");

    PlayMusicStream(musicaFondo);

}

void Game::CrearObjetos()
{
    zonaCajas.Crear(620, 480, 360, 200);

    player = new Player( mundo.GetWorld(), 450, 390);

	//limites del mundo 
    limiteIzquierdo.Crear( mundo.GetWorld(),0, 450, 30, 900, texturaPlataforma);
    limiteDerecho.Crear(mundo.GetWorld(), 1600, 450, 30, 900, texturaPlataforma);
    limiteSuperior.Crear( mundo.GetWorld(), 800,900,1600, 30, texturaPlataforma);

    // Ascensor abajo y centrado
    ascensor = new Ascensor( mundo.GetWorld(), 800,  200);

    // Plataforma izquierda
    plataformaIzquierda.Crear( mundo.GetWorld(), 450, 300, 250, 30, texturaPlataforma);

    // Plataforma derecha
    plataformaDerecha.Crear( mundo.GetWorld(),1150,300, 250, 30, texturaPlataforma);

    // Plataforma superior
    plataformaSuperior.Crear(mundo.GetWorld(), 800, 500, 350, 30, texturaPlataforma );

    // Péndulo arriba de todo
    pendulo = new Pendulo( mundo.GetWorld(),900, 870);

}

void Game::LiberarRecursos()
{
    UnloadTexture(fondo);
    UnloadTexture(texturaPlataforma);
    UnloadTexture(TexturaCaja);
    UnloadMusicStream(musicaFondo);
    UnloadSound(sonidoSalto);
    UnloadSound(sonidoVictoria);
    UnloadSound(sonidoGameOver);
    CloseAudioDevice();

  
}

void Game::Run(){
    while (!WindowShouldClose()){

        ProcesarControles();
        Update();

        BeginDrawing();
        ClearBackground(RAYWHITE);

        Draw();

        EndDrawing();
    }
}

void Game::ProcesarControles(){
    pendulo->seguirMouse();

    if (juegoTerminado){
        if (IsKeyPressed(KEY_R)){
            ReiniciarJuego();
        }
        return;
    }


    if (IsKeyDown(KEY_W)){
        ascensor->Subir();
    }else 
        if (IsKeyDown(KEY_S)){
        ascensor->Bajar();
        } else {
            ascensor->Detener();
        }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        if (cajasSoltadas < 5){
            float xCaja = pendulo->GetX();
            float yCajaPantalla = pendulo->GetY() + 95;
            float yCajaBox2D = 900 - yCajaPantalla;

            cajas.push_back( new Caja( mundo.GetWorld(), xCaja, yCajaBox2D,  50, 50, TexturaCaja ) );
            cajasSoltadas++;
        }
    }

    if (IsKeyDown(KEY_Q)){
        pendulo->GirarIzquierda();
    }else
        if (IsKeyDown(KEY_E)){
        pendulo->GirarDerecha();
        }else{
            pendulo->Detener();
        }

    //movimiento player
    if (IsKeyDown(KEY_LEFT)){
        player->MoverIzquierda();
    }else 
        if (IsKeyDown(KEY_RIGHT)){
        player->MoverDerecha();
        }else{
            player->Detener();
        }

    if (IsKeyPressed(KEY_SPACE))
    {
        player->Saltar();
        PlaySound(sonidoSalto);
    }
    if (IsKeyPressed(KEY_ENTER))
    {
        int cajasEnZona = 0;

        for (int i = 0; i < cajas.size(); i++)
        {
            b2Vec2 pos = cajas[i]->GetBody()->GetPosition();

            float xCaja = pos.x * 30.0f;
            float yCaja = 900 - pos.y * 30.0f;

            if (zonaCajas.Contiene(xCaja, yCaja))
            {
                cajasEnZona++;
            }
        }

        if (cajasEnZona == 5)
        {
            gano = true;
            perdio = false;
            juegoTerminado = true;

            StopMusicStream(musicaFondo);
            PlaySound(sonidoVictoria);
        }
        else
        {
            perdio = true;
            gano = false;
            juegoTerminado = true;




            PlaySound(sonidoGameOver);
        }
    }
}

void Game::Update(){
    if (juegoTerminado)
    {
        return;
    }

    UpdateMusicStream(musicaFondo);

    mundo.Actualizar();
    player->Actualizar();

    b2Body* cuerpoPlayer = player->GetBody();
    b2Vec2 posicionPlayer = cuerpoPlayer->GetPosition();

    bool playerCayoDelMapa = posicionPlayer.y < -5.0f;

    if (playerCayoDelMapa) {
        b2Vec2 posicionInicial;
        posicionInicial.x = 450.0f / 30.0f;
        posicionInicial.y = 390.0f / 30.0f;

        cuerpoPlayer->SetTransform( posicionInicial, 0);

        cuerpoPlayer->SetLinearVelocity(
            b2Vec2(0.0f, 0.0f)
        );
    }
}

void Game::ReiniciarJuego()
{
    for (int i = 0; i < cajas.size(); i++)
    {
        delete cajas[i];
    }

    cajas.clear();

    cajasSoltadas = 0;
    gano = false;
    perdio = false;
    juegoTerminado = false;
    ascensor->Reiniciar();

    PlayMusicStream(musicaFondo);
}

void Game::Draw(){

    DrawTexturePro(fondo, Rectangle{ 0, 0, (float)fondo.width, (float)fondo.height },Rectangle{ 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() }, Vector2{ 0, 0 }, 0, WHITE );
    zonaCajas.Draw();
    DrawText("ZONA DE CARGA", 650, 450, 25, BLACK );
    player->Dibujar();
    plataformaIzquierda.Draw();
    plataformaDerecha.Draw();
    plataformaSuperior.Draw();

    ascensor->Dibujar();

    for (int i = 0; i < cajas.size(); i++){
        cajas[i]->Dibujar();
    }

    if (cajasSoltadas < 5){
        float xCajaColgada = pendulo->GetX();
        float yCajaColgada = pendulo->GetY() + 95;

        // Cadena de la caja
        DrawLineEx( Vector2{ xCajaColgada, pendulo->GetY() }, Vector2{ xCajaColgada, pendulo->GetY() + 70 }, 4, DARKGRAY );

        // Caja colgada del péndulo
        DrawTexturePro( TexturaCaja, Rectangle{  0, 0, (float)TexturaCaja.width, (float)TexturaCaja.height },
            Rectangle{ xCajaColgada, yCajaColgada, 50,50 },
            Vector2{ 25, 25 }, 0,  WHITE );
    }
    pendulo->Dibujar();

    DrawText("ENTER - Verificar cantidad de cajas en zona de carga", 20, 130, 20, WHITE);

    if (gano)
    {
        DrawText("GANASTE", 650, 100, 60, GREEN);
        DrawText("Presiona R para reiniciar", 620, 180, 30, BLACK);
    }

    if (perdio)
    {
        DrawText("PERDISTE", 650, 100, 60, RED);
        DrawText("Presiona R para reiniciar", 620, 180, 30, BLACK);
    }

    DrawText("Mover con mouse tambien CLICK - Soltar caja desde pendulo. Girar pendulo con Q y E", 20, 30, 20, WHITE);
    DrawText("Mano de player con <- -> y espacio", 20, 50, 20, WHITE);
    DrawText(TextFormat("Cajas: %i / 5", cajasSoltadas), 20, 100, 20, WHITE);
    //CloseWindow();
}
