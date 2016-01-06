(*
 * Copyright 1990, 1991 by Mortice Kern Systems Inc.
 * All rights reserved.
 *)
{$B-}		{ short circuit boolean evaluation }
(*
 * the following can be redefined by the user.
 *)
const
	YYLMAX = 100;		(* token and pushback buffer size *)
	YYLMAX1 = 101;		{**** always 1 more then YYLMAX ****}
(*
 * yy_sbuf[0:yyleng-1] contains the states corresponding to yytext.
 * yytext[0:yyleng-1] contains the current token.
 * yytext[yyleng:yy_end-1] contains pushed-back characters.
 * When the user action routine is active,
 * yy_save contains yytext[yyleng], which is set to '\0'.
 * Things are different when YY_PRESERVE is defined. 
 *)
var
	yytext: array[0..YYLMAX1] of char; (* text buffer (really yytext) *)
	yy_sbuf: array[0..YYLMAX1] of yy_state_t;	(* state buffer *)
	yyin : text;
	yyout: text;
const
	YY_INTERACTIVE : integer = 1;	(* bit faster if 0 *)
	yylineno : integer = 1;		(* line number *)
	yyleng : integer = 0;		(* yytext token length *)
	yy_end:	integer = 0;		(* end of pushback *)
	yy_start: integer = 0;		(* start state *)
	yy_lastc: integer = YYNEWLINE;	(* previous char *)
	yybits: array[0..7] of byte = (1, 2, 4, 8, 16, 32, 64, 128 );

{$IFNDEF YY_PRESERVE}
(* the efficient default push-back scheme *)
var
	yy_save: char;		(* saved yytext[yyleng] *)
{$ELSE}
var
	yy_save: array[0..YYLMAX] of char;
const
	yy_push: integer = YYLMAX;
{$ENDIF}

const
@ GLOBAL DECLARATIONS @
(* Need a definition for yywrap to compile
 * Either issue 'use wrap' after compiling /lib/yywrap.pas,
 * or uncomment the following simple definition:
 *)
(* start of commented-out code
function yywrap: integer;
begin
	yywrap := 1;
end; ** end of commented-out code *)

(* built-in functions for yylex *)

function input  : integer; forward;
procedure unput(c: integer); forward;

procedure ECHO (msg: string);
begin
	write(yyout, msg);
end;

(*
 * yygetc: return next character from input
 * Turbo Pascal seems to have a bug in release 4.0
 * wherein Eof(yyin) fails after ^Z is typed on a terminal read
 * Suppress runtime library checking and do our own.
 *)
function yygetc : integer;
var
	ch: char;
begin
	{$I-}		(* suppress I/O runtime library checks *)
	if not Eof(yyin) then begin
		Read(yyin, ch);
		if IOResult <> 0 then
			yygetc := -1
		else
			yygetc := ord(ch)
	end else
		yygetc := -1
	{$I+}		(* reenable I/O checking *)
end;
procedure output(c: char);
begin
	write(yyout, c);
end;
procedure YY_FATAL (msg: string);
begin
	writeln('yylex: ',msg);
	halt(1)
end;
procedure YY_INIT;
begin
	yy_start := 0;
	yyleng := 0;
	yy_end := 0;
	yy_lastc := YYNEWLINE
end;
procedure yy_reset;
begin
	YY_INIT;
	yylineno := 1;
end;
procedure YYBEGIN (state: integer);
begin
	yy_start := state
end;
procedure NLSTATE;
begin
	yy_lastc := YYNEWLINE
end;

(*
 * the following must not be redefined.
 *)
procedure YY_SCANNER; forward;
procedure YY_USER; forward;

{$IFNDEF YY_PRESERVE}
procedure YY_USER;	(* set up yytext for user *)
begin
	yy_save := yytext[yyleng];
	yytext[yyleng] := Char(0)
end;
procedure YY_SCANNER;	(* set up yytext for scanner *)
begin
	yytext[yyleng] := yy_save
end;
{$ELSE}		(* not-so efficient push-back for yytext mungers *)
procedure YY_USER;
var
	i, n: word;
begin
	n := yy_end - yyleng;
	yy_push := YYLMAX - n;
	if n > 0 then
	begin
		{ move(addr(yy_save[yy_push]), addr(yytext[yyleng]), n); }
		for i := 0 to n do
			yy_save[yy_push + i] := yytext[yyleng + i];
		yytext[yyleng] := Char(0)
	end;
