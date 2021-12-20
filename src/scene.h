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

#ifndef __SCENE_H__
#define __SCENE_H__

#include <panda3d/modelRoot.h>

#include "directionalLightNode.h"
#include "color.h"
#include "utils.h"


class Scene: public LocalNodePath<ModelRoot> {

    REGISTER_TYPE("Scene", LocalNodePath<ModelRoot>)

public:
    Scene(const std::string& name="");
    virtual ~Scene() = default;

    DirectionalLightNode* get_light() const;

protected:
    virtual void write_datagram(BamWriter* manager, Datagram& dg) override;
    virtual void fillin(DatagramIterator& scan, BamReader* manager) override;

    virtual void add_for_draw(CullTraverser* trav, CullTraverserData& data) override;

    PT(DirectionalLightNode) _light;

private:
    static TypedWritable *make_from_bam(const FactoryParams& params);
};



#endif // __SCENE_H__