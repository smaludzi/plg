## Simple Prolog

Implementation of Prolog ProL language and WIM virtual machine. Based on W. Reinhard and H. Seidl "Compiler Design, Virtual Machines". Please let me know what you think.

### Ex 1. Towers of Hanoi

    move(N,X,Y,Z) <= N = 1, Z = _, write(move), write(X), write(to), write(Y), nl
    move(N,X,Y,Z) <= N > 1,
                    M is N - 1,
                    move(M,X,Z,Y),
                    move(1,X,Y,_),
                    move(M,Z,Y,X)
        <= move(3,source,target,auxiliary)

    move source to target
    move source to auxiliary
    move target to auxiliary
    move source to target
    move auxiliary to source
    move auxiliary to target
    move source to target

### Ex 2. List concatenation

    list_concat(L1, L2, L3) <= L1 = [], L2 = L3
    list_concat(L1, L2, L3) <= L1 = [H1|T1], L3 = [H1|T3], list_concat(T1,L2,T3)
        <= list_concat([a, b, c], [d, e, f], Result)

    [a, b, c, d, e, f]
    -----------------
    no

### Ex 3. Facts Database

    parent(X, Y) <= X = slawek, Y = kuba
    parent(X, Y) <= X = slawek, Y = maks
    parent(X, Y) <= X = boguslaw, Y = slawek
    parent(X, Y) <= X = boguslaw, Y = beata
    parent(X, Y) <= X = elzbieta, Y = slawek
    parent(X, Y) <= X = elzbieta, Y = beata

    grandparent(X,Y) <= parent(X, Z),
                        parent(Z, Y)

    <= grandparent(X, Y)

    boguslaw
    kuba
    -----------------
    boguslaw
    maks
    -----------------
    elzbieta
    kuba
    -----------------
    elzbieta
    maks
    -----------------
    no




