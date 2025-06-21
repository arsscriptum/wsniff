//==============================================================================
//
//  DejaLib.h
//
//==============================================================================
//  Copyright (C) DejaTools, LLC.  All rights reserved.
//==============================================================================

#ifndef DEJA_LIB_H
#define DEJA_LIB_H

#ifdef DEJA_ENABLED
#undef DEJA_DISABLED
#else
#define DEJA_DISABLED
#endif

#include <stdio.h>

//==============================================================================
//  CONTROL MACROS
//==============================================================================
//
//  To disable ALL DejaLib functionality, simply define DEJA_DISABLED *before*
//  including DejaLib.h.
//
//==============================================================================

//==============================================================================
//  LOGGING FUNCTIONS
//==============================================================================
//
//  DEJA_TRACE          - Issue a normal  message to the Insight Trace Log.
//  DEJA_WARNING        - Issue a warning message to the Insight Trace Log.
//  DEJA_ERROR          - Issue an error  message to the Insight Trace Log.
//  DEJA_BOOKMARK       - Same as DEJA_TRACE, and bookmarks the message.
//
//  DEJA_TRACE_OFF      - Same as DEJA_TRACE   , except disabled by default.
//  DEJA_WARNING_OFF    - Same as DEJA_WARNING , except disabled by default.
//  DEJA_ERROR_OFF      - Same as DEJA_ERROR   , except disabled by default.
//  DEJA_BOOKMARK_OFF   - Same as DEJA_BOOKMARK, except disabled by default.
//
//==============================================================================

void DEJA_TRACE         ( const char*    pChannel, const char*    pFormat, ... );
void DEJA_WARNING       ( const char*    pChannel, const char*    pFormat, ... );
void DEJA_ERROR         ( const char*    pChannel, const char*    pFormat, ... );
void DEJA_BOOKMARK      ( const char*    pChannel, const char*    pFormat, ... );

void DEJA_TRACE_OFF     ( const char*    pChannel, const char*    pFormat, ... );
void DEJA_WARNING_OFF   ( const char*    pChannel, const char*    pFormat, ... );
void DEJA_ERROR_OFF     ( const char*    pChannel, const char*    pFormat, ... );
void DEJA_BOOKMARK_OFF  ( const char*    pChannel, const char*    pFormat, ... );

//------------------------------------------------------------------------------
//  Wide character versions.
//------------------------------------------------------------------------------

void DEJA_TRACE         ( const wchar_t* pChannel, const char*    pFormat, ... );
void DEJA_TRACE         ( const char*    pChannel, const wchar_t* pFormat, ... );
void DEJA_TRACE         ( const wchar_t* pChannel, const wchar_t* pFormat, ... );

void DEJA_WARNING       ( const wchar_t* pChannel, const char*    pFormat, ... );
void DEJA_WARNING       ( const char*    pChannel, const wchar_t* pFormat, ... );
void DEJA_WARNING       ( const wchar_t* pChannel, const wchar_t* pFormat, ... );

void DEJA_ERROR         ( const wchar_t* pChannel, const char*    pFormat, ... );
void DEJA_ERROR         ( const char*    pChannel, const wchar_t* pFormat, ... );
void DEJA_ERROR         ( const wchar_t* pChannel, const wchar_t* pFormat, ... );

void DEJA_BOOKMARK      ( const wchar_t* pChannel, const char*    pFormat, ... );
void DEJA_BOOKMARK      ( const char*    pChannel, const wchar_t* pFormat, ... );
void DEJA_BOOKMARK      ( const wchar_t* pChannel, const wchar_t* pFormat, ... );

void DEJA_TRACE_OFF     ( const wchar_t* pChannel, const char*    pFormat, ... );
void DEJA_TRACE_OFF     ( const char*    pChannel, const wchar_t* pFormat, ... );
void DEJA_TRACE_OFF     ( const wchar_t* pChannel, const wchar_t* pFormat, ... );

void DEJA_WARNING_OFF   ( const wchar_t* pChannel, const char*    pFormat, ... );
void DEJA_WARNING_OFF   ( const char*    pChannel, const wchar_t* pFormat, ... );
void DEJA_WARNING_OFF   ( const wchar_t* pChannel, const wchar_t* pFormat, ... );

void DEJA_ERROR_OFF     ( const wchar_t* pChannel, const char*    pFormat, ... );
void DEJA_ERROR_OFF     ( const char*    pChannel, const wchar_t* pFormat, ... );
void DEJA_ERROR_OFF     ( const wchar_t* pChannel, const wchar_t* pFormat, ... );

void DEJA_BOOKMARK_OFF  ( const wchar_t* pChannel, const char*    pFormat, ... );
void DEJA_BOOKMARK_OFF  ( const char*    pChannel, const wchar_t* pFormat, ... );
void DEJA_BOOKMARK_OFF  ( const wchar_t* pChannel, const wchar_t* pFormat, ... );

