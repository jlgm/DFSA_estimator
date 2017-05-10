#include <bits/stdc++.h>
using namespace std;

struct simulation {
    double totCollisions, totEmpty, totSlots;
    double timeUsed;
};

//params:
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

long double fact(long double n) {
    if (n == 1) return 1;
    return n*fact(n-1);
}

long double simple_factorial(long double a, long double b, long double c, long double d) {
    long double result = 1.0;
    while (a > 1) {
        result = result * a;
        a = a-1;
        if (b > 1){
            result = result/b;
            b = b - 1;
        }

        if (c > 1){
            result = result/c;
            c = c - 1;
        }

        if (d > 1){
            result = result/d;
            d = d - 1;
        }
    }
    return result;
}

long double comb(long double n, long double r, long double p) {
    //n! / ( n - r )!  =  n * ( n - 1) * .. * (n - r + 1 )
    long double ret = 1;
    for (long double i = 0; i < r; i++) {
        ret = (ret*p*(n-i))/(i+1);
    }
    return ret;
}

int chen(int empty, int success, int collisions) {

    long double L = success + empty + collisions;
    long double n = success + 2.0*collisions;

    long double next = 0;
    long double previous = -1;


    while (previous < next) {
        previous = next;
        long double pe = pow((1.0 - (1.0/L)), n);
        long double ps = (n/L)*pow((1-(1/L)), (n-1));
        long double pc = 1-pe-ps;
        next = (simple_factorial(L,empty,success,collisions))*pow(pe,empty)*pow(ps,success)*pow(pc,collisions);
        // next = comb(L,empty,pe)*(comb(success+collisions,success,ps)*pow(pc,collisions));

        // printf("%Lf \n%Lf \n%Lf \n%Lf \n%Lf \n%Lf\n", L, previous, next, pe, ps, pc);
        // int lixo; scanf("%d", &lixo);

        n = n+1;
    }
    return (int)round(n-2);
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
        else if (opt == 3) f_cur = schoute(collisions);
        else if (opt == 4) f_cur = chen(empty, success, collisions);

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

simulation Q() {
    double Q = 4.0, Qfp = Q, prevQ = Q, cc = 0.21183, ci = 0.15;
    //cc = ci = 0.2; //se nao for o fast!

    int totCollisions = 0, totEmpty = 0, totSlots = 0;
    int cur = 0;

    clock_t start, end;

    start = clock();
    while(t_cur > 0) {
        f_cur = (int)pow(2.0,Q);

        int success = 0, collisions = 0, empty = 0;

        for(int i = 0; i < t_cur; i++) {
            int idx = (rand() % f_cur);
            success += (idx == 0);
            if (success == 2) break;
        }

        if (success == 1) {
            t_cur--;
        }
        else if (success > 1) {
            Qfp = min(15.0, Qfp+cc);
            totCollisions++;
        }
        else if (success == 0) {
            Qfp = max(0.0, Qfp-ci);
            totEmpty++;
        }
        Q = round(Qfp);
        totSlots++;
    }
    end = clock();

    simulation result;
    result.totSlots = totSlots;
    result.totCollisions = totCollisions;
    result.totEmpty = totEmpty;
    result.timeUsed = ((double) (end - start)) / CLOCKS_PER_SEC;

    return result;
}

void run() {

    vector<simulation> s[4];
    vector<int> axis;

    int amt = 4; //colocar pra 3 se nao for usar Q

    for(int tags = t_init; tags <= t_max; tags+=t_inc) {
        for(int est = 1; est <= amt; est++) {
            t_cur = tags;
            f_cur = f_init;
            double avgCollisions = 0.0, avgSlots = 0.0, avgEmpty = 0.0, avgTime = 0.0;

            for(int i = 0; i < rep; i++) {
                t_cur = tags;
                f_cur = f_init;
                // simulation tmp = (est < 4) ? dfsa(est) : Q();
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

            s[est-1].push_back(tmp);
        }

        axis.push_back(tags);
    }

    for(int i = 0; i < axis.size(); i++)
        printf("%d ", axis[i]);
    puts("");
    for(int i = 0; i < amt; i++) {
        for(int j = 0; j < s[i].size(); j++)
            printf("%.2lf ", s[i][j].totSlots);
        puts("");
        for(int j = 0; j < s[i].size(); j++)
            printf("%lf ", s[i][j].timeUsed);
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

    // t_cur = t_init;
    // simulation test = Q();
    //
    // printf("%lf\n", test.totSlots);
    // printf("%lf\n", test.totCollisions);

    int cmd;
    // cmd = 1; //slots
    cmd = 2; //tempo
    // cmd = 3; //vazios
    // cmd = 4; //colisões
    printf("%d\n", cmd);

    return 0;
}
