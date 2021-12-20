

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


#ifndef __VIEWPORTMANAGER_H__
#define __VIEWPORTMANAGER_H__

#include <panda3d/pvector.h>
#include <panda3d/pointerTo.h>
#include <stack>


class GraphicsOutput;
class Viewport;

/**
 * @brief Gère plusieurs zones de rendu dans un buffer graphique.
 * Cela permet de créer un mode "split-screen" dans une seule fenêtre pour 
 * afficher la vue de plusieurs joueurs.
*/
           
class ViewportManager {

public:

    enum LayoutPolicy {
        LP_unspecified, /// utiliser les paramètres par defaut
        LP_row,     /// avec deux viewport, les viewport sont empilés
        LP_column   /// avec deux viewport, les viewport sont côtes à côtes
    };

    /**
     * @param buffer buffer graphique dans lequel les viewports sont crées
     * @param layout politique de positionnement vertical/horizontal des 
     * viewports (ne s'applique que quand deux viewports sont présents).
    */
    ViewportManager(GraphicsOutput* buffer, LayoutPolicy layout=LP_unspecified);
    ~ViewportManager();

    /**
     * @brief Si un viewport est non utilisé, celui ci sera renvoyé, sinon
     * un nouveau sera créer dans le buffer
     * @return Viewport alloué
     * @param update Si vrais, alors le layout est recalculé
    */
    Viewport* acquire_viewport(bool update=true);

    /**
     * @brief Créer n viewports, et actualise le layout
     * @param n nombre de viewport a créer
    */
    void acquire_viewports(size_t n);

    /**
     * @brief libère un viewport du buffer
     * @param viewport viewport à libérer
     * @param update Si vrais, alors le layout est actualisé
    */
    void release_viewport(Viewport* viewport, bool update=true); 

    /**
     * @brief libère un viewport du buffer
     * @param id indice du viewport à libérer
     * @param update Si vrais, alors le layout est actualisé
    */
    void release_viewport(size_t id, bool update=true); 

    /**
     * @brief le viewport passé en paramettre est considéré comme plus utilisé 
     * et sera alloué au prochain acquéreur
    */
    void stash_viewport(Viewport* viewport);

    /**
     * @brief libère tous les viewport non utilisés et actualise le layout
    */
    void release_stashed();

    /**
     * @brief retourne le nombre de viewports dans le buffer
     * @return nombre de viewport dans le buffer
    */
    size_t get_num_viewport() const;

    /**
     * @brief récupérer un viewport
     * @param id indice du viewport
     * @return viewport si trouvé, nullptr si id invalide 
    */
    Viewport* get_viewport(size_t id) const;

    /**
     * @brief modifie la politique de positionnement des viewports
     * @param layout type de positionnement
    */
    void set_layout(LayoutPolicy layout);

    /**
     * @brief modifie la marge de séparation entre les viewports
     * @param margin marge en pixel  
    */
    void set_margin(size_t margin);

    /**
     * @brief retourne la marge de séparation entre les viewports
     * @return marge en pixel  
    */
    size_t get_margin() const;

    /**
     * @brief recalcule le layout 
    */
    void update_layout();

    /**
     * @brief retourne la buffer dans lequel les viewports sont crées
     * @return butter graphique 
    */
    GraphicsOutput* get_buffer() const;

private:
    PT(GraphicsOutput) _buffer;

    typedef pvector<PT(Viewport)> Viewports;
    typedef std::stack<PT(Viewport)> StashedViewports;

    StashedViewports _stashed;
    Viewports _viewports;
    LayoutPolicy _layout;
    int _margin;
};

#endif // __VIEWPORTMANAGER_H__