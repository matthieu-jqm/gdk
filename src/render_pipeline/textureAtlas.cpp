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

#include <panda3d/texture.h>
#include "textureAtlas.h"


using namespace std;



TextureAtlas::~TextureAtlas() {
    
    if (_occupancy != nullptr)
        delete _occupancy;
}


TextureAtlas::TextureAtlas(size_t x, size_t y, TileSize tile):
    _num_tile(x,y), _tile_size(tile) 
{
    _texture = new Texture();

    _texture->setup_2d_texture( x << tile, y << tile,
                                Texture::T_unsigned_byte,
                                Texture::F_rgba);

    _texture->set_wrap_u(SamplerState::WM_border_color);
    _texture->set_wrap_v(SamplerState::WM_border_color);

    _occupancy = new bool[x*y];
}


TextureAtlas::TextureAtlas(Texture* texture, TileSize tile): 
    _texture(texture), _tile_size(tile),
    _num_tile(  get_floor_size(texture->get_x_size(), tile),
                get_floor_size(texture->get_x_size(), tile))
{
    _occupancy = new bool[_num_tile.get_x() * _num_tile.get_y()];
}


void TextureAtlas::set_size(LVecBase2i num_tile) {
    _num_tile = num_tile;
}

void TextureAtlas::set_size(size_t x, size_t y) {
    _num_tile.set(x, y);
}

LVecBase2i TextureAtlas::get_size() const {
    return _num_tile;    
}


void TextureAtlas::get_size(size_t& x, size_t& y) const {
    
    x = _num_tile.get_x();
    y = _num_tile.get_y();
}


bool TextureAtlas::reserve_region(LVecBase2i size) {

    LVecBase2i pos;
    
    if (find_free_region(size, pos)) {
        set_region(LVecBase4i(pos.get_x(), pos.get_y(), 
                            size.get_x(), size.get_y()), true);
        return true;
    }
    return false;
}


bool TextureAtlas::reserve_region(size_t x, size_t y) {

    return reserve_region(LVecBase2i(x,y)); 
}


bool TextureAtlas::reserve_region(Texture* tex) {
    
    return reserve_region(LVecBase2i(tex->get_x_size(), tex->get_y_size()));
}


bool TextureAtlas::find_free_region(LVecBase2i size, LVecBase2i& position) {

    nassertr(size.get_x() <= _num_tile.get_x() && size.get_y() <= _num_tile.get_y(), false)

    size_t size_x = _num_tile.get_x() - size.get_x();
    size_t size_y = _num_tile.get_y() - size.get_y();

    for (int i=0; i < size_x; ++i) {
        for (int j=0; j < size_y; ++j) {
            if (is_region_free(LVecBase4i(i,j, size.get_x(), size.get_y()))) {
                position = LVecBase2i(i,j);
                return true;
            }
        }
    }
    return false;
}


bool TextureAtlas::get_tile(size_t x, size_t y) const {
    
    return _occupancy[x*_num_tile.get_x() + y];
}


void TextureAtlas::set_tile(size_t x, size_t y, bool value) {
    
    _occupancy[x*_num_tile.get_x() + y] = value;
}


void TextureAtlas::set_region(LVecBase4i region, bool value) {

    size_t start_x = region.get_x();
    size_t start_y = region.get_y();
    size_t end_x = start_x + region.get_z();
    size_t end_y = start_y + region.get_w();

    for (size_t i=start_x; i<end_x; ++i) {
        for (size_t j=start_y; j<end_y; ++j)
            set_tile(i,j, value);
    }
}


bool TextureAtlas::is_region_free(LVecBase4i region) {
    
    size_t start_x = region.get_x();
    size_t start_y = region.get_y();
    size_t end_x = start_x + region.get_z();
    size_t end_y = start_y + region.get_w();

    for (size_t i=start_x; i<end_x; ++i) {
        for (size_t j=start_y; j<end_y; ++j) {
            if (get_tile(i,j))
                return false;
        }
    }
    return true;
}