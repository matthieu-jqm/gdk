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

#include "renderPipeline.h"
#include "config_render_pipeline.h"
#include "lighting.h"


template<LightType T>
DEFINE_TYPEHANDLE(LightPool<T>)


template<LightType T>
void LightPool<T>::setup(RenderPipeline* pipeline) {
    
    if (_lods.size() < max_light_lods)
        _lods.reserve(max_light_lods - _lods.size());

    while (_lods.size() < max_light_lods)
        _lods.resize(_lods.size() + 1, _lods.size());
}


template<LightType T>
void LightPool<T>::record_node(CullTraverserData& data,
                        const PipelineCullTraverser* trav)
{
    LightPoolBase::record_node(data, trav);

 
    nassertv(_lods.size() > 0)

    const Geom* geom = _lods[0].get_geom();
    CPT(BoundingVolume) volume = geom->get_bounds();

    const GeometricBoundingVolume* gbv = volume->as_geometric_bounding_volume();
    nassertv(gbv != nullptr);

    CPT(TransformState) transform = data.get_internal_transform(trav);
    LPoint3 center = gbv->get_approx_center();
    center = center * transform->get_mat();

    GraphicsStateGuardian* gsg = DCAST(GraphicsStateGuardian, trav->get_gsg());

    int lod = gsg->compute_distance_to(center) * 0.1f;
    int max_lod = _lods.size() - 1;
    lod = clamp(max_lod - lod, 0, max_lod);

    _lods[lod].add_instance(gsg, data.node(), transform);
}



template<LightType T>
void LightPool<T>::post_cull(const PipelineCullTraverser* trav) {

    nassertv(_stage != nullptr)
    int count = 0;

    LightPoolBase::post_cull(trav);

    for (LodType& lod: _lods) {

        size_t num_instances = lod.get_num_instances();

        if (num_instances == 0)
            continue;

        _state.set_shader_input("u_instanceOffset", PTA_int(1, count));
        count += num_instances;

        CullableObject* object = new CullableObject(lod.get_geom(), _state,
                                        TransformState::make_identity());

        object->_instances = lod.get_instances();
        trav->get_cull_handler()->record_object(object, trav);

        lod.reset();
    }
}


template<LightType T>
void LightPool<T>::commit_data(uchar* data) {

    for (LodType& lod: _lods)
        lod.collect_data(data);
}



template<LightType T>
void LightPool<T>::set_stage(Lighting* stage) {

    LightPoolBase::set_stage(stage);
    _state.compose(stage->get_binning_state());
}


template<LightType T>
LightPool<T>::LightPool():
    LightPoolBase(get_light_name(T), sizeof(DataType))
{
    _state.set_shader_input("u_lightType", PTA_int(1, T));
}


template<LightType T>
LightPoolBase* LightPool<T>::make() {
    return new LightPool<T>;
}


template<LightType T>
void LightPool<T>::post_init() {
    _contructors[T] = make;
}


template<LightType T>
TypeHandle LightPool<T>::get_node_type() const {
    return DataType::get_class_type();
}