//==============================================================================
//  GENERAL FUNCTIONALITY
//==============================================================================
//
//  DEJA_FLUSH(false)   - Begin process of sending out any currently buffered
//                        information.  Do not wait for data to be sent.
//  DEJA_FLUSH(true)    - Send out all buffered data.  Do not return until all
//                        data has been sent.
//  DEJA_TERMINATE      - Shut down the DejaLib.  If dynamic memory was 
//                        allocated, it will be released.  All buffers are
//                        flushed.
//
//  DEJA_APP_LABEL      - Give a meaningful name to the application.  
//                        Examples: "Server" or "Client:7".
//                        
//  DEJA_THREAD_INIT    - Explicitly announce the start of a new thread to 
//                        Insight.  Should be executed within the thread.
//  DEJA_THREAD_KILL    - Explicitly announce the end of a thread to Insight.
//                        Should be executed within the thread.
//  DEJA_THREAD_LABEL   - Provide a meaningful name to the current thread.
//                        Examples: "main" or "Physics" or "Worker".
//
//------------------------------------------------------------------------------
//
//  DEJA_SET_AUTO_FLUSH - Turn auto flush on or off.
//
//------------------------------------------------------------------------------
//
//  DEJA_LIB_CONNECTED  - Returns false if the DejaLib failed to connect or is
//                        no longer connected to an Insight session.
//
//  DEJA_LAST_ERROR     - Returns a string describing the last error the DejaLib
//                        encountered.
//
//------------------------------------------------------------------------------
//                        
//  DEJA_SET_PARAMETER  - Change a DejaLib parameter using one of the listed 
//                        deja_parameter identifiers.  See documentation for
//                        list of available parameters.
//
//  DEJA_SET_VALUE      - Set a value to a variable which affects Insight.  See
//                        documentation for list of available options.
//
//------------------------------------------------------------------------------
//                        
//  DEJA_ENUM_VALUE     - Provide an enumeration value and name  to Insight.
//  DEJA_ENUM_ALIAS     - Provide an enumeration value and alias to Insight.
//
//------------------------------------------------------------------------------
//                        
//  If the DejaLib needs to use dynamic memory...
//                        
//  DEJA_SET_MALLOC     - Set the malloc function DejaLib will use.
//  DEJA_SET_FREE       - Set the free   function DejaLib will use.
//
//==============================================================================

      void  DEJA_FLUSH          ( bool Block = true );
      void  DEJA_NEXT_FRAME     ( void );
      void  DEJA_TERMINATE      ( void );
  
      void  DEJA_APP_LABEL      ( const char* pLabelFormat, ... );
  
      void  DEJA_THREAD_INIT    ( void );
      void  DEJA_THREAD_KILL    ( int ExitCode );
      void  DEJA_THREAD_LABEL   ( const char* pLabelFormat, ... );
  
      void  DEJA_SET_AUTO_FLUSH ( bool AutoFlush );
  
      bool  DEJA_LIB_CONNECTED  ( void );
const char* DEJA_LAST_ERROR     ( void );

//------------------------------------------------------------------------------
//  DejaLib configuration parameters.
//------------------------------------------------------------------------------

enum deja_parameter
{
    DEJA_CACHE_SIZE_BYTES_FILE,
    DEJA_CACHE_SIZE_BYTES_LABEL,
    DEJA_CACHE_SIZE_BYTES_CHANNEL,
    DEJA_CACHE_SIZE_BYTES_FORMAT,
    DEJA_CACHE_SIZE_BYTES_ARGUMENT,

    DEJA_CACHE_SIZE_ITEMS_FILE,
    DEJA_CACHE_SIZE_ITEMS_LABEL,
    DEJA_CACHE_SIZE_ITEMS_CHANNEL,
    DEJA_CACHE_SIZE_ITEMS_FORMAT,
    DEJA_CACHE_SIZE_ITEMS_ARGUMENT,

    DEJA_BUFFER_SIZE_BYTES,
    DEJA_BUFFER_COUNT,

    DEJA_XBOX360_THREAD_PROCESSOR
};

void DEJA_SET_PARAMETER ( deja_parameter Parameter, int Value );

//------------------------------------------------------------------------------
//  Insight control settings.
//------------------------------------------------------------------------------

void DEJA_SET_VALUE     ( const char* pVariableName, int Value );

//------------------------------------------------------------------------------
//  Enumeration support.
//------------------------------------------------------------------------------
/*
For enumerated type E:

    void DEJA_ENUM_VALUE( E, int EValue );
    void DEJA_ENUM_ALIAS( E, int EValue, const char* pEAlias );
*/
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  EXAMPLE
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//
//  enum compass { COMPASS_NORTH, COMPASS_SOUTH, COMPASS_EAST, COMPASS_WEST };
//  
//  DEJA_ENUM_VALUE( compass, COMPASS_NORTH );
//  DEJA_ENUM_ALIAS( compass, COMPASS_NORTH, "North" );
//
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//------------------------------------------------------------------------------
//  Dynamic memory support.
//------------------------------------------------------------------------------

typedef void* deja_std_malloc_fn ( size_t                              );
typedef void* deja_dbg_malloc_fn ( size_t, const char* pFile, int Line );
typedef void  deja_std_free_fn   ( void*                               );
typedef void  deja_dbg_free_fn   ( void*,  const char* pFile, int Line );

