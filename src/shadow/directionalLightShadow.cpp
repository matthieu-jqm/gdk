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

#include <panda3d/lightLensNode.h>

#include "shadowBufferManager.h"
#include "directionalLightNode.h"
#include "directionalLightShadow.h"


using namespace std;

DEFINE_TYPEHANDLE(DirectionalLightShadow)


ConfigVariableInt num_shadow_cascade
    ("num-shadow-cascade", 4,
    PRC_DESC("The number of frustum split used for cascaded shadow maps"));


ConfigVariableDouble csm_log_factor
    ("csm-log-factor", 0.5,
    PRC_DESC("Setting this value to 0 will evenly distribute shadow "
            "cascades along the view frustum but will result in undersampling "
            "near the camera and oversampling far from the camera."
            "Higher value has the effect of making cascade smaller as "
            "they are closer to the camera, and thus has a better sample "
            "distribution"));

// void DirectionalLightShadow::setup() {
    
//     ShadowBuffer::setup();

//     ShadowBufferManager* mgr = ShadowBufferManager::get_global_ptr();

//     _texture->setup_2d_texture_array(   _resolution.get_x(),
//                                         _resolution.get_y(),
//                                         num_shadow_cascade * _slots.size(),
//                                         Texture::T_unsigned_byte,
//                                         Texture::F_depth_component);

//     setup_shadow_buffer(mgr->get_host());
// }



// void DirectionalLightShadow::update() {

//     // ShadowBufferManager* mgr = ShadowBufferManager::get_global_ptr();

//     // for (DisplayRegion* dr : mgr->_views) {
        
//     //     NodePath camera = dr->get_camera();
        
//     //     Camera* camera_node = DCAST(Camera, camera.node());

//     //     int lens_index = dr->get_lens_index();
//     //     const Lens* lens = camera_node->get_lens(lens_index);

//     // }
// }

// TypeHandle DirectionalLightShadow::get_light_type() const {
//     return DirectionalLightNode::get_class_type();
// }


// void DirectionalLightShadow::compute_csm(SceneSetup* setup,
//                                             LightLensNode* light)
// {

//     NodePath scene_root = setup->get_scene_root();
//     NodePath cull_center = setup->get_cull_center();
//     NodePath scene_parent = scene_root.get_parent();
//     NodePath light_np = NodePath::any_path(light->as_node());

//     CPT(TransformState) transform =
//         cull_center.get_transform(scene_parent);

//     CPT(TransformState) light_trans =
//         light_np.get_transform(scene_parent);

//     light_trans = light_trans->set_pos(0.0);
//     light_trans = light_trans->set_scale(1.0);

//     transform = transform->compose(light_trans);

//     const Lens* lens = setup->get_lens();

//     float near = lens->get_near();
//     float far = lens->get_far();


//     for (uint8_t i=0; i<_num_splits; ++i) {

//         float split_near = lerp(near, far, compute_split_depth(i));
//         float split_far  = lerp(near, far, compute_split_depth(i + 1));
        
//         PT(Lens) split_lens = lens->make_copy();
//         split_lens->set_near_far(split_near, split_far);

//         PT(BoundingVolume) bounds = split_lens->make_bounds();
//         BoundingHexahedron* gbv = DCAST(BoundingHexahedron,
//             bounds->as_geometric_bounding_volume());

//         gbv->xform(transform->get_mat());

//         LPoint3 bound_min = gbv->get_min();
//         LPoint3 bound_max = gbv->get_max();

//         LPoint3 center = gbv->get_approx_center();
//     }
// }


// float DirectionalLightShadow::compute_split_depth(uint8_t n) const {

//     float t = float(n) / float(_num_splits);
//     return (expf(_log_factor * t) - 1.0f) / (expf(_log_factor) - 1.0f);
// }




DirectionalLightShadow::DirectionalLightShadow(ShadowBuffer* buffer):
    ShadowSlot(buffer)
{
    
}


ShadowSlot* DirectionalLightShadow::make(ShadowBuffer* buffer) {
    return new DirectionalLightShadow(buffer);
}


ShadowBufferInfo DirectionalLightShadow::get_info() {

    ShadowBufferInfo info;
    info.name = "point";
    info.light_type = DirectionalLightNode::get_class_type();
    info.buffer_type = Texture::TT_2d_texture_array;
    info.views = num_shadow_cascade;
    info.flags = 0;
    info.make_slot = make;

    return info;
}
