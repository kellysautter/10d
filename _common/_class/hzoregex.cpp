/*
**----------------------------------------------------------------------
** .Klasse:        TBRegexp
**
** .Version:       01B
**
** .Aenderung:     31.06.1998
**
** .Beschreibung:  Klasse fuer Regular Expression Search
**
** .Basisklassen:
**
**----------------------------------------------------------------------
** .Klassenbeschreibung:
**      Die Klasse stellt einen C++-Wrapper um eine RegExp-Library von
**      GNU dar.
**      Ueber den Konstruktor, bzw. die Methode pattern laesst sich das
**      Suchmuster festlegen, nach dem mit der Methode search gesucht werden
**      kann.
**      Groesse des Suchbereichs: int !!!
**----------------------------------------------------------------------
** .Aenderungsvermerke:
**
** 1999.11.04   DKS   Z10   Cosmetic (so Brief can read it).
** 01A  =  Neuerstellung
**         (20.05.1998)  SL
** 01B  =  Erweiterung um match()
**         (31.06.1998)  SL
**
**----------------------------------------------------------------------
*/

/*----------------------------------------------------------------------
** .include
**----------------------------------------------------------------------
*/
#include "stdafx.h"
#include "hzoregex.h"
#include "regex.h"


/*
**----------------------------------------------------------------------
** .Klasse:       TBRegexp
**
** .Methode:      search
**
** .Beschreibung: Suchen nach einem Pattern
**
** .Parameter:  const char*, s      , I, String in dem gesucht wird
**              size_t     , pos    , I, Anfangsposition der Suche
**              size_t     , len    , I, Range in dem gesucht wird
**
** .Rueckgabewert:  int, bei Erfolg zero-based character-Index
**                       bei Nichtgefunden -1
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**-----------------------------------------------------------------
*/
int TBRegexp::search( const char* s, size_t pos, size_t len )
{
  search_string_ = (char*) s; // Casting away, but is not modified.
  search_pos_ = pos;
  size_t length = lstrlen( s );

  if ( len > length )
    len = length;

  int index
    = re_search( pattern_buf_, s, length, pos, len, registers_ );

  return index;
}


/*
**----------------------------------------------------------------------
** .Klasse:       TBRegexp
**
** .Methode:      match
**
** .Beschreibung: Groesse eines PatternMatches bestimmen
**
** .Parameter:  const char*, s      , I, String in dem gesucht wird
**              size_t     , pos    , I, Anfangsposition der Suche
**              size_t     , len    , I, Range in dem gesucht wird
**
** .Rueckgabewert:  int, Anzahl der Zeichen des Matches
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**-----------------------------------------------------------------
*/
int TBRegexp::match( const char* s, size_t pos, size_t len )
{
  search_string_ = (char*) s;
  search_pos_ = pos;

  return re_match( pattern_buf_, s, len, pos, registers_ );
}


/*
**----------------------------------------------------------------------
** .Klasse:       TBRegexp
**
** .Methode:      compile
**
** .Beschreibung: Initialisierung der Searchengine
**
** .Parameter:  const char*,  pattern,    I, das Suchmuster
**              bool       , use_fastmap, I, TRUE->benutze Fastmap
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Compile the pattern `pattern` into an internal form.
**      On Resumption: I am not usable until a valid pattern is set.
**-----------------------------------------------------------------
*/
void TBRegexp::compile( const char* pattern, bool use_fastmap )
{
  free();
  registers_ = new re_registers;
  pattern_buf_ = new re_pattern_buffer;
  ZeroMemory( pattern_buf_, sizeof( re_pattern_buffer ) );
  ZeroMemory( registers_, sizeof( re_registers ) );
  if ( use_fastmap )
    pattern_buf_->fastmap = (char*) malloc( 256 ); // 256 :-).
  const char* error =
    re_compile_pattern( pattern, lstrlen( pattern ), pattern_buf_ );
}



/*
**----------------------------------------------------------------------
** .Klasse:       TBRegexp
**
** .Methode:      free
**
** .Beschreibung: Freigabe interner Ressourcen
**
** .Parameter:
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Free my internal buffers and reset myself for new pattern setting.
**-----------------------------------------------------------------
*/
void TBRegexp::free()
{
  if ( pattern_buf_ )
    regfree( pattern_buf_ );
  delete registers_;
  delete pattern_buf_;
}

