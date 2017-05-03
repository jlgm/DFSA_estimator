#include <bits/stdc++.h>
using namespace std;

struct simulation {
    int totCollisions, totEmpty, totSlots;
    double timeUsed;
};

//params:
#define LB 1 //lower_bound opt
#define EL 2 //eom-lee opt
#define SC 3 //schoute

const int t_init = 100; //tags iniciais
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

int schoute(int collisions) {
    //just in case
    return ((int)round(2.39*collisions));
}

int eom_lee(int collisions, int success, int fsize) {
    double gama0, gama1, tmp_exp, beta;

    gama1 = 2.0;

    do {
        gama0 = gama1;
        beta = fsize / (gama0*collisions + success);
        tmp_exp = exp(-1.0/beta);
        gama1 = (1.0-tmp_exp) / (beta*(1.0 - ((1.0 + (1.0/beta)) * tmp_exp)));
    } while(abs(gama0-gama1) >= EPS);

    return ((int)round(gama1*collisions));
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
        if (opt == 1) f_cur = lowerbound(collisions);
        else if (opt == 2) f_cur = eom_lee(collisions, success, f_cur);
        else if (opt = 3) f_cur = schoute(collisions);

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

void run() {

    for(int tags = t_init; tags <= t_max; tags+=t_inc) {

        t_cur = tags;
        f_cur = f_init;
        double avgCollisions = 0.0, avgSlots = 0.0, avgEmpty = 0.0, avgTime = 0.0;

        for(int i = 0; i < rep; i++) {
            t_cur = tags;
            f_cur = f_init;
            simulation tmp = dfsa(EL);
            avgCollisions += (double)tmp.totCollisions;
            avgSlots += (double)tmp.totSlots;
            avgEmpty += (double)tmp.totEmpty;
            avgTime += (double)tmp.timeUsed;
        }

        avgCollisions/=(double)rep, avgSlots/=(double)rep;
        avgEmpty/=(double)rep, avgTime/=(double)rep;

        printf("numero de etiquetas: %d\nSlots: %lf \nTempo: %lf \nVazios: %lf \nColisão: %lf\n\n", tags, avgSlots, avgTime, avgEmpty, avgCollisions);
    }

}

int main() {
    srand(time(NULL)); //seed for rand
    run();

    return 0;
}
