#include <stdlib.h>
#include <algorithm>
#include <fstream>
#include <limits>

#include "Functions.h"

#include "FeatureMap.h"

using namespace std;

FeatureMap::FeatureMap ()
{
}

FeatureMap::~FeatureMap ()
{
    for_each (mFeatures.begin (), mFeatures.end (),
        MapDelete<string, Feature*> ());
}

void FeatureMap::defineFeature (const string& feature, FeatureType type)
{
    if (mFeatures.count (feature) != 0) {
        throw invalid_argument ("feature '" + feature +
            "' already defined in feature map");
    }

    if (type != FT_NOMINAL) {
        mFeatures [feature] = new ContinuousFeature ();
    } else {
        mFeatures [feature] = new NominalFeature ();
    }
}

void FeatureMap::addInterval (const string& feature, int value,
    float lower, float upper)
{
    map <string, Feature*>::const_iterator iter = mFeatures.find (
        feature);

    if(iter == mFeatures.end ()) {
        throw new out_of_range ("Feature '" + feature + "' not in map");
    }

    iter->second->addInterval (value, lower, upper);
}

int FeatureMap::mapFeature (const string& feature, int value) const
{
    map <string, Feature*>::const_iterator iter = mFeatures.find (
        feature);

    if(iter == mFeatures.end ()) {
        throw new out_of_range ("Feature '" + feature + "' not in map");
    }

    return iter->second->map (value);
}

int FeatureMap::mapFeature (const string& feature, float value) const
{
    map <string, Feature*>::const_iterator iter = mFeatures.find (
        feature);

    if(iter == mFeatures.end ()) {
        throw new out_of_range ("Feature '" + feature + "' not in map");
    }

    return iter->second->map (value);
}

void FeatureMap::saveToFile (const string& filename) const
{
    ofstream file (filename.c_str ());

    float inf = numeric_limits<float>::infinity ();

    map <string, Feature*>::const_iterator iter = mFeatures.begin (),
        end = mFeatures.end ();
    for (;iter != end; iter ++) {
        Feature::interval_t intervals = iter->second->getIntervals ();
        Feature::interval_t::const_iterator iIter = intervals.begin (),
            iEnd = intervals.end ();
        for (;iIter != iEnd; iIter ++) {
            float lower = iIter->second.first;
            float upper = iIter->second.second;
            file << iter->first << "\t" << iIter->first << "\t";
            if (lower == -inf) {
                file << "-INF";
            } else {
                file << lower;
            }
            file << "\t";
            if (upper == inf) {
                file << "INF";
            } else {
                file << lower;
            }
            file << endl;
        }
    }

    file.close ();
}

void FeatureMap::loadFromFile (const string& filename)
{
    ifstream file (filename.c_str ());

    float inf = numeric_limits<float>::infinity ();

    while (!file.eof ()) {
        string name, value, lower, upper;
        file >> name >> value >> lower >> upper;
        if (name.empty () || value.empty () || lower.empty ()
            || upper.empty ())
        {
            break;
        }
        
        int v = atoi (value.c_str ());
        float l = 0;
        if (lower == "-INF") {
            l = -inf;
        } else {
            l = (float) atof (lower.c_str ());
        }
        float u = 0;
        if (upper == "INF") {
            u = inf;
        } else {
            u = (float) atof (upper.c_str ());
        }
        addInterval (name, v, l, u);
    }

    file.close ();
}
