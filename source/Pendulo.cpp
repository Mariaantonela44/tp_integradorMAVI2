#include "Pendulo.h"

const float SCALE = 30.0f;
const int SCREEN_HEIGHT = 900;

Pendulo::Pendulo(b2World* world, float x, float y){
    b2BodyDef anchorDef;
    anchorDef.position.Set(x / SCALE, y / SCALE);
    anchor = world->CreateBody(&anchorDef);

    b2CircleShape anchorShape;
    anchorShape.m_radius = 6.0f / SCALE;
    anchor->CreateFixture(&anchorShape, 0.0f);

    b2BodyDef armDef;
    armDef.type = b2_dynamicBody;
    armDef.position.Set(x / SCALE, (y - 55) / SCALE);

    arm = world->CreateBody(&armDef);

    b2PolygonShape armShape;
    armShape.SetAsBox(8.0f / SCALE, 45.0f / SCALE);

    b2FixtureDef fixture;
    fixture.shape = &armShape;
    fixture.density = 1.0f;
    fixture.friction = 0.4f;

    arm->CreateFixture(&fixture);

    b2RevoluteJointDef jointDef;
    jointDef.Initialize( anchor, arm, anchor->GetWorldCenter());

    jointDef.enableMotor = true;
    jointDef.maxMotorTorque = 80.0f;

    joint = (b2RevoluteJoint*)world->CreateJoint(&jointDef);
}

void Pendulo::MoverIzquierda(){
    anchor->SetTransform( b2Vec2(anchor->GetPosition().x - 0.05f, anchor->GetPosition().y), anchor->GetAngle() );
    arm->SetTransform( b2Vec2(arm->GetPosition().x - 0.05f, arm->GetPosition().y), arm->GetAngle() );
}

void Pendulo::MoverDerecha(){
    anchor->SetTransform(b2Vec2(anchor->GetPosition().x + 0.05f, anchor->GetPosition().y), anchor->GetAngle());
    arm->SetTransform( b2Vec2(arm->GetPosition().x + 0.05f, arm->GetPosition().y), arm->GetAngle() );
}

void Pendulo::GirarIzquierda(){
    joint->SetMotorSpeed(2.0f);
}

void Pendulo::GirarDerecha(){
    joint->SetMotorSpeed(-2.0f);
}

void Pendulo::Detener(){
    joint->SetMotorSpeed(0.0f);
}

float Pendulo::GetX(){
    float centroX = arm->GetPosition().x * SCALE;
    float angulo = arm->GetAngle();

    return centroX + sinf(angulo) * 45.0f;
}

float Pendulo::GetY(){
    float centroY = SCREEN_HEIGHT - arm->GetPosition().y * SCALE;
    float angulo = arm->GetAngle();

    return centroY + cosf(angulo) * 45.0f;
}

void Pendulo::seguirMouse(){
    float mouseX = GetMouseX() / SCALE;

    float diferencia = mouseX - anchor->GetPosition().x;

    anchor->SetTransform( b2Vec2(mouseX, anchor->GetPosition().y), anchor->GetAngle());

    arm->SetTransform(b2Vec2(arm->GetPosition().x + diferencia, arm->GetPosition().y), arm->GetAngle());
}


void Pendulo::Dibujar(){
    b2Vec2 posicionAncla = anchor->GetPosition();
    b2Vec2 posicionBrazo = arm->GetPosition();

    Vector2 puntoAncla ={posicionAncla.x * SCALE, SCREEN_HEIGHT - posicionAncla.y * SCALE };

    Vector2 centroBrazo ={ posicionBrazo.x * SCALE, SCREEN_HEIGHT - posicionBrazo.y * SCALE};

    // Cuerda del pendulo
    DrawLineV(puntoAncla, centroBrazo, MAROON);

    // Punto fijo superior
    DrawCircleV( puntoAncla, 6, BLACK );

    // Brazo del pendulo
    Rectangle brazo ={ centroBrazo.x, centroBrazo.y, 16,90};

    Vector2 origenBrazo ={ 8, 45};

    float rotacion = -arm->GetAngle() * RAD2DEG;

    DrawRectanglePro( brazo, origenBrazo, rotacion, RED );
}