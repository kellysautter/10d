{$B-}	{ short circuit boolean evaluations }
$@
$L{LINE 2 $P}
(*
 * Automaton to interpret LALR(1) tables.
 *
 *	Alterable constants:
 *		YYSSIZE - size of state and value stacks (default 150).
 *		YYTABSIZE - always 1 more then YYSSIZE
 *
 *	Unavailable:
 *		YYERROR - simulate an error
 *			Instead, in Pascal, code the action
 *				goto errlabel;
 *			to get the same effect as YYERROR.
 *		YYACCEPT - return 0 from yyparse
 *			Instead, in Pascal, code the action
 *				retval := 0; goto yyComplete;
 *		YYABORT - return 1 from yyparse
 *			Instead, in Pascal, code the action
 *				retval := 1; goto yyComplete;
 *		YYRETURN(value) - return value from yyparse.
 *			Instead, in Pascal, code the action
 *				retval := value; goto yyComplete;
 *	Functions:
 *		YYRECOVERING - true if syntax error detected,
 *			and not recovered yet.
 *	Procedures:
 *		yyclearin - clear the lookahead token.
 *		yyerrok - forgive a pending error
 *		YYREAD - ensure yychar contains a lookahead token by
 *			reading one if it does not.  See also YYSYNC.
 *	Preprocessor flags:
 *		YYDEBUG - includes debug code.  The parser will print
 *			a travelogue of the parse if this is defined
 *			and yydbug is non-zero. (default off)
 *		YYSTATIC - By default, the state and value stacks are
 *			allocated as Pascal local variables, taking up
 *			stack space. If YYSTATIC is defined, these
 *			stacks are defined as data objects.
 *		YYALLOC - Dynamically allocate both the state and value stacks
 *			by calling New() and Dispose().
 *		YYSYNC - if defined, yacc guarantees to fetch a
 *			lookahead token before any action, even if
 *			it is not needed for a decision.
 *			If YYSYNC is defined, YYREAD will never be
 *			necessary unless the user explicitly sets yychar = -1
 *
 *	Copyright (c) 1990, 1991 by Mortice Kern Systems Inc.
 *	based on material Copyright (c) 1983, by the University of Waterloo
 *)
