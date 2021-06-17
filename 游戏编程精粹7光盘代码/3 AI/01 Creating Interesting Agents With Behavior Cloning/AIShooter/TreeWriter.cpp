#include <string>
#include <limits>

#include "TreeWriter.h"

using namespace std;

TreeWriter::TreeWriter(const FeatureMap& featureMap, const TreeNode *turnTree,
    const TreeNode *accelTree, const TreeNode *fireTree)
: mFeatureMap (featureMap), mTurnTree (turnTree), mAccelTree (accelTree),
  mFireTree (fireTree)
{
}

TreeWriter::~TreeWriter()
{
}

void TreeWriter::writeToLua (const std::string& path) const
{
    ofstream file (path.c_str ());

    file << "function setupFeatureMap()" << endl;

    float inf = std::numeric_limits<float>::infinity ();

    const std::map <std::string, FeatureMap::Feature*>& features
        = mFeatureMap.getFeatures ();
    std::map <std::string, FeatureMap::Feature*>::const_iterator iter
        = features.begin (), end = features.end ();
    for (;iter != end; iter ++) {
        FeatureMap::Feature::interval_t intervals
            = iter->second->getIntervals ();
        FeatureMap::Feature::interval_t::const_iterator iIter
            = intervals.begin (), iEnd = intervals.end ();
        for (;iIter != iEnd; iIter ++) {
            file << "  AIShooter.addInterval (\""
                << iter->first << "\", " << iIter->first << ", ";
            if (iIter->second.first == inf) {
                file << "math.huge";
            } else if (iIter->second.first == -inf) {
                file << "-math.huge";
            } else {
                file << iIter->second.first;
            }
            file << ", ";
            if (iIter->second.second == inf) {
                file << "math.huge";
            } else if (iIter->second.second == -inf) {
                file << "-math.huge";
            } else {
                file << iIter->second.second;
            }
            file << ");" << endl;
        }
    }

    file << "end" << endl << endl;

    file << "function turnTree()" << endl;
    luaWriteTurnNode ("  ", mTurnTree, file);
    file << "end" << endl << endl;

    file << "function accelTree()" << endl;
    luaWriteAccelNode ("  ", mAccelTree, file);
    file << "end" << endl << endl;

    file << "function fireTree()" << endl;
    luaWriteFireNode ("  ", mFireTree, file);
    file << "end" << endl << endl;

    file.close ();
}

void TreeWriter::luaWriteTurnNode (const std::string& indent,
    const TreeNode* node, ostream& stream) const
{
    if (!node->hasChildren ()) {
        string action ("UNKNOWN");
        switch (node->getValue ()) {
        case 1: action = "left"; break;
        case 2: action = "right"; break;
        default: action = "none"; break;
        }
        stream << indent << "Agent.turn(\"" << action << "\");" << endl;
        return;
    }

    const std::multimap<TreeNode*, int>& children = node->getChildren ();

    std::multimap<TreeNode*, int>::const_iterator iter = children.begin (),
        end = children.end ();
    for(;iter != end; iter ++) {
        stream << indent;
        if (iter == children.begin ()) {
            stream << "if ";
        } else {
            stream << "elseif ";
        }

        // Find the range of values that use this node
        int lowest = iter->second, highest = iter->second;

        std::multimap<TreeNode*, int>::const_iterator iter2 = iter;
        for (;iter2 != end && iter2->first == iter->first; iter2 ++) {
            if (iter2->second < lowest) {
                lowest = iter2->second;
            }
            if (iter2->second > highest) {
                highest = iter2->second;
            }
        }
        if (lowest == highest) {
            stream << "GameState." << node->getName () << " == " << lowest;
        } else {
            stream << "GameState." << node->getName () << " >= " << lowest
                << " and GameState." << node->getName () << " <= " << highest;
        }
        // Reset the iterator to the last in the group
        iter = iter2;
        iter --;

        stream << " then" << endl;
        luaWriteTurnNode (indent + "  ", iter->first, stream);
    }
    stream << indent << "end" << endl;
}

void TreeWriter::luaWriteAccelNode (const std::string& indent,
    const TreeNode* node, ostream& stream) const
{
    if (!node->hasChildren ()) {
        string action ("UNKNOWN");
        switch (node->getValue ()) {
        case 1: action = "normal"; break;
        case 2: action = "reverse"; break;
        default: action = "none"; break;
        }
        stream << indent << "Agent.accel(\"" << action << "\");" << endl;
        return;
    }

    const std::multimap<TreeNode*, int>& children = node->getChildren ();

    std::multimap<TreeNode*, int>::const_iterator iter = children.begin (),
        end = children.end ();
    for(;iter != end; iter ++) {
        stream << indent;
        if (iter == children.begin ()) {
            stream << "if ";
        } else {
            stream << "elseif ";
        }

        // Find the range of values that use this node
        int lowest = iter->second, highest = iter->second;

        std::multimap<TreeNode*, int>::const_iterator iter2 = iter;
        for (;iter2 != end && iter2->first == iter->first; iter2 ++) {
            if (iter2->second < lowest) {
                lowest = iter2->second;
            }
            if (iter2->second > highest) {
                highest = iter2->second;
            }
        }
        if (lowest == highest) {
            stream << "GameState." << node->getName () << " == " << lowest;
        } else {
            stream << "GameState." << node->getName () << " >= " << lowest
                << " and GameState." << node->getName () << " <= " << highest;
        }
        // Reset the iterator to the last in the group
        iter = iter2;
        iter --;

        stream << " then" << endl;
        luaWriteAccelNode (indent + "  ", iter->first, stream);
    }
    stream << indent << "end" << endl;
}

void TreeWriter::luaWriteFireNode (const std::string& indent,
    const TreeNode* node, ostream& stream) const
{
    if (!node->hasChildren ()) {
        if (node->getValue ()) {
            stream << indent << "Agent.fire(\"fire\");" << endl;
        } else {
            stream << indent << "Agent.fire(\"none\")" << endl;
        }
        return;
    }

    const std::multimap<TreeNode*, int>& children = node->getChildren ();

    std::multimap<TreeNode*, int>::const_iterator iter = children.begin (),
        end = children.end ();
    for(;iter != end; iter ++) {
        stream << indent;
        if (iter == children.begin ()) {
            stream << "if ";
        } else {
            stream << "elseif ";
        }

        // Find the range of values that use this node
        int lowest = iter->second, highest = iter->second;

        std::multimap<TreeNode*, int>::const_iterator iter2 = iter;
        for (;iter2 != end && iter2->first == iter->first; iter2 ++) {
            if (iter2->second < lowest) {
                lowest = iter2->second;
            }
            if (iter2->second > highest) {
                highest = iter2->second;
            }
        }
        if (lowest == highest) {
            stream << "GameState." << node->getName () << " == " << lowest;
        } else {
            stream << "GameState." << node->getName () << " >= " << lowest
                << " and GameState." << node->getName () << " <= " << highest;
        }
        // Reset the iterator to the last in the group
        iter = iter2;
        iter --;

        stream << " then" << endl;
        luaWriteFireNode (indent + "  ", iter->first, stream);
    }
    stream << indent << "end" << endl;
}
