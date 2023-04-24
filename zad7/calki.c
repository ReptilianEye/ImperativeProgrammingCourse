//
// Created by piotr on 24.04.2023.
//
#include <stdio.h>
#include <math.h>

#define TEST 1   // 1 - dla testowania, 0 - dla sprawdzarki

#define RECURS_LEVEL_MAX  10
#define N_MAX             10

/////////////////////////////////////////////////////////////////////
// 7.1 Calki jednokrotne

// Przyklady podcalkowych funkcji jednej zmiennej

typedef double (*Func1vFp)(double);  // Definiowanie nazwy dla typu wskaznika do funkcji jedej zmiennej

double f_poly(double x) {  //    polynomial  a[0]+a[1]x+ ... + a[n]x^n
    double result = 2 * pow(x, 5) - 4 * pow(x, 4) + 3.5 * pow(x, 2) + 1.35 * x - 6.25;
    return result;
}

double
f_rat(double x) {  // Przyklad funkcji podcalkowej Proba z funkcja f(x) = 1/((x-0.5)*(x-0.5)+0.01) w przedziale od 0 do 3.
    double result = 1 / (pow((x - 0.5), 2) + 0.01);
    return result;
}

double f_exp(double x) {  // Przyklad funkcji podcalkowej
    double result = 2 * x * exp(-1.5 * x) - 1;
    return result;
}

double f_trig(double x) {  // Przyklad funkcji podcalkowej
    double result = x * tan(x) - 1;
    return result;
}

// Obliczanie kwadratur złożonych dla funkcji jednej zmiennej

double quad_rect_left(Func1vFp f1, double a, double b, int n) {  // Prostokatow leftpoint
    double result;
    double h = (b - a) / n;
    double sum = 0;
    for (int i = 0; i < n; i++) {
        sum = sum + f1(a + i * h);
    }
    result = sum * h;
    return result;

}

double quad_rect_right(Func1vFp f1, double a, double b, int n) {  // Prostokatow rightpoint
    double result;
    double h = (b - a) / n;
    double sum = 0;
    for (int i = 0; i < n; i++) {
        sum = sum + f1(b - i * h);
    }
    result = sum * h;
    return result;
}

double quad_rect_mid(Func1vFp f1, double a, double b, int n) {  // Prostokatow midpoint
    double result;
    double h = (b - a) / n;
    double sum = 0;
    for (int i = 0; i < n / 2 + 1; i++) {
        sum = sum + f1((a + b) / 2 - i * h);
        if (i > 0)
            sum = sum + f1((a + b) / 2 + i * h);
    }
    result = sum * h;
    return result;
}

double quad_trap(Func1vFp func, double a, double b, int n) {  // Trapezow
    double result = (b - a) / 2 * (func(a) + func(b));
    return result;
}

double quad_simpson(Func1vFp f, double a, double b, int n) {
    double h = (b - a) / n;

    // Array for storing value of x and f(x)
    double x[n], fx[n];

    // Calculating values of x and f(x)
    for (int i = 1; i <= n; i++) {
        x[i] = a + i * h;
        fx[i] = f(x[i]);
    }

    // Calculating result
    double res = 0;
    for (int i = 1; i <= n; i++) {
        if (i == 0 || i == n)
            res += fx[i];
        else if (i % 2 != 0)
            res += 4 * fx[i];
        else
            res += 2 * fx[i];
    }
    res = res * (h / 3);
    return res;
}

// Definiowanie nazwy dla typu wskaznika do funkcji obliczającej kwadraturę funkcji jednej zmiennej
typedef double (*QuadratureFp)(Func1vFp, double a, double b, int n);

// Definicja i inicjowanie tablicy wskaznikow do funkcji jednej zmiennej, w kolejności: f_poly, f_rat, f_trig, f_exp.
Func1vFp func_tab[4] = {
        f_poly, f_rat, f_trig, f_exp};
// Definicja i inicjowanie tablicy wskaznikow do kwadratur dla funkcji jednej zmiennej, w kolejności:
// quad_rect_left, quad_rect_right, quad_rect_mid, quad_trap, quad_simpson.
QuadratureFp quad_tab[5] = {
        quad_rect_left, quad_rect_right, quad_rect_mid, quad_trap, quad_simpson};

// Oblicza i zwraca wartosc wskazanej indeksem quad_no kwadratury dla wskazanej indeksem fun_no funkcji
// w przedziale [a,b] z podzialem na n podprzedzialow.
double quad_select(int fun_no, int quad_no, double a, double b, int n) {
    double result = quad_tab[quad_no](func_tab[fun_no], a, b, n);
    return result;
}

// Algorytm adaptacyjny obliczania kwadratury,
double recurs(Func1vFp f, double a, double b, double S, double delta, QuadratureFp quad, int level) {
    int n = 1;
    double left_s = quad(f, a, (a + b) / 2, n); //jakos obliczyc prosta;
    double right_s = quad(f, a, (a + b) / 2, n);
    if (fabs(left_s + right_s - S) <= delta)
        return left_s + right_s;
    else if (level < RECURS_LEVEL_MAX)
        return recurs(f, a, (a + b) / 2, left_s, delta, quad, level + 1) +
               recurs(f, (a + b) / 2, b, left_s, delta, quad, level + 1);
    else return NAN;
}

