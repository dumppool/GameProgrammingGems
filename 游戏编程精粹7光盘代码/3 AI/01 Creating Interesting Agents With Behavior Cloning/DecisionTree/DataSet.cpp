#include <assert.h>
#include <math.h>

#include <algorithm>
#include <functional>
#include <stdexcept>
#include <set>
#include <fstream>

#include "FeatureMap.h"
#include "Functions.h"

#include "DataSet.h"

using namespace std;

static float log2 (float f)
{
    static float b = log (2.0f);
    return log (f) / b;
}

void DataSet::addRow (const row_t& row)
{
    addRow (row, mColumns);
}

void DataSet::addRawRow (const raw_row_t& row, const FeatureMap& featureMap)
{
    row_t newRow;

    raw_row_t::const_iterator iter = row.begin (), end = row.end ();
    for (;iter != end; iter ++) {
        newRow [iter->first] = featureMap.mapFeature (iter->first, iter->second);
    }

    addRow (newRow);
}

float DataSet::gain (const string& columnName,
    const col_t& column, const string& targetName,
    const col_set_t& workingSet)
{
    col_t targetColumn = getColumn (targetName, workingSet);
    if (column.empty () || targetColumn.empty ()) {
        return 0;
    }

    size_t total = column.size ();

    int columnMax = *max_element (column.begin (),
        column.end ());

    map<int, int> counts = for_each (column.begin (),
        column.end (), CountOccurrences ());

    int targetMax = *max_element (targetColumn.begin (),
        targetColumn.end ());
    float gain = 0.0f;

    map<int, int>::const_iterator iter = counts.begin (),
        end = counts.end ();
    for (; iter != end; iter ++) {
        // Get only the values in the target column which correspond to
        // the current value
        col_t testColumn (targetColumn.size ());
        transform (column.begin (), column.end (), targetColumn.begin (),
            testColumn.begin (), Select (iter->first, -1));
        testColumn.erase (remove (testColumn.begin (), testColumn.end (), -1),
            testColumn.end ());
        if (testColumn.empty ()) {
            continue;
        }

        gain += ((float) iter->second / (float) total) * info (testColumn, targetMax); 
    }

    float ret = info (targetColumn, targetMax) - gain;

    return ret;
}

void DataSet::saveToFile (const string& filename) const
{
    ofstream file (filename.c_str ());

    int rows = (int) mColumns.begin ()->second.size();
    col_set_t::const_iterator iter = mColumns.begin (),
        end = mColumns.end ();
    for(; iter != end; iter ++) {
        if(iter != mColumns.begin ()) {
            file << "\t";
        }
        file << iter->first;
    }
    file << "\tEND" << endl;

    for (int i = 0; i < rows; i ++) {
        row_t row = getRow (i, mColumns);
        row_t::const_iterator iter = row.begin (),
            end = row.end ();
        for (;iter != end; iter ++) {
            if (iter != row.begin ()) {
                file << "\t";
            }
            file << iter->second;
        }
        file << endl;
    }

    file.close ();
}

void DataSet::loadFromFile (const string& filename)
{
    ifstream file (filename.c_str ());

    vector<string> names;

    while (!file.eof ()) {
        string name;
        file >> name;
        if (name == "END") {
            break;
        }
        names.push_back (name);
    }

    if (file.eof ()) {
        throw runtime_error ("DataSet file is too short");
    }

    while (!file.eof ()) {
        row_t row;
        for (size_t i = 0; i < names.size (); i ++) {
            int val = 0;
            file >> val;
            row [names [i]] = val;
        }
        addRow (row);
    }

    file.close ();
}

TreeNode* DataSet::learnTree (const string& targetName,
    const set<string>& ignoreSet, unsigned int delay, unsigned int offset,
    unsigned int threshold)
{
    col_set_t workingSet = mColumns;

    col_set_t::iterator iter = workingSet.find (targetName);
    if (iter == workingSet.end ()) {
        throw invalid_argument ("Column '" + targetName + "' not found.");
    }

    int numRows = (int) iter->second.size ();
    for (int i = 0; i < numRows; i ++) {
        if (iter->second [i] != 0) {
            row_t row = getRow (i, workingSet);
            for (int j = 0; j < 10; j ++) {
                addRow (row, workingSet);
            }
        }
    }

    if (delay > 0) {
        for_each (workingSet.begin (), workingSet.end (),
            RemoveSomeRows (delay));
    }

    if (offset > 0) {
        col_t& col = iter->second;
        rotate (col.begin (), col.end () - offset, col.end ());
    }

    col_set_t reducedSet = reduceSet (ignoreSet, workingSet);

    pair<string, col_t> best = getBest (targetName, reducedSet);
    return learnNode (targetName, best.first, best.second, reducedSet,
        threshold);
}

