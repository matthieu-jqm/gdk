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

#ifndef __INSTANCEMANAGER_H__
#define __INSTANCEMANAGER_H__

#include <panda3d/geomNode.h>
#include <panda3d/instanceList.h>
#include "render_utils.h"
#include "utils.h"

class InstanceManager;
class InstancePool;

template<class T>
class InstancedObject: public T {

    REGISTER_TEMPLATE("InstancedObject", T)

public:
    template<class... Args>
    InstancedObject(const std::string& name, Args&&... args);

    virtual ~InstancedObject() = default;
    virtual bool is_renderable() const override;
    virtual bool safe_to_combine() const override;
    virtual bool safe_to_flatten() const override;
    
protected:
    virtual void add_for_draw(CullTraverser* trav, CullTraverserData& data) override;
    InstanceManager* get_manager();

    PT(InstancePool) _pool;
};



class InstancePool: public TypedReferenceCount, public Namable {

    REGISTER_TYPE("InstancePool", TypedReferenceCount, Namable)

public:
    InstancePool(const std::string& name);
    virtual ~InstancePool() = default;

    virtual void add_instance(CullTraverser* trav, CullTraverserData& data);
    virtual void collect_instances(CullTraverser* trav, CullTraverserData& data);
};




class InstanceManager: public PandaNode {

    REGISTER_TYPE("InstanceManager", PandaNode)

public:
    InstanceManager(const std::string& name="");
    virtual ~InstanceManager() = default;

    InstancePool* get_instance_pool(TypeHandle type) const;

    void add_instance_pool(TypeHandle type, InstancePool* pool);

    virtual bool is_renderable() const override;
    virtual bool safe_to_combine() const override;
    virtual bool safe_to_flatten() const override;

private:
    static Geom* get_geom_data();

    typedef std::map<TypeHandle, PT(InstancePool)> InstancePools;
    InstancePools _pools;


    virtual void add_for_draw(CullTraverser* trav, CullTraverserData& data) override;
};


// template<class T>
// class InstancePoolSSBO: public InstancePool {

//     REGISTER_TEMPLATE("InstancePoolSSBO", InstancePool)

// public:
//     InstancePoolSSBO(const std::string& name,
//                     const Geom* geom, const RenderState* state, 
//                     CPT_InternalName id);

//     virtual ~InstancePoolSSBO() = default;


//     ShaderBufferData<T> _data;

// private:
//     CPT(RenderState) get_state();
    
//     virtual void add_instance(CullTraverser* trav, CullTraverserData& data);
//     virtual void collect_instances(CullTraverser* trav, CullTraverserData& data);

//     CPT_InternalName _buffer_id;
// };


#include "instanceManager.T"


#endif // __INSTANCEMANAGER_H__