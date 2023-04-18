//
// Created by piotr on 11.04.2023.
//
// 5.2.1 Triangularyzacja, wyznacznik i rozwiazanie Ax=b dla  macierzy kwadratowej.
// Wersja rozszerzona: Wybor wiersza z maksymalna waroscia |elementu glownego|
// Przy wymianie wierszy nalezy zastosowac wetor permutacji indeksow wierszy.
// Jezeli maksymalna wartosc |elementu glownego| < eps, to wyznacznik = 0.
// Zwraca wyznacznik det. Jezeli =0,  to triangularyzacja moze byc niedokonczona.
// Jezeli wyznacznik != 0, i b,x != NULL,
// to w wektorze x umieszcza rozwiazanie ukladu rownan Ax=b.

double gauss(double A[][SIZE], const double b[], double x[], size_t n, double eps) {
    int I[n];
    double R[n];
    for (int i=0; i<n; ++i) {
        I[i] = i;
        R[i] = b[i];
    }
    int a = 0;
    int c = 0;
    int p;
    int piwot;
    int liczba_zamian = 0;
    double scalar;
    while (a < n && c < n) {
        piwot = a;
        for (int i=a+1; i<n; ++i) {
            if (abs(A[I[i]][c]) > abs(A[I[piwot]][c])) {
                piwot = i;
            }
        }

        if (A[I[piwot]][c] == 0) {
            c = c+1;
        }

        else {
            if (piwot != a) {
                p = I[a];
                I[a] = piwot;
                I[piwot] = p;
                liczba_zamian = liczba_zamian + 1;
            }

            for (int i=a+1; i<n; ++i) {
                scalar = A[I[i]][c]/A[I[a]][c];
                A[I[i]][c] = 0;
                for (int j=c+1; j<n; ++j) {
                    A[I[i]][j] = A[I[i]][j] - (scalar*A[I[a]][j]);
                }

                R[I[i]] = R[I[i]] - (scalar*R[I[a]]);
            }
        }

        a = a+1;
        c = c+1;
    }

    double wyznacznik;
    if (liczba_zamian%2 == 1) {
        wyznacznik = -1;
    }
    else {
        wyznacznik = 1;
    }
    for (int i=0; i<n; ++i) {
        //if (A[I[i]][i] < eps) {
        //wyznacznik = 0;
        //break;
        //}
        wyznacznik = wyznacznik * A[I[i]][i];
    }

    int z = n-1;
    int y = 0;
    double skladnik;
    double suma;
    while (z >= 0) {
        skladnik = 0;
        for (int i=0; i<y; ++i) {
            skladnik = skladnik + (A[I[z]][n-1-i])*x[n-1-i];
        }
        suma = R[I[z]] - skladnik;
        x[n-1-y] = suma/A[I[z]][z];
        z = z-1;
        y = y+1;
    }

    return wyznacznik;
}

// 5.2.2
// Zwraca wyznacznik i w tablicy B macierz odwrotna (jezlei wyznacznik != 0)
// Jezeli maksymalna bezwzgledna wartosc elementu glownego < eps,
// to funkcja zwraca wartosc wyznacznika det = 0.
// Funkcja zmienia wartosci takze w tablicy A.

double matrix_inv(double A[][SIZE], double B[][SIZE], size_t n, double eps) {
    double C[n][n];
    for (int i=0; i<n; ++i) {
        for (int j=0; j<n; ++j) {
            if (i==j) {
                C[i][j] = 1;
            }
            else {
                C[i][j] = 0;
            }
        }
    }

    int I[n];
    for (int i=0; i<n; ++i) {
        I[i] = i;
    }
    int a = 0;
    int liczba_zamian = 0;
    int c = 0;
    int p;
    int piwot;
    double scalar;
    while (a < n && c < n) {
        piwot = a;
        for (int i=a+1; i<n; ++i) {
            if (abs(A[I[i]][c]) > abs(A[I[piwot]][c])) {
                piwot = i;
            }
        }

        if (A[I[piwot]][c] == 0) {
            c = c+1;
        }

        else {
            if (piwot != a) {
                p = I[a];
                I[a] = piwot;
                I[piwot] = p;
                liczba_zamian = liczba_zamian+1;
            }

            for (int i=a+1; i<n; ++i) {
                scalar = A[I[i]][c]/A[I[a]][c];
                A[I[i]][c] = 0;
                for (int j=c+1; j<n; ++j) {
                    A[I[i]][j] = A[I[i]][j] - (scalar*A[I[a]][j]);
                }
                for (int j=0; j<n; ++j) {
                    C[I[i]][j] = C[I[i]][j] - (scalar*C[I[a]][j]);
                }
            }
        }

        a = a+1;
        c = c+1;
    }

    a = n-1;
    c = n-1;

    while (a >= 0 & c >= 0) {
        piwot = a;
        for (int i=a-1; i>=0; i=i-1) {
            if (abs(A[I[i]][c]) > abs(A[I[piwot]][c])) {
                piwot = i;
            }
        }

        if (A[I[piwot]][c] == 0) {
            c = c-1;
        }

        else {
            if (piwot != a) {
                p = I[a];
                I[a] = I[piwot];
                I[piwot] = p;
                liczba_zamian = liczba_zamian+1;
            }

            for (int i=a-1; i>=0; i=i-1) {
                scalar = A[I[i]][c]/A[I[a]][c];
                A[I[i]][c] = 0;
                for (int j=c-1; j>=0; j=j-1) {
                    A[I[i]][j] = A[I[i]][j] - (scalar*A[I[a]][j]);
                }
                for (int j=0; j<n; ++j) {
                    C[I[i]][j] = C[I[i]][j] - (scalar*C[I[a]][j]);
                }
            }
        }

        a = a-1;
        c = c-1;
    }

    double wyznacznik;
    if (liczba_zamian%2 == 1) {
        wyznacznik = -1;
    }
    else {
        wyznacznik = 1;
    }

    for (int i=0; i<n; ++i) {
        scalar = 1/A[I[i]][i];
        wyznacznik = wyznacznik*A[I[i]][i];

        for (int j=0; j<n; ++j) {
            C[I[i]][j] = C[I[i]][j]*scalar;
        }
    }

    for (int i=0; i<n; ++i) {
        for (int j=0; j<n; ++j) {
            B[i][j] = C[I[i]][j];
        }
    }

    return wyznacznik;
}