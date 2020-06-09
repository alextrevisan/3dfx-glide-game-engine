#ifndef COMPONENT_H
#define COMPONENT_H


class Component
{
public:
    Component();
    Component(const Component&) = delete;
    virtual ~Component();
};

#endif // COMPONENT_H
