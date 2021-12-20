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


#ifndef __MATERIALSPOOL_H__
#define __MATERIALSPOOL_H__

#include "../utils.h"

class ShaderMaterial;


class MaterialsPool {

public:
    static bool has_material(const Filename& filename);
    static bool verify_material(const Filename& filename);
    static ShaderMaterial* get_material(const Filename& filename);
    static ShaderMaterial* load_material(const Filename& filename);

    static void add_material(const Filename& filename, ShaderMaterial* material);

    static void release_material(const Filename& filename);

    static void release_all_materials();

    static int garbage_collect();

    static void list_contents(std::ostream& out);
    static void list_contents();
    static void write(std::ostream& out);

private:
    bool ns_has_material(const Filename& filename) const;
    ShaderMaterial* ns_get_material(const Filename& filename);
    ShaderMaterial* ns_load_material(const Filename& filename);
    void ns_add_material(const Filename& filename, ShaderMaterial* material);

    void ns_release_material(const Filename& filename);

    void ns_release_all_materials();
    int ns_garbage_collect();
    void ns_list_contents(std::ostream& out) const;


    static MaterialsPool* get_ptr();

    MaterialsPool() = default;
    MaterialsPool(const MaterialsPool& copy) = delete;
    MaterialsPool& operator =(const MaterialsPool& copy) = delete;

    ShaderMaterial* ns_get_material() const;


    static Singleton<MaterialsPool> _global_ptr;

    LightMutex _lock;
    typedef pmap<Filename,  PT(ShaderMaterial) > Materials;
    Materials _materials;
};

#endif // __MATERIALSPOOL_H__