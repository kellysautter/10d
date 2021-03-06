#ifndef __CMIDLIDS_H__
#define __CMIDLIDS_H__

//////////////////////////////////////////////////////
//
//  Dispatch IDs
//
//

// ICodeSense
#define DISPID_CMPROP_COLORSYNTAX             0
#define DISPID_CMPROP_DISPLAYWHITESPACE       1
#define DISPID_CMPROP_EXPANDTABS             2
#define DISPID_CMPROP_SMOOTHSCROLLING          3
#define DISPID_CMPROP_READONLY                4
#define DISPID_CMPROP_LINETOOLTIPS             5
#define DISPID_CMPROP_DISPLAYLEFTMARGIN       6
#define DISPID_CMPROP_ENABLECOLUMNSEL          7
#define DISPID_CMPROP_ENABLEDRAGDROP          8
#define DISPID_CMPROP_OVERTYPE                9
#define DISPID_CMPROP_SEARCHCASESENSITIVE       10
#define DISPID_CMPROP_REPLACEPRESERVECASE       11
#define DISPID_CMPROP_SEARCHWHOLEWORD          12
#define DISPID_CMPROP_MODIFIED                13
#define DISPID_CMPROP_ENABLECRLF             14
#define DISPID_CMPROP_GLOBALPROPS             15
#define DISPID_CMPROP_VSCROLLVISIBLE          16
#define DISPID_CMPROP_HSCROLLVISIBLE          17
#define DISPID_CMPROP_ENABLEVSPLITTER          18
#define DISPID_CMPROP_ENABLEHSPLITTER          19
#define DISPID_CMPROP_LANGUAGE                20
#define DISPID_CMPROP_TABSIZE                21
#define DISPID_CMPROP_TOPINDEX                22
#define DISPID_CMPROP_UNDOLIMIT             23
#define DISPID_CMPROP_CURRENTVIEW             24
#define DISPID_CMPROP_TEXTLENGTH             25
#define DISPID_CMPROP_LINECOUNT             26
#define DISPID_CMPROP_CURRENTWORD            27
#define DISPID_CMPROP_CURRENTWORDLENGTH         28
#define DISPID_CMPROP_AUTOINDENTMODE          29
#define DISPID_CMPROP_VIEWCOUNT             30
#define DISPID_CMPROP_VSPLITTERPOS            31
#define DISPID_CMPROP_HSPLITTERPOS            32
#define DISPID_CMPROP_FINDTEXT               33
#define DISPID_CMPROP_REPLACETEXT            34
#define DISPID_CMMETH_GETWORD               35
#define DISPID_CMMETH_GETWORDLENGTH            36
#define DISPID_CMMETH_GETTOPINDEX            37
#define DISPID_CMMETH_SETTOPINDEX            38
#define DISPID_CMMETH_GETSEL               39
#define DISPID_CMMETH_SETSEL               40
#define DISPID_CMMETH_GETSELFROMPOINT         41
#define DISPID_CMMETH_GETVISIBLELINECOUNT      42
#define DISPID_CMMETH_GETLINE               43
#define DISPID_CMMETH_GETLINELENGTH            44
#define DISPID_CMMETH_SETCOLOR               45
#define DISPID_CMMETH_GETCOLOR               46
#define DISPID_CMMETH_OPENFILE               47
#define DISPID_CMMETH_SAVEFILE               48
#define DISPID_CMMETH_INSERTFILE            49
#define DISPID_CMMETH_INSERTTEXT            50
#define DISPID_CMMETH_REPLACETEXT            51
#define DISPID_CMMETH_HITTEST               52
#define DISPID_CMMETH_ADDTEXT               53
#define DISPID_CMMETH_DELETELINE            54
#define DISPID_CMMETH_INSERTLINE            55
#define DISPID_CMMETH_SELECTLINE            56
#define DISPID_CMMETH_DELETESEL               57
#define DISPID_CMMETH_REPLACESEL            58
#define DISPID_CMMETH_EXECUTECMD            59
#define DISPID_CMMETH_CANUNDO               60
#define DISPID_CMMETH_CANREDO               61
#define DISPID_CMMETH_CANCUT               62
#define DISPID_CMMETH_CANCOPY               63
#define DISPID_CMMETH_CANPASTE               64
#define DISPID_CMMETH_UNDO                  65
#define DISPID_CMMETH_REDO                  66
#define DISPID_CMMETH_CUT                  67
#define DISPID_CMMETH_COPY                  68
#define DISPID_CMMETH_PASTE                  69
#define DISPID_CMMETH_CLEARUNDOBUFFER         70
#define DISPID_CMMETH_ISRECORDINGMACRO         71
#define DISPID_CMMETH_ISPLAYINGMACRO         72
#define DISPID_CMMETH_SETDLGPARENT            73
#define DISPID_CMPROP_SELTEXT               74
#define DISPID_CMPROP_SELBOUNDS             75
#define DISPID_CMMETH_SETFONTSTYLE            76
#define DISPID_CMMETH_GETFONTSTYLE            77
#define DISPID_CMPROP_SEARCHREGEXP            78
#define DISPID_CMMETH_SETITEMDATA            79
#define DISPID_CMMETH_GETITEMDATA            80
#define DISPID_CMMETH_SETLINESTYLE            81
#define DISPID_CMMETH_GETLINESTYLE            82
#define DISPID_CMMETH_SETBOOKMARK            83
#define DISPID_CMMETH_GETBOOKMARK            84
#define DISPID_CMMETH_SETALLBOOKMARKS         85
#define DISPID_CMMETH_GETALLBOOKMARKS         86
#define DISPID_CMPROP_LINENUMBERS            87
#define DISPID_CMPROP_LINENUMBERSTART         88
#define DISPID_CMPROP_LINENUMBERSTYLE         89
#define DISPID_CMMETH_POSFROMCHAR            90
#define DISPID_CMPROP_HIDESEL                91
#define DISPID_CMPROP_HIGHLIGHTEDLINE         92
#define DISPID_CMPROP_NORMALIZECASE            93
#define DISPID_CMMETH_SETDIVIDER            94
#define DISPID_CMMETH_GETDIVIDER            95
#define DISPID_CMPROP_OVERTYPECARET             96
#define DISPID_CMMETH_SETMARGINIMAGES         97
#define DISPID_CMMETH_GETMARGINIMAGES         98
#define DISPID_CMPROP_HIMAGELIST            99
#define DISPID_CMPROP_IMAGELIST               100
#define DISPID_CMMETH_PRINT                  101
#define DISPID_CMMETH_SETCARETPOS            102
#define DISPID_CMPROP_TEXTLENGTHLOGICAL         103
#define DISPID_CMPROP_WORDLENGTHLOGICAL         104
#define DISPID_CMPROP_CURRENTWORDLENGTHLOGICAL   105
#define DISPID_CMMETH_VIEWCOLTOBUFFERCOL      106
#define DISPID_CMMETH_BUFFERCOLTOVIEWCOL      107
#define DISPID_CMPROP_SELLENGTH             108
#define DISPID_CMPROP_SELLENGTHLOGICAL         109
#define DISPID_CMPROP_BORDERSTYLE            110
#define DISPID_CMPROP_HWND                  111
#define DISPID_CMPROP_STEALKEYS               112

