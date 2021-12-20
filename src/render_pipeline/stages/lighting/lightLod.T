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


template<LightType T>
LightLod<T>::LightLod(int n):
    _instances(new InstanceList),
    _geom(make_geom(n))
{
    _scale_bias = 1.0f / cosf(M_PI / (_segments << n));
}

template<LightType T>
InstanceList* LightLod<T>::get_instances() const {
    return _instances;
}

template<LightType T>
const Geom* LightLod<T>::get_geom() const {
    return _geom;
}

template<LightType T>
size_t LightLod<T>::get_num_instances() const {
    return _instances->size();
}


template<LightType T>
void LightLod<T>::collect_data(uchar*& data) const {

    size_t data_size = sizeof(DataType) * _buffer.size();

    if (data_size > 0) {
        memcpy(data, _buffer.data(), data_size);
        data += data_size;
    }
}


template<LightType T>
void LightLod<T>::reset() {
    _instances->clear();
    _buffer.clear();
}