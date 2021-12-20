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

#ifndef __TEXTUREATLAS_H__
#define __TEXTUREATLAS_H__


#include "utils.h"

typedef ShortSizePoT TileSize;

class Texture;

class TextureAtlas {

public:


    TextureAtlas(size_t x=32, size_t y=32, TileSize tile=SST_128);
    TextureAtlas(Texture* texture, TileSize tile=SST_128);
    ~TextureAtlas();

    void set_size(LVecBase2i num_tile);
    void set_size(size_t x, size_t y);
    LVecBase2i get_size() const;
    void get_size(size_t& x, size_t& y) const;

    bool reserve_region(LVecBase2i size);
    bool reserve_region(size_t x, size_t y);
    bool reserve_region(Texture* tex);

    void set_texture(Texture* texture);
    Texture* get_texture() const;

private:

    bool is_region_free(LVecBase4i region);
    bool find_free_region(LVecBase2i size, LVecBase2i& position);
    void set_region(LVecBase4i region, bool value);

    bool get_tile(size_t x, size_t y) const;
    void set_tile(size_t x, size_t y, bool value);

    PT(Texture) _texture;
    LVecBase2i _num_tile;
    TileSize _tile_size;

    bool* _occupancy;
};


#endif // __TEXTUREATLAS_H__