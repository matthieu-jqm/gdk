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

#include <panda3d/loader.h>

#include "asyncTextureLoader.h"

using namespace std;


// DEFINE_TYPEHANDLE(AsyncTextureLoader)


void AsyncTextureLoader::do_fillin_rawdata(
        CData* cdata, DatagramIterator& scan, BamReader* manager) 
{
    cdata->_x_size = scan.get_uint32();
    cdata->_y_size = scan.get_uint32();
    cdata->_z_size = scan.get_uint32();

    if (manager->get_file_minor_ver() >= 30) {
        cdata->_pad_x_size = scan.get_uint32();
        cdata->_pad_y_size = scan.get_uint32();
        cdata->_pad_z_size = scan.get_uint32();
    } else {
        do_set_pad_size(cdata, 0, 0, 0);
    }

    cdata->_num_views = 1;
    if (manager->get_file_minor_ver() >= 26) {
        cdata->_num_views = scan.get_uint32();
    }
    cdata->_component_type = (ComponentType)scan.get_uint8();
    cdata->_component_width = scan.get_uint8();
    cdata->_ram_image_compression = CM_off;

    if (manager->get_file_minor_ver() >= 1) {
        cdata->_ram_image_compression = (CompressionMode)scan.get_uint8();
    }

    int num_ram_images = 1;
    if (manager->get_file_minor_ver() >= 3) {
        num_ram_images = scan.get_uint8();
    }

    cdata->_ram_images.clear();
    cdata->_ram_images.reserve(num_ram_images);

    size_t* offsets = (size_t*)alloca(sizeof(size_t) * num_ram_images);
    size_t* sizes = (size_t*)alloca(sizeof(size_t) * num_ram_images);

    for (int n = 0; n < num_ram_images; ++n) {
        
        cdata->_ram_images.push_back(RamImage());
        cdata->_ram_images[n]._page_size = get_expected_ram_page_size();

        if (manager->get_file_minor_ver() >= 1) {
            cdata->_ram_images[n]._page_size = scan.get_uint32();
        }

        // fill the cdata->_image buffer with image data
        size_t u_size = scan.get_uint32();

        // Protect against large allocation.
        if (u_size > scan.get_remaining_size()) {
            gobj_cat.error()
            << "RAM image " << n << " extends past end of datagram, is texture corrupt?\n";
            return;
        }

        offsets[n] = scan.get_current_index();
        sizes[n] = u_size;

        scan.skip_bytes(u_size);
    }

    const Datagram& datagram = scan.get_datagram();

    for (int n = num_ram_images - 1; n >= 0; --n) {

        // Thread::sleep(0.5);

        const uchar* ptr = (const uchar*)datagram.get_data() + offsets[n];
        cdata->_ram_images[n]._image = PTA_uchar(ptr, ptr + sizes[n], Texture::get_class_type());

        cdata->inc_image_modified();
    }

    cdata->_loaded_from_image = true;
}

// void AsyncTextureLoader::load() {

    // DisplayRegion
    // if (_current_display_region != nullptr) {
    // priority = _current_display_region->get_texture_reload_priority();
    // }
    // Loader* loader = Loader::get_global_ptr();

    // string task_name = string("reload:") + tc->get_texture()->get_name();
    // PT(AsyncTaskManager) task_mgr = _loader->get_task_manager();

    // // See if we are already loading this task.
    // AsyncTaskCollection orig_tasks = task_mgr->find_tasks(task_name);
    // size_t num_tasks = orig_tasks.get_num_tasks();
    // for (size_t ti = 0; ti < num_tasks; ++ti) {
    // AsyncTask *task = orig_tasks.get_task(ti);
    // if (task->is_exact_type(TextureReloadRequest::get_class_type()) &&
    //     ((TextureReloadRequest *)task)->get_texture() == tc->get_texture()) {
    //     // This texture is already queued to be reloaded.  Don't queue it again,
    //     // just make sure the priority is updated, and return.
    //     task->set_priority(std::max(task->get_priority(), priority));
    //     return (AsyncFuture *)task;
    // }
    // }

    // // This texture has not yet been queued to be reloaded.  Queue it up now.
    // PT(AsyncTask) request =
    // new TextureReloadRequest(task_name,
    //                             _prepared_objects, tc->get_texture(),
    //                             _supports_compressed_texture);
    // request->set_priority(priority);
    // _loader->load_async(request);
// }


TypedWritable* AsyncTextureLoader::make_from_bam(const FactoryParams& params) {
    PT(AsyncTextureLoader) dummy = new AsyncTextureLoader;
    return dummy->make_this_from_bam(params);
}
  

PT(Texture) AsyncTextureLoader::make() {
    return new AsyncTextureLoader;
}


void AsyncTextureLoader::register_with_read_factory() {

    BamReader::get_factory()->register_factory(get_class_type(), make_from_bam);
}
