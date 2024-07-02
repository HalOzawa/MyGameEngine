#pragma once
#include "Quad.h"
#include "Transform.h"

class Dice :
    public Quad
{

public:
	Dice();
	~Dice();
	virtual HRESULT Initialize();
	virtual void Draw(Transform &transform);
};

