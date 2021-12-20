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


#ifndef __STATE_H__
#define __STATE_H__

#include <panda3d/typedReferenceCount.h>
#include <panda3d/namable.h>

#include "utils.h"

class FSM;


/**
 * @brief Classe abstraite d'un état d'automate fini.
 * @brief Cette classe n'est pas sensée être instantié directement.
 * Chaque état doit hériter de cette classe.
 */
class State: public Namable, public TypedReferenceCount {

    REGISTER_TYPE("State", Namable, TypedReferenceCount)

public:

    enum Flag {
        F_update        = 1,
        F_always_update = 2
    };

    virtual ~State() = default;

    /**
     * @brief Appelée lorsque l'état vien d'être ectivé.
     */
    virtual void enter();

    /** 
     * @brief Appelée chaque frames.
     */
    virtual void update();

    /**
     * @brief Appelé lorsque l'état est désactivé.
     */
    virtual void exit();

    Flag get_flags() const;
    FSM* get_fsm() const;

protected:
    Flag _flags;
    
    /// @param name : nom de l'état
    State(const std::string& name, Flag flags=F_update);

private:
    FSM* _fsm;

    friend class FSM;
};


#endif // __STATE_H__