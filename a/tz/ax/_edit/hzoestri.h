#ifndef _HZOESTRI_H_
#define _HZOESTRI_H_

/*
**----------------------------------------------------------------------
**
** .Name:          HZOESTRI.H
**
** .Version:       01B
**
** .Aenderung:     16.07.1998
**
** .Beschreibung:  Header fuer TBEdit Control
**
**----------------------------------------------------------------------
** .Modulbeschreibung:
**
**  Enthaelt Stringdefines, die das TBEdit Control fuer die Registrierung
**  verwendet.
**  REGISTRYKEY legt den Key in HKEY_LOCAL_MACHINE fest, der
**  verwendet wird.
**
**----------------------------------------------------------------------
** .Aenderungsvermerke:
**
** 01A  =  Neuerstellung
**         (15.05.1998)  SL
** 01B  =  Neue Registry-Eintraege fuer UNDO und Kommentarpuffer
**         (16.07.1998)  SL
** 01C  =  Cosmetic (so Brief can read it)
**         (16.03.2000)  DKS
**
**----------------------------------------------------------------------
*/




/*----------------------------------------------------------------------
** .define
**----------------------------------------------------------------------
*/
#define PROGNAME            "TBEditor"
#define REGISTRYKEY         "SOFTWARE\\Ton Beller\\TBEditor"
#define DATEIENG            "unknown.txt"
#define DATEIGER            "datei.txt"


#define REGFONT             "Font"
#define REGSIZE             "Size"
#define REGBACK             "ColorBack"
#define REGFORE             "ColorFore"
#define REGWORD             "ColorKey"
#define REGCOMMENT          "ColorComment"
#define REGTABSTOP          "Tabstop"
#define REGUNDOLEVEL        "UndoLevel"
#define REGBUFFERSIZE       "BufferSize"


#endif
