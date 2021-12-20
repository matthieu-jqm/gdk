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

#include "entity.h"
#include "component.h"

using namespace std;


void Entity::render(RenderStage* stage) {
    
    for (Component* component : _components)
        component->render(stage);
}

void Entity::update() {
    
    for (Component* component : _components)
        component->update();
}


void Entity::add_component(Component* component) {
    
    _components.insert(component);
}


Component* Entity::get_component(TypeHandle type) const {

    for (Component* component : _components) {
        if (component->is_exact_type(type))
            return component;
    }
    return nullptr;
}
