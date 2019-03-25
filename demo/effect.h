#pragma once

#include "noncopyable.h"

class effect : private noncopyable
{
public:
    effect(int width, int height);
    virtual ~effect() = default;

    virtual void redraw(long elapsed) = 0;

protected:
    int width_;
    int height_;
};
