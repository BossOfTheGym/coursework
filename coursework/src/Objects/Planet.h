#pragma once


#include "../Common.h"

#include "../Entity/ITimeVarying.h"
#include "../Model/Model.h"


class Planet : public ITimeVarying
{
public:
    Planet(const Model& model);

    Planet(const Planet& satellite);

    Planet(Planet&& satellite);


    Planet& operator = (const Planet& satellite);

    Planet& operator = (Planet&& satellite);



    Model& model();

    const Model& model() const;


    void thrust(const Vec3& dir);


    //ITime varying
    virtual void update(float t, float dt) override;


private:
    Model m_model;
};