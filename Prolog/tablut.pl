
%Autor: ei09093 & ei10011

%------------------------------------------------
%		      SOCKETS                   -
%------------------------------------------------

:-use_module(library(sockets)).
:-use_module(library(random)).

port(60072).

server:-
	port(Port),
	socket_server_open(Port,Socket),
	socket_server_accept(Socket, _Client, Stream, [type(text)]),
	server_loop(Stream),
	socket_server_close(Socket),
	write('Server Exit'),nl.

server_loop(Stream) :-
	repeat,
		read(Stream, ClientRequest),
		write('Received: '), write(ClientRequest), nl,
		server_input(ClientRequest, ServerReply),
		format(Stream, '~q.~n', [ServerReply]),
		write('Send: '), write(ServerReply), nl,
		flush_output(Stream),
	(ClientRequest == bye; ClientRequest == end_of_file), !.

server_input(initialize, ok(Board)):-
	initBoard(Board), !.
server_input(execute(X-Y,Xf-Yf, Board), ok(NewBoard)):-
	move(X-Y,Xf-Yf, Board, NewBoard), !.
server_input(calculate(Player, Board), ok(Xi-Yi,Xf-Yf, NewBoard)):-
	makeIAMove(Player,Board,NewBoard,Xi-Yi,Xf-Yf), !.
server_input(game_end(Board), ok(Winner)):-
	end_game(Board, Winner), !.
server_input(bye, ok):-!.
server_input(end_of_file, ok):-!.
server_input(_, invalid) :- !.


%end_game(Board,Winner):- end_game_aux(Board,Winner,1-1).

%end_game_aux(Board,swedish,X-Y):- X < 10, isWinSwedish(X-Y,Board).
%end_game_aux(Board,moscov,X-Y):- X < 10, isWinMoscov(X-Y,Board).
%end_game_aux(Board,Player,X-Y):- X < 9, X1 is X+1, end_game_aux(Board,Player,X1-Y).
%end_game_aux(Board,Player,_-Y):- Y < 9, Y1 is Y+1, end_game_aux(Board,Player,1-Y1).

end_game(Board,swedish):- getPosition(1-1,rei,Board,X-Y), isWinSwedish(X-Y,Board).
end_game(Board,moscov):- getPosition(1-1,rei,Board,X-Y), moscovWin(X-Y,Board).

moscovWin(X-Y, Board):-findPosPiece(rei,X-Y,Board), X1 is X-1, findPosPiece(Enemy1,X1-Y,Board),  X2 is X+1, findPosPiece(Enemy2,X2-Y,Board), Y1 is Y+1, findPosPiece(Enemy3,X-Y1,Board), Y2 is Y-1, findPosPiece(Enemy4,X-Y2,Board), enemies(rei,Enemy1), enemies(rei,Enemy2), enemies(rei,Enemy3), enemies(rei,Enemy4).

%peças
swedish(s01).
swedish(s02).
swedish(s03).
swedish(s04).
swedish(s05).
swedish(s06).
swedish(s07).
swedish(s08).
swedish(rei).

muscovites(m01).
muscovites(m02).
muscovites(m03).
muscovites(m04).
muscovites(m05).
muscovites(m06).
muscovites(m07).
muscovites(m08).
muscovites(m09).
muscovites(m10).
muscovites(m11).
muscovites(m12).
muscovites(m13).
muscovites(m14).
muscovites(m15).
muscovites(m16).

%--------------------------------------------------------
%-              INICIALIZAÇÃO DO TABULEIRO              -
%--------------------------------------------------------
initBoard3([[' ',' ',' ',' ',m06,' ',' ',' ',' '],
		   [' ',' ',s07,' ',m08,' ',' ',' ',' '],
		   [' ',' ',' ',' ',' ',' ',' ',' ',' '],
		   [s08,' ',s01,' ',' ',' ',m12,' ',m09],
		   [m02,' ',' ',' ',rei,' ',' ',m11,m10],
		   [m03,' ',s02,' ',s05,m05,m07,' ',' '],
		   [' ',' ',' ',' ',s06,' ',' ',' ',' '],
		   [' ',' ',' ',' ',m16,' ',' ',' ',' '],
           [' ',' ',' ',m13,m14,' ',' ',' ',' ']]).

initBoard2([[' ',' ',' ',' ',m06,' ',' ',' ',' '],
		   [' ',' ',s02,' ',m08,' ',' ',' ',' '],
		   [' ',' ',m05,' ',' ',' ',' ',' ',' '],
		   [s07,m01,' ',' ',s01,m12,' ',' ',m09],
		   [m02,' ',m04,' ','[X]',rei,s04,m11,m10],
		   [m03,' ',s08,' ',s05,m15,m07,' ',' '],
		   [' ',' ',' ',' ',s06,' ',' ',' ',' '],
		   [' ',' ',' ',' ',m16,' ',' ',' ',' '],
           [' ',' ',' ',m13,m14,' ',' ',' ',' ']]).

initBoard([[' ',' ',' ',m05,m06,m07,' ',' ',' '],
		    [' ',' ',' ',' ',m08,' ',' ',' ',' '],
		    [' ',' ',' ',' ',s02,' ',' ',' ',' '],
		    [m01,' ',' ',' ',s01,' ',' ',' ',m09],
		    [m02,m04,s08,s07,rei,s03,s04,m11,m10],
		    [m03,' ',' ',' ',s05,' ',' ',' ',m12],
		    [' ',' ',' ',' ',s06,' ',' ',' ',' '],
		    [' ',' ',' ',' ',m16,' ',' ',' ',' '],
            [' ',' ',' ',m13,m14,m15,' ',' ',' ']]).

%--------------------------------------------------------
%-           IMPRESSÃO DO TABULEIRO DE JOGO             -
%--------------------------------------------------------