// Funkcja inicjująca rekurencję
double init_recurs(Func1vFp f, double a, double b, double delta, QuadratureFp quad) {
    double S = quad(f, a, b, 1);
    double result = recurs(f, a, b, S, delta, quad, 0);
    return result;
}

///////////////////////////////////////////////////////////////
// 7.2 Calki dwukrotne:

typedef double Func2vFp(double, double);  // Definiowanie nazwy dla typu wskaznika do funkcji dwoch zmiennych

// Przykladowe funkcje dwoch zmiennych:
double func2v_1(double x, double y) {  //fxy1
    return sin(x / y);
}

double func2v_2(double x, double y) {
    return 2 - x * x - y * y * y;
}

// Przykladowe funkcje brzegu obszaru calkowania (do calki podwojnej nad obszarem normalnym)
double lower_bound1(double x) {
    return sin(x);
}

double upper_bound1(double x) {
    return x * x + 1;
}

double lower_bound2(double x) {
    return 0.7 * exp(-2 * x * x);
}

double upper_bound2(double x) {
    return sin(10 * x);
}

// Metoda prostokatow (leftpoint) oblicza przyblizenie calki podwojnej nad obszarem prostokatnym
double dbl_integr(Func2vFp f, double x1, double x2, int nx, double y1, double y2, int ny) {

    double sum = 0;
    double hx = (x2 - x1) / nx;
    double hy = (y2 - y1) / ny;
    for (int i = 0; i < ny; i++) {

        double fxsum = 0;
        double y = y1 + i * hy;
        for (int j = 0; j < nx; j++) {
            fxsum = fxsum + f(x1 + j * hx, y);
        }
        sum = sum + fxsum * hx;
    }
    return sum*hy;
}

// Oblicza kwadrature prostokatow midpoint dla calki podwojnej nad obszarem normalnym wzgledem osi 0x
double dbl_integr_normal_1(Func2vFp f, double x1, double x2, int nx, double hy, Func1vFp fg, Func1vFp fh) {

}

// Oblicza kwadrature prostokatow leftpoint dla calki podwojnej nad obszarami normalnymi wzgledem osi 0x
double
dbl_integr_normal_n(Func2vFp f, double x1, double x2, int nx, double y1, double y2, int ny, Func1vFp fg, Func1vFp fh) {
}

///////////////////////////////////////////////////////////////
// 7.3 Calki wielokrotne:

typedef double (*FuncNvFp)(const double *, int);

typedef int (*BoundNvFp)(const double *, int);

// Przykladowa funkcja trzech zmiennych:
double func3v(const double v[], int n) {
    return v[0] - v[1] + 2 * v[2];
}

// Przykładowy predykat w przestrzeni 3-wymiarowej
int bound3v(const double v[], int n) {
    if (v[0] > 0 && v[0] < 0.5 && v[1] * v[1] + (v[2] - 1) * (v[2] - 1) < 1) return 1;   // walec
    return 0;
}

// Przykladowa funkcja n zmiennych:
double funcNv(const double v[], int n) {
    double fv = 1.;
    for (int i = 1; i < n; ++i) fv += sin(i * v[i]);
    return fv;
}

// Przykładowy predykat w przestrzeni n-wymiarowej
int boundNv(const double v[], int n) {
    double r = 0.0;
    for (int i = 0; i < n; ++i) r += (v[i] - 1) * (v[i] - 1);  // hiperkula n-wymiarowa
    if (r > 1.) return 0;
    return 1;
}

// Obliczanie calek wielokrotnych

// Oblicza calke potrojna "nad" prostopadloscianem z predykatem wykluczajacym jego czesci (jezeli boundary != NULL).
// Metoda prostokatow wstecz (rightpoint) wzdluz kazdej zmiennej.
double trpl_quad_rect(FuncNvFp f, const double variable_lim[][2], const int tn[], BoundNvFp boundary) {
}

// Oblicza calke wielokrotna (funkcji n zmiennych) "nad" n wymiarowym hiperprostopadloscianem z predykatem wykluczajacym jego czesci (jezeli boundary != NULL).
// Metoda prostokatow midpoint wzdluz kazdej zmiennej.
void recur_quad_rect_mid(double *psum, FuncNvFp f, int variable_no, double tvariable[], const double variable_lim[][2],
                         const int tn[], int level, BoundNvFp boundary) {}

