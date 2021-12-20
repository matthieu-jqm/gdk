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

#ifndef __PIPELINENODE_H__
#define __PIPELINENODE_H__


#include <unordered_map>
#include <panda3d/texture.h>

#include "serializable.h"
#include "IOPort.h"



#define INPUT_PORT(type, name) \
    InputPort<type>& name = make_input_port<type>(#name);

#define OUTPUT_PORT(type, name) \
    OutputPort<type>& name = make_output_port<type>(#name);


class PipelineNode: public Namable, public Serializable {

    REGISTER_TYPE("PipelineNode", Namable, TypedWritableReferenceCount)

public:
    virtual ~PipelineNode() = default;

    bool connect(PipelineNode* other, const std::string& output, 
                const std::string& input);

    typedef std::map<std::string, PT(InputPortBase)> InputPorts;
    typedef std::map<std::string, PT(OutputPortBase)> OutputPorts;

    int get_num_input() const;
    int get_num_output() const;

    InputPortBase* get_input(const std::string& name);
    OutputPortBase* get_output(const std::string& name);

    const InputPorts get_inputs() const;
    const OutputPorts get_outputs() const;

protected:
    template<class D>
    InputPort<D>& make_input_port(const std::string& name);

    template<class D>
    OutputPort<D>& make_output_port(const std::string& name);

    PipelineNode(const std::string& name="");

    virtual void write_datagram(BamWriter* manager, Datagram& me) override;
    virtual void fillin(DatagramIterator& scan, BamReader *manager) override;
    virtual int complete_pointers(TypedWritable** p_list, BamReader* manager) override;

    virtual void do_traverse();

    InputPortBase* add_input(const std::string& name, 
                            TypeHandle type=get_type_handle(Texture));

    OutputPortBase* add_output(const std::string& name, 
                            TypeHandle type=get_type_handle(Texture));
 

    void add_input(InputPortBase* input);
    void add_output(OutputPortBase* output);

    void collect_inputs();

private:
    typedef std::vector<std::pair<std::string, std::string>> ReadInputs;
    ReadInputs _read_inputs;

    static TypedWritable* make_from_bam(const FactoryParams& params);

    bool _visited;

    InputPorts _inputs;
    OutputPorts _outputs;

    friend class InputPortBase;
    friend class OutputPortBase;
    friend class RenderStage;
    friend class RenderPipeline;
};

#include "pipelineNode.T"

#endif // __PIPELINENODE_H__