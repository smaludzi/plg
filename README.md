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

### Ex 2. List Concatenation

    list_concat(L1, L2, L3) <= L1 = [], L2 = L3
    list_concat(L1, L2, L3) <= L1 = [H1|T1], L3 = [H1|T3], list_concat(T1,L2,T3)
        <= list_concat([a, b, c], [d, e, f], Result)

    [a, b, c, d, e, f]
    -----------------
    no

### Ex 3. List Permutations

    list_delete(X, P1, L1) <= P1 = [X|L1]
    list_delete(X, P1, P2) <= P1 = [Y|L2], P2 = [Y|L1], list_delete(X, L2, L1)

    list_perm(L, P2) <= L = [], P2 = []
    list_perm(L, P2) <= P2 = [X|P], list_delete(X, L, L1), list_perm(L1, P)

        <= list_perm([a, b, c], X)

    [a, b, c]
    -----------------
    [a, c, b]
    -----------------
    [b, a, c]
    -----------------
    [b, c, a]
    -----------------
    [c, a, b]
    -----------------
    [c, b, a]
    -----------------
    no

### Ex 4. Cut Operator

Without cut.

    f(X) <= X = a
    f(X) <= X = b
    f(X) <= X = c
        <= f(X), f(Y)

    a a
    -----------------
    a b
    -----------------
    a c
    -----------------
    b a
    -----------------
    b b
    -----------------
    b c
    -----------------
    c a
    -----------------
    c b
    -----------------
    c c
    -----------------
    no

With cut.

    f(X) <= X = a
    f(X) <= X = b
    f(X) <= X = c
     <= f(X), !, f(Y)

    a a
    -----------------
    a b
    -----------------
    a c
    -----------------
    no

### Ex 5. Facts Database

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