viewBoard(Board) :- write('   1   2   3   4   5   6   7   8   9'),nl, printBoard(1, Board).
%*********************** imprime o tabuleiro ******************
printBoard(_,[]) :- write(' +---+---+---+---+---+---+---+---+---+').
printBoard(N,[L|R]) :- write(' +---+---+---+---+---+---+---+---+---+'), nl, write(N), printLine(L), nl, N1 is N + 1, printBoard(N1, R).

%*********************** imprime uma linha ******************
printLine([]) :- write('|').
printLine([E|R]) :- write('|'), printElement(E), printLine(R).

%*********************** imprime um elemento ******************
printElement(' ') :- write('   ').
printElement(E) :- write(E).

printPlayer(Player) :- (Player == moscov), write('Mosvite'); write('Swedish').

changePlayer(moscov, swedish).
changePlayer(swedish, moscov).

verifyPiecePlayed(moscov, Piece) :- muscovites(Piece).
verifyPiecePlayed(swedish, Piece) :- swedish(Piece).


%----------------------->VERIFICAR<----------------------
%--------------------------------------------------------
%-                     CICLO DE JOGO                    -
%--------------------------------------------------------

titulo:-
write('  %%%%%%%%  %%%%%%%%  %%%%%%%   %%        %%    %%   %%%%%%%%'),nl,
write('  %%%%%%%%  %%    %%  %%%%%%%%  %%        %%    %%   %%%%%%%%'),nl,
write('     %%     %%    %%  %%    %%  %%        %%    %%      %%   '),nl,
write('     %%     %%    %%  %%     %  %%        %%    %%      %%   '),nl,
write('     %%     %%    %%  %%    %%  %%        %%    %%      %%   '),nl,
write('     %%     %%%%%%%%  %%%%%%%   %%        %%    %%      %%   '),nl,
write('     %%     %%%%%%%%  %%    %%  %%        %%    %%      %%   '),nl,
write('     %%     %%    %%  %%     %  %%        %%    %%      %%   '),nl,
write('     %%     %%    %%  %%    %%  %%        %%    %%      %%   '),nl,
write('     %%     %%    %%  %%%%%%%%  %%%%%%%%  %%%%%%%%      %%   '),nl,
write('     %%     %%    %%  %%%%%%%   %%%%%%%%  %%%%%%%%      %%   '),nl,
write('                   Written by Pedro Costa and Damien Rosa'),
nl,nl.


game:- titulo,
write('1. NewGame'), nl, write('2. Instruction'), nl, read(Res),
	(Res == 1,
		write('1. User VS User'), nl,
		write('2. User VS Muscovite(IA)'), nl,
		write('3. User VS Swedish(IA)'), nl,
	        write('4. IA vs IA'), nl, read(Option),
			(Option == 1, init; Option == 2, initIA(moscov);Option == 3, initIA(swedish); Option == 4 , initIAvsIA ));
	(Res == 2 , write('Coming soon'), nl, geme).

init :- initBoard(B), viewBoard(B), play(moscov, B).
initIA(Ia):- initBoard3(B), viewBoard(B), playIA(moscov,B,Ia).
initIAvsIA:- initBoard(B), viewBoard(B), playIAvsIA(moscov,B).

playIAvsIA(Player,Board):- nl,printPlayer(Player),nl,
	             makeIAMove(Player,Board,NewBoard,_-_,X-Y),winIAvsIA(Player,X-Y,NewBoard).

playIA(Player,Board,Ia):-nl,printPlayer(Player),nl,
	                 Player == Ia, makeIAMove(Ia,Board,NewBoard,_-_,X-Y), winIA(Player,X-Y,NewBoard,Ia);
			 write('Choose piece: '), nl,
			 write('X: '), read(Xi),
			 write('Y: '), read(Yi),
	                 findPosPiece(Piece,Xi-Yi,Board),
	                 write('Piece choosen: '), write(Piece), nl,
			 (verifyPiecePlayed(Player, Piece) , continuegameIa(Player, Xi-Yi, Board,Ia);
	                 (write('This piece is not available to you! Please retry!'), nl,viewBoard(Board), playIA(Player, Board,Ia))).


continuegameIa(Player, Xi-Yi, Board,Ia) :-
	write('New X: '), read(X),
	write('New Y: '), read(Y),
	(move(Xi-Yi,X-Y,Board,NewBoard), winIA(Player,X-Y,NewBoard,Ia));
	(write('This move is impossible! Please retry!'), nl,viewBoard(Board), playIA(Player, Board,Ia)).


%play(Player,Board)
play(Player,Board) :-
	nl, printPlayer(Player), nl,
	write('Choose piece: '), nl,
	write('X: '), read(Xi),
	write('Y: '), read(Yi),
	findPosPiece(Piece,Xi-Yi,Board),
	write('Piece choosen: '), write(Piece), nl,
	(verifyPiecePlayed(Player, Piece) , continuegame(Player, Xi-Yi, Board);
	(write('This piece is not available to you! Please retry!'), nl,viewBoard(Board), play(Player, Board))).

continuegame(Player, Xi-Yi, Board) :-
	write('New X: '), read(X),
	write('New Y: '), read(Y),
	(move(Xi-Yi,X-Y,Board,NewBoard), win(Player,X-Y,NewBoard));
	(write('This move is impossible! Please retry!'), nl,viewBoard(Board), play(Player, Board)).

win(swedish,X-Y,Board):- isWinSwedish(X-Y,Board), write('Swedish win').
win(moscov,X-Y,Board):- isWinMoscov(X-Y,Board), write('Muscovites win').
win(Player,_-_,Board):- viewBoard(Board), changePlayer(Player,Second), play(Second, Board).

winIA(swedish,X-Y,Board,_):- isWinSwedish(X-Y,Board), write('Swedish win').
winIA(moscov,X-Y,Board,_):- isWinMoscov(X-Y,Board), write('Muscovites win').
winIA(Player,_-_,Board,Ia):- viewBoard(Board), changePlayer(Player,Second), playIA(Second, Board,Ia).

