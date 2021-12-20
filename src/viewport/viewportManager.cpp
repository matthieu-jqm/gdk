// Copyright (C) 2021 Matthieu Jacquemet, Riyad Ennouara, Nicolas Lerray
// 
// This file is part of Among Z.
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


#include <panda3d/graphicsOutput.h>

#include "viewportManager.h"
#include "viewport.h"
#include "config_gdk.h"

using namespace std;


istream& operator >> (istream& stream, ViewportManager::LayoutPolicy& policy) {

    string word;
    stream >> word;

    if (word == "row")
        policy = ViewportManager::LP_row;
    else if (word == "column")
        policy = ViewportManager::LP_column;
    else
        gdk_error("Invalid layout policy : " << word);

    return stream;
}


ViewportManager::ViewportManager(GraphicsOutput* output, LayoutPolicy layout):
    _buffer(output),
    _margin(0)
{   

    ConfigVariableEnum<LayoutPolicy> viewport_layout_policy
        ("viewport-layout-policy", LP_column,
        PRC_DESC("Specify how the two viewports will be displayed in"
                "one window. Whene set to row the viewports will be stacked"
                "on top of each other, else if column is set they will be"
                "displayed side by sie"));

    if (layout == LP_unspecified)
        layout = viewport_layout_policy;

    _layout = layout;
}


ViewportManager::~ViewportManager() {
    
    for (Viewport* viewport: _viewports) {
        viewport->_mgr = nullptr;
        _buffer->remove_display_region(viewport);
    }
}


Viewport* ViewportManager::acquire_viewport(bool update) {

    // first check if there is a stashed viewport
    if (!_stashed.empty()) {
        PT(Viewport) viewport = _stashed.top();
        _stashed.pop();

        return viewport;
    }

    PT(Viewport) viewport = new Viewport(this);

    _viewports.push_back(viewport);

    if (update)
        update_layout();

    return viewport;
}


void ViewportManager::acquire_viewports(size_t n) {
    
    while (n--) {
        PT(Viewport) viewport = new Viewport(this);
        _viewports.push_back(viewport);
        _stashed.push(viewport);
    }
    
    update_layout();
}


void ViewportManager::release_viewport(Viewport* viewport, bool update) {

    auto found = find(_viewports.begin(), _viewports.end(), viewport);

    if (found != _viewports.end()) {
        _buffer->remove_display_region(viewport);
        _viewports.erase(found);
        if (update)
            update_layout();
    }
}


void ViewportManager::release_viewport(size_t id, bool update) {
    
    nassertv(id < _viewports.size())

    _buffer->remove_display_region(_viewports[id]);
    _viewports.erase(_viewports.begin() + id);

    if (update)
        update_layout();
}


void ViewportManager::stash_viewport(Viewport* viewport) {
    
    _stashed.push(viewport);
}


void ViewportManager::release_stashed() {
    
    _stashed = StashedViewports();
    update_layout();
}


size_t ViewportManager::get_num_viewport() const {
    
    return _viewports.size();
}


Viewport* ViewportManager::get_viewport(size_t id) const {
    
    nassertr(id < _viewports.size(), nullptr)
    return _viewports[id];
}


void ViewportManager::set_layout(LayoutPolicy layout) {
    
    _layout = layout;
    update_layout();
}


void ViewportManager::set_margin(size_t margin) {

    _margin = margin;
    update_layout();
}


size_t ViewportManager::get_margin() const {
    
    return _margin;
}


void ViewportManager::update_layout() {
    
    int num_viewport = get_num_viewport();

    float margin_x = 0.0f;
    float margin_y = 0.0f;

    if (_buffer->has_size()) {
        margin_x = float(_margin) / _buffer->get_x_size();
        margin_y = float(_margin) / _buffer->get_y_size();
    }

    switch (num_viewport) {

    case 0: 
        break;
    case 1: 
        _viewports[0]->set_dimensions(0.0f, 1.0f, 0.0f, 1.0f);
        break;
    case 2:
        if (_layout == LP_row) {
            _viewports[0]->set_dimensions(
                0.0f, 1.0f, 0.5f + margin_y * 0.5f, 1.0f);
            
            _viewports[1]->set_dimensions(
                0.0f, 1.0f, 0.0f, 0.5f - margin_y * 0.5f);
        } else {
            _viewports[0]->set_dimensions(
                0.0f, 0.5f - margin_x * 0.5f, 0.0f, 1.0f);
            
            _viewports[1]->set_dimensions(
                0.5f + margin_x * 0.5f, 1.0f, 0.0f, 1.0f);
        }
        break;
    default: {

        int num_col = ceil(sqrt(num_viewport));
        int num_row = num_col;
        int num_cell = num_row * num_col;
        int col = ceil((float)num_viewport / num_col);

        float width = (1.0f - margin_x * (num_col - 1)) / num_col;
        float height = (1.0f - margin_y * (num_row - 1)) / num_row;

        float l, r, b, t;
        t = 1.0f;
        b = t - height;
        int id = 0;

        for (int i=0; i<num_row; ++i) {

            if (i == num_row - 1.0f) {
                col = num_viewport - num_col * i;
                l = (1.0f - width * col - margin_x * (col - 1.0f)) / 2.0f;
            } else
                l = 0.0f;

            r = l + width;

            for (int j=0; j<col; ++j) {
                _viewports[id++]->set_dimensions(l, r, b, t);
                l += width + margin_x;
                r = l + width;
            }
            t -= height + margin_y;
            b = t - height;
        }
        break;
    }
    }
}

GraphicsOutput* ViewportManager::get_buffer() const {
    
    return _buffer;
}







    // else if (num_viewport <= 4) {
    //     float margin_x = 0.0f;
    //     float margin_y = 0.0f;

    //     if (_buffer->has_size()) {
    //         margin_x = float(_margin)/_buffer->get_x_size() * 0.5;
    //         margin_y = float(_margin)/_buffer->get_y_size() * 0.5;
    //     }
        
    //     if (num_viewport == 2) {

    //         if (_layout == LP_row) {
    //             _viewports[0]->set_dimensions(0.0, 1.0, 0.5+margin_y, 1.0);
    //             _viewports[1]->set_dimensions(0.0, 1.0, 0.0, 0.5-margin_y);
    //         } else {
    //             _viewports[0]->set_dimensions(0.0, 0.5-margin_x, 0.0, 1.0);
    //             _viewports[1]->set_dimensions(0.5+margin_x, 1.0, 0.0, 1.0);
    //         }
    //     } else {
    //         float x_l = 0.5 - margin_x;
    //         float x_r = 0.5 + margin_x;
    //         float y_b = 0.5 - margin_y;
    //         float y_t = 0.5 + margin_y;

    //         _viewports[0]->set_dimensions(0.0, x_l, y_t, 1.0);
    //         _viewports[1]->set_dimensions(x_r, 1.0, y_t, 1.0);

    //         if (num_viewport == 3) {
    //             x_l = 0.25 + margin_x*0.5;
    //             x_r = 0.75 - margin_x*0.5;
    //             _viewports[2]->set_dimensions(x_l, x_r, 0.0, y_b);
    //         } else {
    //             _viewports[2]->set_dimensions(0.0, x_l, 0.0, y_b);
    //             _viewports[3]->set_dimensions(x_r, 1.0, 0.0, y_b);
    //         }
    //     }
    // }