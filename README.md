Simple Prolog


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

