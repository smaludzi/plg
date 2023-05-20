Simple Prolog

    Ex 1.

    list_concat(L1, L2, L3) <= L1 = [], L2 = L3
    list_concat(L1, L2, L3) <= L1 = [H1|T1], L3 = [H1|T3], list_concat(T1,L2,T3)
        <= list_concat([a, b, c], [d, e, f], Result)

    [a, b, c, d, e, f]
    -----------------
    no

    Ex 2.

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




