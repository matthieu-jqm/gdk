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

#ifndef __ASSETMANAGER_H__
#define __ASSETMANAGER_H__

#include <panda3d/typeHandle.h>
#include <panda3d/pointerTo.h>
#include "typeUtils.h"


template<class T>
class Singleton;
class PandaNode;


class AssetManager: private TypeUtils<std::string> {

public:
    using TypeUtils<std::string>::MatchType;

    ~AssetManager() = default;

    static AssetManager* ptr();

    static std::string get_location(TypeHandle type, MatchType match=MT_best);
    static void set_location(TypeHandle type, const std::string& location);

    static PT(PandaNode) make_asset(TypeHandle type, const std::string& name,
                                    MatchType match=MT_best);

    static PandaNode* get_asset(TypeHandle type, const std::string& name,
                                    MatchType match=MT_best);

    static std::string get_default_location();
    static void set_default_location(const std::string& location);

private:
    AssetManager() = default;

    std::string ns_get_location(TypeHandle type, MatchType match=MT_best) const;

    void ns_set_location(TypeHandle type, const std::string& location);

    PT(PandaNode) ns_make_asset(TypeHandle type, const std::string& name,
                                MatchType match=MT_best);

    PandaNode* ns_get_asset(TypeHandle type, const std::string& name,
                                MatchType match=MT_best);

    std::string ns_get_default_location() const;

    void ns_set_default_location(const std::string& location);


    typedef pmap<TypeHandle, std::string> Locations;
    Locations _locations;
    std::string _default_location;

    static LightMutex _lock;

    friend class Singleton<AssetManager>;
    static Singleton<AssetManager> _global_ptr;
};


#endif // __ASSETMANAGER_H__