
enum EasingFunction {

    EF_linear,

    EF_in_sin,
    EF_out_sin,
    EF_in_out_sin,

    EF_in_quad,
    EF_out_quad,
    EF_in_out_quad,

    EF_in_cubic,
    EF_out_cubic,
    EF_in_out_cubic,

    EF_in_quart,
    EF_out_quart,
    EF_in_out_quart,

    EF_in_quint,
    EF_out_quint,
    EF_in_out_quint,

    EF_in_expo,
    EF_out_expo,
    EF_in_out_expo,

    EF_in_circ,
    EF_out_circ,
    EF_in_out_circ,

    EF_in_back,
    EF_out_back,
    EF_in_out_back,

    EF_in_elastic,
    EF_out_elastic,
    EF_in_out_elastic,

    EF_in_bounce,
    EF_out_bounce,
    EF_in_out_bounce,
};


float ease_in_sin(float t);
float ease_out_sin(float t);
float ease_in_out_sin(float t);
float ease_in_quad(float t);
float ease_out_quad(float t);
float ease_in_out_quad(float t);
float ease_in_cubic(float t);
float ease_out_cubic(float t);
float ease_in_out_cubic(float t);
float ease_in_quart(float t);
float ease_out_quart(float t);
float ease_in_out_quart(float t);
float ease_in_quint(float t);
float ease_out_quint(float t);
float ease_in_out_quint(float t);
float ease_in_expo(float t);
float ease_out_expo(float t);
float ease_in_out_expo(float t);
float ease_in_circ(float t);
float ease_out_circ(float t);
float ease_in_out_circ(float t);
float ease_in_back(float t);
float ease_out_back(float t);
float ease_in_out_back(float t);
float ease_in_elastic(float t);
float ease_out_elastic(float t);
float ease_in_out_elastic(float t);
float ease_in_bounce(float t);
float ease_out_bounce(float t);
float ease_in_out_bounce(float t);

float ease(float t, EasingFunction function);

template<class T>
inline T lerp(T a, T b, float t) {
    return a + t * (b - a);
}

template<class T>
inline T ease(T a, T b, float t, EasingFunction function=EF_linear) {
    
    if (function != EF_linear)
        t = ease(t, function);
    
    return lerp(a, b, t);
}