const
	YYERRCODE = 256;		{ value of `error' }
	YYSSIZE = 150;			{ If changed, update YYTABSIZE! }
	YYTABSIZE = 151;		{ *** 1 more then YYSSIZE *** }
{$IFDEF YYDEBUG}
	yydbug : integer = 0;		{ debug flag & tables }
{$$ENDIF}

(* global exported *)

type
	yytablerange = 0..YYTABSIZE;
	yyvaluetable = array [yytablerange] of YYSTYPE;
	yystatetable = array [yytablerange] of integer;
	pyyvaluetable = ^ yyvaluetable;
	pyystatetable = ^ yystatetable;
{$IFDEF YYDEBUG}
	yytypetable = array [yytablerange] of integer;
	pyytypetable = ^ yytypetable;
{$$ENDIF}
var
	yyval :	YYSTYPE;	{ $$ }
	yypvt:	integer;	{ index of $n }
	yychar:	integer;	{ current token }
	yyerrflag: integer;	{ error flag }
	yynerrs: integer;	{ error count }
	retval:	integer;	{ yyparse() return value }
{$IFDEF YYSTATIC}
	ivalues:	yyvaluetable;
	istates:	yystatetable;
{$IFDEF YYDEBUG}
	itypes:		yytypetable;
{$$ENDIF}
{$$ENDIF}

{$IFDEF YYDEBUG}
type
	yyTraceItems = record
		state, lookahead, errflag: integer;
		done: boolean;
		rule, npop: integer;
		states: pyystatetable;
		nstates: integer;
		values: pyyvaluetable;
		nvalues: integer;
		types: pyytypetable;
	end;
	pyyTraceItems = ^ yyTraceItems;

function yyGetType(tok: integer): integer; forward;

procedure yyShowState(tp: pyyTraceItems); forward;
procedure yyShowReduce(tp: pyyTraceItems); forward;
procedure yyShowGoto(tp: pyyTraceItems); forward;
procedure yyShowShift(tp: pyyTraceItems); forward;
procedure yyShowErrRecovery(tp: pyyTraceItems); forward;
procedure yyShowErrDiscard(tp: pyyTraceItems); forward;

procedure yyShowRead(val: integer); forward;
{$$ENDIF}

procedure yyerrok;
begin
	yyerrflag := 0;
end;

procedure yyclearin;
begin
	yychar := -1;
{$IFDEF YYDEBUG}
	if yydbug <> 0 then
		yyShowRead(-1);
{$$ENDIF}
end;

function YYRECOVERING : boolean;
begin
	if yyerrflag <> 0 then
		YYRECOVERING := true
	else
		YYRECOVERING := false
end;

procedure YYREAD;
begin
	if yychar < 0 then
	begin
		yychar := yylex;
		if yychar < 0 then yychar := 0;
{$IFDEF YYDEBUG}
		if yydbug <> 0 then
			yyShowRead(yychar);
{$$ENDIF}
	end
end;

{$IFDEF YYDEBUG}
procedure yymsg(msg: string; val: integer);
begin
	writeln;
	writeln('yacc bug: ',msg,val);
end;
{$$ENDIF}

function yyparse: integer;

(* Start of locals *)
label
	yyerrlabel,			{ YYERROR comes here }
	yyinError,			{ come here on syntax error }
	yyStack,			{ start new entry }
	yyEncore,			{ lookup next action }
	yygo_on,			{ non-break in case statement }
	yyContin,			{ continue in C loop }
	yyComplete;			{ normal exit from yyparse() }
var
	yyi, yyj, yystate: integer;		{ table and state lookup }
	yyps:	integer;			{ index on state stack }
	yypv:	integer;			{ index on value stack }
	yyp, yyq: integer;			{ temp indexes }
	yyv:		pyyvaluetable;		{ allocated value stack }
	yys:		pyystatetable;		{ allocated state stack }
{$IFDEF YYDEBUG}
	yytypev:	pyytypetable;		{ type assignments }
{$$ENDIF}
{$IFNDEF YYSTATIC}
{$IFDEF YYALLOC}
	save_yylval, save_yyval: YYSTYPE;
	save_yypvt: integer;
	save_yychar, save_yyerrflag, save_yynerrs: integer;
{$$ELSE}
	ivalues:	yyvaluetable;
	istates:	yystatetable;
{$IFDEF YYDEBUG}
	itypes:		yytypetable;
{$$ENDIF}
{$$ENDIF}
{$$ENDIF}

{$IFDEF YYDEBUG}
var
	yytp: integer;
	yyruletype: integer;
	YY_TRACE_ITEM: yyTraceItems;

function YY_TRACE: pyyTraceItems;
begin
	with YY_TRACE_ITEM do begin
		state := yystate; lookahead := yychar; errflag := yyerrflag;
		done := False;
		states := addr(yys^[1]); nstates := yyps;
		values := addr(yyv^[1]); nvalues := yypv;
		types := addr(yytypev^[1]);
		rule := yyi; npop := yyj
	end;
	YY_TRACE := @ YY_TRACE_ITEM
end;
{$$ENDIF}

$A

begin	{ start of code for yyparse() }

{$IFDEF YYSTATIC}
	yyv := @ ivalues;
	yys := @ istates;
{$IFDEF YYDEBUG}
	yytypev := @ itypes;
{$$ENDIF}
{$$ELSE}
{$IFDEF YYALLOC}
	New(yyv);
	New(yys);
{$IFDEF YYDEBUG}
	New(yytypev);
{$$ENDIF}
	if (yyv = nil) or (yys = nil)
{$IFDEF YYDEBUG}
	or (yytypev = nil)
{$$ENDIF}
	then begin
		yyerror('Not enough space for parser stacks');
		retval := 1;
		goto yyComplete;
	end;
	save_yylval := yylval;
	save_yyval := yyval;
	save_yypvt := yypvt;
	save_yychar := yychar;
	save_yyerrflag := yyerrflag;
	save_yynerrs := yynerrs;
{$$ELSE}
	yyv := @ ivalues;
	yys := @ istates;
{$IFDEF YYDEBUG}
	yytypev := @ itypes;
{$$ENDIF}
{$$ENDIF}
{$$ENDIF}
	yynerrs := 0;
	yyerrflag := 0;
	yyclearin;
	yyps := 0;
	yypv := 0;
	yystate := YYS0;		(* start state *)
{$IFDEF YYDEBUG}
	yytp := 0;
	yyi := 0;
	yyj := 0;
{$$ENDIF}

yyStack:

{$IFDEF YYDEBUG}
	{ assert (unsigned)yystate < yynstate }
	if word(yystate) >= yynstate then
	begin
		yymsg('state ', yystate);
		retval := 1;
		goto yyComplete;
	end;
{$$ENDIF}
	Inc(yyps); Inc(yypv);
	if yyps > YYSSIZE then
	begin
		yyerror('Parser stack overflow');
		retval := 1;
		goto yyComplete;
	end;
	yys^[yyps] := yystate;	 	(* stack current state *)
	yyv^[yypv] := yyval;		(* ... and value *)

{$IFDEF YYDEBUG}
	inc(yytp);
	yytypev^[yytp] := yyruletype;	(* ... and type *)

	if yydbug <> 0 then begin
		yyShowState(YY_TRACE);
		if YY_TRACE_ITEM.done then begin
			retval := -1; goto yyComplete
		end
	end;
{$$ENDIF}

	(*
	 *	Look up next action in action table.
	 *)

yyEncore:

{$IFDEF YYSYNC}
	YYREAD;
{$$ENDIF}

	if yystate >= Sizeof(yypact)/Sizeof(yypact[0]) then
	 	(* simple state *)
		yyi := yystate - YYDELTA	(* reduce in any case *)
	else begin
		yyp := yypact[yystate];
		if yyact[yyp] >= 0 then
		begin
			(* Look for a shift on yychar *)
{$IFNDEF YYSYNC}
			YYREAD;
{$$ENDIF}
			yyq := yyp;
			yyi := yychar;
			while yyi < yyact[yyp] do
				Inc(yyp);
			Inc(yyp);
			if yyi = yyact[yyp-1] then
			begin
				yystate := not yyact[yyq + (yyq - yyp)];
{$IFDEF YYDEBUG}
				if yydbug <> 0 then begin
					yyruletype := yyGetType(yychar);
					yyShowShift(YY_TRACE);
					if YY_TRACE_ITEM.done then begin
						retval := -1; goto yyComplete
					end
				end;
{$$ENDIF}
				(* stack what yylex() set *)
				yyval := YYSTYPE(yylval);
				yyclearin;		(* clear token *)
				if yyerrflag <> 0 then (* successful shift *)
					yyerrflag := yyerrflag - 1;
				goto yyStack;
			end
		end;

		(*
	 	 *	Fell through - take default action
	 	 *)

		if yystate >= Sizeof(yydef)/Sizeof(yydef[0]) then
			goto yyinError;
		yyi := yydef[yystate];
		if yyi < 0 then
		begin
		(* default == reduce? *)
			(* Search exception table *)
{$IFDEF YYDEBUG}
		{ assert (unsigned)~yyi < sizeof(yyex)/sizeof(yyex[0]) }
			if word(not yyi) >= Sizeof(yyex)/Sizeof(yyex[0]) then
			begin
				yymsg('exception ',yystate);
				retval := 1;
				goto yyComplete;
			end;
{$$ENDIF}
			yyp := not yyi;
			{ yyp := &yyex[not yyi]; }
{$IFDEF YYSYNC}
			YYREAD;
{$$ENDIF}
			yyi := yyex[yyp];
			while (yyi >= 0) and (yyi <> yychar) do
			begin
				Inc(yyp, 2);
				yyi := yyex[yyp];
			end;
			yyi := yyex[yyp+1];
{$IFDEF YYDEBUG}
			{ assert yyi >= 0 }
			if yyi < 0 then
			begin
				yymsg('Ex table not reduce ', yyi);
				retval := 1;
				goto yyComplete;
			end
{$$ENDIF}
		end
	end;

{$IFDEF YYDEBUG}
	{ assert (unsigned)yyi < yynrule }
	if word(yyi) >= yynrule then
	begin
		yymsg('reduce ',yyi);
		retval := 1;
		goto yyComplete;
	end;
{$$ENDIF}
	yyj := yyrlen[yyi];
{$IFDEF YYDEBUG}
	if yydbug <> 0 then begin
		yyShowReduce(YY_TRACE);
		if YY_TRACE_ITEM.done then begin
			retval := -1; goto yyComplete
		end
	end;
	dec(yytp, yyj);
{$$ENDIF}
	Dec(yyps, yyj);		(* pop stacks *)
	yypvt := yypv;		(* save top *)
	Dec(yypv, yyj);
	yyval := yyv^[yypv+1];	(* default action $$ = $1 *)
{$IFDEF YYDEBUG}
	yyruletype := yyRules[yyrmap[yyi]].tokentype;
{$$ENDIF}

	case yyi of		(* perform semantic action *)
		$A
$L{LINE 237 "$P"}
	YYrACCEPT:
	    begin
		retval := 0;
		goto yyComplete;
	    end;
	YYrERROR:
		goto yyinError;
	end;

	(*
	 *	Look up next state in goto table.
	 *)

	yyp := yypgo[yyi];
	{ yyp := &yygo[yypgo[yyi]]; }
	yyq := yyp;
	Inc(yyp);
	yyi := yys^[yyps];
	while yyi < yygo[yyp] do
		Inc(yyp);
	if yyi = yygo[yyp] then
		yystate := not yygo[yyq + (yyq - yyp)]
	else
		yystate := not yygo[yyq];
{$IFDEF YYDEBUG}
	if yydbug <> 0 then begin
		yyShowGoto(YY_TRACE);
		if YY_TRACE_ITEM.done then begin
			retval := -1; goto yyComplete
		end
	end;
{$$ENDIF}
	goto yyStack;

yyerrlabel:	;		(* come here from YYERROR *)
(*
#pragma used yyerrlabel
 *)
	yyerrflag := 1;
	if yyi = YYrERROR then begin
		Dec(yyps);
		Dec(yypv);
{$IFDEF YYDEBUG}
		Dec(yytp);
{$$ENDIF}
	end;
	
yyinError:
	case yyerrflag of

	0:		(* new error *)
	    begin
		yynerrs := yynerrs + 1;
		yyi := yychar;
		yyerror('Syntax error');
		if yyi <> yychar then
		begin
			(* user has changed the current token *)
			(* try again *)
			yyerrflag := yyerrflag + 1;	(* avoid loops *)
			goto yyEncore;
		end;
		goto yygo_on;
	    end;

	1, 2:		(* partially recovered *)
	    begin
	    yygo_on:
		yyerrflag := 3;	(* need 3 valid shifts to recover *)

		(*
		 *	Pop states, looking for a
		 *	shift on `error'.
		 *)

		while yyps > 0 do
		begin
			if yys^[yyps] >= Sizeof(yypact)/Sizeof(yypact[0]) then
				goto yyContin;
			{ yyp := &yyact[yypact[yys^[yyps]]]; }
			yyp := yypact[yys^[yyps]];
			yyq := yyp;
			while YYERRCODE < yyact[yyp] do
				Inc(yyp);
			Inc(yyp);
			if YYERRCODE = yyact[yyp-1] then
			begin
				yystate := not yyact[yyq + (yyq - yyp)];
				goto yyStack;
			end;

			(* no shift in this state *)
{$IFDEF YYDEBUG}
			if (yydbug <> 0) and (yyps > 1) then begin
				yyShowErrRecovery(YY_TRACE);
				if YY_TRACE_ITEM.done then begin
					retval := -1; goto yyComplete
				end
			end;
{$$ENDIF}
			(* pop stacks; try again *)
		yyContin:
			Dec(yyps); Dec(yypv);
{$IFDEF YYDEBUG}
			Dec(yytp);
{$$ENDIF}
		end; { while }
		(* no shift on error - abort *)
	    end;

	3:
	    begin
		(*
		 *	Erroneous token after
		 *	an error - discard it.
		 *)

		if yychar <> 0 then (* but not EOF *)
		begin
{$IFDEF YYDEBUG}
			if yydbug <> 0 then begin
				yyShowErrDiscard(YY_TRACE);
				if YY_TRACE_ITEM.done then begin
					retval := -1; goto yyComplete
				end
			end;
{$$ENDIF}
			yyclearin;
			goto yyEncore;	(* try again in same state *)
		end
	    end
	end; { case }

	retval := 1;		{ failed }

