#include <bits/stdc++.h>
using namespace std;

struct simulation {
    int totCollisions, totEmpty, totSlots;
    double timeUsed;
};

//params:
#define LB 1 //lower_bound opt
#define EL 2 //eom-lee opt

const int t_init = 500; //tags iniciais
const int t_inc = 100; //incremento do numero de tags
const int t_max = 1000; //maximo de tags

const int f_init = 64; //frame size inicial
const int rep = 2000; //simulações

const double EPS = 1e-3;

int t_cur;
int f_cur;

int lowerbound(int collisions) {
    return 2*collisions;
}

// int schoute(int collisions) {
//     return ((int)round(2.39*collisions));
// }

int eomlee(int collisions, int success, int fsize) {
    long double gama0, gama1, tmp_exp, beta;

    beta = (long double) DBL_MAX; //infinito
    gama1 = 2.0;

    do {
        gama0 = gama1;
        beta = fsize / (gama0*collisions + success);
        tmp_exp = exp(-1.0/beta);
        gama1 = (1.0-tmp_exp) / (beta*(1.0 - ((1.0 + (1.0/beta)) * tmp_exp)));
    } while(abs(gama0-gama1) >= EPS);

    return (int)(gama1*collisions);
}

simulation dfsa(int opt) {

    int totCollisions = 0, totEmpty = 0, totSlots = f_cur;
    simulation result;
    clock_t start, end;

    start = clock();
    while(t_cur > 0) {
        int frame[f_cur];
        memset(frame, 0, sizeof(frame));

        int success = 0, collisions = 0, empty = 0;

        for(int i = 0; i < t_cur; i++) {
            int idx = (rand() % f_cur);
            frame[idx] += 1;
        }

        for(int i = 0; i < f_cur; i++) {
            collisions += (frame[i] >= 2);
            success += (frame[i]==1);
            empty += (frame[i]==0);
        }

        t_cur -= success;
        if (opt == LB) f_cur = lowerbound(collisions);
        else if (opt == EL) f_cur = eomlee(collisions, success, f_cur);

        totCollisions += collisions;
        totSlots += f_cur;
        totEmpty += empty;
    }
    end = clock();

    result.totCollisions = totCollisions;
    result.totSlots = totSlots;
    result.totEmpty = totEmpty;
    result.timeUsed = ((double) (end - start)) / CLOCKS_PER_SEC;

    return result;
}


int main() {

    std::srand(std::time(0)); //seed for rand

    t_cur = t_init;
    f_cur = f_init;

    double avgCollisions = 0.0, avgSlots = 0.0, avgEmpty = 0.0, avgTime = 0.0;
    for(int i = 0; i < rep; i++) {
        t_cur = t_init;
        f_cur = f_init;
        simulation tmp = dfsa(LB);
        //simulation tmp = dfsa(EL);
        avgCollisions += (double)tmp.totCollisions;
        avgSlots += (double)tmp.totSlots;
        avgEmpty += (double)tmp.totEmpty;
        avgTime += (double)tmp.timeUsed;
    }

    avgCollisions/=(double)rep, avgSlots/=(double)rep;
    avgEmpty/=(double)rep, avgTime/=(double)rep;

    printf("numero de etiquetas: %d\nSlots: %lf \nTempo: %lf \nVazios: %lf \nColisão: %lf\n\n", t_init, avgSlots, avgTime, avgEmpty, avgCollisions);

    return 0;
}
