/***************************************************************************
  file : $URL$
  version : $LastChangedRevision$  $LastChangedBy$
  date : $LastChangedDate$
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 * Copyright (C) 2007 by Johan De Taeye                                    *
 *                                                                         *
 * This library is free software; you can redistribute it and/or modify it *
 * under the terms of the GNU Lesser General Public License as published   *
 * by the Free Software Foundation; either version 2.1 of the License, or  *
 * (at your option) any later version.                                     *
 *                                                                         *
 * This library is distributed in the hope that it will be useful,         *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser *
 * General Public License for more details.                                *
 *                                                                         *
 * You should have received a copy of the GNU Lesser General Public        *
 * License along with this library; if not, write to the Free Software     *
 * Foundation Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA *
 *                                                                         *
 ***************************************************************************/

/** @file freppleinterface.h
  * @brief Public header file for C and C++.
  *
  * This is the public header file for high-level access to the library
  * functionality.<br>
  * The methods listed provide also a safe interface API for accessing the
  * library functionality from C, C++, Visual Basic and other programming
  * languages.
  *
  * When extending the library, use the header file frepple.h instead.<br>
  * It provides a more detailed API to interface with frePPLe.
  */

#ifndef FREPPLE_INTERFACE_H
#define FREPPLE_INTERFACE_H

#ifdef __cplusplus
#include <string>
#endif

// For a windows shared library we use the C calling convention: __stdcall.
// Only such functions can be called from VBA...
// For cygwin we don't use the __stdcall, but still need the export/import.
#undef DECLARE_EXPORT
#if defined(WIN32) && !defined(DOXYGEN)
  #ifdef __CYGWIN__
    #ifdef FREPPLE_CORE
      #define DECLARE_EXPORT(type) __declspec (dllexport) type
    #else
      #define DECLARE_EXPORT(type) __declspec (dllimport) type
    #endif
  #else
    #ifdef FREPPLE_CORE
      #define DECLARE_EXPORT(type) __declspec (dllexport) type __stdcall
    #else
      #define DECLARE_EXPORT(type) __declspec (dllimport) type __stdcall
    #endif
  #endif
#else
  #define DECLARE_EXPORT(type) type
#endif

/** This method returns a version string. */
DECLARE_EXPORT(const char*) FreppleVersion();

/** This function should be called once when the client application starts,
  * and before calling any other function in the API.<br>
  * The parameter is a string with the name of the Frepple home directory.
  * If the parameter is NULL, the setting of the environment variable
  * FREPPLE_HOME is used instead.
  *
  * This method is synchroneous, i.e. it returns only when the complete
  * processing is finished. The method can throw exceptions, and the client
  * is responsible for defining the correct handlers for these.
  */
DECLARE_EXPORT(void) FreppleInitialize(const char*);

/** The character buffer pointed to by the first parameter contains data in
  * XML format that is passed on to Frepple for processing.<br>
  * The second argument specifies whether frepple should validate the data
  * against the XSD schema.<br>
  * The last argument specifies whether Frepple needs to perform only the
  * validation and skip the actual processing.
  *
  * The client is responsible for the memory management in the data buffer.
  *
  * This method is synchroneous, i.e. it returns only when the complete
  * processing is finished. The method can throw exceptions, and the client
  * is responsible for defining the correct handlers for these.
  */
DECLARE_EXPORT(void) FreppleReadXMLData(char*, bool, bool);

/** The first parameter is the name of a file that contains data in XML
  * format for Frepple processing. If a NULL pointer is passed, frepple
  * will read from the standard input.<br>
  * The second argument specifies whether frepple should validate the data
  * against the XSD schema.<br>
  * The last argument specifies whether Frepple needs to perform only the
  * validation and skip the actual processing.
  *
  * This method is synchroneous, i.e. it returns only when the complete
  * processing is finished. The method can throw exceptions, and the client
  * is responsible for defining the correct handlers for these.
  */
DECLARE_EXPORT(void) FreppleReadXMLFile(const char*, bool, bool);

/** Calling this function will save the Frepple data in the file that
  * is passed as the argument.
  *
  * This method is synchroneous, i.e. it returns only when the complete
  * processing is finished. The method can throw exceptions, and the client
  * is responsible for defining the correct handlers for these.
  */
DECLARE_EXPORT(void) FreppleSaveFile(char*);

#ifdef __cplusplus
/** Calling this function returns a text buffer with the frepple data
  * model.
  *
  * This method can consume a lot of memory for big models!<br>
  * This method is synchroneous, i.e. it returns only when the complete
  * processing is finished. The method can throw exceptions, and the client
  * is responsible for defining the correct handlers for these.
  *
  * This function is only available when using C++. The same functionality
  * is available to C with the function FreppleWrapperSaveString.
  * @see FreppleWrapperSaveString
  */
DECLARE_EXPORT(std::string) FreppleSaveString();
#endif

/** This function causes the frepple executable to shut down in an orderly
  * way.
  *
  * This method is synchroneous, i.e. it returns only when the complete
  * processing is finished. The method can throw exceptions, and the client
  * is responsible for defining the correct handlers for these.
  */
DECLARE_EXPORT(void) FreppleExit();

#ifdef __cplusplus
/** Echo a message in the frePPLe log stream (which is either a file or
  * the standard output stream).
  *
  * This function is only available when using C++. The same functionality
  * is available to C with the function FreppleLog(const char*).
  */
DECLARE_EXPORT(void) FreppleLog(const std::string);
#endif
/** Echo a message in the frePPLe log stream (which is either a file or
  * the standard output stream).
  */
DECLARE_EXPORT(void) FreppleLog(const char*);

/* The functions listed below can be called from C. */
#ifdef __cplusplus
extern "C"
{
#endif

  /** Same as FreppleInitialize, but catches all exceptions and returns a
    * status instead.
    *
    * Use this function when calling the library from C or VB applications.
    * @see FreppleInitialize
    */
  DECLARE_EXPORT(int) FreppleWrapperInitialize(const char*);

  /** Same as FreppleReadXMLData, but catches all exceptions and returns a
    * status instead.
    *
    * Use this function when calling the library from C or VB applications.
    * @see FreppleReadXMLData
    */
  DECLARE_EXPORT(int) FreppleWrapperReadXMLData(char*, bool, bool);

  /** Same as FreppleReadXMLFile, but catches all exceptions and returns a
    * status instead.
    *
    * Use this function when calling the library from C or VB applications.
    * @see FreppleReadXMLFile
    */
  DECLARE_EXPORT(int) FreppleWrapperReadXMLFile(const char*, bool, bool);

  /** Same as FreppleSaveFile, but catches all exceptions and returns a
    * status instead.
    *
    * Use this function when calling the library from C or VB applications.
    * @see FreppleSaveFile
    */
  DECLARE_EXPORT(int) FreppleWrapperSaveFile(char*);

  /** Same as FreppleSaveString, but catches all exceptions, returns a status
    * code and also leaves the memory buffer management to the user.<br>
    * This function can consume a lot of memory for big models!
    *
    * Use this function when calling the library from C or VB applications.
    * @see FreppleSaveString
    */
  DECLARE_EXPORT(int) FreppleWrapperSaveString(char*, unsigned long);

  /** Same as FreppleExit, but catches all exceptions and returns a
    * status instead.
    *
    * Use this function when calling the library from C or VB applications.
    * @see FreppleExit
    */
  DECLARE_EXPORT(int) FreppleWrapperExit();

#ifdef __cplusplus
}  // End of "extern C"
#endif

#endif    // End of FREPPLE_INTERFACE_H