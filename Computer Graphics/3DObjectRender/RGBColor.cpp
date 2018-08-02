

#include "RGBColor.hpp"


RGBColor::RGBColor() {
    
}

RGBColor::RGBColor(float rr, float gg, float bb) {
    r = rr;
    g = gg;
    b = bb;
}

RGBColor RGBColor::addColor(const RGBColor &c) {
    return RGBColor(r+c.r, g+c.g, b+c.b);
}


RGBColor RGBColor::crossColor(const RGBColor &c) {
    return RGBColor(r*c.r, g*c.g, b*c.b);
}

RGBColor RGBColor::mulColor(float s) {
    return RGBColor(r*s, g*s, b*s);
}


void RGBColor::saturateColor() {
    r = r < 1.0? r:1.0;
    g = g < 1.0? g:1.0;
    b = b < 1.0? b:1.0;
}


void RGBColor::showColor() {
    cout<<"r:"<<r<<"g:"<<g<<"b:"<<b<<endl;
}