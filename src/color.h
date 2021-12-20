
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

#ifndef __COLOR_H__
#define __COLOR_H__

#include <panda3d/aa_luse.h>
#include <string>
 

class Color: public LColor {

public:
    Color();
    Color(const Color& copy);
    Color(const LVecBase4f& copy);
    Color(const std::string& hex_value);
    Color(float r, float g, float b, float a=1.0f);
    Color(unsigned char r, unsigned char g, unsigned char b, float a=1.0f);
    ~Color() = default;

    static Color black();
    static Color white();
    static Color red();
    static Color green();
    static Color blue();
    static Color yellow();
    static Color cyan();
    static Color magenta();
    static Color transparent();

    static Color from_hsv(float h, float s, float v, float a=1.0f);

    void set_temperature(float kelvin);
    void set_hsv(float h, float s, float v);

    float get_h() const;
    float get_s() const;
    float get_v() const;

    void set_h(float h);
    void set_s(float s);
    void set_v(float v);

    void set_r(float r);
    void set_g(float g);
    void set_b(float b);
    void set_a(float a);
    
    void set_r(unsigned char r);
    void set_g(unsigned char g);
    void set_b(unsigned char b);
    void set_a(unsigned char a);

    unsigned char get_r_c() const;
    unsigned char get_g_c() const;
    unsigned char get_b_c() const;
    unsigned char get_a_c() const;

    float get_r() const;
    float get_g() const;
    float get_b() const;
    float get_a() const;
};

#endif // __COLOR_H__