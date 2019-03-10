#pragma once


#include "../Common.h"

#include "../Entity/ITimeVarying.h"
#include "../Model/Model.h"


class Satellite : public ITimeVarying
{
public:
    Satellite(const Model& model);

    Satellite(const Satellite& satellite);

    Satellite(Satellite&& satellite);


    Satellite& operator = (const Satellite& satellite);

    Satellite& operator = (Satellite&& satellite);



    Model& model();

    const Model& model() const;


    void thrust(const Vec3& dir);


    //ITime varying
    virtual void update(float t, float dt) override;


private:
    Model m_model;
};