winIAvsIA(swedish,X-Y,Board):- isWinSwedish(X-Y,Board), write('Swedish win').
winIAvsIA(moscov,X-Y,Board):- isWinMoscov(X-Y,Board), write('Muscovites win').
winIAvsIA(Player,_-_,Board):- viewBoard(Board), changePlayer(Player,Second), nl, read(_), playIAvsIA(Second, Board).


%--------------------------------------------------------
%-                  MOVIMENTAÇÃO DA PEÇA                -
%--------------------------------------------------------
%*********************** encontra uma peça numa posição ******************
findPosPiece(Piece, X-1, [H|_]) :- findPosPieceCol(Piece,X, H). %encontra posição da peça no tabuleiro
findPosPiece(Piece, X-Y, [_|T]) :- Y>1, Y1 is Y-1, findPosPiece(Piece,X-Y1,T).

%*********************** encontra uma peça numa posição na coluna ******************
findPosPieceCol(H,1, [H|_]) :- H \= ' ', H\='[X]'.
findPosPieceCol(Piece,X, [_|T]) :- X>1, X1 is X-1, findPosPieceCol(Piece,X1, T).

%*********************** adiciona peça ******************
%addPiece(Piece, X-Y, Board). %adiciona peça no tabuleiro


%*********************** remover peça ******************
%removePiece(Piece, 1-Y, [H|_]). %remove peça do tabuleiro

addPiece(Piece, X-Y, BoardIni, EndBoard) :- setLine(Piece, X-Y, BoardIni, EndBoard).
setLine(Piece, X-Y, [H|T], [H|T2]) :- Y>1, Y1 is Y-1, setLine(Piece, X-Y1, T, T2).
setLine(Piece, X-Y, [H|T], [H2|T]) :- Y=1, setColumn(Piece, X, H, H2).

setColumn(Piece, X, [H|T], [H|T2]) :- X>1, X1 is X-1, setColumn(Piece,X1,T,T2).
setColumn(Piece, 1, [_|T], [Piece|T]).


%verifyWay(Xi-Yi, Xf-Yf, Board). %verifica se existe alguma peça pelo "caminho"
%move(Piece, iPos, fPos, Board). %acção de realizar um movimento de uma peça


move(5-5,Xf-Yf,Board,NewBoard):-findPosPiece(rei,5-5,Board),verifyWay(5-5,Xf-Yf,Board), addPiece('[X]',5-5,Board,MiddleBoard1), addPiece(rei,Xf-Yf,MiddleBoard1,MiddleBoard2),X1 is Xf-1, verifyCapture(X1-Yf,MiddleBoard2), addPiece(' ',X1-Yf,MiddleBoard2,NewBoard).

move(5-5,Xf-Yf,Board,NewBoard):-findPosPiece(rei,5-5,Board),verifyWay(5-5,Xf-Yf,Board), addPiece('[X]',5-5,Board,MiddleBoard1), addPiece(rei,Xf-Yf,MiddleBoard1,MiddleBoard2),X1 is Xf+1, verifyCapture(X1-Yf,MiddleBoard2), addPiece(' ',X1-Yf,MiddleBoard2,NewBoard).

move(5-5,Xf-Yf,Board,NewBoard):-findPosPiece(rei,5-5,Board),verifyWay(5-5,Xf-Yf,Board), addPiece('[X]',5-5,Board,MiddleBoard1), addPiece(rei,Xf-Yf,MiddleBoard1,MiddleBoard2),Y1 is Yf-1, verifyCapture(Xf-Y1,MiddleBoard2), addPiece(' ',Xf-Y1,MiddleBoard2,NewBoard).

move(5-5,Xf-Yf,Board,NewBoard):-findPosPiece(rei,5-5,Board),verifyWay(5-5,Xf-Yf,Board), addPiece('[X]',5-5,Board,MiddleBoard1), addPiece(rei,Xf-Yf,MiddleBoard1,MiddleBoard2),Y1 is Yf+1, verifyCapture(Xf-Y1,MiddleBoard2), addPiece(' ',Xf-Y1,MiddleBoard2,NewBoard).

move(5-5,Xf-Yf,Board,NewBoard):-findPosPiece(rei,5-5,Board),verifyWay(5-5,Xf-Yf,Board), addPiece('[X]',5-5,Board,MiddleBoard), addPiece(rei,Xf-Yf,MiddleBoard,NewBoard).


move(Xi-Yi,Xf-Yf,Board,NewBoard):-findPosPiece(Piece,Xi-Yi,Board),verifyWay(Xi-Yi,Xf-Yf,Board), addPiece(' ',Xi-Yi,Board,MiddleBoard1), addPiece(Piece,Xf-Yf,MiddleBoard1,MiddleBoard2),X1 is Xf-1, verifyCapture(X1-Yf,MiddleBoard2), addPiece(' ',X1-Yf,MiddleBoard2,NewBoard).

move(Xi-Yi,Xf-Yf,Board,NewBoard):-findPosPiece(Piece,Xi-Yi,Board),verifyWay(Xi-Yi,Xf-Yf,Board), addPiece(' ',Xi-Yi,Board,MiddleBoard1), addPiece(Piece,Xf-Yf,MiddleBoard1,MiddleBoard2),X1 is Xf+1, verifyCapture(X1-Yf,MiddleBoard2), addPiece(' ',X1-Yf,MiddleBoard2,NewBoard).

move(Xi-Yi,Xf-Yf,Board,NewBoard):-findPosPiece(Piece,Xi-Yi,Board),verifyWay(Xi-Yi,Xf-Yf,Board), addPiece(' ',Xi-Yi,Board,MiddleBoard1), addPiece(Piece,Xf-Yf,MiddleBoard1,MiddleBoard2),Y1 is Yf+1, verifyCapture(Xf-Y1,MiddleBoard2), addPiece(' ',Xf-Y1,MiddleBoard2,NewBoard).

