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

#ifndef __CONFIG_GDK_H__
#define __CONFIG_GDK_H__

#include <panda3d/notifyCategoryProxy.h>

NotifyCategoryDeclNoExport(gdk);

#define GDK_SHADER_PATH "/gdk/shader/"


#ifdef NOTIFY_DEBUG
    // Non-release build:
    #define gdk_debug(msg) \
        if (gdk_cat.is_debug()) \
            gdk_cat->debug() << msg << std::endl; \
        else {}
#else
    // Release build:
    #define gdk_debug(msg) ((void)0);
#endif

#define gdk_info(msg) \
  gdk_cat->info() << msg << std::endl

#define gdk_warning(msg) \
  gdk_cat->warning() << msg << std::endl

#define gdk_error(msg) \
  gdk_cat->error() << msg << std::endl


void init_gdk();

#endif // __CONFIG_GDK_H__