yyComplete:		{ exit yyparse by coming here }

{$IFDEF YYALLOC}
	yylval := save_yylval;
	yyval := save_yyval;
	yypvt := save_yypvt;
	yychar := save_yychar;
	yyerrflag := save_yyerrflag;
	yynerrs := save_yynerrs;
	Dispose(yys);
	Dispose(yyv);
{$$ENDIF}

	yyparse := retval;
end; { yyparse }

{$IFDEF YYDEBUG}
(*
 * Return type of token
 *)
function yyGetType(tok: integer): integer;
var	i: integer;
label	done;
begin
	i := yyntoken - 1;
	while i > 0 do begin
		if yyTokenTypes[i].token = tok then begin
			yyGetType := yyTokenTypes[i].tokenType;
			goto done
		end;
		Dec(i);
	end;
	yyGetType := 0;
done:	;
end;
(*
 * Print a token legibly.
 *)
function yyptok(tok: integer) : string;
var	i: integer;
label	done;
begin
	i := yyntoken - 1;
	while i > 0 do begin
		if yyTokenTypes[i].token = tok then begin
			yyptok := yyTokenTypes[i].name;
			goto done
		end;
		Dec(i)
	end;
	yyptok := '';
done:	;
end;

const	yyExpandOpen: boolean = False;
var	yyExpandFile: file of char;