move(Xi-Yi,Xf-Yf,Board,NewBoard):-findPosPiece(Piece,Xi-Yi,Board),verifyWay(Xi-Yi,Xf-Yf,Board), addPiece(' ',Xi-Yi,Board,MiddleBoard1), addPiece(Piece,Xf-Yf,MiddleBoard1,MiddleBoard2),Y1 is Yf-1, verifyCapture(Xf-Y1,MiddleBoard2), addPiece(' ',Xf-Y1,MiddleBoard2,NewBoard).

move(Xi-Yi,Xf-Yf,Board,NewBoard):-findPosPiece(Piece,Xi-Yi,Board),verifyWay(Xi-Yi,Xf-Yf,Board),addPiece(' ',Xi-Yi,Board,MiddleBoard), addPiece(Piece,Xf-Yf,MiddleBoard,NewBoard).

verifyWay(Xi-Yi,Xf-Yf,Board) :- Yi==Yf, Xf > Xi , X is Xi+1,verifyLine(X-Yi,Xf-Yf,Board).
verifyWay(Xi-Yi,Xf-Yf,Board) :- Xi==Xf, Yf > Yi , Y is Yi+1, verifyColumn(Xi-Y,Xf-Yf,Board).
verifyWay(Xi-Yi,Xf-Yf,Board) :- Yi==Yf, Xi > Xf , X is Xi-1,verifyLine(Xf-Yf,X-Yi,Board).
verifyWay(Xi-Yi,Xf-Yf,Board) :- Xi==Xf, Yi > Yf , Y is Yi-1, verifyColumn(Xf-Yf,Xi-Y,Board).

verifyLine(Xi-Yi,Xf-_,[_|T]) :- Yi > 1, Y1 is Yi-1,verifyLine(Xi-Y1,Xf-Y1,T).
verifyLine(Xi-1,Xf-1,[H|_]) :- verifyLineAux(Xi,Xf,H).
verifyLineAux(1,0,_).
verifyLineAux(Xi,Xf,[_|T]) :- Xi > 1, X1 is Xi-1,X2 is Xf-1, verifyLineAux(X1,X2,T).
verifyLineAux(1,Xf,[H|T]) :- Xf > 0 , H == ' ', X is Xf - 1, verifyLineAux(1,X,T).

verifyColumn(Xi-Yi,Xf-Yf,[_|T]):- Yi > 1 , Y1 is Yi-1, Y2 is Yf-1, verifyColumn(Xi-Y1,Xf-Y2,T).
verifyColumn(Xi-1,Xf-Yf,[H|T]) :- Yf > 1 , verifyColumnAux(Xi,H), Y is Yf-1, verifyColumn(Xi-1,Xf-Y,T).
verifyColumn(Xi-1,_-1,[H|_]) :- verifyColumnAux(Xi,H).

verifyColumnAux(1,[' '|_]).
verifyColumnAux(X,[_|T]):- X > 1,X1 is X-1, verifyColumnAux(X1,T).

isValidMove(Xi-Yi,Xf-Yf) :- Xi == Xf; Yi == Yf.

%--------------------------------------------------------
%-                  VALIDAÇÃO DE JOGADA                 -
%--------------------------------------------------------
%*********************** inimigos ******************
enemies(_,'[X]').
enemies('[X]',_).
enemies(Piece1, Piece2) :- muscovites(Piece1), swedish(Piece2).
enemies(Piece1, Piece2) :- muscovites(Piece2), swedish(Piece1). %verificar captura|fim de jogo.
%*********************** no konakis ******************
inKonakis(X-Y) :- X == 5, Y == 5. %verifica se a posição for a posição trono

%*********************** posição vazia ******************
isEmpty(X-1, [H|_]):- isEmptyCol(X,H).
isEmpty(X-Y, [_|T]):- Y1 is Y-1, isEmpty(X-Y1,T).

isEmptyCol(1,[H|_]):- H == ' '.
isEmptyCol(X,[_|T]):- X1 is X-1,isEmptyCol(X1,T).

%*********************** nos limites do tabuleiro ******************
inLimits(X-Y):- X>0 , X<9 , Y>0 , Y<9. %verifica se posição final está dentro dos limites

%*********************** verifica a posição para onde se pretende jogar ******************
verifyPos(Pos, Board):- %verifica se celula ocupada, se celula trono ou se fora dos limites antes da confirmação da jogada.
	\+inKonakis(Pos), isEmpty(Pos, Board), inLimits(Pos). %se se verificar isto tudo então jogada válida

%--------------------------------------------------------
%-                  VALIDAÇÃO DA CAPTURA                -
%--------------------------------------------------------
%verifyCapture(Pos, Board). %verifica se há possibilidade de captura e em caso afirmativo remove a peça capturada

verifyCapture(X-Y,Board):-findPosPiece(Piece,X-Y,Board),Piece\=rei,X1 is X+1,findPosPiece(Enemy1,X1-Y,Board), X2 is X-1, findPosPiece(Enemy2,X2-Y,Board), enemies(Piece,Enemy1), enemies(Piece,Enemy2).
verifyCapture(X-Y,Board):-findPosPiece(Piece,X-Y,Board),Piece\=rei,Y1 is Y+1,findPosPiece(Enemy1,X-Y1,Board), Y2 is Y-1, findPosPiece(Enemy2,X-Y2,Board), enemies(Piece,Enemy1), enemies(Piece,Enemy2).


%--------------------------------------------------------
%-                 VALIDAÇÃO DE FIM JOGO                -
%--------------------------------------------------------
%isWinSwedish(rei, Pos, Board).
isWinSwedish(X-Y,Board):-findPosPiece(rei,X-Y,Board), isOnLimits(X-Y).

isOnLimits(1-_).
isOnLimits(9-_).
isOnLimits(_-1).
isOnLimits(_-9).

%isWinMoscov(rei, Board). %verifica se o rei tem algum movimento possivel.
isWinMoscov(X-Y,Board):- X1 is X+1, findPosPiece(rei,X1-Y,Board), findPosPiece(Enemy1,X-Y,Board),  X2 is X+2, findPosPiece(Enemy2,X2-Y,Board), Y1 is Y+1, findPosPiece(Enemy3,X1-Y1,Board), Y2 is Y-1, findPosPiece(Enemy4,X1-Y2,Board), enemies(rei,Enemy1), enemies(rei,Enemy2), enemies(rei,Enemy3), enemies(rei,Enemy4).

