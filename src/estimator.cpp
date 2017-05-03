#include <bits/stdc++.h>
using namespace std;

struct simulation {
    int totCollisions, totEmpty, totSlots;
    double timeUsed;
};

//params:
#define LB 1 //lower_bound opt
#define EL 2 //eom-lee opt

const int t_init = 100; //tags iniciais
const int t_inc = 100; //incremento do numero de tags
const int t_max = 1000; //maximo de tags

const int f_init = 64; //frame size inicial
const int rep = 2000; //simulações

const double EPS = 1e-4;

int t_cur;
int f_cur;

int lowerbound(int collisions) {
    return 2*collisions;
}

int schoute(int collisions) {
    return ((int)round(2.39*collisions));
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
            empty += (frame[i]==0);
            success += (frame[i]==1);
            collisions += (frame[i] >= 2);
        }

        t_cur -= success;
        f_cur = lowerbound(collisions);

        totEmpty += empty;
        totCollisions += collisions;
        totSlots += f_cur;
    }
    end = clock();

    result.totSlots = totSlots;
    result.totCollisions = totCollisions;
    result.totEmpty = totEmpty;
    result.timeUsed = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("%d\n", totSlots);
    printf("%d\n", totEmpty);
    printf("%d\n", totCollisions);
    printf("%lf\n", result.timeUsed);

    return result;
}

int main() {

    std::srand(std::time(0)); //seed for rand

    t_cur = t_init;
    f_cur = f_init;

    dfsa(1);

    return 0;
}
