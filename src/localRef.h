
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

#ifndef __LOCALREF_H__
#define __LOCALREF_H__

#include <panda3d/typedReferenceCount.h>


/**
 * @brief Classe utilisée en interne par LocalRef.
 */
class _DestructLocalRef {

private:
    _DestructLocalRef(PandaNode* object): 
        _object(object), _clear_flag(false) {}

    ~_DestructLocalRef() {
        if (_clear_flag)
            _object->unref();
    }

    PandaNode* _object;
    bool _clear_flag;

    template<class T>
    friend class LocalRef;
};


/**
 * @brief Permet de palier le problème des références circulaire de PointerTo
 * @details Cette classe doit être utilisé quand l'on stock un objet
 * de type T dans un objet de type Obj, et que la classe T stock une  
 * réference sur l'objet de type Obj. En temps normal l'objet de type Obj
 * n'est jamais détruis car son compteur de référence n'attend jamais 0
 * à cause de la référence sur elle même qu'elle stock, ce qui produit une 
 * fuite de mémoire.
 */
template<class T>
class LocalRef: private _DestructLocalRef, public T {

public:
    /**
     * @param object objet référencé stockant l'object de type T 
     * @param args paramettres à passé au constructeur de la classe T
     */
    // template <class Obj, class... Args,
    //     std::enable_if_t<std::is_constructible<T, Args&&...>::value, int> = 0>
    // LocalRef(Obj* object, Args&&... args): 
    //     _DestructLocalRef(object),
    //     T(std::forward<Args>(args)...)
    // {
    //     // cancel the ref made by parent class
    //     _object->unref();
    // }

    template <class Obj, class... Args>
    LocalRef(Obj* object, Args&&... args): 
        _DestructLocalRef(object),
        T(std::forward<Args>(args)...)
    {
        // cancel the ref made by parent class
        _object->unref();
    }


    ~LocalRef() {
        // we need to ref twice since the parent class 
        // may destroy it if the reference counter reaches zero.
        // We also have to set the clear flag to signal _DestructLocalRef
        // that our second ref needs to be canceled.
        if (_object->get_ref_count() == 0) {
            _object->ref();
            _clear_flag = true;
        }
        _object->ref();
    }
};

#endif // __LOCALREF_H__