// Changed due to new hWnd property in CodeMax 2.1.0.13
//#define DISPID_CMPROP_CURRENTTOKEN            111
#define DISPID_CMPROP_CURRENTTOKEN            150
#define DISPID_CMMETH_UPDATECONTROLPOSITIONS   151


// ICodeSenseEvents
#define DISPID_NTFY_CHANGE            0
#define DISPID_NTFY_HSCROLL            1
#define DISPID_NTFY_VSCROLL            2
#define DISPID_NTFY_SELCHANGE         3
#define DISPID_NTFY_VIEWCHANGE         4
#define DISPID_NTFY_MODIFIEDCHANGE      5
#define DISPID_NTFY_SHOWPROPS         6
#define DISPID_NTFY_PROPSCHANGE         7
#define DISPID_NTFY_CREATE            8
#define DISPID_NTFY_DESTROY            9
#define DISPID_NTFY_SETFOCUS         10
#define DISPID_NTFY_KILLFOCUS         11
#define DISPID_NTFY_CLICK            12
#define DISPID_NTFY_DBLCLK            13
#define DISPID_NTFY_RDBLCLK            14
#define DISPID_NTFY_RETURN            15
#define DISPID_NTFY_RCLICK            16
#define DISPID_NTFY_DELETELINE         17
#define DISPID_NTFY_CMDFAILURE         18
#define DISPID_NTFY_REGISTEREDCMD      19
#define DISPID_NTFY_KEYDOWN            20
#define DISPID_NTFY_KEYUP            21
#define DISPID_NTFY_KEYPRESS         22
#define DISPID_NTFY_MOUSEDOWN         23
#define DISPID_NTFY_MOUSEUP            24
#define DISPID_NTFY_MOUSEMOVE         25
#define DISPID_NTFY_OVERTYPECHANGE      26
#define DISPID_NTFY_FINDWRAPPED         27
#define DISPID_NTFY_CODELIST         28
#define DISPID_NTFY_CODELISTSELMADE      29
#define DISPID_NTFY_CODELISTCANCEL      30
#define DISPID_NTFY_CODELISTCHAR      31
#define DISPID_NTFY_CODETIP            32
#define DISPID_NTFY_CODETIPCANCEL      33
#define DISPID_NTFY_CODETIPUPDATE      34
#define DISPID_NTFY_CODELISTSELWORD      35
#define DISPID_NTFY_CODELISTSELCHANGE   36
#define DISPID_NTFY_CODELISTHOTTRACK   37
#define DISPID_NTFY_CODETIPINITIALIZE   38

