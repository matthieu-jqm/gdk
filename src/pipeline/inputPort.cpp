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
#include "outputPort.h"


using namespace std;

DEFINE_TYPEHANDLE(InputPortBase)


bool InputPortBase::connect(PipelineNode* parent, const string& name) {
    
    nassertr(parent != nullptr, false)

    OutputPortBase* port = parent->get_output(name);
    nassertr(port != nullptr, false)

    return connect(port);
}


bool InputPortBase::connect(OutputPortBase* port) {

    nassertr(port != nullptr && port->get_data_type() == _type, false)

    port->connected();
    _endpoint = port;

    return true;
}


void InputPortBase::disconnect() {
    
    if (PT(OutputPortBase) endpoint = _endpoint.lock())
        endpoint->disconnected();
    
    _endpoint = nullptr;
}


InputPortBase::InputPortBase(const string& name, 
                            TypeHandle type,
                            PipelineNode* node): 
    Port(name, type, node),
    _endpoint(nullptr)
{

}


InputPortBase::~InputPortBase() {
    
    disconnect();
}


OutputPortBase* InputPortBase::get_endpoint() const {
    
    if (PT(OutputPortBase) endpoint = _endpoint.lock())
        return endpoint;
    
    return nullptr;
}