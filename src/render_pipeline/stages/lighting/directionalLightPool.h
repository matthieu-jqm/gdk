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

#ifndef __DIRECTIONALLIGHTPOOL_H__
#define __DIRECTIONALLIGHTPOOL_H__


#include "lightPool.h"


class DirectionalLightPool: public LightPoolBase {

    REGISTER_TYPE("DirectionalLightPool", LightPoolBase)

public:
    virtual ~DirectionalLightPool() = default;

protected:
    using DataType = LightData<LT_directional>;

    DirectionalLightPool();

    virtual void record_node(CullTraverserData& data,
                            const PipelineCullTraverser* trav) override;

    virtual TypeHandle get_node_type() const override;

private:
    static void post_init();
    static LightPoolBase* make();
};


#endif // __DIRECTIONALLIGHTPOOL_H__