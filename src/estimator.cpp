#include <bits/stdc++.h>
using namespace std;

struct simulation {
    double totCollisions, totEmpty, totSlots;
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

    result.totCollisions = (double)totCollisions;
    result.totSlots = (double)totSlots;
    result.totEmpty = (double)totEmpty;
    result.timeUsed = ((double) (end - start)) / CLOCKS_PER_SEC;

    return result;
}

void run() {

    vector<simulation> s[3];
    vector<int> axis;

    for(int tags = t_init; tags <= t_max; tags+=t_inc) {
        for(int est = 1; est <= 3; est++) {
            t_cur = tags;
            f_cur = f_init;
            double avgCollisions = 0.0, avgSlots = 0.0, avgEmpty = 0.0, avgTime = 0.0;

            for(int i = 0; i < rep; i++) {
                t_cur = tags;
                f_cur = f_init;
                simulation tmp = dfsa(est);
                avgCollisions += tmp.totCollisions;
                avgSlots += tmp.totSlots;
                avgEmpty += tmp.totEmpty;
                avgTime += tmp.timeUsed;
            }

            avgCollisions/=(double)rep, avgSlots/=(double)rep;
            avgEmpty/=(double)rep, avgTime/=(double)rep;

            //printf("numero de etiquetas: %d\nSlots: %lf \nTempo: %lf \nVazios: %lf \nColisão: %lf\n\n", tags, avgSlots, avgTime, avgEmpty, avgCollisions);
            simulation tmp;
            tmp.totCollisions = avgCollisions;
            tmp.totSlots = avgSlots;
            tmp.totEmpty = avgEmpty;
            tmp.timeUsed = avgTime;

            if (est == 1) s[0].push_back(tmp);
            else if (est == 2) s[1].push_back(tmp);
            else if (est == 3) s[2].push_back(tmp);
        }
        axis.push_back(tags);
    }

    for(int i = 0; i < axis.size(); i++)
        printf("%d ", axis[i]);
    puts("");
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < s[i].size(); j++)
            printf("%.2lf ", s[i][j].totSlots);
        puts("");
        for(int j = 0; j < s[i].size(); j++)
            printf("%.2lf ", s[i][j].timeUsed);
        puts("");
        for(int j = 0; j < s[i].size(); j++)
            printf("%.2lf ", s[i][j].totEmpty);
        puts("");
        for(int j = 0; j < s[i].size(); j++)
            printf("%.2lf ", s[i][j].totCollisions);
        puts("");
    }

}

int main() {
    srand(time(NULL)); //seed for rand

    run();

    int cmd;
    cmd = 1; //slots
    // cmd = 2; //tempo
    // cmd = 3; //vazios
    // cmd = 4; //colisões
    printf("%d\n", cmd);

    return 0;
}
