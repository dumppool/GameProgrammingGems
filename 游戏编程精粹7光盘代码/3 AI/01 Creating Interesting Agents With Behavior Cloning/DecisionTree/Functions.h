#ifndef __DECISIONTREE_FUNCTIONS_H__
#define __DECISIONTREE_FUNCTIONS_H__

#include <map>
#include <vector>

/*
 * This file contains function objects for use in the DataSet using the
 * standard C++ <algorithm> and <functional> headers.
 */

/**
 * Compares values in a <int, int> map.
 */
class ValueLess
{
public:
    bool operator() (const std::pair<int, int>& a,
        const std::pair<int, int>& b)
    {
        return a.second < b.second;
    }
};

/**
 * Calculates the mode of a given collection of integers.
 * If more than one exists, one is randomly selected.
 */
class Majority
{
public:
    void operator() (int elem)
    {
        mCounts [elem] ++;
    }

    // Return Majority value
    operator int() const
    {
        return std::max_element (mCounts.begin (), mCounts.end (),
            ValueLess ())->first;
    }
private:
    std::map<int, int> mCounts;
};

/**
 * Count occurrences of a value in a <int, int> map.
 */
class CountOccurrences
{
public:
    void operator() (int elem)
    {
        mCounts [elem] ++;
    }

    operator std::map<int, int>() const
    {
        return mCounts;
    }
public:
    std::map<int, int> mCounts;
};

/**
 * Remove a given number of rows from every map element in an
 * <int, vector<int> > map.
 */
class RemoveSomeRows
{
public:
    RemoveSomeRows (int num)
        : mNum (num) { }

    void operator() (std::pair<const std::string, std::vector<int> >& e)
    {
        e.second.erase (e.second.begin (), e.second.begin () + mNum);
    }
private:
    int mNum;
};

/**
 * Delete every map element pointer.
 */
template<typename _K, typename _V>
class MapDelete
{
public:
    void operator() (const std::pair<_K, _V>& a)
    {
        delete a.second;
    }
};

/**
 * Select elements from one container based on elements in a second container.
 */
class Select
{
public:
    /**
     * \param selector the value to check for in the right hand container
     * \param defaultValue the value to return if the right hand
     * does not match the selector
     */
    Select (int selector, int defaultValue)
        : mSelector (selector), mDefaultValue (defaultValue)
    { }

    int operator() (int lhs, int rhs)
    {
        if (lhs == mSelector) {
            return rhs;
        }

        return mDefaultValue;
    }
private:
    int mSelector;
    int mDefaultValue;
};

#endif
