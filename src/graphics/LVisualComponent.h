#ifndef LVISUALCOMPONENT_H
#define LVISUALCOMPONENT_H
/*

Abstract class for visual components

*/
class LVisualComponent {
    public:
        virtual void render() = 0;
        virtual void free() = 0;
};

#endif