// IRange
#define DISPID_RANGE_START_LINE         0
#define DISPID_RANGE_START_COL         1
#define DISPID_RANGE_END_LINE         2
#define DISPID_RANGE_END_COL         3
#define DISPID_RANGE_IS_EMPTY         4
#define DISPID_RANGE_EMPTY            5
#define DISPID_RANGE_NORMALIZE         6
#define DISPID_RANGE_COLUMN_SEL         7

// IPosition
#define DISPID_POS_LINE               0
#define DISPID_POS_COL               1

// ILanguage
#define DISPID_LANG_CASESENSITIVE      0
#define DISPID_LANG_KEYWORDS         1
#define DISPID_LANG_OPERATORS         2
#define DISPID_LANG_SINGLELINECOMMENTS   3
#define DISPID_LANG_MULTILINECOMMENTS1   4
#define DISPID_LANG_MULTILINECOMMENTS2   5
#define DISPID_LANG_SCOPEKEYWORDS1      6
#define DISPID_LANG_SCOPEKEYWORDS2      7
#define DISPID_LANG_STRINGDELIMS      8
#define DISPID_LANG_ESCAPECHAR         9
#define DISPID_LANG_TERMINATORCHAR      10
#define DISPID_LANG_TAGELEMENTNAMES      11
#define DISPID_LANG_TAGATTRIBUTENAMES   12
#define DISPID_LANG_TAGENTITIES         13
#define DISPID_LANG_STYLE            14

// IGlobals
#define DISPID_GBLPROP_FINDMRULIST            0
#define DISPID_GBLPROP_REPLACEMRULIST         1
#define DISPID_GBLMETH_REGISTERLANGUAGE         2
#define DISPID_GBLMETH_UNREGISTERLANGUAGE      3
#define DISPID_GBLMETH_GETHOTKEYS            4
#define DISPID_GBLMETH_SETHOTKEYS            5
#define DISPID_GBLMETH_RESETDEFAULTHOTKEYS      6
#define DISPID_GBLMETH_GETHOTKEYSFORCMD         7
#define DISPID_GBLMETH_GETHOTKEYSFORCMDLENGTH   8
#define DISPID_GBLMETH_REGISTERHOTKEY         9
#define DISPID_GBLMETH_UNREGISTERHOTKEY         10
#define DISPID_GBLMETH_GETCOMMANDSTRING         11
#define DISPID_GBLMETH_GETMACRO               12
#define DISPID_GBLMETH_SETMACRO               13
#define DISPID_GBLMETH_REGISTERCOMMAND         14
#define DISPID_GBLMETH_UNREGISTERCOMMAND      15
#define DISPID_GBLMETH_GETLANGUAGEDEF         16
#define DISPID_GBLMETH_UNREGISTERALLLANGUAGES   17

