#include "math.h"
#include "easing.h"


float ease_in_sin(float t) {
	return sinf(1.5707963f * t);
}

float ease_out_sin(float t) {
	return 1.0f + sinf(M_PI_2 * (--t));
}

float ease_in_out_sin(float t) {
	return 0.5f * (1 + sinf(M_PI * (t - 0.5f)));
}

float ease_in_quad(float t) {
    return t * t;
}

float ease_out_quad(float t) { 
    return t * (2.0f - t);
}

float ease_in_out_quad(float t) {
    return t < 0.5f ? 2.0f * t * t : t * (4.0f - 2.0f * t) - 1.0f;
}

float ease_in_cubic(float t) {
    return t * t * t;
}

float ease_out_cubic(float t) {
    return 1.0f + (--t) * t * t;
}

float ease_in_out_cubic(float t) {
    return t < 0.5f ? 4.0f * t * t * t : 1.0f + (--t) * (2.0f * (--t)) * (2.0f * t);
}

float ease_in_quart(float t) {
    t *= t;
    return t * t;
}

float ease_out_quart(float t) {
    t = (--t) * t;
    return 1.0f - t * t;
}

float ease_in_out_quart(float t) {
    if(t < 0.5f) {
        t *= t;
        return 8.0f * t * t;
    } else {
        t = (--t) * t;
        return 1.0f - 8.0f * t * t;
    }
}

float ease_in_quint(float t) {
    float t2 = t * t;
    return t * t2 * t2;
}

float ease_out_quint(float t) {
    float t2 = (--t) * t;
    return 1.0f + t * t2 * t2;
}

float ease_in_out_quint(float t) {
    float t2;
    if(t < 0.5f) {
        t2 = t * t;
        return 16.0f * t * t2 * t2;
    } else {
        t2 = (--t) * t;
        return 1.0f + 16.0f * t * t2 * t2;
    }
}

float ease_in_expo(float t) {
    return (pow(2.0f, 8.0f * t ) - 1.0f) / 255.0f;
}

float ease_out_expo(float t) {
    return 1 - pow(2.0f, -8.0f * t);
}

float ease_in_out_expo(float t) {
    if(t < 0.5f)
        return (pow(2.0f, 16.0f * t) - 1.0f) / 510.0f;
    else
        return 1.0f - 0.5f * pow( 2.0f, -16.0f * (t - 0.5f));
}

float ease_in_circ(float t) {
    return 1.0f - sqrt(1.0f - t);
}

float ease_out_circ(float t) {
    return sqrt(t);
}

float ease_in_out_circ(float t) {
    if(t < 0.5f)
        return (1.0f - sqrt(1.0f - 2.0f * t)) * 0.5f;
    else
        return (1.0f + sqrt(2.0f * t - 1.0f)) * 0.5f;
}

float ease_in_back(float t) {
    return t * t * (2.70158f * t - 1.70158f);
}

float ease_out_back(float t) {
    return 1.0f + (--t) * t * (2.70158f * t + 1.70158f);
}

float ease_in_out_back(float t) {
    if(t < 0.5f)
        return t * t * (7.0f * t - 2.5f) * 2.0f;
    else
        return 1.0f + (--t) * t * 2.0f * (7.0f * t + 2.5f);
}

float ease_in_elastic(float t) {
    float t2 = t * t;
    return t2 * t2 * sinf(t * M_PI * 4.5f);
}

float ease_out_elastic(float t) {
    float t2 = (t - 1.0f) * (t - 1.0f);
    return 1.0f - t2 * t2 * cosf(t * M_PI * 4.5f);
}

float ease_in_out_elastic(float t) {
    float t2;
    if(t < 0.45f) {
        t2 = t * t;
        return 8.0f * t2 * t2 * sinf(t * M_PI * 9.0f);
    } else if (t < 0.55f)
        return 0.5f + 0.75f * sinf(t * M_PI * 4.0f);
    else {
        t2 = (t - 1.0f) * (t - 1.0f);
        return 1.0f - 8.0f * t2 * t2 * sinf(t * M_PI * 9.0f);
    }
}

float ease_in_bounce(float t) {
    return pow(2.0f, 6.0f * (t - 1.0f)) * abs(sinf(t * M_PI * 3.5f));
}

float ease_out_bounce(float t) {
    return 1.0f - pow(2.0f, -6.0f * t) * abs(cosf(t * M_PI * 3.5f));
}

float ease_in_out_bounce(float t) {
    if(t < 0.5f)
        return 8.0f * pow(2.0f, 8.0f * (t - 1)) * abs(sinf(t * M_PI * 7.0f));
    else
        return 1.0f - 8.0f * pow(2.0f, -8.0f * t) * abs(sinf(t * M_PI * 7.0f));
}


float ease(float t, EasingFunction function) {

    switch (function) {
        case EF_linear:         return t;
        case EF_in_sin:         return ease_in_sin(t);
        case EF_out_sin:        return ease_in_out_sin(t);
        case EF_in_quad:        return ease_in_quad(t);
        case EF_out_quad:       return ease_out_quad(t);
        case EF_in_out_quad:    return ease_in_out_quad(t);
        case EF_in_cubic:       return ease_in_cubic(t);
        case EF_out_cubic:      return ease_out_cubic(t);
        case EF_in_out_cubic:   return ease_in_out_cubic(t);
        case EF_in_quart:       return ease_in_quart(t);
        case EF_out_quart:      return ease_out_quart(t);
        case EF_in_out_quart:   return ease_in_out_quart(t);
        case EF_in_quint:       return ease_in_quint(t);
        case EF_out_quint:      return ease_out_quint(t);
        case EF_in_out_quint:   return ease_in_out_quint(t);
        case EF_in_expo:        return ease_in_expo(t);
        case EF_out_expo:       return ease_out_expo(t);
        case EF_in_out_expo:    return ease_in_out_expo(t);
        case EF_in_circ:        return ease_in_circ(t);
        case EF_out_circ:       return ease_out_circ(t);
        case EF_in_out_circ:    return ease_in_out_circ(t);
        case EF_in_back:        return ease_in_back(t);
        case EF_out_back:       return ease_out_back(t);
        case EF_in_out_back:    return ease_in_out_back(t);
        case EF_in_elastic:     return ease_in_elastic(t);
        case EF_out_elastic:    return ease_out_elastic(t);
        case EF_in_out_elastic: return ease_in_out_elastic(t);
        case EF_in_bounce:      return ease_in_bounce(t);
        case EF_out_bounce:     return ease_out_bounce(t);
        case EF_in_out_bounce:  return ease_in_out_bounce(t);
    }
    return 0.0f;
}