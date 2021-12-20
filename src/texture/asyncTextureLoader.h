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


#ifndef __ASYNCTEXTURELOADER_H__
#define __ASYNCTEXTURELOADER_H__


#include <panda3d/texture.h>
#include "utils.h"


class AsyncTextureLoader: public Texture {

    // REGISTER_TYPE("AsyncTextureLoader", Texture)

public:
    AsyncTextureLoader() = default;
    virtual ~AsyncTextureLoader() = default;

    static PT(Texture) make();

    static void register_with_read_factory();

protected:
    virtual void do_fillin_rawdata(CData* cdata, DatagramIterator& scan,
                                    BamReader* manager) override;

private:
    static TypedWritable* make_from_bam(const FactoryParams& params);
};

#endif // __ASYNCTEXTURELOADER_H__