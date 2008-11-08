
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


#ifndef PRESAGE
#define PRESAGE

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string>
#include <vector>

#include "presageException.h"

class Configuration;
class ProfileManager;
class Profile;
class ContextTracker;
class PluginRegistry;
class Predictor;
class Selector;

/** \brief Presage, the intelligent predictive text entry platform.
 */
class Presage {
  public:
    /** Creates and initializes presage.
     */
    Presage();
    /** Creates and initializes presage with supplied configuration.
     * \param config path to configuration file
     */
    Presage(const std::string config);
    ~Presage();

    /** \brief Obtain a prediction, and notify presage of newly
     *         entered text (if any, else empty string).
     *
     * This method notifies presage that new text was entered by
     * the user and requests that presage generates a prediction
     * based on the newly updated context.
     *
     * It is possible to invoke this method passing an empty string as
     * argument, in cases where no text was entered by the user since
     * the previous invocation.
     *
     * This method returns a prediction based on the current
     * context. The prediction is a std::vector of std::string's.
     *
     * \param str user entered text
     * \return prediction
     *
     */
    std::vector<std::string> predict(std::string str);

    /** \brief Notifies presage that new text was entered.
     *
     * Notifies presage that new text was entered, without
     * requesting a new prediction to be generated.
     *
     * \param str user entered text
     */
    void update(std::string str);

    /** \brief Informs presage that a prediction was successful.
     *
     * Informs presage that a prediction was successful. The
     * successful prediction is passed in as argument. Presage
     * updates its internal tracker with the successful prediction.
     * 
     * \param str successful prediction
     */
    void complete(std::string str);

    /** \brief Returns the text entered so far.
     *
     * \return context, text entered so far.
     */
    std::string context() const;

    /** \brief Returns true if a context change occured.
     *
     * \return true if a context change occured after the last update
     * or predict calls, or false otherwise.
     */
    bool context_change() const;

    /** \brief Returns the current prefix.
     *
     * \return prefix
     */
    std::string prefix() const;

    /** \brief Gets the value of specified configuration variable.
     *
     * Programmatically get the value currently assigned to the
     * specified configuration \param variable
     *
     * \return value assigned to configuration variable.
     */
    std::string config(const std::string variable) const;

    /** \brief Sets the value of specified configuration variable.
     *
     * Programmatically set the specified configuration \param
     * variable to \param value . This will override the setting read
     * from the configuration file in use.
     *
     */
    void config(const std::string variable, const std::string value);

  private:
    ProfileManager* profileManager;
    Profile*        profile;
    Configuration*  configuration;
    PluginRegistry* pluginRegistry;
    ContextTracker* contextTracker;
    Predictor*      predictor;
    Selector*       selector;

};

/** \mainpage 

\section intro_section Introduction

Presage is an intelligent predictive text entry
platform. Presage exploits <a
href="http://en.wikipedia.org/wiki/Redundancy_%28information_theory%29">redundant
information</a> embedded in natural languages to generate
predictions. Presage's modular and pluggable <a
href="?q=node/14">architecture</a> allows its <a
href="http://en.wikipedia.org/wiki/Language_modeling">language
model</a> to be extended and customized to utilize statistical,
syntactic, and semantic information sources.

A predictive text entry system attempts to improve ease and speed of
textual input. Word prediction consists in computing which word tokens
or word completions are most likely to be entered next. The system
analyses the text already entered and combines the information thus
extracted with other information sources to calculate a set of most
probable tokens.

A typical presage-based application would display the set of most
probable tokens (i.e. a list of suggestions) to the user and
automatically enter the desired token after the user selects it. If
the list of suggestions does not contain the desired word, the user
continues entering text until the correct suggestion is offered or
until the user is done entering text.

Presage is fundamentally different from predictive <a
href="http://en.wikipedia.org/wiki/T9_%28predictive_text%29">input
technologies commonly found on mobile phones</a>, which might more
accurately be described as 'disambiguating text entry' rather than
'predictive text entry' systems.  Such systems do not try to guess
what the user intends to write in the future, only to determine what
they most-likely intend to write in the present, given their past
input.  Presage, on the other hand, actively predicts the what the
user intends to write, and only reverts to <a
href="http://en.wikipedia.org/wiki/Word_completion">word
completion</a> mode if the prediction did not contain the desired
token.

Presage is <a
href="http://www.gnu.org/philosophy/free-sw.html">free
software</a>. It is distributed under the term of the <a
href="http://www.gnu.org/copyleft/gpl.html">General Public
License</a>.

\author Matteo Vescovi 

\section getting_started_section Getting started
\include getting_started.txt

*/

#endif // PRESAGE