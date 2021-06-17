#ifndef __DECISIONTREE_FEATUREMAP_H__
#define __DECISIONTREE_FEATUREMAP_H__

#include <stdexcept>
#include <limits>
#include <string>
#include <map>
#include <math.h>

class FeatureMap
{
public:
    enum FeatureType
    {
        FT_NOMINAL,
        FT_CONTINUOUS
    };

    /**
     * A feature definition
     */
    class Feature
    {
    public:
        /// Interval type
        typedef std::map<int, std::pair<float, float> > interval_t;

        virtual ~Feature () { }        

        /**
         * Add an interval to this feature definition
         * \param value the decision value to define an interval for
         * \param lower the lower bound of the interval
         * \param upper the upper bound of the interval
         */
        virtual void addInterval (int value, float lower, float upper) = 0;

        /**
         * Map an integer value.
         * \param value the value to map
         * \return the mapped value
         */
        virtual int map (int value) const = 0;

        /**
         * Map a float value.
         * \param value the value to map
         * \return the mapped value
         * \note Maps the float value in the appropriate interval to its value
         */
        virtual int map (float value) const = 0;

        /**
         * \return the feature's type
         */
        virtual FeatureType getType () const = 0;

        /**
         * \return the feature's intervals
         */
        virtual interval_t getIntervals () const = 0;
    };

    FeatureMap ();
    ~FeatureMap ();

    /**
     * Define a new feature.
     * \param feature the name of the feature
     * \param type the type of feature
     */
    void defineFeature (const std::string& feature, FeatureType type);

    /**
     * Add an interval to an existing feature
     * \param feature the name of the feature
     * \paramm value the value to define an interval for
     * \param lower the lower bound of the interval
     * \param upper the upper bound of the interval
     */
    void addInterval (const std::string& feature, int value,
        float lower, float upper);

    /**
     * Map an integer value on a feature.
     * \param feature the name of the feature
     * \param value the value to map
     * \return the mapped value
     * \see Feature::map()
     */
    int mapFeature (const std::string& feature, int value) const;

    /**
     * Map a float value on a feature.
     * \param feature the name of the feature
     * \param value the value to map
     * \return the mapped value
     * \see Feature::map()
     */
    int mapFeature (const std::string& feature, float value) const;

    /**
     * \return the defined features
     */
    const std::map <std::string, FeatureMap::Feature*>& getFeatures () const
    {
        return mFeatures;
    }

    /**
     * Save this feature map to a file.
     * \param filename the file to save to
     */
    void saveToFile (const std::string& filename) const;

    /**
     * Load a feature map from a file.
     * \param filename the file to load from
     */
    void loadFromFile (const std::string& filename);
private:
    class NominalFeature : public Feature
    {
    public:
        virtual ~NominalFeature () { }

        void addInterval (int value, float lower, float upper)
        {
            throw new std::invalid_argument (
                "Nominal features have no intervals");
        }

        int map (int value) const
        {
            return value;
        }

        int map (float value) const
        {
            return (int) floor (value + 0.5f);
        }

        FeatureType getType () const
        {
            return FT_NOMINAL;
        }

        interval_t getIntervals () const
        {
            return interval_t ();
        }
    };

    class ContinuousFeature : public Feature
    {
    public:
        virtual ~ContinuousFeature () { }

        void addInterval (int value, float lower, float upper)
        {
            mIntervals [value] = std::make_pair (lower, upper);
        }

        int map (int value) const
        {
            throw new std::invalid_argument (
                "Continuous features cannot map int values");
        }

        int map (float value) const
        {
            if (mIntervals.empty ()) {
                throw new std::out_of_range ("Feature has no intervals!");
            }

            interval_t::const_iterator iter = mIntervals.begin (),
                end = mIntervals.end ();
            for (;iter != end; iter ++) {
                if (value >= iter->second.first
                    && value < iter->second.second)
                {
                    return iter->first;
                }
            }
            return 0;
        }

        FeatureType getType () const
        {
            return FT_CONTINUOUS;
        }

        interval_t getIntervals () const
        {
            return mIntervals;
        }
    private:
        interval_t mIntervals;
    };
    
    std::map <std::string, Feature*> mFeatures;
};

#endif