int main(void) {
    setbuf(stdout, 0);

    int to_do, n, nx, ny, integrand_fun_no, method_no, flag;
    double a, b, x1, x2, y1, y2, hy, sum, delta;
    double tvariable[N_MAX], variable_lim[N_MAX][2];
    int tn[N_MAX];

    if (TEST) printf("Wpisz numer testu [1, 7]: ");
//    scanf("%d", &to_do);
    to_do = 3;
    switch (to_do) {
        case 1: // 7.1.1 wybor funkcji i metody
            if (TEST) printf("Wpisz przedzial calkowania i liczbe podprzedzialow: ");
            a = 0;
            b = 0.75;
            n = 25;
//            scanf("%lf %lf %d", &a, &b, &n);
            for (int q = 0; q < 5; ++q) {
                for (int f = 0; f < 4; ++f) {
                    printf("%.5f ", quad_select(f, q, a, b, n));
                }
                printf("\n");
            }
            break;
        case 2: // 7.1.2 rekurencyjny algorytm adaptacyjny
            if (TEST) printf("Nr funkcji (0-3) i metody (0-4): ");
//            scanf("%d %d", &integrand_fun_no, &method_no);
            integrand_fun_no = 1;
            method_no = 4;
            if (TEST) printf("Wpisz przedzial calkowania i tolerancje bledu: ");
            a = 0, b = 3, delta = 0.01;
//            scanf("%lf %lf %lf", &a, &b, &delta);
            printf("%.5f\n", init_recurs(func_tab[integrand_fun_no], a, b, delta, quad_tab[method_no]));
            break;
        case 3: // 7.2.1 calka podwojna nad prostokatem
            if (TEST) printf("Wpisz przedzial calkowania i liczbe podprzedzialow wzdluz x: ");
            x1 = 0, x2 = 1, nx = 100, y1 = 0, y2 = 1, ny = 100;
//            scanf("%lf %lf %d", &x1, &x2, &nx);
            if (TEST) printf("Wpisz przedzial calkowania i liczbe podprzedzialow wzdluz y: ");
//            scanf("%lf %lf %d", &y1, &y2, &ny);
            printf("%.5f\n", dbl_integr(func2v_2, x1, x2, nx, y1, y2, ny));
            break;
        case 4: // 7.2.2 calka podwojna nad obszarem normalnym
            if (TEST) printf("Wpisz przedzial calkowania i liczbe podprzedzialow zmiennej x: ");
            scanf("%lf %lf %d", &x1, &x2, &nx);
            if (TEST) printf("Wpisz dlugosc podprzedzialu wzdluz y: ");
            scanf("%lf", &hy);
            printf("%.5f\n", dbl_integr_normal_1(func2v_2, x1, x2, nx, hy, lower_bound2, upper_bound2));
            break;
        case 5: // 7.2.3 calka podwojna nad wieloma obszarami normalnymi
            if (TEST) printf("Wpisz przedzial calkowania i liczbe podprzedzialow wzdluz x: ");
            scanf("%lf %lf %d", &x1, &x2, &nx);
            if (TEST) printf("Wpisz przedzial calkowania i liczbe podprzedzialow wzdluz y: ");
            scanf("%lf %lf %d", &y1, &y2, &ny);
            printf("%.5f\n", dbl_integr_normal_n(func2v_2, x1, x2, nx, y1, y2, ny, lower_bound2, upper_bound2));
            break;
        case 6: // 7.3.1 calka potrojna po prostopadloscianie
            if (TEST) printf("Wpisz przedzial calkowania i liczbe podprzedzialow 1. zmiennej: ");
            scanf("%lf %lf %d", &variable_lim[0][0], &variable_lim[0][1], tn);
            if (TEST) printf("Wpisz przedzial calkowania i liczbe podprzedzialow 2. zmiennej: ");
            scanf("%lf %lf %d", &variable_lim[1][0], &variable_lim[1][1], tn + 1);
            if (TEST) printf("Wpisz przedzial calkowania i liczbe podprzedzialow 3. zmiennej: ");
            scanf("%lf %lf %d", &variable_lim[2][0], &variable_lim[2][1], tn + 2);
            if (TEST)
                printf("Wpisz 1 gdy ograniczenie obszaru całkowania ma byc aktywne; 0 - w przeciwnym przypadku: ");
            scanf("%d", &flag);
            printf("%.5f\n", trpl_quad_rect(func3v, variable_lim, tn, flag ? bound3v : NULL));
            break;
        case 7: // 7.3.3 calka n-wymiarowa na hiperprostopadloscianie
            if (TEST) printf("Wpisz liczbe zmiennych <= %d: ", N_MAX);
            scanf("%d", &n);
            if (n > N_MAX) break;
            for (int i = 0; i < n; ++i) {
                if (TEST) printf("Wpisz przedzial calkowania i liczbe podprzedzialow %d. zmiennej: ", i + 1);
                scanf("%lf %lf %d", &variable_lim[i][0], &variable_lim[i][1], tn + i);
            }
            if (TEST)
                printf("Wpisz 1 gdy ograniczenie obszaru całkowania ma byc aktywne; 0 - w przeciwnym przypadku: ");
            scanf("%d", &flag);
            sum = 0.;
            recur_quad_rect_mid(&sum, funcNv, n, tvariable, variable_lim, tn, 0, flag ? boundNv
                                                                                      : NULL);//        recur_quad_rect_mid(&sum, func3v_1, n, tvariable, variable_lim, tn, 0, flag?bound3v_2:NULL);
//        recur_quad_rect(&sum, funcNv, n, tvariable, variable_lim, tn, tqr, 0, flag?boundNv:NUL
            break;
        default:
            printf("Numer testu spoza zakresu [1, 7] %d", to_do);
    }
    return 0;
}
