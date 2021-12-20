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

DEFINE_TYPEHANDLE(OutputPortBase)


OutputPortBase::OutputPortBase(const string& name, 
                                TypeHandle type, 
                                PipelineNode* node):
        Port(name, type, node)
{    
    node->add_output(this);
}


void OutputPortBase::connected() {
    
    _node->ref();
}

void OutputPortBase::disconnected() {
    
    unref_delete(_node);
}