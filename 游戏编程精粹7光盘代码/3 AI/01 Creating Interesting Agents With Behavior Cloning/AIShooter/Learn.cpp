#include <iostream>
#include <time.h>

#include <DataSet.h>
#include <FeatureMap.h>
#include <TreeNode.h>

#include "TreeWriter.h"

using namespace std;

int main(int argc, char **argv)
{
    try {
        if (argc < 3) {
            cout << "Usage:" << endl << endl;
            cout << "  learn [<options>] <dataset_file(s)> <output_script_file>" << endl;
            cout << endl << "    Options:" << endl << endl;
            cout << "      -s <skip>    Skip <skip> entries at the beginning of the data" << endl;
            cout << "                   (Default: 100)" << endl;
            cout << "      -b <min>     Branch the tree only on at least <min> entries of data" << endl;
            cout << "                   (Default: 100)" << endl;
            return 1;
        }

        int skip = 100;
        int branch = 100;
        vector<string> dataFiles;
        string scriptFile;
        
        for (int i = 1; i < argc; i ++) {
            if (!strcmp (argv [i], "-s")) {
                if (i++ == argc) {
                    cout << "Invalid arguments" << endl;
                    return 2;
                }
                skip = atoi (argv [i]);
                if (skip == 0) {
                    cout << "Invalid skip value" << endl;
                    return 3;
                }
                cout << "Initial skip time set to " << skip << " entries ("
                    << skip * 10 << "ms)" << endl;
            } else if (!strcmp (argv [i], "-b")) {
                if (i++ == argc) {
                    cout << "Invalid arguments" << endl;
                    return 2;
                }
                branch = atoi (argv [i]);
                if (branch == 0) {
                    cout << "Invalid branch value" << endl;
                    return 3;
                }
                cout << "Minimum branch time set to " << branch << " entries ("
                    << branch * 10 << "ms)" << endl;
            } else {
                if (i + 1 == argc) {
                    scriptFile = argv [i];
                } else {
                    dataFiles.push_back (argv [i]);
                }
            }
        }

        if (dataFiles.empty ()) {
            cout << "No input file(s) specified!" << endl;
            return 3;
        }

        if (scriptFile.empty ()) {
            cout << "No output file specified!" << endl;
            return 3;
        }

        DataSet ds;
        for (size_t i = 0; i < dataFiles.size (); i ++) {
            ds.loadFromFile (dataFiles [i]);
        }

        FeatureMap fm;
        fm.defineFeature ("opp_distance", FeatureMap::FT_CONTINUOUS);
        fm.defineFeature ("opp_dir_to", FeatureMap::FT_CONTINUOUS);
        fm.defineFeature ("opp_dir_from", FeatureMap::FT_CONTINUOUS);
        fm.defineFeature ("opp_vnorm", FeatureMap::FT_CONTINUOUS);
        fm.defineFeature ("opp_ddir_to", FeatureMap::FT_CONTINUOUS);
        fm.defineFeature ("opp_ddir_from", FeatureMap::FT_CONTINUOUS);
        fm.defineFeature ("opp_hp", FeatureMap::FT_CONTINUOUS);
        fm.defineFeature ("opp_turning", FeatureMap::FT_NOMINAL);
        fm.defineFeature ("opp_accel", FeatureMap::FT_NOMINAL);
        fm.defineFeature ("my_hp", FeatureMap::FT_CONTINUOUS);
        fm.defineFeature ("my_turning", FeatureMap::FT_NOMINAL);
        fm.defineFeature ("my_accel", FeatureMap::FT_NOMINAL);
        fm.loadFromFile ("features.dat");

        time_t start = time (0);

        cout << "Beginning training (This may take a while)..." << endl;

        std::set<std::string> ignore;
        ignore.insert ("my_accel");
        ignore.insert ("my_fire");
        TreeNode *turnTree = ds.learnTree ("my_turning", ignore, skip, 15, 50);

        ignore.clear ();
        ignore.insert ("my_turning");
        ignore.insert ("my_fire");
        TreeNode *accelTree = ds.learnTree ("my_accel", ignore, skip, 15, 50);

        ignore.clear ();
        ignore.insert ("my_turning");
        ignore.insert ("my_accel");
        TreeNode *fireTree = ds.learnTree ("my_fire", ignore, skip, 15, 50);

        cout << "...done training!" << endl;

        time_t elapsed = time (0) - start;

        time_t hours = (elapsed / 3600);
        time_t minutes = (elapsed - (hours * 3600)) / 60;
        time_t seconds = (elapsed - (hours * 3600) - (minutes * 60));

        cout << "It took " << hours << " hour(s), "
            << minutes << " minute(s) and "
            << seconds << " second(s)." << endl;

        cout << "Writing Lua script..." << endl;

        TreeWriter treeWriter (fm, turnTree, accelTree, fireTree);
        treeWriter.writeToLua (scriptFile);

        cout << "...done." << endl;

    } catch (std::exception e) {
        cerr << "Exception caught: " << e.what () << endl;
        return 1;
    }

    return 0;
}