isWinMoscov(X-Y,Board):- X1 is X-1, findPosPiece(rei,X1-Y,Board), findPosPiece(Enemy1,X-Y,Board),  X2 is X-2, findPosPiece(Enemy2,X2-Y,Board), Y1 is Y+1, findPosPiece(Enemy3,X1-Y1,Board), Y2 is Y-1, findPosPiece(Enemy4,X1-Y2,Board), enemies(rei,Enemy1), enemies(rei,Enemy2), enemies(rei,Enemy3), enemies(rei,Enemy4).

isWinMoscov(X-Y,Board):- Y1 is Y+1, findPosPiece(rei,X-Y1,Board), findPosPiece(Enemy1,X-Y,Board),  Y2 is Y+2, findPosPiece(Enemy2,X-Y2,Board), X1 is X+1, findPosPiece(Enemy3,X1-Y1,Board), X2 is X-1, findPosPiece(Enemy4,X2-Y1,Board), enemies(rei,Enemy1), enemies(rei,Enemy2), enemies(rei,Enemy3), enemies(rei,Enemy4).

isWinMoscov(X-Y,Board):- Y1 is Y-1, findPosPiece(rei,X-Y1,Board), findPosPiece(Enemy1,X-Y,Board),  Y2 is Y-2, findPosPiece(Enemy2,X-Y2,Board), X1 is X+1, findPosPiece(Enemy3,X1-Y1,Board), X2 is X-1, findPosPiece(Enemy4,X2-Y1,Board), enemies(rei,Enemy1), enemies(rei,Enemy2), enemies(rei,Enemy3), enemies(rei,Enemy4).


%--------------------------------------------------------------
%-               INTELIGENCIA ARTIFICIAL		      -
%--------------------------------------------------------------


getPosition(_-Y,Piece,[H|_],Xf-Y):- getPositionCol(1,Piece,H,Xf).
getPosition(_-Y,Piece,[_|T],Xf-Yf):- Y1 is Y+1, getPosition(_-Y1,Piece,T,Xf-Yf).

getPositionCol(X,Piece,[Piece|_],X).
getPositionCol(X,Piece,[_|T],Xf):- X1 is X+1, getPositionCol(X1,Piece,T,Xf).

makeIAMove(moscov,Board,NewBoard,X-Y,Xf-Yf):- searchPieceToCaptureKing(Board,_,X-Y,Xf-Yf,1), move(X-Y,Xf-Yf,Board,NewBoard).
makeIAMove(moscov,Board,NewBoard,X-Y,Xf-Yf):- searchPieceToCapture(Board,_,X-Y,Xf-Yf,1), move(X-Y,Xf-Yf,Board,NewBoard).
makeIAMove(swedish,Board,NewBoard,X-Y,Xf-Yf):-kingMove(Board,X-Y,Xf-Yf,1), move(X-Y,Xf-Yf,Board,NewBoard).
makeIAMove(swedish,Board,NewBoard,X-Y,Xf-Yf):-searchMoscovToCapture(Board,_,X-Y,Xf-Yf,1), move(X-Y,Xf-Yf,Board,NewBoard).
makeIAMove(Player,Board,NewBoard,X-Y,Xf-Yf):- think(Player,Board,_,X-Y,Xf-Yf,1), move(X-Y,Xf-Yf,Board,NewBoard).
makeIAMove(Player,Board,NewBoard,X-Y,Xf-Yf):- moveSomewhere(Player,Board,_,X-Y,Xf-Yf,1), move(X-Y,Xf-Yf,Board,NewBoard).

think(Player,Board,Piece,X-Y,Xf-Yf,N):- N < 50,getRandomPiece(Player,Piece), getPosition(1-1,Piece,Board,X-Y), getRandomDirection(D), searchPos(X-Y,Board,Xf-Yf,Piece,D,1).
think(Player,Board,Piece,X-Y,Xf-Yf,N):- N < 50, N1 is N+1,think(Player,Board,Piece,X-Y,Xf-Yf,N1).

moveSomewhere(swedish,Board,Piece,X-Y,Xf-Y,N):- getSwedish(N,Piece), getPosition(1-1,Piece,Board,X-Y), canMoveByDirection(X-Y,Board,right), Xf is X+1.
moveSomewhere(swedish,Board,Piece,X-Y,Xf-Y,N):- getSwedish(N,Piece), getPosition(1-1,Piece,Board,X-Y), canMoveByDirection(X-Y,Board,left), Xf is X+1.
moveSomewhere(swedish,Board,Piece,X-Y,X-Yf,N):- getSwedish(N,Piece), getPosition(1-1,Piece,Board,X-Y), canMoveByDirection(X-Y,Board,down), Yf is Y+1.
moveSomewhere(swedish,Board,Piece,X-Y,X-Yf,N):- getSwedish(N,Piece), getPosition(1-1,Piece,Board,X-Y), canMoveByDirection(X-Y,Board,up), Yf is Y-1.
moveSomewhere(swedish,Board,Piece,X-Y,Xf-Yf,N):- N < 9, N1 is N+1, moveSomewhere(swedish,Board,Piece,X-Y,Xf-Yf,N1).
moveSomewhere(moscov,Board,Piece,X-Y,Xf-Y,N):- getMoscov(N,Piece), getPosition(1-1,Piece,Board,X-Y), canMoveByDirection(X-Y,Board,right), Xf is X+1.
moveSomewhere(moscov,Board,Piece,X-Y,Xf-Y,N):- getMoscov(N,Piece), getPosition(1-1,Piece,Board,X-Y), canMoveByDirection(X-Y,Board,left), Xf is X+1.
moveSomewhere(moscov,Board,Piece,X-Y,X-Yf,N):- getMoscov(N,Piece), getPosition(1-1,Piece,Board,X-Y), canMoveByDirection(X-Y,Board,down), Yf is Y+1.
moveSomewhere(moscov,Board,Piece,X-Y,X-Yf,N):- getMoscov(N,Piece), getPosition(1-1,Piece,Board,X-Y), canMoveByDirection(X-Y,Board,up), Yf is Y-1.
moveSomewhere(moscov,Board,Piece,X-Y,Xf-Yf,N):- N < 16, N1 is N+1, moveSomewhere(moscov,Board,Piece,X-Y,Xf-Yf,N1).



