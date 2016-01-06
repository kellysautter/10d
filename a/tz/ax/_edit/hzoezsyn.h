#ifndef _HZOEZSYN_H_
#define _HZOEZSYN_H_

/*
**----------------------------------------------------------------------
** .Klasse:        ZeidonSyntax
**
** .Version:       01E
**
** .Aenderung:     09.11.2000
**
** .Beschreibung:  Syntaxtabelle fuer Zeidon syntax Coloring
**
** .Basisklassen:  SyntaxColoring
**
**----------------------------------------------------------------------
** .Klassenbeschreibung:
**      Die Tabelle enthaelt die Zeidon - Keywords, die gecolored werden
**      sollen. Neue Keywords koennen belieig eingefuegt werden. Die
**      Reihenfolge ist bedeutungslos.  Gross-/Kleinschreibung ist
**      bedeutungslos.
**      Wichtig: KEY_ARR_SIZE ist mit 30 definiert. Sollen laengere
**               Keywords definiert werden, muss der Wert in HZOESYN.H
**               geaendert werden.
**      Wichtig: Eine Erweiterung der Tabelle erfordert einen Recompile
**----------------------------------------------------------------------
** .Aenderungsvermerke:
**
** 01A  =  Neuerstellung
**         (20.05.1998)  SL
** 01B  =  Weitere Keywords eingefuegt
**         (02.07.1998)  SL
** 01C  =  Neue Keywords: FIRST und OR
**         (17.02.1999)  SL
** 01D  =  Cosmetic (so Brief can read it)
**         (16.03.2000)  DKS
** 01E  =  add UPDATE to list of keywords
**         (09.11.2000)  TMV
** 01F  =  add RESULT to list of keywords
**         (26.06.2001)  TMV
**
**----------------------------------------------------------------------
*/

static char cZeidon[][KEY_ARR_SIZE] = {
        "ACTIVATE",
        "AFTER",
        "AND",
        "ANY",
        "APPLICATION",
        "AS",
        "AT",
        "ATTRIBUTE",
        "ATTRIBUTENAME",
        "BASED",
        "BEFORE",
        "CASE",
        "CHARACTER",
        "COMMIT",
        "CONSTRAINT",
        "CONTINUE",
        "CREATE",
        "CURSOR",
        "DECIMAL",
        "DELETE",
        "DERIVED",
        "DIALOG",
        "DOMAIN",
        "DOES",
        "EACH",
        "ELSE",
        "EMPTY",
        "END",
        "ENTITY",
        "ENTITYNAME",
        "EXCLUDE",
        "EXIST",
        "EXISTS",
        "FIRST",
        "FOR",
        "FROM",
        "GET",
        "GLOBAL",
        "IF",
        "INCLUDE",
        "INTEGER",
        "IS",
        "LAST",
        "LEVEL",
        "LIKE",
        "LOCAL",
        "LOD",
        "LOOP",
        "MULTIPLE",
        "NAME",
        "NAMED",
        "NEXT",
        "NONE",
        "NULLW",
        "OBJECT",
        "OF",
        "ON",
        "OPERATION",
        "OR",
        "OTHERWISE",
        "PREVIOUS",
        "REGISTERED",
        "RESTRICTING",
        "RESULT",
        "RETURN",
        "RETURNED",
        "ROOTONLY",
        "ROOTONLYCONTINUE",
        "ROOTONLYMULTIPLE",
        "SET",
        "SHORT",
        "SHORTCUTKEY",
        "SINGLE",
        "SINGLEFORUPDATE",
        "STRING",
        "SUBTASK",
        "SYSTEM",
        "TASK",
        "TO",
        "TRANSFORMATION",
        "UNTIL",
        "UPDATE"
        "USING",
        "VIEW",
        "WHERE",
        "WHILE",
        "WITHIN"
    };

#endif
