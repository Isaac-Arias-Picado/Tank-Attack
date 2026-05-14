#pragma once
class Object {
protected:
    int id;           // identificador del objeto
    bool activo;      // si esta vivo en el juego

public:
    Object();
    virtual ~Object();

    int getId() const;
    bool isActivo() const;
    void setActivo(bool activo);

    virtual const char* getTipo() const = 0;
};