// IHotKey
#define DISPID_HOTKEY                     0
#define DISPID_HOTKEY_MOD1                  1
#define DISPID_HOTKEY_VK1                  2
#define DISPID_HOTKEY_MOD2                  3
#define DISPID_HOTKEY_VK2                  4

// IRange
#define DISPID_RECT_LEFT                  0
#define DISPID_RECT_TOP                     1
#define DISPID_RECT_RIGHT                  2
#define DISPID_RECT_BOTTOM                  3

// ICodeList
#define DISPID_CODELIST_HWND                  0
#define DISPID_CODELIST_HIMAGELIST               1
#define DISPID_CODELIST_SELECTEDITEM            2
#define DISPID_CODELIST_ADDITEM                  3
#define DISPID_CODELIST_GETITEMTEXT               4
#define DISPID_CODELIST_GETITEMDATA               5
#define DISPID_CODELIST_FINDSTRING               6
#define DISPID_CODELIST_DESTROY                  7
#define DISPID_CODELIST_DELETEITEM               8
#define DISPID_CODELIST_ITEMCOUNT               9
#define DISPID_CODELIST_ENABLEHOTTRACKING         10
#define DISPID_CODELIST_SORTSTYLE               11
#define DISPID_CODELIST_INSERTITEM               12
#define DISPID_CODELIST_TEXTCOLOR               13
#define DISPID_CODELIST_BACKCOLOR               DISPID_BACKCOLOR
#define DISPID_CODELIST_FONT                  DISPID_FONT

// ICodeTip
#define DISPID_CODETIP_HWND                     0
#define DISPID_CODETIP_HIGHLIGHTBEGINPOS         1
#define DISPID_CODETIP_HIGHLIGHTENDPOS            2
#define DISPID_CODETIP_TIPTEXT                  3
#define DISPID_CODETIP_DESTROY                  4
#define DISPID_CODETIP_TIPTEXTLENGTH            5
#define DISPID_CODETIP_TEXTCOLOR               6
#define DISPID_CODETIP_BACKCOLOR               DISPID_BACKCOLOR
#define DISPID_CODETIP_FONT                     DISPID_FONT

// ICodeTipHighlight
#define DISPID_CODETIPHIGHLIGHT_HIGHLIGHTSTARTPOS   50
#define DISPID_CODETIPHIGHLIGHT_HIGHLIGHTENDPOS      51

// ICodeTipFuncHighlight
#define DISPID_CODETIPFUNCHIGHLIGHT_ARGUMENT      50

// ICodeTipMultiFunc
#define DISPID_CODETIPMULTIFUNC_FUNCTIONCOUNT      100
#define DISPID_CODETIPMULTIFUNC_CURRENTFUNCTION      101


//////////////////////////////////////////////////////
//
//  Help IDs
//
//

// Stock properties
#define HELPID_FONT                     50
#define HELPID_ENABLED                  51
#define HELPID_TEXT                     52
#define HELPID_HWND                     53

