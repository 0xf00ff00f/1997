#pragma once

#include "effect.h"

class text : public effect
{
public:
    using effect::effect;

    void redraw(unsigned time) override;
};
