#include "Ascensor.h"

const float SCALE = 30.0f;
const int SCREEN_HEIGHT = 900;

Ascensor::Ascensor(b2World* world, float x, float y)
{
    // Riel estático
    b2BodyDef railDef;
    railDef.position.Set(x / SCALE, y / SCALE);

    rail = world->CreateBody(&railDef);

    b2PolygonShape railShape;
    railShape.SetAsBox(10.0f / SCALE, 200.0f / SCALE);

    rail->CreateFixture(&railShape, 0.0f);

    // Plataforma dinámica
    b2BodyDef platformDef;
    platformDef.type = b2_dynamicBody;
    platformDef.position.Set(x / SCALE, (y - 120) / SCALE);

    platform = world->CreateBody(&platformDef);

    b2PolygonShape platformShape;
    platformShape.SetAsBox(100.0f / SCALE, 15.0f / SCALE);

    b2FixtureDef fixture;
    fixture.shape = &platformShape;
    fixture.density = 2.0f;
    fixture.friction = 0.8f;

    platform->CreateFixture(&fixture);

    // Pared izquierda del ascensor
    b2PolygonShape paredIzquierda;
    paredIzquierda.SetAsBox(8.0f / SCALE, 50.0f / SCALE, b2Vec2(-95.0f / SCALE, 50.0f / SCALE),  0.0f);

    platform->CreateFixture(&paredIzquierda, 1.0f);

    // Pared derecha del ascensor
    b2PolygonShape paredDerecha;
    paredDerecha.SetAsBox( 8.0f / SCALE, 50.0f / SCALE, b2Vec2(95.0f / SCALE, 50.0f / SCALE), 0.0f );

    platform->CreateFixture(&paredDerecha, 1.0f);

    // Joint prismático
    b2PrismaticJointDef jointDef;

    jointDef.Initialize( rail, platform, platform->GetWorldCenter(), b2Vec2(0.0f, 1.0f) );

    jointDef.enableLimit = true;
    jointDef.lowerTranslation = -4.0f;
    jointDef.upperTranslation = 10.0f;

    jointDef.enableMotor = true;
    jointDef.maxMotorForce = 1000.0f;

    joint = (b2PrismaticJoint*)world->CreateJoint(&jointDef);
}

void Ascensor::Subir(){
    joint->SetMotorSpeed(3.0f);
}

void Ascensor::Bajar(){
    joint->SetMotorSpeed(-3.0f);
}

void Ascensor::Detener(){
    joint->SetMotorSpeed(0.0f);
}

void Ascensor::Reiniciar(){
    platform->SetTransform( b2Vec2(800.0f / SCALE, (200.0f - 120.0f) / SCALE), 0 );

    platform->SetLinearVelocity( b2Vec2(0.0f, 0.0f) );
}

void Ascensor::Dibujar()
{
    // Posición del riel
    b2Vec2 posicionRiel = rail->GetPosition();

    float xRiel = posicionRiel.x * SCALE;
    float yRiel = SCREEN_HEIGHT - posicionRiel.y * SCALE;

    DrawRectangle(xRiel - 10,yRiel - 200, 20,400,GRAY );

    // Posición de la plataforma
    b2Vec2 posicionPlataforma = platform->GetPosition();

    float xPlataforma = posicionPlataforma.x * SCALE;
    float yPlataforma = SCREEN_HEIGHT - posicionPlataforma.y * SCALE;

    // Soporte izquierdo
    DrawRectangle( xPlataforma - 105,  yPlataforma - 80, 16,100, BLUE);

    // Soporte derecho
    DrawRectangle( xPlataforma + 89, yPlataforma - 80, 16, 100, BLUE );

    // Plataforma principal
    Rectangle plataforma = { xPlataforma, yPlataforma, 200,30 };

    Vector2 centroRotacion ={  100,  15 };

    float rotacion = -platform->GetAngle() * RAD2DEG;

    DrawRectanglePro( plataforma, centroRotacion, rotacion,  BLUE );
}