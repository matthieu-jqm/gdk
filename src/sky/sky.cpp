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

#include "scene.h"
#include "sky.h"


using namespace std;

DEFINE_TYPEHANDLE(Sky)


float gamma(float value, float gamma) {
    return pow(value, 1.0f / gamma);
}


Sky::CData::CData():
    _atmosphere_density(1.2),
    _atmosphere_height(800.0f),
    _atmosphere_intensity(3.0f),
    _atmosphere_color(0.546f, 0.312f, 1.0f),
    _atmosphere_inscattering(0.615f, 0.826f, 0.860f),
    _atmosphere_extinction(0.636f, 0.958f, 0.351f),
    _mie_scattering(4.0f),
    _mie_anisotropy(0.76f),
    _distance_scale(2.0f),
    _atmosphere_falloff(1.0f),
    _radiance_gamma(1.0f),
    _ground_albedo(0.0f, 0.127f, 0.214f),
    _ground_offset(0.0f)
{
    
}

Sky::CData::CData(const CData& copy):
    _atmosphere_density(copy._atmosphere_density),
    _atmosphere_height(copy._atmosphere_height),
    _atmosphere_intensity(copy._atmosphere_intensity),
    _atmosphere_color(copy._atmosphere_color),
    _atmosphere_inscattering(copy._atmosphere_inscattering),
    _atmosphere_extinction(copy._atmosphere_extinction),
    _mie_scattering(copy._mie_scattering),
    _mie_anisotropy(copy._mie_anisotropy),
    _distance_scale(copy._distance_scale),
    _atmosphere_falloff(copy._atmosphere_falloff),
    _radiance_gamma(copy._radiance_gamma),
    _ground_albedo(copy._ground_albedo),
    _ground_offset(copy._ground_offset)
{
    
}

CycleData* Sky::CData::make_copy() const {
    return new CData(*this);
}


void Sky::CData::write_datagram(BamWriter* manager, Datagram& dg) const {

    dg.add_float32(_atmosphere_density);
    dg.add_float32(_atmosphere_height);
    dg.add_float32(_atmosphere_intensity);

    _atmosphere_color.write_datagram(dg);
    _atmosphere_inscattering.write_datagram(dg);
    _atmosphere_extinction.write_datagram(dg);

    dg.add_float32(_mie_scattering);
    dg.add_float32(_mie_anisotropy);

    dg.add_float32(_distance_scale);
    dg.add_float32(_atmosphere_falloff);
    dg.add_float32(_radiance_gamma);

    _ground_albedo.write_datagram(dg);
    dg.add_float32(_ground_offset);
}


void Sky::CData::fillin(DatagramIterator& scan, BamReader* manager) {

    _atmosphere_density = scan.get_float32();
    _atmosphere_height = scan.get_float32();
    _atmosphere_intensity = scan.get_float32();

    _atmosphere_color.read_datagram(scan);
    _atmosphere_inscattering.read_datagram(scan);
    _atmosphere_extinction.read_datagram(scan);

    _mie_scattering = scan.get_float32();
    _mie_anisotropy = scan.get_float32();

    _distance_scale = scan.get_float32();
    _atmosphere_falloff = scan.get_float32();
    _radiance_gamma = scan.get_float32();

    _ground_albedo.read_datagram(scan);
    _ground_offset = scan.get_float32();
}

void Sky::write_datagram(BamWriter* manager, Datagram& dg) {
    
    Component::write_datagram(manager, dg);
}

void Sky::fillin(DatagramIterator& scan, BamReader* manager) {
    
    Component::fillin(scan, manager);
}


void Sky::render(RenderStage* stage) {
    
}


void Sky::update() {
    
    CDReader cdata(_cycler);

    PT(DirectionalLightNode) light = nullptr;

    if (PT(Scene) scene = _scene.lock())
        light = scene->get_light();
    
    if (light == nullptr)
        return;


	Color light_color = light->get_color();
    float intensity = light->get_energy();
    float diameter = light->get_angular_diameter();
    float height = cdata->_atmosphere_height;
    float density = cdata->_atmosphere_density;


    NodePath np(light);
    CPT(TransformState) transform = np.get_net_transform();
    LMatrix3 mat = transform->get_mat3();

	// Calculating the sun intensity and color
	LVector3 ray = normalize(-mat.get_row(2));
	LVector3 p = ray * 100000;
	LVector3 eye(0.0f, 0.0f, 0.5); 

	float a = 1.0f - exp(p.get_z() / height);
	float b = exp(eye.get_z() / height) * density * 0.1f;
	float fog = -(a * b) / ray.get_z();

	fog = pow(fog, 1.1f);
	fog = pow(fog, cdata->_atmosphere_falloff);
	fog *= -cdata->_distance_scale;


    Color scatter = cdata->_atmosphere_inscattering * fog;
    Color absorption = cdata->_atmosphere_extinction * fog;

    Color atmosphere_color = cdata->_atmosphere_color;
    Color light_radiance = light_color * intensity;

	float scatR = (1.0f - exp(scatter.get_r())) * atmosphere_color.get_r();
	float scatG = (1.0f - exp(scatter.get_g())) * atmosphere_color.get_g();
	float scatB = (1.0f - exp(scatter.get_b())) * atmosphere_color.get_b();
	
	float absorptR = exp(absorption.get_r()) * light_radiance.get_r();
	float absorptG = exp(absorption.get_g()) * light_radiance.get_g();
	float absorptB = exp(absorption.get_b()) * light_radiance.get_b();

	
	// Calculate basic attenuation based on sun diameter
	float denom = 2.0f / diameter + 1.0f;
	float lamp_intensity = 1.0f / (denom * denom);

    float G = cdata->_radiance_gamma;

	light_color.set_r(gamma((scatR + absorptR) * lamp_intensity, G));
    light_color.set_g(gamma((scatG + absorptG) * lamp_intensity, G));
    light_color.set_b(gamma((scatB + absorptB) * lamp_intensity, G));

	float vert = ray.dot(LVector3::down());
    light_color *= smoothstep(vert, -diameter * 0.5f, diameter * 0.5f);

    _light_color[0] = light_color.get_xyz();
    _angular_diameter[0] = diameter;
}


TypedWritable* Sky::make_from_bam(const FactoryParams& params) {
    
    Sky* me = new Sky;
    DatagramIterator scan;
    BamReader* manager;

    parse_params(params, scan, manager);
    me->fillin(scan, manager);
    manager->register_finalize(me);

    return me;
}