// ICodeSense
#define HELPID_CONTROL                      100
#define HELPID_CMPROP_COLORSYNTAX             101
#define HELPID_CMPROP_DISPLAYWHITESPACE       102
#define HELPID_CMPROP_EXPANDTABS             103
#define HELPID_CMPROP_SMOOTHSCROLLING          104
#define HELPID_CMPROP_READONLY                105
#define HELPID_CMPROP_LINETOOLTIPS             106
#define HELPID_CMPROP_DISPLAYLEFTMARGIN       107
#define HELPID_CMPROP_ENABLECOLUMNSEL          108
#define HELPID_CMPROP_ENABLEDRAGDROP          109
#define HELPID_CMPROP_OVERTYPE                110
#define HELPID_CMPROP_SEARCHCASESENSITIVE       111
#define HELPID_CMPROP_REPLACEPRESERVECASE       112
#define HELPID_CMPROP_SEARCHWHOLEWORD          113
#define HELPID_CMPROP_MODIFIED                114
#define HELPID_CMPROP_ENABLECRLF             115
#define HELPID_CMPROP_GLOBALPROPS             116
#define HELPID_CMPROP_VSCROLLVISIBLE          117
#define HELPID_CMPROP_HSCROLLVISIBLE          118
#define HELPID_CMPROP_ENABLEVSPLITTER          119
#define HELPID_CMPROP_ENABLEHSPLITTER          120
#define HELPID_CMPROP_LANGUAGE                121
#define HELPID_CMPROP_TABSIZE                122
#define HELPID_CMPROP_TOPINDEX                123
#define HELPID_CMPROP_UNDOLIMIT             124
#define HELPID_CMPROP_CURRENTVIEW             125
#define HELPID_CMPROP_TEXTLENGTH             126
#define HELPID_CMPROP_LINECOUNT             127
#define HELPID_CMPROP_CURRENTWORD            128
#define HELPID_CMPROP_CURRENTWORDLENGTH       129
#define HELPID_CMPROP_AUTOINDENTMODE          130
#define HELPID_CMPROP_VIEWCOUNT             131
#define HELPID_CMPROP_VSPLITTERPOS            132
#define HELPID_CMPROP_HSPLITTERPOS            133
#define HELPID_CMPROP_FINDTEXT               134
#define HELPID_CMPROP_REPLACETEXT            135
#define HELPID_CMMETH_GETWORD               136
#define HELPID_CMMETH_GETWORDLENGTH            137
#define HELPID_CMMETH_GETTOPINDEX            138
#define HELPID_CMMETH_SETTOPINDEX            139
#define HELPID_CMMETH_GETSEL               140
#define HELPID_CMMETH_SETSEL               141
#define HELPID_CMMETH_GETSELFROMPOINT         142
#define HELPID_CMMETH_GETVISIBLELINECOUNT      143
#define HELPID_CMMETH_GETLINE               144
#define HELPID_CMMETH_GETLINELENGTH            145
#define HELPID_CMMETH_SETCOLOR               146
#define HELPID_CMMETH_GETCOLOR               147
#define HELPID_CMMETH_OPENFILE               148
#define HELPID_CMMETH_SAVEFILE               149
#define HELPID_CMMETH_INSERTFILE            150
#define HELPID_CMMETH_INSERTTEXT            151
#define HELPID_CMMETH_REPLACETEXT            152
#define HELPID_CMMETH_HITTEST               153
#define HELPID_CMMETH_ADDTEXT               154
#define HELPID_CMMETH_DELETELINE            155
#define HELPID_CMMETH_INSERTLINE            156
#define HELPID_CMMETH_SELECTLINE            157
#define HELPID_CMMETH_DELETESEL               158
#define HELPID_CMMETH_REPLACESEL            159
#define HELPID_CMMETH_EXECUTECMD            160
#define HELPID_CMMETH_CANUNDO               161
#define HELPID_CMMETH_CANREDO               162
#define HELPID_CMMETH_CANCUT               163
#define HELPID_CMMETH_CANCOPY               164
#define HELPID_CMMETH_CANPASTE               165
#define HELPID_CMMETH_UNDO                  166
#define HELPID_CMMETH_REDO                  167
#define HELPID_CMMETH_CUT                  168
#define HELPID_CMMETH_COPY                  169
#define HELPID_CMMETH_PASTE                  170
#define HELPID_CMMETH_CLEARUNDOBUFFER         171
#define HELPID_CMMETH_ISRECORDINGMACRO         172
#define HELPID_CMMETH_ISPLAYINGMACRO         173
#define HELPID_CMMETH_SETDLGPARENT            174
#define HELPID_CMPROP_SELTEXT               175
#define HELPID_CMPROP_SELBOUNDS               176
#define HELPID_CMMETH_SETFONTSTYLE            177
#define HELPID_CMMETH_GETFONTSTYLE            178
#define HELPID_CMPROP_SEARCHREGEXP             179
#define HELPID_CMMETH_SETITEMDATA            180
#define HELPID_CMMETH_GETITEMDATA            181
#define HELPID_CMMETH_SETLINESTYLE            182
#define HELPID_CMMETH_GETLINESTYLE            183
#define HELPID_CMMETH_SETBOOKMARK            184
#define HELPID_CMMETH_GETBOOKMARK            185
#define HELPID_CMMETH_SETALLBOOKMARKS         186
#define HELPID_CMMETH_GETALLBOOKMARKS         187
#define HELPID_CMPROP_LINENUMBERS            188
#define HELPID_CMPROP_LINENUMBERSTART         189
#define HELPID_CMPROP_LINENUMBERSTYLE         190
#define HELPID_CMMETH_POSFROMCHAR            191
#define HELPID_CMPROP_HIDESEL                192
#define HELPID_CMPROP_HIGHLIGHTEDLINE         193
#define HELPID_CMPROP_NORMALIZECASE            194
#define HELPID_CMMETH_SETDIVIDER            195
#define HELPID_CMMETH_GETDIVIDER            196
#define HELPID_CMPROP_OVERTYPECARET             197
#define HELPID_CMMETH_SETMARGINIMAGES         198
#define HELPID_CMMETH_GETMARGINIMAGES         199
#define HELPID_CMPROP_HIMAGELIST            200
#define HELPID_CMPROP_IMAGELIST               201
#define HELPID_CMMETH_ABOUTBOX               202
#define HELPID_CMMETH_PRINT                  203
#define HELPID_CMMETH_SETCARETPOS            204
#define HELPID_CMPROP_TEXTLENGTHLOGICAL         205
#define HELPID_CMPROP_WORDLENGTHLOGICAL         206
#define HELPID_CMPROP_CURRENTWORDLENGTHLOGICAL   207
#define HELPID_CMMETH_VIEWCOLTOBUFFERCOL      208
#define HELPID_CMMETH_BUFFERCOLTOVIEWCOL      209
#define HELPID_CMPROP_SELLENGTH             210
#define HELPID_CMPROP_SELLENGTHLOGICAL         211
#define HELPID_CMPROP_BORDERSTYLE            212
#define HELPID_CMPROP_HWND                  213
#define HELPID_CMPROP_STEALKEYS               214