canMoveByDirection(X-Y,Board,right):- X1 is X+1, isEmpty(X1-Y,Board).
canMoveByDirection(X-Y,Board,left):- X1 is X-1, isEmpty(X1-Y,Board).
canMoveByDirection(X-Y,Board,up):- Y1 is Y-1, isEmpty(X-Y1,Board).
canMoveByDirection(X-Y,Board,down):- Y1 is Y+1, isEmpty(X-Y1,Board).


getRandomPiece(swedish, Piece) :- random(1,10,N), getSwedish(N,Piece).
getRandomPiece(moscov, Piece) :- random(1,17,N), getMoscov(N,Piece).


getSwedish(1,s01).
getSwedish(2,s02).
getSwedish(3,s03).
getSwedish(4,s04).
getSwedish(5,s05).
getSwedish(6,s06).
getSwedish(7,s07).
getSwedish(8,s08).
getSwedish(9,rei).

getMoscov(1,m01).
getMoscov(2,m02).
getMoscov(3,m03).
getMoscov(4,m04).
getMoscov(5,m05).
getMoscov(6,m06).
getMoscov(7,m07).
getMoscov(8,m08).
getMoscov(9,m09).
getMoscov(10,m11).
getMoscov(11,m11).
getMoscov(12,m12).
getMoscov(13,m13).
getMoscov(14,m14).
getMoscov(15,m15).
getMoscov(16,m16).

enemyAround(X-Y,Piece,Board):- X1 is X+1,findPosPiece(Enemy,X1-Y,Board), enemies(Piece,Enemy).
enemyAround(X-Y,Piece,Board):- X1 is X-1,findPosPiece(Enemy,X1-Y,Board), enemies(Piece,Enemy).

enemyAround(X-Y,Piece,Board):- Y1 is Y+1,findPosPiece(Enemy,X-Y1,Board), enemies(Piece,Enemy).
enemyAround(X-Y,Piece,Board):- Y1 is Y-1,findPosPiece(Enemy,X-Y1,Board), enemies(Piece,Enemy).


getRandomDirection(Direction):- random(1,5,N), getDirection(Direction,N).

getDirection(horizontalright,1).
getDirection(verticaldown,2).
getDirection(horizontalleft,3).
getDirection(verticalup,4).


searchPos(X-Y,Board,Xf-Yf,Piece,Direction,Tries):- Tries < 5,searchPosByDirection(X-Y,Piece,Board,Xf-Yf,Direction).
searchPos(X-Y,Board,Xf-Yf,Piece,Direction,Tries):- Tries < 5,getDirection(Direction,N), N1 is N+1, getDirection(NewDir,N1),T is Tries+1, searchPos(X-Y,Board,Xf-Yf,Piece,NewDir,T).
searchPos(X-Y,Board,Xf-Yf,Piece,Direction,Tries):- Tries < 5,getDirection(Direction,N),N==4,T is Tries+1, searchPos(X-Y,Board,Xf-Yf,Piece,horizontalright,T).



searchPosByDirection(X-Y,Piece,Board,X-Y,horizontalright):- isEmpty(X-Y,Board),enemyAround(X-Y, Piece,Board).
searchPosByDirection(X-Y,Piece,Board,Xf-Yf,horizontalright):- X1 is X+1,X < 10,isEmpty(X1-Y,Board), searchPosByDirection(X1-Y,Piece,Board,Xf-Yf,horizontalright).

searchPosByDirection(X-Y,Piece,Board,X-Y,verticaldown):- isEmpty(X-Y,Board),enemyAround(X-Y,Piece,Board).
searchPosByDirection(X-Y,Piece,Board,Xf-Yf,verticaldown):- Y1 is Y+1,Y1 < 10, isEmpty(X-Y1,Board), searchPosByDirection(X-Y1,Piece,Board,Xf-Yf,verticaldown).

searchPosByDirection(X-Y,Piece,Board,X-Y,horizontalleft):- isEmpty(X-Y,Board),enemyAround(X-Y, Piece,Board).
searchPosByDirection(X-Y,Piece,Board,Xf-Yf,horizontalleft):- X1 is X-1,X > 0,isEmpty(X1-Y,Board), searchPosByDirection(X1-Y,Piece,Board,Xf-Yf,horizontalleft).

searchPosByDirection(X-Y,Piece,Board,X-Y,verticalup):- isEmpty(X-Y,Board),enemyAround(X-Y,Piece,Board).
searchPosByDirection(X-Y,Piece,Board,Xf-Yf,verticalup):- Y1 is Y-1,Y1 >0, isEmpty(X-Y1,Board), searchPosByDirection(X-Y1,Piece,Board,Xf-Yf,verticalup).





%----------------------------------------------------------------------
%                        MOSCOV OBJECTIVES                            -
%----------------------------------------------------------------------

enemyDir(X-Y,Piece,Board,baixo):- Y1 is Y+1, findPosPiece(Enemy,X-Y1,Board), enemies(Piece,Enemy),Y2 is Y1+1, findPosPiece(Friend,X-Y2,Board), enemies(Enemy,Friend).
enemyDir(X-Y,Piece,Board,cima):- Y1 is Y-1, findPosPiece(Enemy,X-Y1,Board), enemies(Piece,Enemy), Y2 is Y1-1, findPosPiece(Friend,X-Y2,Board), enemies(Enemy,Friend).
enemyDir(X-Y,Piece,Board,esquerda):- X1 is X-1, findPosPiece(Enemy,X1-Y,Board), enemies(Piece,Enemy), X2 is X1-1, findPosPiece(Friend,X2-Y,Board), enemies(Enemy,Friend).
enemyDir(X-Y,Piece,Board,direita):- X1 is X+1, findPosPiece(Enemy,X1-Y,Board), enemies(Piece,Enemy), X2 is X1+1, findPosPiece(Friend,X2-Y,Board), enemies(Enemy,Friend).