void DEJA_SET_MALLOC    ( deja_std_malloc_fn*  pStdMallocFn  );
void DEJA_SET_MALLOC    ( deja_dbg_malloc_fn*  pDbgMallocFn  );
void DEJA_SET_FREE      ( deja_std_free_fn*    pStdFreeFn    );
void DEJA_SET_FREE      ( deja_dbg_free_fn*    pDbgFreeFn    );

//==============================================================================
//  HEAP OPERATIONS
//==============================================================================
//
//  DEJA_LOG_MALLOC         - Log a memory allocation   from malloc.
//  DEJA_LOG_REALLOC        - Log a memory reallocation from realloc.
//  DEJA_LOG_FREE           - Log a memory free         from free.
//
//  DEJA_LOG_NEW            - Log a memory allocation   from new.
//  DEJA_LOG_DELETE         - Log a memory free         from delete.
//
//  DEJA_LOG_NEW_ARRAY      - Log a memory allocation   from new [].
//  DEJA_LOG_DELETE_ARRAY   - Log a memory free         from delete [].
//
//==============================================================================

void DEJA_LOG_MALLOC        ( void* pMemory, size_t Size );
void DEJA_LOG_MALLOC        ( void* pMemory, size_t Size, const char*    pFileName, int Line );
void DEJA_LOG_MALLOC        ( void* pMemory, size_t Size, const wchar_t* pFileName, int Line );

void DEJA_LOG_REALLOC       ( void* pNewMem, void* pOldMem, size_t NewSize );
void DEJA_LOG_REALLOC       ( void* pNewMem, void* pOldMem, size_t NewSize, const char*    pFileName, int Line );
void DEJA_LOG_REALLOC       ( void* pNewMem, void* pOldMem, size_t NewSize, const wchar_t* pFileName, int Line );

void DEJA_LOG_FREE          ( void* pMemory );
void DEJA_LOG_FREE          ( void* pMemory, const char*    pFileName, int Line );
void DEJA_LOG_FREE          ( void* pMemory, const wchar_t* pFileName, int Line );

void DEJA_LOG_NEW           ( void* pMemory, size_t Size );
void DEJA_LOG_NEW           ( void* pMemory, size_t Size, const char*    pFileName, int Line );
void DEJA_LOG_NEW           ( void* pMemory, size_t Size, const wchar_t* pFileName, int Line );

void DEJA_LOG_DELETE        ( void* pMemory );
void DEJA_LOG_DELETE        ( void* pMemory, const char*    pFileName, int Line );
void DEJA_LOG_DELETE        ( void* pMemory, const wchar_t* pFileName, int Line );

void DEJA_LOG_NEW_ARRAY     ( void* pMemory, size_t Size );
void DEJA_LOG_NEW_ARRAY     ( void* pMemory, size_t Size, const char*    pFileName, int Line );
void DEJA_LOG_NEW_ARRAY     ( void* pMemory, size_t Size, const wchar_t* pFileName, int Line );

void DEJA_LOG_DELETE_ARRAY  ( void* pMemory );
void DEJA_LOG_DELETE_ARRAY  ( void* pMemory, const char*    pFileName, int Line );
void DEJA_LOG_DELETE_ARRAY  ( void* pMemory, const wchar_t* pFileName, int Line );

//==============================================================================
//  CONTEXT
//==============================================================================
//
//  DEJA_CONTEXT        - Enter and name a context.  Only one allowed per scope.
//                        Exit is automatic when execution leaves the scope.
//                        The parameter must be a literal string.
//
//==============================================================================

void DEJA_CONTEXT       ( const char* pContextLabel );                         

//==============================================================================
//  CONSOLE FUNCTIONS - UNDER DEVELOPMENT
//==============================================================================
//
//  DEJA_CONSOLE_READ   - Used to read command lines entered via the Insight
//                        Console view.  If a command line is available, it is
//                        copied into the buffer provided.  Otherwise, the 
//                        first character of the buffer is set to '\0'.  If a
//                        command line is longer than the specified buffer size,
//                        the command line will be clipped.  (The current 
//                        maximum command line size is 256 characters.)
//
//  DEJA_CONSOLE_WRITE  - Sends text to the Insight Console view.
//
//  DEJA_CONSOLE_ECHO   - Used to echo back console command lines to Insight so
//                        as to properly reflect *when* the command line was 
//                        processed.  If command lines are not echoed back,
//                        Insight will automatically display the text as it was
//                        entered in the Console window but will not show a time
//                        value.      
//
//==============================================================================

void    DEJA_CONSOLE_READ   ( char* pBuffer, int BufferSize );

void    DEJA_CONSOLE_WRITE  ( const char*    pNFormat, ... );
void    DEJA_CONSOLE_WRITE  ( const wchar_t* pWFormat, ... );

void    DEJA_CONSOLE_ECHO   ( const char*    pCommandLine );

//==============================================================================
//  PRIVATE
//==============================================================================

#include "DejaPrivate.h"

//==============================================================================
#endif // DEJA_LIB_H
//==============================================================================
