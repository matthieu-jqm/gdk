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


#ifndef __LIGHTPOOL_H__
#define __LIGHTPOOL_H__


#include "instanceManager.h"
#include "shaderState.h"
#include "lightData.h"
#include "cullHandlerHook.h"
#include "lightLod.h"

class Lighting;
class DirectionalLightNode;


class LightPoolBase: public NodeHandler, public Namable {

    REGISTER_TYPE("LightPool", NodeHandler, Namable)

public:
    static LightPoolBase* make(LightType type);
    virtual ~LightPoolBase() = default;

    virtual void set_stage(Lighting* stage);

    void collect_data(vector_uchar& data, int& n);

protected:
    LightPoolBase(const std::string& name, size_t data_size);
    
    void set_data(void* data);
    void set_data_size(size_t size);

    virtual void record_node(CullTraverserData& data,
                            const PipelineCullTraverser* trav) override;

    virtual void post_cull(const PipelineCullTraverser* trav) override;

    virtual void commit_data(uchar* data);

    typedef LightPoolBase* PoolConstructor();
    typedef std::array<PoolConstructor*, LT_COUNT> PoolConstructors;

    Lighting* _stage;

    static PoolConstructors _contructors;

private:
    size_t _min_buffer_size;
    size_t _data_size;

    class CData : public CycleData {
    public:
        CData();
        CData(const CData& copy);

        virtual CycleData* make_copy() const override;
        TypeHandle get_parent_type() const override {
            return LightPoolBase::get_class_type();
        }
    
        vector_uchar _data;
        int _num_lights;
    };

    PipelineCycler<CData> _cycler;
    typedef CycleDataReader<CData> CDReader;
    typedef CycleDataWriter<CData> CDWriter;
};



template<LightType T>
class LightPool: public LightPoolBase {

    REGISTER_TEMPLATE("LightPool", LightPoolBase)

public:
    virtual ~LightPool() = default;

protected:
    LightPool();

    using LodType = LightLod<T>;
    using DataType = LightData<T>;

    virtual TypeHandle get_node_type() const override;

    virtual void setup(RenderPipeline* pipeline) override;

    virtual void post_cull(const PipelineCullTraverser* trav) override;

    virtual void record_node(CullTraverserData& data,
                            const PipelineCullTraverser* trav) override;

    virtual void set_stage(Lighting* stage) override;

    virtual void commit_data(uchar* data) override;

private:
    static void post_init();
    static LightPoolBase* make();

    typedef std::vector<LodType> Lods;
    Lods _lods;

    ShaderState _state;
};


#include "lightPool.T"


#endif // __LIGHTPOOL_H__






































// class DrawLightCallback: public CallbackObject {

//     REGISTER_TYPE("DrawLightCallback", CallbackObject)

// public:
//     ALLOC_DELETED_CHAIN(DrawLightCallback);

//     DrawLightCallback() = default;
//     ~DrawLightCallback() = default;

//     virtual void do_callback(CallbackData* cdata) override;
// };



// class PointLightPool: public LightPool {

//     REGISTER_TYPE("PointLightPool", InstancePool)

// public:
//     PointLightPool();
//     virtual ~PointLightPool() = default;

//     virtual Geom* make_geom(int n) const override;

// protected:
//     virtual float compute_distance(PandaNode* node, 
//                             const TransformState* transform, 
//                             const CullTraverser* trav) override;

//     virtual vector_uchar get_data() const override;

// private:
//     mutable std::vector<PointLightData> _data;
// };



// class SpotLightPool: public LightPool {

//     REGISTER_TYPE("SpotLightPool", InstancePool)

// public:
//     SpotLightPool();
//     virtual ~SpotLightPool() = default;

//     virtual Geom* make_geom(int n) const override;

// protected:
//     virtual float compute_distance(PandaNode* node, 
//                             const TransformState* transform, 
//                             const CullTraverser* trav) override;

//     virtual vector_uchar get_data() const override;

// private:
//     mutable std::vector<SpotLightData> _data;
// };

