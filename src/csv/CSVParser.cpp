#include <iostream>
#include <fstream>
#include <panda3d/pnotify.h>
#include <sstream>
#include "CSVParser.h"

using namespace std;
using CSVRows = CSVParser::CSVRows;


CSVParser::CSVParser(const string& path, char delim) {

    read_file(path);
}


bool CSVParser::read_file(const string& path, char delim) {
    
    ifstream file(path, ifstream::in);

    if (!file.is_open())
        return false;

    string line;

    _data.clear();

    while (getline(file, line)) {
        CSVRow row(*this);
        string token;
        stringstream ss(line);

        while (getline(ss, token, delim))
            row.push_back(token);

        _data.emplace_back(row);
    }

    file.close();

    if (_data.empty())
        return false;
    
    size_t num_col = get_num_cols();

    // check if all rows have the same number of columns
    for (CSVRow& row: _data) {
        if (row.size() != num_col) {
            _data.clear();
            return false;
        }
    }

    return true;
}


bool CSVParser::write_file(const string& path, char delim) const {
    
    ofstream file(path, ifstream::out);

    if (!file.is_open())
        return false;

    for (const CSVRow& row: _data) {
        size_t num_col = row.size();
        for (size_t i=0; i<num_col; ++i)
            file << row[i] << (i == num_col - 1 ? '\n' : delim);
    }

    file.close();

    return true;
}


size_t CSVParser::get_num_rows() const {
    
    return _data.size();
}


size_t CSVParser::get_num_cols() const {

    if (_data.size() > 0)    
        return _data[0].size();
    
    return 0;
}


CSVRow& CSVParser::get_row(size_t index) {

    return _data[index];    
}


CSVRows CSVParser::filter_rows(const IndexValuesFilter& filter, int max) {

    CSVRows res;
    size_t num_col = get_num_cols();

    for (CSVRow& row: _data) {
        bool match = true;

        for (const pair<size_t, string>& check: filter) {
            nassertr(check.first < num_col, CSVRows())

            if (row.get_string(check.first) != check.second) {
                match = false; break;
            }
        }
        if (match)
            res.push_back(&row);

        if (--max == 0)
            break;
    }
    return res;
}


CSVRows CSVParser::filter_rows(const KeyValuesFilter& filter, int max) {
    
    IndexValuesFilter new_filter;

    // first, we need to convert column names into column indices
    for (const pair<string, string>& check: filter) {
        int id = get_col_id(check.first);
        nassertr(id != -1, CSVRows())

        new_filter[id] = check.second;
    }
    return filter_rows(new_filter);
}


int CSVParser::get_col_id(const string& name) const {
    
    if (!_data.empty()) {
    
        CSVRow& header = _data[0];
        CSVRow::const_iterator it = find(header.begin(), header.end(), name);

        if (it != header.end())
            return it - header.begin();
    }
    return -1;
}


bool CSVParser::is_valid() const {

    return _data.size() > 0;    
}


string CSVRow::get_string(int col) const {
    
    nassertr(col < size(), "")
    return at(col);
}


int CSVRow::get_int(int col) const {
    
    nassertr(col < size(), 0)
    return stoi(at(col));
}


float CSVRow::get_float(int col) const {

    nassertr(col < size(), 0.0f)
    return stof(at(col));
}


string CSVRow::get_string(  const string& colname, 
                            const string& default_value) const
{
    int id = _parser.get_col_id(colname);

    if (id != -1)
        return at(id);

    return default_value;
}


int CSVRow::get_int(const string& colname, int default_value) const {

    int id = _parser.get_col_id(colname);

    if (id != -1)
        return get_int(id);

    return default_value;
}


float CSVRow::get_float(const string& colname, float default_value) const {
    
    int id = _parser.get_col_id(colname);

    if (id != -1)
        return get_float(id);

    return default_value;
}


ostream& operator <<(ostream& stream, const CSVRow& row) {

    for (int i=0; i<row.size(); ++i)
        stream << row.get_string(i) << ' ';
    
    return stream;
}


CSVRow::CSVRow(const CSVParser& parser): _parser(parser) {
    
}
