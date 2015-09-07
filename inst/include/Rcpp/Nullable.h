// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; indent-tabs-mode: nil; -*-
//
// Nullable.h: Rcpp R/C++ interface class library -- SEXP container which can be NULL
//
// Copyright (C) 2015         Dirk Eddelbuettel
//
// This file is part of Rcpp.
//
// Rcpp is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// Rcpp is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Rcpp.  If not, see <http://www.gnu.org/licenses/>.

#ifndef Rcpp_Nullable_h
#define Rcpp_Nullable_h

// This class could possibly be written in a templated manner too, and we looked
// into this.  However, as an exception is thrown as soon as an actual proxy
// object is accessed _when it was initialized with NULL_ we found no
// satisfactory solution.
//
// We looked into the safe_bool_idiom [1] but found that more trouble than is
// warranted here.  We first and foremost want an operator SEXP() which got in
// the way of redefining operator bool.
// [1] http://www.artima.com/cppsource/safebool.html)

namespace Rcpp {

    class Nullable {
    public:

        /**
         * Empty no-argument constructor of a Nullable object
         *
         * set validator to FALSE
         */
        inline Nullable() : m_sexp(NULL), m_set(false) {}

        /**
         * Standard constructor of a Nullable object
         *
         * @param SEXP is stored
         */
        inline Nullable(SEXP t) : m_sexp(t), m_set(true) {}

        /**
         * Copy constructor for Nullable object
         *
         * @param SEXP is used to update internal copy
         */
        inline Nullable &operator=(SEXP sexp) {
            m_sexp = sexp;
            m_set = true;
            return *this;
        }

        /**
         * operator SEXP() to return nullable object
         *
         * @throw 'not initialized' if object has not been set
         */
        inline operator SEXP() {
            checkIfSet();
            return m_sexp;
        }

        /**
         * get() accessor for object
         *
         * @throw 'not initialized' if object has not been set
         */
        inline SEXP get() {
            checkIfSet();
            return m_sexp;
        }

        /**
         * Boolean test for NULL
         *
         * @throw 'not initialized' if object has not been set
         */
        inline bool isNull() {
            checkIfSet();
            return Rf_isNull(m_sexp);
        }

        /**
         * Boolean test for not NULL
         *
         * @throw 'not initialized' if object has not been set
         */
        inline bool isNotNull() {
            return ! isNull();
        }

        /**
         * Test function to check if object has been initialized
         *
         */
        inline bool isSet(void) { return m_set; }

    private:
        SEXP m_sexp;
        bool m_set;

        inline void checkIfSet(void) {
            if (!m_set) {
                throw ::Rcpp::exception("Not initialized");
            }
        }
    };
}

#endif
