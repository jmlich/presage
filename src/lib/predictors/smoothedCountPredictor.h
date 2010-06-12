
/******************************************************
 *  Presage, an extensible predictive text entry system
 *  ---------------------------------------------------
 *
 *  Copyright (C) 2008  Matteo Vescovi <matteo.vescovi@yahoo.co.uk>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
                                                                             *
                                                                **********(*)*/


#ifndef PRESAGE_SMOOTHEDCOUNTPREDICTOR
#define PRESAGE_SMOOTHEDCOUNTPREDICTOR

#include "predictor.h"
#include "../core/dispatcher.h"

#include <assert.h>
#include <stdlib.h>    // double atof( const char* )

#if defined(HAVE_SQLITE3_H) 
# include <sqlite3.h>
#elif defined(HAVE_SQLITE_H)
# include <sqlite.h>
#else
# error "SQLite is required. Please install SQLite."
#endif

/** Smoothed count statistical predictor
 *
 */
class SmoothedCountPredictor : public Predictor, public Observer {
public:
    SmoothedCountPredictor(Configuration*, ContextTracker*);
    ~SmoothedCountPredictor();

    virtual Prediction predict(const size_t size, const char** filter) const;

    virtual void learn(const std::vector<std::string>& change);

    virtual void update (const Observable* variable);

    void set_unigram_weight (const std::string& value);
    void set_bigram_weight (const std::string& value);
    void set_trigram_weight (const std::string& value);
    void set_dbfilename (const std::string& value);

private:
    std::string strtolower( const std::string& ) const;

#if defined(HAVE_SQLITE_H)
    sqlite* db;
#elif defined(HAVE_SQLITE3_H)
    sqlite3* db;
#endif

    double      unigram_weight;
    double      bigram_weight;
    double      trigram_weight;
    std::string dbfilename;
    int         MAX_PARTIAL_PREDICTION_SIZE;

    static const char* LOGGER;
    static const char* UNIGRAM_WEIGHT;
    static const char* BIGRAM_WEIGHT;
    static const char* TRIGRAM_WEIGHT;
    static const char* DBFILENAME;

    Dispatcher<SmoothedCountPredictor> dispatcher;
};


/** Structure used to passed all necessary data through to callback function buildPrediction.
 *
 * This structure is meant to be passed as fourth argument of function
 * sqlite_exec. This argument is passed straight to function register
 * as callback function by sqlite_exec. The fourth argument has to be
 * a pointer to void. Function buildPrediction receives this void
 * pointer as its first argument. It is cast back to CallBack* before
 * each data member is used.
 *
 * predPtr is a pointer to a Prediction object used to hold partial prediction results.
 * predSize is an integer used to indicate the maximum amount of suggestions to retrieve.
 *
 */
struct CallbackData {
    Prediction* predPtr;
    int predSize;
};

/** Sqlite callback function that builds a prediction objects from the results returned by sqlite_exec call.
 *
 */
int buildPrediction( void*, int, char**, char** );


#endif // PRESAGE_SMOOTHEDCOUNTPREDICTOR