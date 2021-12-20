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

#include "directionalLightPool.h"
#include "directionalLightNode.h"


using namespace std;


DEFINE_TYPEHANDLE(DirectionalLightPool)


DirectionalLightPool::DirectionalLightPool():
    LightPoolBase(get_light_name(LT_directional), sizeof(DataType))
{
    // _min_buffer_size = sizeof(DataType);
}



void DirectionalLightPool::record_node(CullTraverserData& data,
                                const PipelineCullTraverser* trav)
{
    LightPoolBase::record_node(data, trav);


    CPT(TransformState) transform = data.get_internal_transform(trav);
    LMatrix4 mat = transform->get_mat();

    DirectionalLightNode* light = DCAST(DirectionalLightNode, data.node());

    DataType light_data;

    light_data.color = light->get_color().get_xyz() * light->get_energy();
    light_data.shadow_id = light->get_shadow_slot();
    light_data.direction = -normalize(mat.get_row3(2));
    light_data.angular_diameter = light->get_angular_diameter();

    set_data(&light_data);
}


TypeHandle DirectionalLightPool::get_node_type() const {
    return DataType::get_class_type();    
}


void DirectionalLightPool::post_init() {
    _contructors[LT_directional] = make;
}


LightPoolBase* DirectionalLightPool::make() {
    return new DirectionalLightPool;
}