searchPieceToCapture(Board,Piece,X-Y,Xf-Yf,N):- getMoscov(N,Piece),getPosition(1-1,Piece,Board,X-Y),getRandomDirection(D), findCaptureMove(X-Y,Board,Xf-Yf,Piece,D,1).
searchPieceToCapture(Board,Piece,X-Y,Xf-Yf,N):- N < 16, N1 is N+1, searchPieceToCapture(Board,Piece,X-Y,Xf-Yf,N1).

findCaptureMove(X-Y,Board,Xf-Yf,Piece,Direction,Tries):- Tries < 5, searchCapturePos(X-Y,Piece,Board,Xf-Yf,Direction).
findCaptureMove(X-Y,Board,Xf-Yf,Piece,Direction,Tries):- Tries < 5, getDirection(Direction,N), N1 is N+1, getDirection(NewDir,N1), T is Tries+1,findCaptureMove(X-Y,Board,Xf-Yf,Piece,NewDir,T).
findCaptureMove(X-Y,Board,Xf-Yf,Piece,Direction,Tries):- Tries < 5, getDirection(Direction,N),N==4,T is Tries+1,findCaptureMove(X-Y,Board,Xf-Yf,Piece,horizontalright,T).

searchCapturePos(X-Y,Piece,Board,X-Y,horizontalright):- isEmpty(X-Y,Board), enemyDir(X-Y,Piece,Board,baixo).
searchCapturePos(X-Y,Piece,Board,X-Y,horizontalright):- isEmpty(X-Y,Board), enemyDir(X-Y,Piece,Board,cima).
searchCapturePos(X-Y,Piece,Board,X-Y,horizontalright):- isEmpty(X-Y,Board), enemyDir(X-Y,Piece,Board,direita).
searchCapturePos(X-Y,Piece,Board,X-Y,horizontalright):- isEmpty(X-Y,Board), enemyDir(X-Y,Piece,Board,esquerda).
searchCapturePos(X-Y,Piece,Board,Xf-Yf,horizontalright):- X1 is X+1, X<10,isEmpty(X1-Y,Board),searchCapturePos(X1-Y,Piece,Board,Xf-Yf,horizontalright).

searchCapturePos(X-Y,Piece,Board,X-Y,horizontalleft):- isEmpty(X-Y,Board), enemyDir(X-Y,Piece,Board,baixo).
searchCapturePos(X-Y,Piece,Board,X-Y,horizontalleft):- isEmpty(X-Y,Board), enemyDir(X-Y,Piece,Board,cima).
searchCapturePos(X-Y,Piece,Board,X-Y,horizontalleft):- isEmpty(X-Y,Board), enemyDir(X-Y,Piece,Board,direita).
searchCapturePos(X-Y,Piece,Board,X-Y,horizontalleft):- isEmpty(X-Y,Board), enemyDir(X-Y,Piece,Board,esquerda).
searchCapturePos(X-Y,Piece,Board,Xf-Yf,horizontalleft):- X1 is X-1, X>0,isEmpty(X1-Y,Board),searchCapturePos(X1-Y,Piece,Board,Xf-Yf,horizontalleft).

searchCapturePos(X-Y,Piece,Board,X-Y,verticalup):- isEmpty(X-Y,Board), enemyDir(X-Y,Piece,Board,baixo).
searchCapturePos(X-Y,Piece,Board,X-Y,verticalup):- isEmpty(X-Y,Board), enemyDir(X-Y,Piece,Board,cima).
searchCapturePos(X-Y,Piece,Board,X-Y,verticalup):- isEmpty(X-Y,Board), enemyDir(X-Y,Piece,Board,direita).
searchCapturePos(X-Y,Piece,Board,X-Y,verticalup):- isEmpty(X-Y,Board), enemyDir(X-Y,Piece,Board,esquerda).
searchCapturePos(X-Y,Piece,Board,Xf-Yf,verticalup):- Y1 is Y-1, Y>0,isEmpty(X-Y1,Board),searchCapturePos(X-Y1,Piece,Board,Xf-Yf,verticalup).

searchCapturePos(X-Y,Piece,Board,X-Y,verticaldown):- isEmpty(X-Y,Board), enemyDir(X-Y,Piece,Board,baixo).
searchCapturePos(X-Y,Piece,Board,X-Y,verticaldown):- isEmpty(X-Y,Board), enemyDir(X-Y,Piece,Board,cima).
searchCapturePos(X-Y,Piece,Board,X-Y,verticaldown):- isEmpty(X-Y,Board), enemyDir(X-Y,Piece,Board,direita).
searchCapturePos(X-Y,Piece,Board,X-Y,verticaldown):- isEmpty(X-Y,Board), enemyDir(X-Y,Piece,Board,esquerda).
searchCapturePos(X-Y,Piece,Board,Xf-Yf,verticaldown):- Y1 is Y+1, Y<10,isEmpty(X-Y1,Board),searchCapturePos(X-Y1,Piece,Board,Xf-Yf,verticaldown).


% KING OBJECTIVE

kingAround(X-Y,Board):- X1 is X+1, findPosPiece(rei,X1-Y,Board).
kingAround(X-Y,Board):- X1 is X-1, findPosPiece(rei,X1-Y,Board).
kingAround(X-Y,Board):- Y1 is Y+1, findPosPiece(rei,X-Y1,Board).
kingAround(X-Y,Board):- Y1 is Y-1, findPosPiece(rei,X-Y1,Board).

