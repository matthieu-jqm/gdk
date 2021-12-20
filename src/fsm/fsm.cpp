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


#include "fsm.h"

using namespace std;



void FSM::pop_state() {
    
    if (!_states_stack.empty()) {
        State* state = _states_stack.top();
        state->exit();

        _states_stack.pop();
    }
}


void FSM::push_state(const string& name) {

    States::const_iterator it = _states.find(name);
    nassertv(it != _states.end())

    State* state = it->second;

    if (!_states_stack.empty()) {

        if (state == _states_stack.top())
            return;

        _states_stack.top()->exit();
    }

    _states_stack.push(state);
    state->enter();
}


int FSM::get_num_states() const {
    
    return _states.size();
}


int FSM::get_num_stacked_states() const {

    return _states_stack.size();    
}


State* FSM::get_state(const string& name) {
    
    States::const_iterator it = _states.find(name);

    if (it != _states.end())
        return it->second;

    return nullptr;
}


State* FSM::get_active_state() {
    
    if (!_states_stack.empty())
        return _states_stack.top();
    
    return nullptr;
}


void FSM::add_state(State* state) {
    
    state->_fsm = this;
    _states[state->get_name()] = state;
}



void FSM::update() {

    if (_states_stack.empty())
        return;

    for (int i=0; i<_states_stack.size() - 1; ++i) {
        
        State* state = _states_stack[i];
        State::Flag flags = state->get_flags();
        
        if (flags & State::F_always_update)
            state->update();
    }

    State* top = _states_stack.top();
    if (top->get_flags() & (State::F_update | State::F_always_update))
        top->update();
}


void FSM::clear_states() {
    
    if (!_states_stack.empty()) {
        State* top = _states_stack.top();
        top->exit();
    }

    _states.clear();
    _states_stack.clear();
}


void FSM::StatesStack::push(State* state) {
    
    push_back(state);
}


void FSM::StatesStack::pop() {
    
    nassertv(!empty());
    pop_back();
}

State* FSM::StatesStack::top() const {
    
    nassertr(!empty(), nullptr);
    return back();
}


FSM::~FSM() {

    clear_states();
}
