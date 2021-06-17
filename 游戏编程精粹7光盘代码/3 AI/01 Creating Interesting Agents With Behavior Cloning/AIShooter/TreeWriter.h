#ifndef __AISHOOTER_TREEWRITER_H__
#define __AISHOOTER_TREEWRITER_H__

#include <fstream>

#include <TreeNode.h>
#include <FeatureMap.h>

/**
 * This class takes a decision tree and writes it to a usable script file.
 * Currently supports Lua scripts.
 */
class TreeWriter
{
public:
    /**
     * Initialize a tree writer.
     * \param featureMap the feature map to write to the script
     * \param turnTree the turning decision tree to write
     * \param accelTree the acceleration decision tree to write
     * \param fireTree the firing decision tree to write
     */
    TreeWriter (const FeatureMap& featureMap, const TreeNode *turnTree,
        const TreeNode *accelTree, const TreeNode *fireTree);
    ~TreeWriter ();

    /**
     * Write the trees to a Lua script file.
     * \param path the file to write the script to
     */
    void writeToLua (const std::string& path) const;
private:
    /**
     * Recursive function that writes a given turn tree node to a stream as
     * a Lua "if" statement.
     * \param indent the current indentation string (e.g. "  ")
     * \param node the node to write
     * \param stream the output stream to write to
     */
    void luaWriteTurnNode (const std::string& indent,
        const TreeNode* node, std::ostream& stream) const;

    /**
     * Recursive function that writes a given accel tree node to a stream as
     * a Lua "if" statement.
     * \param indent the current indentation string (e.g. "  ")
     * \param node the node to write
     * \param stream the output stream to write to
     */
    void luaWriteAccelNode (const std::string& indent,
        const TreeNode* node, std::ostream& stream) const;

    /**
     * Recursive function that writes a given fire tree node to a stream as
     * a Lua "if" statement.
     * \param indent the current indentation string (e.g. "  ")
     * \param node the node to write
     * \param stream the output stream to write to
     */
    void luaWriteFireNode (const std::string& indent,
        const TreeNode* node, std::ostream& stream) const;

    const FeatureMap& mFeatureMap;
    const TreeNode *mTurnTree;
    const TreeNode *mAccelTree;
    const TreeNode *mFireTree;
};

#endif
