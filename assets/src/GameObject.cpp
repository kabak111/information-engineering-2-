#include "GameObject.h"

bool GameObject::isAlive() const
{
    return alive;
}

void GameObject::destroy()
{
    alive = false;
}
