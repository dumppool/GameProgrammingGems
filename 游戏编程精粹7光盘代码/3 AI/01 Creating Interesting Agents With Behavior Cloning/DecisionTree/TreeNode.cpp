#include <assert.h>

#include "TreeNode.h"

TreeNode::TreeNode(const std::string& name, int min, int max)
: mName (name), mMin (min), mMax (max),
  mValue (std::numeric_limits<int>::min ())
{
}

TreeNode::TreeNode(int value)
: mValue (value)
{
}

TreeNode::~TreeNode()
{
    std::multimap<TreeNode*, int>::iterator iter = mChildren.begin (),
        end = mChildren.end ();
    for (;iter != end; iter ++) {
        delete iter->first;
    }
}

bool TreeNode::hasValue () const
{
    return (mValue != std::numeric_limits<int>::min ());
}

bool TreeNode::hasChildren () const
{
    return !mChildren.empty ();
}

void TreeNode::addChild (int value, TreeNode* node)
{
    assert (mValue == std::numeric_limits<int>::min ());

    mChildren.insert (std::make_pair (node, value));
}

const std::string& TreeNode::getName () const
{
    return mName;
}

int TreeNode::getValue () const
{
    assert (mChildren.empty ());

    return mValue;
}

const std::multimap<TreeNode*, int>& TreeNode::getChildren () const
{
    return mChildren;
}

void TreeNode::fillIn ()
{
    assert (!mChildren.empty ());

    std::map<int, TreeNode*> assignments;

    // List all of the values that we have child nodes for
    std::multimap<TreeNode*, int>::const_iterator iter = mChildren.begin (),
        end = mChildren.end ();
    for (;iter != end; iter ++) {
        assignments [iter->second] = iter->first;
    }

    // Fill in all missing values
    for (int i = mMin; i <= mMax; i ++) {
        // If this value already exists, skip it
        if (assignments.count (i)) {
            continue;
        }
    
        std::map<int, TreeNode*>::const_iterator next
            = assignments.upper_bound (i), prev = next;

        if (prev != assignments.begin ()) {
            prev --;
        }

        if (next == assignments.end ()) {
            next --;
        }

        int nextDist = abs (i - next->first),
            prevDist = abs (i - prev->first);

        if (prevDist < nextDist) {
            mChildren.insert (std::make_pair (prev->second, i));
        } else {
            mChildren.insert (std::make_pair (next->second, i));
        }
    }
}