(*
 * get state num from YYStatesFile
 *)
procedure yygetState(var s:string; num: integer);
var	i: integer;
	ch: char;
label	done;
begin
	{$I-}
	if not yyExpandOpen then begin
		assign(yyExpandFile, YYStatesFile);
		reset(yyExpandFile);
		if IOresult <> 0 then begin
			s := concat('error opening file ''',
				YYStatesFile, '''');
			exit;
		end;
		yyExpandOpen := True;
	end;
	seek(yyExpandFile, yyStates[num]);
	if IOresult <> 0 then begin
		s := concat('error seeking file ''', YYStatesFile, '''');
		exit;
	end;
	i := 1;
	read(yyExpandFile, ch);
	if IOresult <> 0 then begin
		s := concat('error reading file ''', YYStatesFile, '''');
		exit;
	end;
	while ch <> #0 do begin
		s[i] := ch;
		inc(i);
		if i > (YY_MAX_SHOW - 4) then begin
			s[i] := ' '; inc(i);
			s[i] := '.'; inc(i);
			s[i] := '.'; inc(i);
			s[i] := '.'; inc(i);
			goto done;
		end;
		read(yyExpandFile, ch);
		if IOresult <> 0 then begin
			s := concat('error reading file ''',
				YYStatesFile, '''');
			exit;
		end;
	end;
	{$I+}
done:	s[0] := chr(i-1);
end;

(*
 * Expand encoded string into printable representation
 * Used to decode yyStates and yyRules strings.
 *)
procedure yyExpandName(num: integer; isRule: boolean;
		       var buf: string);
var
	i, j, n, cnt, btype: integer;
	slen: integer;
	endp: integer;
	bp, sp: integer;
	s: string;
	cp: string;
label	full;
label	getN;
label	done;
begin
	if isRule then
		s := yyRules[num].name
	else
		yygetState(s, num);
	endp := YY_MAX_SHOW - 8;
	bp := 1;	(* buf pointer *)
	sp := 1;	(* s pointer *)
	slen := length(s);
	while sp <= slen do begin
		if bp > endp then begin
		full:	buf[bp] := ' '; inc(bp);
			buf[bp] := '.'; inc(bp);
			buf[bp] := '.'; inc(bp);
			buf[bp] := '.'; inc(bp);
			buf[bp] := #10;
			buf[0] := chr(bp);
			exit
		end else if s[sp] = '%' then begin
			btype := 0;
			cnt := yynvar;
			goto getN
		end else if s[sp] = '&' then begin
			btype := 1;
			cnt := yyntoken;
		getN:	if cnt < 100 then
				i := 2
			else if cnt < 1000 then
				i := 3
			else
				i := 4;
			n := 0;
			while i > 0 do begin
				dec(i); inc(sp);
				n := (n * 10) + ord(s[sp]) - ord('0')
			end;
			if btype = 0 then begin
				if n >= yynvar then
					cp := '<range err>'
				else
					cp := yysvar[n];
			end else if n >= yyntoken then
				cp := '<range err>'
			else
				cp := yytokenTypes[n].name;
			i := length(cp);
			if i + bp > endp then
				goto full;
			j := 1;
			while j <= i do begin
				buf[bp] := cp[j];
				inc(bp); inc(j);
			end;
		end else begin
			buf[bp] := s[sp]; inc(bp);
		end;
		inc(sp);
	end;
	buf[0] := chr(bp - 1);
done:	;
end;
{$IFNDEF YYTRACE}
procedure yyShowState(tp: pyyTraceItems);
begin
	writeln('state ',yysmap[tp^.state],' (',tp^.state,'), char ',
		yyptok(tp^.lookahead),' (',tp^.lookahead,')');
end;
procedure yyShowReduce(tp: pyyTraceItems);
begin
	writeln('reduce ', yyrmap[tp^.rule], ' (',tp^.rule,'), pops ',
		tp^.states^[tp^.nstates - tp^.npop], ' (',
		yysmap[tp^.states^[tp^.nstates - tp^.npop]], ')');
end;
procedure yyShowRead(val: integer);
begin
	writeln('read ',yyptok(val),' (',val,')');
end;
procedure yyShowGoto(tp: pyyTraceItems);
begin
	writeln('goto ',yysmap[tp^.state],' (',tp^.state,')');
end;
procedure yyShowShift(tp: pyyTraceItems);
begin
	writeln('shift ',yysmap[tp^.state],' (',tp^.state,')');
end;
procedure yyShowErrRecovery(tp: pyyTraceItems);
var top: pyystatetable;
begin
	top := addr(tp^.states^[tp^.nstates-2]);
	writeln( 'Error recovery pops state ', yysmap[top^[1]],
		' (',top^[1], '), uncovers ',yysmap[top^[0]],
		' (',top^[0], ')');
end;
procedure yyShowErrDiscard(tp: pyyTraceItems);
begin
	write('Error recovery discards ', yyptok(tp^.lookahead),
		' (',tp^.lookahead,'), ');
end;
{$$ENDIF}
{$$ENDIF}

$T
$H
{$IFDEF YYTRACE}
{$DEFINE YYDEBUG}
{$$ENDIF}
{$IFDEF YYDEBUG}
type	yyTypedRules = record
		name : string;
		tokenType: integer;
	end;
type	yyNamedType = record
		name : string;
		token : integer;
		tokenType: integer;
	end;
{$$ENDIF}