end;
procedure YY_SCANNER;
var
	i, n: word;
begin
	n := YYLMAX - yy_push;
	if n > 0 then
	begin
		{ move(addr(yytext[yyleng]), addr(yytext[yy_push]), n); }
		for i := 0 to n do
			yytext[yyleng + i] := yy_save[yy_push + i];
		yy_end := yyleng + n
	end
end;
{$ENDIF}

procedure yyless (n: integer) ;
begin
	if (n >= 0) and (n <= yy_end) then
	begin
		YY_SCANNER;
		yyleng := n;
		YY_USER
	end
end;

(* The following (yycomment) can be changed *)

(*
 * yycomment(trailing-comment-string)
 *	skip comment input, accounting for newlines
 *)
procedure yycomment (mat: string);
var
	c:	integer;
	icp:	integer;
	done:	boolean;
begin
	icp := 1;
	done := false;
	while (icp <= length(mat)) and (not done) do begin
		c := input;
		if c = -1 then
		    begin
			YY_FATAL('end of file in comment');
			done := true
		    end
		else if c <> ord(mat[icp]) then
		    begin
			if c = ord(mat[1]) then
				icp := 2
			else
				icp := 1;
		    end
		else
			inc(icp)
	end;
end;


(*
 * The actual lexical scanner
 *)
function yylex : integer;
@ LOCAL DECLARATIONS @
var
	retval, c, i, yyst, yybase: integer;
	yyfmin, yyfmax: integer;	(* yy_la_act index of final states *)
	yyoldi, yyoleng: integer;	(* base i, yyleng before look-ahead *)
	yyeof: integer;			(* 1 if eof has already been read *)
label
	yy_again,
	yy_contin,
	yy_jammed,
	yy_search,
	yy_found,
	done_loop,
	loop_exit,
	yy_reject,
	yy_more,
	yylexReturn;