// Changed due to new hWnd property in CodeMax 2.1.0.13
//#define HELPID_CMPROP_CURRENTTOKEN            213
#define HELPID_CMPROP_CURRENTTOKEN            250
#define HELPID_CMMETH_UPDATECONTROLPOSITIONS   251


// IRange
#define HELPID_RANGE               200
#define HELPID_RANGE_START_LINE         201
#define HELPID_RANGE_START_COL         202
#define HELPID_RANGE_END_LINE         203
#define HELPID_RANGE_END_COL         204
#define HELPID_RANGE_IS_EMPTY         205
#define HELPID_RANGE_EMPTY            206
#define HELPID_RANGE_NORMALIZE         207
#define HELPID_RANGE_COLUMN_SEL         208

// IPosition
#define HELPID_POS                  300
#define HELPID_POS_LINE               301
#define HELPID_POS_COL               302

// ILanguage
#define HELPID_LANG                  400
#define HELPID_LANG_CASESENSITIVE      401
#define HELPID_LANG_KEYWORDS         402
#define HELPID_LANG_OPERATORS         403
#define HELPID_LANG_SINGLELINECOMMENTS   404
#define HELPID_LANG_MULTILINECOMMENTS1   405
#define HELPID_LANG_MULTILINECOMMENTS2   406
#define HELPID_LANG_SCOPEKEYWORDS1      407
#define HELPID_LANG_SCOPEKEYWORDS2      408
#define HELPID_LANG_STRINGDELIMS      409
#define HELPID_LANG_ESCAPECHAR         410
#define HELPID_LANG_TERMINATORCHAR      411
#define HELPID_LANG_TAGATTRIBUTENAMES   412
#define HELPID_LANG_TAGENTITIES         413
#define HELPID_LANG_TAGELEMENTNAMES      414
#define HELPID_LANG_STYLE            415

