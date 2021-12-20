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

using namespace std;

DEFINE_TYPEHANDLE(Scene)


void Scene::add_for_draw(CullTraverser* trav, CullTraverserData& data) {
    
    if (trav->is_of_type(PipelineCullTraverser::get_class_type())) {
        PipelineCullTraverser* traverser = (PipelineCullTraverser*)trav;

        traverser->record_node(data);
    }
}


TypedWritable* Scene::make_from_bam(const FactoryParams& params) {
    
    TypedWritable* me = new TypedWritable;
    DatagramIterator scan;
    BamReader* manager;

    parse_params(params, scan, manager);
    me->fillin(scan, manager);
    manager->register_finalize(me);

    return me;
}


Scene::Scene(const std::string& name):
    LocalNodePath<ModelRoot>(name)
{
    
}


DirectionalLightNode* Scene::get_light() const {
    return _light;
}

void Scene::write_datagram(BamWriter* manager, Datagram& dg) {
    LocalNodePath<ModelRoot>::write_datagram(manager, dg);
}

void Scene::fillin(DatagramIterator& scan, BamReader* manager) {
    LocalNodePath<ModelRoot>::fillin(scan , manager);
}
