#ifndef __DECISIONTREE_DATASET_H__
#define __DECISIONTREE_DATASET_H__

#include <string>
#include <map>
#include <set>
#include <vector>

#include "TreeNode.h"

class FeatureMap;

class DataSet
{
public:
    /** A row of data */
    typedef std::map<std::string, int> row_t;
    /** A row of raw float data */
    typedef std::map<std::string, float> raw_row_t;
    /** A data column */
    typedef std::vector<int> col_t;
    /** A set of columns of data */
    typedef std::map<std::string, col_t> col_set_t;

    /**
     * Add a row of data to the set
     *
     * \param row a row of data to add
     * \note The row must have the same number of columns as any
     * existing data.
     */
    void addRow (const row_t& row);

    /**
     * Add a row of raw float data to the set
     *
     * \param row a row of data to add
     * \param featureMap a feature map for mapping this row's data
     *
     * \note The row must have the same number of columns as any
     * existing data. 
     */
    void addRawRow (const raw_row_t& row, const FeatureMap& featureMap);

    /**
     * Calculates the information gain on a given column
     *
     * \param columnName the name of the column
     * \param column the column of data to calculate the information gain on
     * \param targetName the name of the class column
     *
     * \return the column's information gain
     */
    static float gain (const std::string& columnName,
        const col_t& column,
        const std::string& targetName,
        const col_set_t& workingSet);

    /**
     * Save this data set to a special tab delimited file
     *
     * \param filename the name of the file to save to
     */
    void saveToFile (const std::string& filename) const;

    /**
     * Load a data set from a special tab delimited file
     *
     * \param filename the name of the file to load from
     */
    void loadFromFile (const std::string& filename);

    /**
     * Create a decision tree by learning from this data set
     *
     * \param targetName the target class column
     * \param ignoreSet a set of columns to omit from learning
     * \param delay number of rows to ignore from beginning
     * \param offset number of rows to offset the class column by (used to
     * simulate human reaction time)
     * \param threshold the minimum number of entries to branch the tree on
     *
     * \return the root node of the decision tree
     */
    TreeNode* learnTree (const std::string& targetName,
        const std::set<std::string>& ignoreSet, unsigned int delay,
        unsigned int offset, unsigned int threshold);

    /**
     * \return a reference to all columns in this data set
     */
    const col_set_t& getColumns () const
    {
        return mColumns;
    }

    /**
     * Reduce all of the columns in a working set by eliminating all rows
     * that do not satisfy the given condition(s)
     *
     * \param columnName the column name to check for
     * \param value the value to check for
     * \param workingSet the working set to reduce
     *
     * \return the reduced set of columns
     */
    static col_set_t getAllWhere (const std::string& columnName,
        int value, const col_set_t& workingSet);

    /**
     * Get a specific column from a working set
     * 
     * \param columnName the name of the column to get
     * \param workingSet the working set to get the column from
     *
     * \return the selected column
     */
    static col_t getColumn (const std::string& columnName,
        const col_set_t& workingSet);
private:
    static void addRow (const row_t& row, col_set_t& columns);
    static float info (col_t column, int max);
    static col_set_t reduceSet (const std::set<std::string>& ignoreSet,
        const col_set_t& workingSet);
    static row_t getRow (int row, const col_set_t& workingSet);
    static std::pair<std::string, col_t> getBest (const std::string& targetName, 
        const col_set_t& workingSet);

    static TreeNode* learnNode (const std::string& targetName,
        const std::string& columnName, const col_t& column,
        const col_set_t& workingSet, unsigned int threshold);

    col_set_t mColumns;
};

#endif