void DataSet::addRow (const row_t& row, col_set_t& columns)
{
    if (!columns.empty () && row.size () != columns.size ()) {
        throw invalid_argument (
            "Row length does not match existing number of columns");
    }

    row_t::const_iterator iter = row.begin (), end = row.end ();

    for (; iter != end; iter ++) {
        columns [iter->first].push_back (iter->second);
    }
}

float DataSet::info (col_t column, int max)
{
    size_t total = column.size ();
    assert (total != 0);

    vector<int> counts (max + 1);
    for (size_t i = 0; i < total; i ++) {
        counts [column [i]] ++;
    }

    float bits = 0.0f;

    for(int i = 0; i <= max; i ++) {
        int c = counts [i];
        if (c == 0) {
            continue;
        }

        bits -= c * log2 ((float) c);
    }

    bits += (float) total * log2 ((float) total);
    return bits / total;
}

DataSet::col_set_t DataSet::reduceSet (const set<string>& ignoreSet,
    const col_set_t& workingSet)
{
    col_set_t columns;

    col_set_t::const_iterator iter = workingSet.begin (),
        end = workingSet.end ();
    for (;iter != end; iter ++) {
        if (!ignoreSet.count (iter->first)) {
            columns [iter->first] = iter->second;
        }
    }

    return columns;
}

DataSet::row_t DataSet::getRow (int row, const col_set_t& workingSet)
{
    row_t tempRow;

    map<string, col_t>::const_iterator iter
        = workingSet.begin (), end = workingSet.end ();
    for(; iter != end; iter ++) {
        tempRow [iter->first] = iter->second [row];
    }

    return tempRow;
}

DataSet::col_t DataSet::getColumn (const string& columnName,
    const col_set_t& workingSet)
{
    col_set_t::const_iterator iter = workingSet.find (columnName);
    if (iter == workingSet.end ()) {
        throw invalid_argument ("Column '" + columnName + "' not found.");
    }

    return iter->second;
}

DataSet::col_set_t DataSet::getAllWhere (const std::string& columnName,
    int value, const col_set_t& workingSet)
{
    size_t totalRows = workingSet.begin ()->second.size ();

    col_set_t reducedSet;

    for(size_t i = 0; i < totalRows; i ++) {
        row_t row = getRow ((int) i, workingSet);

        bool use = true;
        row_t::const_iterator iter = row.find (columnName);
        
        if (iter->second == value) {
            addRow (row, reducedSet);
        }
    }

    return reducedSet;
}

pair<string, DataSet::col_t> DataSet::getBest (
    const string& targetName, const col_set_t& workingSet)
{
    float bestGain = 0;

    col_set_t::const_iterator iter = workingSet.begin (),
        end = workingSet.end (), best = end;
    for (;iter != end; iter ++) {
        // Don't include the target
        if (iter->first == targetName) {
            continue;
        }

        float g = gain (iter->first, iter->second, targetName, workingSet);
        if (g > bestGain) {
            bestGain = g;
            best = iter;
        }
    }

    if (best != end) {
        return *best;
    }

    return pair<string, DataSet::col_t> ();
}

TreeNode* DataSet::learnNode (const string& targetName,
    const string& columnName, const col_t& column,
    const col_set_t& workingSet, unsigned int threshold)
{
    col_t newTarget = getColumn (targetName, workingSet);
    int majority = for_each (newTarget.begin (), newTarget.end (),
        Majority ());
    float purity = (float) count (column.begin (), column.end (), majority)
        / (float) column.size ();

    if (column.size () <= threshold || purity >= 0.99f) {
        return new TreeNode (majority);
    }

    int max = *max_element (column.begin (), column.end ());

    TreeNode *node = new TreeNode (columnName, 0, max);

    for (int i = 0; i <= max; i ++) {
        col_set_t newWorkingSet = getAllWhere (columnName, i, workingSet);
        
        newWorkingSet.erase (columnName);

        if (newWorkingSet.empty ()) {
            continue;
        }

        pair<string, col_t> best = getBest (targetName, newWorkingSet);

        node->addChild (i, learnNode (targetName, best.first, best.second,
            newWorkingSet, threshold));
    }

    node->fillIn ();

    return node;
}
