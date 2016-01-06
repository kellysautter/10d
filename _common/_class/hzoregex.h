#ifndef _HZOREGEX_H_
#define _HZOREGEX_H_

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
#include <sys\\types.h>
#include <windows.h>


struct re_pattern_buffer;
struct re_registers;



class TBRegexp
{
  public:
    TBRegexp();
    TBRegexp( const char* pattern, bool use_fastmap = false );
    ~TBRegexp();

    void pattern( const char* pattern, bool use_fastmap = false );
    int search( const char* s, size_t pos, size_t len );
    int match( const char* s, size_t pos, size_t len );

  private:
    void compile( const char* pattern, bool use_fastmap );
    void free();

    TBRegexp( const TBRegexp& );
    TBRegexp& operator=( const TBRegexp& );

  private:
    re_pattern_buffer* pattern_buf_;
    re_registers* registers_;
    char* search_string_;
    size_t search_pos_;
};



/*
**----------------------------------------------------------------------
** .Klasse:       TBRegexp
**
** .Methode:      Default Konstruktor
**
** .Beschreibung: Initialisierung der Klasse ohne Suchmuster
**
** .Parameter:
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Default constructor. Construct myself with no pattern. The pattern
**      must be set using `pattern` function before calling any `search`
**      function.
**-----------------------------------------------------------------
*/
inline TBRegexp::TBRegexp() :
  pattern_buf_( 0 ),
  registers_( 0 ),
  search_string_( 0 ),
  search_pos_( 0 )
  {
  }


/*
**----------------------------------------------------------------------
** .Klasse:       TBRegexp
**
** .Methode:      Konstruktor
**
** .Beschreibung: Initialisierung der Klasse mit Suchmuster
**
** .Parameter:
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**       Construct myself with pattern `pattern`. `pattern` will be used in
**       all functions until it is reset. Uses fastmap buffer if
**       `use_fastmap` (default false) is true. It is recommended to use
**       fastmap when matching large strings.
**-----------------------------------------------------------------
*/
inline TBRegexp::TBRegexp( const char* pattern, bool use_fastmap ) :
  pattern_buf_( 0 ),
  registers_( 0 ),
  search_string_( 0 ),
  search_pos_( 0 )
{
    compile( pattern, use_fastmap );
}


/*
**----------------------------------------------------------------------
** .Klasse:       TBRegexp
**
** .Methode:      Default Destruktor
**
** .Beschreibung: Freigabe der Ressourcen
**
** .Parameter:
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**-----------------------------------------------------------------
*/
inline TBRegexp::~TBRegexp()
{
    free();
}


/*
**----------------------------------------------------------------------
** .Klasse:       TBRegexp
**
** .Methode:      pattern
**
** .Beschreibung: Angabe des Suchmusters
**
** .Parameter:  const char *, pattern_    , I, das Suchmuster
**              bool        , use_fastmap , I, BOOL -> FastMap benutzen
**
** .Rueckgabewert:
**
** ---------------------------------------------------------------------
**
** .Methodenbeschreibung:
**      Reset my pattern to `pattern`. Uses fastmap buffer if
**      `use_fastmap` (default false) is true. It is recommended to use
**      fastmap when matching large strings.
**      On Resumption: I am not usable until a valid pattern is set.
**-----------------------------------------------------------------
*/
inline void TBRegexp::pattern( const char* pattern_, bool use_fastmap )
{
    compile( pattern_, use_fastmap );
}


#endif