begin
	yyeof := 0;
	i := yyleng;
	YY_SCANNER;

  yy_again:
	yyleng := i;
	(* determine previous char. *)
	if i > 0 then
		yy_lastc := Integer(yytext[i-1]);
	(* scan previously accepted token adjusting yylineno *)
	while (i > 0) do begin
		dec(i);
		if Integer(yytext[i]) = YYNEWLINE then
			inc(yylineno)
	end;
	(* adjust pushback *)
	dec(yy_end, yyleng);
	{ move(yytext, addr(yytext[yyleng]), word(yy_end)); }
	for i := 0 to yy_end-1 do
		yytext[i] := yytext[yyleng + i];
	i := 0;

  yy_contin:
	yyoldi := i;

	(* run the state machine until it jams *)
	if yy_lastc = YYNEWLINE then
		yyst := yy_begin[yy_start + 1]
	else
		yyst := yy_begin[yy_start];
	yy_sbuf[i] := yyst;

	repeat
	    begin
{$IFDEF YY_DEBUG}
		writeln('<state ',yyst,' i = ',i,'>');
{$ENDIF}
		if i >= YYLMAX then
			YY_FATAL('Token buffer overflow');

		(* get input char *)
		if i < yy_end then
			c := Integer(yytext[i])    (* get pushback char *)
		else begin
			if yyeof = 0 then
{$IFDEF YYTRACE}
				c := yt_getc
{$ELSE}
				c := yygetc
{$ENDIF}
			else
				c := -1;
			if c <> -1 then
			begin
				yy_end := i+1;
				yytext[i] := chr(c)
			end
			else begin	(* end of file *)
				if i = yyoldi then	(* no token *)
				begin
					yyeof := 0;
					if yywrap <> 0 then
					begin
						retval := 0;
						goto yylexReturn
					end
					else
						goto yy_again
				end
				else begin
					yyeof := 1;	(* don't re-read EOF *)
					goto loop_exit
				end
			end
		end;
{$IFDEF YY_DEBUG}
		writeln('<input ',c,' = ',c,'>');
{$ENDIF}

		(* look up next state *)
		yybase := yy_base[yyst] + c;
		while (yybase > yy_nxtmax) or (yy_check[yybase] <> yyst) do
		begin
			if yyst = yy_endst then
				goto yy_jammed;
			yyst := yy_default[yyst];
			yybase := yy_base[yyst] + c
		end;
		yyst := yy_next[yybase];
	    yy_jammed:
		inc(i);
		yy_sbuf[i] := yyst
	    end;
	until (yyst = yy_endst) or (YY_INTERACTIVE <> 0)
		and (yy_base[yyst] > yy_nxtmax)
		and (yy_default[yyst] = yy_endst) ;


loop_exit:

{$IFDEF YY_DEBUG}
	writeln('<stopped ',yyst,', i = ',i,'>');
{$ENDIF}
	if yyst <> yy_endst then
		inc(i);

  yy_search:
	(* search backward for a final state *)
	dec(i);
	while i > yyoldi do
	begin
		yyst := yy_sbuf[i];
		yyfmin := yy_final[yyst];
		yyfmax := yy_final[yyst+1];
		if yyfmin < yyfmax then
			goto yy_found;	(* found final state(s) *)
		dec(i);
	end;
	(* no match, default action *)
	i := yyoldi + 1;
	output(char(yytext[yyoldi]));
	goto yy_again;

  yy_found:
{$IFDEF YY_DEBUG}
	writeln('<final state ',yyst,', i = ',i,'>');
{$ENDIF}
	yyoleng := i;		(* save length for REJECT *)
	
	(* pushback look-ahead RHS *)
	c := (integer(yy_la_act[yyfmin]) shr 9) - 1;

	if c >= 0 then
	begin		(* trailing context? *)
		while true do
		begin
			dec(i);
			if i < yyoldi then	(* no / *)
			begin
				i := yyoleng;
				goto done_loop
			end else begin
				yyst := yy_sbuf[i];
				if (yy_look[c*YY_LA_SIZE+word(yyst) div 8]
					<> 0)
				 and (yybits[word(yyst) mod 8] <> 0) then
					goto done_loop;
			end
		end;
	done_loop:
	end;

	(* perform action *)
	yyleng := i;
	YY_USER;
	case (yy_la_act[yyfmin] and $1FF) of
@ ACTION CODE @
	end;
	YY_SCANNER;
	i := yyleng;
	goto yy_again;			(* action fell though *)

  yy_reject:
	YY_SCANNER;
	i := yyoleng;			(* restore original yytext *)
	inc(yyfmin);
	if yyfmin < yyfmax then
		goto yy_found		(* another final state, same length *)
	else
		goto yy_search;		(* try shorter yytext *)

  yy_more:
	YY_SCANNER;
	i := yyleng;
	if i > 0 then
		yy_lastc := Integer(yytext[i-1]);
	goto yy_contin;

yylexReturn:
	yylex := retval
end;

(*
 * user callable input/unput functions.
 *)
(* get input char with pushback *)
function input: integer;
var
	c: integer;
begin
{$IFNDEF YY_PRESERVE}
	if yy_end > yyleng then
	begin
		dec(yy_end);
		{ move(addr(yytext[yyleng]), addr(yytext[yyleng+1]),
			word(yy_end-yyleng)); }
		for c := 0 to yy_end-yyleng-1 do
			yytext[yyleng+c] := yytext[yyleng+1+c];
		c := ord(yy_save);
		YY_USER
{$ELSE}
	if yy_push < YYLMAX then
	begin
		c := ord(yy_save[yy_push]);
		inc(yy_push)
{$ENDIF}
	end else
{$IFDEF YYTRACE}
		c := yt_getc;
{$ELSE}
		c := yygetc;
{$ENDIF}
	yy_lastc := c;
	if c = YYNEWLINE then
		inc(yylineno);
	input := c;
end;

(* pushback char *)
procedure unput(c: integer);
var
	i: word;
begin
{$IFNDEF YY_PRESERVE}
	if yy_end >= YYLMAX then
		YY_FATAL('Push-back buffer overflow');
	if yy_end > yyleng then
	begin
		yytext[yyleng] := yy_save;
		{ move(addr(yytext[yyleng+1]), addr(yytext[yyleng]),
			word(yy_end-yyleng)); }
		for i := 0 to yy_end - yyleng - 1 do
			yytext[yyleng+i+1] := yytext[yyleng];
		yytext[yyleng] := Char(0)
	end;
	inc(yy_end);
	yy_save := chr(c);
{$ELSE}
	if yy_push <= 0 then
		YY_FATAL('Push-back buffer overflow');
	dec(yy_push);
	yy_save[yy_push] := chr(c);
{$ENDIF}
	if c = YYNEWLINE then
		dec(yylineno);
end;

@ end of yylex.c @
