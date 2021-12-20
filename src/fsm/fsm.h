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


#ifndef __FSM_H__
#define __FSM_H__

#include <stack>
#include <panda3d/pointerTo.h>
#include "state.h"

/**
 * @brief Automate à pile
 */
class FSM {

public:
    FSM() = default;
    ~FSM();

    /**
     * @brief Retire le dernier état de la pile.
     */
    void pop_state();

    /** 
     * @brief Transitionne vers un nouvel état dont le nom est passé en paramettre.
     * @param name : nom du nouvelle état.
     * @pre Un état avec le nom passé en paramettre doit être présent dans l'automate.
     */
    void push_state(const std::string& name);

    /**
     * @brief Retourne le nombre d'états dans l'automate.
     */
    int get_num_states() const;
    
    /**
     * @brief Retourne le nombre d'états dans la pile d'états.
     */
    int get_num_stacked_states() const;

    /**
     * @brief Retourne un pointeur vers l'état associé au nom passé en paramettre.
     * @post Si aucun état dans l'automate ne porte ce nom, un pointeur nul sera retourné.
     * @param name : nom de l'état a chercher.
     */
    State* get_state(const std::string& name);

    /**
     * @brief Retourne l'état actif.
     * @post Si aucun état n'est actif, un pointeur nul sera retourné.
     */
    State* get_active_state();

    /**
     * @brief Ajoute un état à l'automate
     * @param state : état a ajouter
     */
    void add_state(State* state);

    /**
     * @brief Appelé chaque frame
     */
    void update();

    void clear_states();

private:

    typedef std::vector<PT(State)> StateVector;

    class StatesStack: private StateVector {
    public:
        StatesStack() = default;
        ~StatesStack() = default;
        
        void push(State*);
        void pop();
        State* top() const;

        using StateVector::size;
        using StateVector::clear;
        using StateVector::empty;
        using StateVector::operator[];
    };

    typedef std::map<std::string, PT(State)> States;
    States _states;

    StatesStack _states_stack;
};

#endif // __FSM_H__