searchPieceToCaptureKing(Board,Piece,X-Y,Xf-Yf,N):-getMoscov(N,Piece), getPosition(1-1,Piece,Board,X-Y),getRandomDirection(D), searchKing(X-Y,Board,Xf-Yf,D,1).
searchPieceToCaptureKing(Board,Piece,X-Y,Xf-Yf,N):- N < 16, N1 is N+1, searchPieceToCaptureKing(Board,Piece,X-Y,Xf-Yf,N1).

searchKing(X-Y,Board,Xf-Yf,Direction,Tries):- Tries < 5, searchKingByDirection(X-Y,Board,Xf-Yf,Direction).
searchKing(X-Y,Board,Xf-Yf,Direction,Tries):- Tries < 5, getDirection(Direction,N), N1 is N+1, getDirection(NewDir,N1), T is Tries+1, searchKing(X-Y,Board,Xf-Yf,NewDir,T).
searchKing(X-Y,Board,Xf-Yf,Direction,Tries):- Tries < 5, getDirection(Direction,N), N==4, T is Tries+1, searchKing(X-Y,Board,Xf-Yf,horizontalright,T).


searchKingByDirection(X-Y,Board,X-Y,horizontalright):- isEmpty(X-Y,Board), kingAround(X-Y,Board).
searchKingByDirection(X-Y,Board,Xf-Yf,horizontalright):- X1 is X+1, isEmpty(X1-Y,Board),searchKingByDirection(X1-Y,Board,Xf-Yf,horizontalright).

searchKingByDirection(X-Y,Board,X-Y,horizontalleft):- isEmpty(X-Y,Board), kingAround(X-Y,Board).
searchKingByDirection(X-Y,Board,Xf-Yf,horizontalleft):- X1 is X-1, isEmpty(X1-Y,Board),searchKingByDirection(X1-Y,Board,Xf-Yf,horizontalleft).

searchKingByDirection(X-Y,Board,X-Y,verticalup):- isEmpty(X-Y,Board), kingAround(X-Y,Board).
searchKingByDirection(X-Y,Board,Xf-Yf,verticalup):- Y1 is Y-1, isEmpty(X-Y1,Board),searchKingByDirection(X-Y1,Board,Xf-Yf,verticalup).

searchKingByDirection(X-Y,Board,X-Y,verticaldown):- isEmpty(X-Y,Board), kingAround(X-Y,Board).
searchKingByDirection(X-Y,Board,Xf-Yf,verticaldown):- Y1 is Y+1, isEmpty(X-Y1,Board),searchKingByDirection(X-Y1,Board,Xf-Yf,verticaldown).


%-----------------------------------------------------------
%		      SWEDISH OBJECTIVES                   -
%-----------------------------------------------------------

searchMoscovToCapture(Board,Piece,X-Y,Xf-Yf,N):- getSwedish(Piece,N), getPosition(1-1,Piece,Board,X-Y),getRandomDirection(D), findCaptureMove(X-Y,Board,Xf-Yf,Piece,D,1).
searchMoscovToCapture(Board,Piece,X-Y,Xf-Yf,N):- N < 9, N1 is N+1, searchMoscovToCapture(Board,Piece,X-Y,Xf-Yf,N1).

kingMove(Board,X-Y,Xf-Yf,_):- getPosition(1-1,rei,Board,X-Y),kingCanEscape(Board,X-Y,Xf-Yf).
kingMove(Board,X-Y,Xf-Yf,N):- getDirection(D,N),getPosition(1-1,rei,Board,X-Y),kingCanEscapeByDirection(Board,X-Y,Xf-Yf,D).
kingMove(Board,X-Y,Xf-Yf,N):- N < 5, N1 is N+1,kingMove(Board,X-Y,Xf-Yf,N1).


kingCanEscape(Board,X-Y,X-1):-getPosition(1-1,rei,Board,X-Y), verifyWay(X-Y,X-1,Board).
kingCanEscape(Board,X-Y,X-9):-getPosition(1-1,rei,Board,X-Y), verifyWay(X-Y,X-9,Board).
kingCanEscape(Board,X-Y,1-Y):-getPosition(1-1,rei,Board,X-Y), verifyWay(X-Y,1-Y,Board).
kingCanEscape(Board,X-Y,9-Y):-getPosition(1-1,rei,Board,X-Y), verifyWay(X-Y,9-Y,Board).

kingCanEscapeByDirection(Board,X-Y,X-Y,horizontalright):- isEmpty(X-Y,Board), kingCanEscape(Board,X-Y,_-_).
kingCanEscapeByDirection(Board,X-Y,Xf-Yf,horizontalright):- X1 is X+1, isEmpty(X1-Y,Board), kingCanEscapeByDirection(Board,X1-Y,Xf-Yf,horizontalright) .

kingCanEscapeByDirection(Board,X-Y,X-Y,horizontalleft):- isEmpty(X-Y,Board), kingCanEscape(Board,X-Y,_-_).
kingCanEscapeByDirection(Board,X-Y,Xf-Yf,horizontalleft):- X1 is X-1, isEmpty(X1-Y,Board), kingCanEscapeByDirection(Board,X1-Y,Xf-Yf,horizontalright) .


kingCanEscapeByDirection(Board,X-Y,X-Y,verticalup):- isEmpty(X-Y,Board), kingCanEscape(Board,X-Y,_-_).
kingCanEscapeByDirection(Board,X-Y,Xf-Yf,verticalup):- Y1 is Y-1, isEmpty(X-Y1,Board), kingCanEscapeByDirection(Board,X-Y1,Xf-Yf,verticalup) .


kingCanEscapeByDirection(Board,X-Y,X-Y,verticaldown):- isEmpty(X-Y,Board), kingCanEscape(Board,X-Y,_-_).
kingCanEscapeByDirection(Board,X-Y,Xf-Yf,verticaldown):- Y1 is Y+1, isEmpty(X-Y1,Board), kingCanEscapeByDirection(Board,X-Y1,Xf-Yf,verticaldown) .