// IGlobals
#define HELPID_GLOBALS                     500
#define HELPID_GBLPROP_FINDMRULIST            501
#define HELPID_GBLPROP_REPLACEMRULIST         502
#define HELPID_GBLMETH_REGISTERLANGUAGE         503
#define HELPID_GBLMETH_UNREGISTERLANGUAGE      504
#define HELPID_GBLMETH_GETHOTKEYS            505
#define HELPID_GBLMETH_GETHOTKEYSLENGTH         506
#define HELPID_GBLMETH_SETHOTKEYS            507
#define HELPID_GBLMETH_RESETDEFAULTHOTKEYS      508
#define HELPID_GBLMETH_GETHOTKEYSFORCMD         509
#define HELPID_GBLMETH_GETHOTKEYSFORCMDLENGTH   510
#define HELPID_GBLMETH_REGISTERHOTKEY         511
#define HELPID_GBLMETH_UNREGISTERHOTKEY         512
#define HELPID_GBLMETH_GETCOMMANDSTRING         513
#define HELPID_GBLMETH_GETMACRO               514
#define HELPID_GBLMETH_GETMACROLENGTH         515
#define HELPID_GBLMETH_SETMACRO               516
#define HELPID_GBLMETH_REGISTERCOMMAND         517
#define HELPID_GBLMETH_UNREGISTERCOMMAND      518
#define HELPID_GBLMETH_GETLANGUAGEDEF         519
#define HELPID_GBLMETH_UNREGISTERALLLANGUAGES   520

// IHotKey
#define HELPID_HOTKEY                     600
#define HELPID_HOTKEY_MOD1                  601
#define HELPID_HOTKEY_VK1                  602
#define HELPID_HOTKEY_MOD2                  603
#define HELPID_HOTKEY_VK2                  604

// IRange
#define HELPID_RECT                700
#define HELPID_RECT_LEFT            700
#define HELPID_RECT_TOP               701
#define HELPID_RECT_RIGHT            702
#define HELPID_RECT_BOTTOM            703

// ICodeList
#define HELPID_CODELIST                        800
#define HELPID_CODELIST_HWND                  801
#define HELPID_CODELIST_HIMAGELIST               802
#define HELPID_CODELIST_SELECTEDITEM            803
#define HELPID_CODELIST_ADDITEM                  804
#define HELPID_CODELIST_GETITEMTEXT               805
#define HELPID_CODELIST_GETITEMDATA               806
#define HELPID_CODELIST_FINDSTRING               807
#define HELPID_CODELIST_DESTROY                  808
#define HELPID_CODELIST_DELETEITEM               809
#define HELPID_CODELIST_ITEMCOUNT               810
#define HELPID_CODELIST_ENABLEHOTTRACKING         811
#define HELPID_CODELIST_SORTSTYLE               812
#define HELPID_CODELIST_INSERTITEM               813
#define HELPID_CODELIST_TEXTCOLOR               814

#define HELPID_CODELIST_BACKCOLOR               830
#define HELPID_CODELIST_FONT                  831

// ICodeTip
#define HELPID_CODETIP                        850
#define HELPID_CODETIP_HWND                     851
#define HELPID_CODETIP_HIGHLIGHTBEGINPOS         852
#define HELPID_CODETIP_HIGHLIGHTENDPOS            853
#define HELPID_CODETIP_TIPTEXT                  854
#define HELPID_CODETIP_DESTROY                  855
#define HELPID_CODETIP_TIPTEXTLENGTH            856
#define HELPID_CODETIP_TEXTCOLOR               857

#define HELPID_CODETIP_BACKCOLOR               870
#define HELPID_CODETIP_FONT                     871

// ICodeTipHighlight
#define HELPID_CODETIPHIGHLIGHT                  900
#define HELPID_CODETIPHIGHLIGHT_HIGHLIGHTSTARTPOS   901
#define HELPID_CODETIPHIGHLIGHT_HIGHLIGHTENDPOS      902

// ICodeTipFuncHighlight
#define HELPID_CODETIPFUNCHIGHLIGHT               950
#define HELPID_CODETIPFUNCHIGHLIGHT_ARGUMENT      951

// ICodeTipMultiFunc
#define HELPID_CODETIPMULTIFUNC                  1000
#define HELPID_CODETIPMULTIFUNC_FUNCTIONCOUNT      1001
#define HELPID_CODETIPMULTIFUNC_CURRENTFUNCTION      1002


//////////////////////////////////////////////////////

#endif//#ifndef __CMIDLIDS_H__
