
#ifndef RGBColor_hpp
#define RGBColor_hpp

#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <iostream>

using namespace std;

class RGBColor {
public:
    float r;
    float g;
    float b;
    RGBColor();
    
    RGBColor(float rr, float gg, float bb);
    
    RGBColor addColor(const RGBColor& c);
    
    RGBColor mulColor(float s);
    
    RGBColor crossColor(const RGBColor& c);
    
    void saturateColor();
    
    void showColor();
    
    static inline RGBColor black(){ return RGBColor(0,0,0); }
    static inline RGBColor white(){ return RGBColor(1,1,1); }
    static inline RGBColor red()  { return RGBColor(1,0,0); }
    static inline RGBColor green(){ return RGBColor(0,1,0); }
    static inline RGBColor blue() { return RGBColor(0,0,1); }
    static inline RGBColor yellow()  { return RGBColor(1,1,0); }
    static inline RGBColor cyan(){ return RGBColor(0,1,1); }
    static inline RGBColor purple() { return RGBColor(1,0,1); }
};
#endif /* RGBColor_hpp */
