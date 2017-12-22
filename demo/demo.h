#pragma once

#include "noncopyable.h"
#include "effect.h"
#include <memory>

class demo : private noncopyable
{
public:
    demo(int width, int height);
    ~demo();

    void redraw();

private:
    int width_;
    int height_;
    std::unique_ptr<effect> effect_;
};