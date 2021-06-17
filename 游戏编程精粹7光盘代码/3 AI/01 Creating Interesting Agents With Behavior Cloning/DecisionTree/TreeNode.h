#ifndef __DECISIONTREE_TREENODE_H__
#define __DECISIONTREE_TREENODE_H__

#include <map>
#include <limits>
#include <string>

class TreeNode
{
public:
    /**
     * Create a decision node
     * \param name the name of the feature this node branches on
     * \param min the minimum decision value
     * \param max the maximum decision value
     */
    TreeNode(const std::string& name, int min, int max);

    /**
     * Create a leaf node
     * \param value the value of this node
     */
    TreeNode(int value);

    /**
     * Standard destructor
     */
    ~TreeNode();

    /**
     * \return true if this node has a value (leaf node)
     */
    bool hasValue () const;

    /**
     * \return true if this node has children (decision node)
     */
    bool hasChildren () const;

    /**
     * Add a child to this node.
     * \param value the decision value of the new branch
     * \param node the node to attach
     * \note Must be a decision node, and value must be within min and max
     */
    void addChild (int value, TreeNode* node);

    /**
     * \return the node's feature name
     */
    const std::string& getName () const;

    /**
     * \return the node's value
     */
    int getValue () const;

    /**
     * \return a multimap of this node's children
     */
    const std::multimap<TreeNode*, int>& getChildren () const;

    /**
     * Fill in missing values with their nearest neighbors.
     * Uses min and max to determine missing values.
     * \note Must be a decision node
     */
    void fillIn ();
private:
    std::string mName;
    int mMin, mMax;
    int mValue;
    std::multimap<TreeNode*, int> mChildren;
};

#endif
