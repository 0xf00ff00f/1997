#pragma once

#include "noncopyable.h"

class effect : private noncopyable
{
public:
    effect(int width, int height);
    virtual ~effect() = default;

    virtual void redraw() = 0;

private:
    int width_;
    int height_;
};
