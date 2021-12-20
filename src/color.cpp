// Copyright (C) 2021 Matthieu Jacquemet, Riyad Ennouara, Nicolas Lerray
// 
// This file is part of Among Z.
// 
// Among Z is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// Among Z is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with Among Z.  If not, see <http://www.gnu.org/licenses/>.

#include "color.h"
#include "utils.h"


using namespace std;


void Color::set_temperature(float kelvin) {
    
    float temp = clamp(kelvin, 1000.0f, 40000.0f) / 100.0f;
    float r, g, b;

    if(temp <= 66.0f){ 
        r = 1.0f; 
        g = 0.39008157876f*log(temp) - 0.63184144378f;

    } else {
        r = 1.2929361861f*pow(temp-60.0f, -0.1332047592f);
        g = 1.1298908609f*pow(temp-60.0f, -0.0755148492f);
    }

    if (temp >= 66.0f)
        b = 1.0f;
    else if(temp <= 19.0f)
        b = 0.0f;
    else
        b = 0.54320678911f*log(temp-10.0f) - 305.0447927307f;

    set_x(saturate(r));
    set_y(saturate(g));
    set_z(saturate(b));
}


void Color::set_hsv(float h, float s, float v) {

    float r, g, b;
    float rem = h*3.0;
    int region = rem;
    rem -= region;
    float rei = 1.0-rem;

    switch (region) {
        case 0:  r=rei; g=rem; b=0.0;
            break;
        case 1:  r=0.0; g=rei; b=rem;
            break;
        case 2:  r=rem; g=0.0; b=rei;
            break;
        default: r=1.0; g=0.0; b=0.0;
            break;
    }
    
    float value = min(v, 1.0f);
    float c = (1.0 - min(s, 1.0f))*value;
    float m = s*value;

    set_x(r*m + c);
    set_y(g*m + c);
    set_z(b*m + c);
}


float Color::get_h() const {
    
    float cmax = max({get_x(),get_y(),get_z()});
    float cmin = min({get_x(),get_y(),get_z()});
    float delta = cmax - cmin;
    float h = 0.0f;

    if (cmax == get_x())
        h = (get_y() - get_z())/delta;
    else if (cmax == get_y())
        h = (get_z() - get_x())/delta + 2.0f;
    else if (cmax == get_z())
        h = (get_x() - get_y())/delta + 4.0f;

    h /= 6.0f;

    if (h < 0.0f)
        h += 1.0f;
    
    return h;
}


float Color::get_s() const {
    
    float cmax = max({get_x(),get_y(),get_z()});
    float cmin = min({get_x(),get_y(),get_z()});
    float delta = cmax - cmin;

    if (cmax == 0.0)
        return 0.0f;
    else
        return delta/cmax;
}


float Color::get_v() const {
    
    return max({get_x(),get_y(),get_z()});
}


void Color::set_h(float h) {
    
    set_hsv(h, get_s(), get_v());
}


void Color::set_s(float s) {
    
    set_hsv(get_h(), s, get_v());
}


void Color::set_v(float v) {
    
    set_hsv(get_h(), get_s(), v);
}


void Color::set_r(float r) {
    
    set_x(saturate(r));
}


void Color::set_g(float g) {
    
    set_y(saturate(g));
}


void Color::set_b(float b) {
    
    set_z(saturate(b));
}


float Color::get_r() const {
    
    return get_x();
}


float Color::get_g() const {

    return get_y();
}


float Color::get_b() const {

    return get_z();
}


Color::Color(float r, float g, float b, float a): 
    LColor(saturate(r), saturate(g), saturate(b), saturate(a))
{
}


Color::Color(): LColor(0.0,0.0,0.0,1.0) {
    
}

Color Color::black() {
    return Color();
}

Color Color::white() {
    return Color(1.0f,1.0f,1.0f);
}

Color Color::red() {
    return Color(1.0f,0.0f,0.0f);
}

Color Color::green() {
    return Color(0.0f,1.0f,0.0f);
}

Color Color::blue() {
    return Color(0.0f,0.0f,1.0f);
}

Color Color::yellow() {
    return Color(1.0f,1.0f,0.0f);
}

Color Color::cyan() {
    return Color(0.0f,1.0f,1.0f);
}

Color Color::magenta() {
    return Color(1.0f,0.0f,1.0f);
}

Color Color::transparent() {
    return Color(0.0f,0.0f,0.0f,0.0f);
}

/**
 * @brief Constructeur pour initialiser la couleur à partir de son code hexadecimal
 * @code Color("ffffff"); // pour du blanc
*/
Color::Color(const string& hex_value) {

    nassertv(hex_value.length() == 6)

    unsigned char r, g, b;   
    stringstream ss;

    ss << hex << hex_value;
    ss >> r >> g >> b;

    set_x(float(r) / 255.0f); 
    set_y(float(g) / 255.0f);
    set_z(float(b) / 255.0f);
    
    set_w(1.0f);
}

Color::Color(const Color& copy):
    LColor(copy)
{
    
}

Color::Color(const LVecBase4f& copy):
    LColor(copy)
{
    
}

// void Color::to_hsv(float& h, float& s, float& v) const {

//     float cmax = max({get_x(),get_y(),get_z()});
//     float cmin = min({get_x(),get_y(),get_z()});
//     float delta = cmax - cmin;

//     if (delta == 0.0)
//         h = 0.0;
//     else if (cmax == get_x())
//         h = (get_y() - get_z())/delta;
//     else if (cmax == get_y())
//         h = (get_z() - get_x())/delta + 2;
//     else if (cmax == get_z())
//         h = (get_x() - get_y())/delta + 4;

//     h /= 6;
//     if (h < 0)
//         h += 1;

//     if (cmax == 0.0)
//         s = 0.0;
//     else
//         s = delta/cmax;
// }




// Color::Color(const ColorPalette& code) {
    
//     switch(code) {
//         case C_black:   set_x(0); set_y(0), set_z(0);
//             break;
//         case C_white:   set_x(1); set_y(1), set_z(1);
//             break;
//         case C_red:     set_x(1); set_y(0), set_z(0);
//             break;
//         case C_green:   set_x(0); set_y(1), set_z(0);
//             break;
//         case C_blue:    set_x(0); set_y(0), set_z(1);
//             break;
//         case C_yellow:  set_x(1); set_y(1), set_z(0);
//             break;
//         case C_cyan:    set_x(0); set_y(1), set_z(1);
//             break;
//         case C_magenta: set_x(1); set_y(0), set_z(1);
//             break;
//     }
// }



