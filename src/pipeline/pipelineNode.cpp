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

#include "pipelineNode.h"


using namespace std;


DEFINE_TYPEHANDLE(PipelineNode)


PipelineNode::PipelineNode(const string& name): 
    Namable(name),
    _visited(false)
{
    
}


bool PipelineNode::connect(PipelineNode* other, const string& output, 
                            const string& input) 
{
    InputPortBase* port = get_input(input);
    nassertr(port != nullptr, false)

    return port->connect(other, output);
}


int PipelineNode::get_num_input() const {
    
    return _inputs.size();
}


int PipelineNode::get_num_output() const {
    
    return _outputs.size();
}


InputPortBase* PipelineNode::get_input(const string& name) {

    InputPorts::iterator res = _inputs.find(name);
    nassertr(res != _inputs.end(), nullptr);

    return res->second;
}


OutputPortBase* PipelineNode::get_output(const string& name) {

    OutputPorts::iterator res = _outputs.find(name);
    nassertr(res != _outputs.end(), nullptr);

    return res->second;
}


const PipelineNode::InputPorts PipelineNode::get_inputs() const {
    return _inputs;
}


const PipelineNode::OutputPorts PipelineNode::get_outputs() const {
    return _outputs;
}


void PipelineNode::do_traverse() {
    
}


void PipelineNode::add_input(InputPortBase* input) {

    _inputs[input->get_name()] = input;    
}


void PipelineNode::collect_inputs() {

    if (_visited) // prevent a node from being visited multiple time
        return;

    unordered_set<PipelineNode*> parents;

    for (const auto& input: _inputs) {
        OutputPortBase* endpoint = input.second->get_endpoint();

        if (endpoint != nullptr)
            parents.insert(endpoint->get_node());
    }
    // post order traversal to collect all inputs
    for (PipelineNode* node: parents)
        node->collect_inputs();

    do_traverse(); // once we have collected all inputs, we can setup this node
    _visited = true;
}


TypedWritable* PipelineNode::make_from_bam(const FactoryParams& params) {

    PipelineNode* me = new PipelineNode;
    DatagramIterator scan;
    BamReader* manager;

    parse_params(params, scan, manager);
    me->fillin(scan, manager);

    return me;
}


InputPortBase* PipelineNode::add_input(const string& name, TypeHandle type) {

    InputPortBase* port = new InputPortBase(name, type, this);
    _inputs[name] = port;
    return port;
}


void PipelineNode::add_output(OutputPortBase* output) {

    _outputs[output->get_name()] = output;    
}


OutputPortBase* PipelineNode::add_output(const string& name, TypeHandle type) {

    OutputPortBase* port = new OutputPortBase(name, type, this);
    _outputs[name] = port;
    return port;
}


void PipelineNode::write_datagram(BamWriter* manager, Datagram& me) {
    
    if (_visited) // prevent a node from being visited multiple time
        return;

    me.add_string(get_name());
    Serializable::write_datagram(manager, me);

    unordered_set<PipelineNode*> nodes;
    vector<InputPortBase*> inputs;

    for (InputPorts::value_type& input: _inputs) {
        OutputPortBase* endpoint = input.second->get_endpoint();

        if (endpoint != nullptr) {
            nodes.insert(endpoint->get_node());
            inputs.push_back(input.second);
        }
    }
    
    me.add_uint8(inputs.size());

    for (InputPortBase* input: inputs) {
        OutputPortBase* endpoint = input->get_endpoint();

        me.add_string(input->get_name());
        me.add_string(endpoint->get_name());
        manager->write_pointer(me, endpoint->get_node());
    }

    for (PipelineNode* node: nodes)
        node->write_datagram(manager, me);

    _visited = true;
}


void PipelineNode::fillin(DatagramIterator& scan, BamReader *manager) {

    set_name(scan.get_string());
    Serializable::fillin(scan, manager);

    uint8_t num_inputs = scan.get_uint8();

    for (int i=0; i<num_inputs; ++i) {
        string input = scan.get_string();
        string output = scan.get_string();

        _read_inputs.push_back(make_pair(input, output));
        manager->read_pointer(scan);
    }
}


int PipelineNode::complete_pointers(TypedWritable** p_list, BamReader* manager) {
    
    int n = Serializable::complete_pointers(p_list, manager);

    for (ReadInputs::value_type entry: _read_inputs) {

        InputPorts::iterator it = _inputs.find(entry.first);
        PipelineNode* node = DCAST(PipelineNode, p_list[n++]);

        if (it != _inputs.end())
            it->second->connect(node, entry.second);
    }

    return n;
}


