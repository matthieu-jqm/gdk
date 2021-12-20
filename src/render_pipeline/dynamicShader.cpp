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

#include "dynamicShader.h"
#include <panda3d/virtualFileSystem.h>


using namespace std;


DynamicShader::DynamicShader(GraphicsStateGuardianBase* gsg, 
                            const string& path, uint version): 
    ShaderGenerator(gsg)
{
    
    VirtualFileSystem* vfs = VirtualFileSystem::get_global_ptr();
    _file = vfs->open_read_file(path, false);
    _body << "#version " << version << endl;
}


DynamicShader::~DynamicShader() {
    
    VirtualFileSystem* vfs = VirtualFileSystem::get_global_ptr();
    vfs->close_read_file(_file);
}


string DynamicShader::finish() const {

    string line;
    getline(*_file, line);

    if (line.rfind("#version ") == 0)
        _file->seekg(line.size());

    istreambuf_iterator<char> eos;
    return _body.str() + string(istreambuf_iterator<char>(*_file), eos);
}