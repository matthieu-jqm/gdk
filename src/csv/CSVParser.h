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

#ifndef __CSVPARSER_H__
#define __CSVPARSER_H__

#include <string>
#include <vector>
#include <map>

class CSVParser;

/**
 * @brief ligne de donnée d'un fichier .csv
 */
class CSVRow: private std::vector<std::string> {
public:
    CSVRow(const CSVParser& parser);
    ~CSVRow() = default;

    /**
     * @brief recupere une chaine de caratère dans la colonne dont l'indice est passé en paramettre
     * @param col indice de la colonne
     */
    std::string get_string(int col) const;

    /**
     * @brief recupere un entier dans la colonne dont l'indice est passé en paramettre
     * @param col indice de la colonne
     */
    int get_int(int col) const;

    /**
     * @brief recupere un réel dans la colonne dont l'indice est passé en paramettre
     * @param col indice de la colonne
    */
    float get_float(int col) const;

    /**
     * @brief recupere une chaine de caratère dans la colonne dont le nom est passé en paramettre
     * @param colname nom de la colonne
     * @param default_value valeur retourné si aucune colonne n'a été trouvé
     */
    std::string get_string(const std::string& colname, 
                            const std::string& default_value="") const;

    /**
     * @brief recupere un entier dans la colonne dont le nom est passé en paramettre
     * @param colname nom de la colonne
     * @param default_value valeur retourné si aucune colonne n'a été trouvé
     */
    int get_int(const std::string& colname, int default_value=0) const;

    /**
     * @brief recupere un réel dans la colonne dont le nom est passé en paramettre
     * @param colname nom de la colonne
     * @param default_value valeur retourné si aucune colonne n'a été trouvé
     */
    float get_float(const std::string& colname, float default_value=0.0f) const;

    friend std::ostream& operator <<(std::ostream& stream, const CSVRow& row);

    using std::vector<std::string>::size;

private:
    const CSVParser& _parser;

    friend class CSVParser;
};


/**
 * @brief classe permettant de lire et ecrire des fichier .csv
 */
class CSVParser {

public:
    typedef std::vector<CSVRow*> CSVRows;
    typedef std::map<size_t, std::string> IndexValuesFilter;
    typedef std::map<std::string, std::string> KeyValuesFilter;

    /**
     * @param path chemin vers le ficher .csv
     * @param delim délimiteur de colonne
     */
    CSVParser(const std::string& path, char delim=',');
    CSVParser() = default;
    ~CSVParser() = default;

    /**
     * @brief lit les données depuis un ficher
     * @param path chemin vers le ficher .csv
     * @param delim délimiteur de colonne
     */
    bool read_file(const std::string& path, char delim=',');

    /**
     * @brief écrit les données dans un ficher
     * @param path chemin vers le ficher .csv
     * @param delim délimiteur de colonne
     */
    bool write_file(const std::string& path, char delim=',') const;

    /**
     * @brief retourn le nombre de ligne dans la table de donnée
     */
    size_t get_num_rows() const;

    /**
     * @brief retourn le nombre de colonne dans la table de donnée
     */
    size_t get_num_cols() const;

    /**
     * @brief récupère une ligne dans la table de donnée
     * @param index indice de la ligne
     */
    CSVRow& get_row(size_t index);


    /**
     * @brief récupère toutes les ligne dont toutes les valeurs correspondants aux
     * indices sont identiques au filtre
     * @param filter map entier -> chaine de charactere
     * @param max nombre maximum de ligne a retourner, laisser 0 pour toutes les lignes
     */
    CSVRows filter_rows(const IndexValuesFilter& filter, int max=0);


    /**
     * @brief récupère toutes les ligne dont toutes les valeurs correspondants aux
     * noms de colonne sont identiques au filtre
     * @param filter map entier -> chaine de charactere
     * @param max nombre maximum de ligne a retourner, laisser 0 pour toutes les lignes
     */
    CSVRows filter_rows(const KeyValuesFilter& matches, int max=0);

    /**
     * @brief récupère l'indice de la colonne dont le nom est passé en paramèttre
     * @param name nom de la colonne
     * @return indice de la colonne si trouvé, -1 sinon
     */
    int get_col_id(const std::string& name) const;

    bool is_valid() const;

private:

    typedef std::vector<CSVRow> Datas;

    mutable Datas _data;
};

#endif // __CSVPARSER_H__