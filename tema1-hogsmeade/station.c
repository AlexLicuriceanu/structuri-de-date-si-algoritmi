#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "station.h"

int checkStation(TrainStation *station, int platform) {

    if (platform < 0 || platform > station->platforms_no-1) return 0;
    if (!station->platforms || !station->platforms[platform]) return 0;
    return 1;
}

/* Creeaza o gara cu un numar fix de peroane.
 * 
 * platforms_no: numarul de peroane ale garii
 *
 * return: gara creata
 */
TrainStation* open_train_station(int platforms_no) {
    TrainStation *newStation = malloc(sizeof(TrainStation));
    if (newStation == NULL) {
        printf("Memory allocation error.\n");
        exit(1);
    }

    newStation->platforms_no = platforms_no;
    newStation->platforms = malloc(sizeof(Train*) * platforms_no);

    for(int i=0; i<platforms_no; i++) {
        newStation->platforms[i] = malloc(sizeof(Train));
        newStation->platforms[i]->locomotive_power = -1;
        newStation->platforms[i]->train_cars = NULL;
    }

    return newStation;
}


/* Elibereaza memoria alocata pentru gara.
 *
 * station: gara existenta
 */
void close_train_station(TrainStation *station) {
    if (station != NULL) {
        for (int i=0; i<station->platforms_no; i++) {

            TrainCar *temp = NULL;
            TrainCar *remove = station->platforms[i]->train_cars;
            while(remove) {
                temp = remove->next;
                free(remove);
                remove = temp;
            }
            free(station->platforms[i]);
        }
        free(station);
    }
}


/* Afiseaza trenurile stationate in gara.
 *
 * station: gara existenta
 * f: fisierul in care se face afisarea
 */
void show_existing_trains(TrainStation *station, FILE *f) {
    if (station != NULL) {
        for(int i=0; i<station->platforms_no; i++) {
            fprintf(f, "%d: ", i);
            printf("%d: ", i);

            if (station->platforms[i]->locomotive_power != -1) {
                fprintf(f, "(%d)", station->platforms[i]->locomotive_power);
                printf("(%d)", station->platforms[i]->locomotive_power);
                TrainCar *temp = station->platforms[i]->train_cars;
                while(temp) {
                    fprintf(f, "-|%d|", temp->weight);
                    printf("-|%d|", temp->weight);
                    temp = temp->next;
                }
            }
            fprintf(f, "\n");
            printf("\n");
        }
    }
}


/* Adauga o locomotiva pe un peron.
 * 
 * station: gara existenta
 * platform: peronul pe care se adauga locomotiva
 * locomotive_power: puterea de tractiune a locomotivei
 */
void arrive_train(TrainStation *station, int platform, int locomotive_power) {
    if (checkStation(station, platform) && station->platforms[platform]->locomotive_power == -1) {
        Train *newTrain = malloc(sizeof(Train));
        newTrain->locomotive_power = locomotive_power;
        newTrain->train_cars = NULL;
        station->platforms[platform] = newTrain;
    }
}


/* Elibereaza un peron.
 * 
 * station: gara existenta
 * platform: peronul de pe care pleaca trenul
 */
void leave_train(TrainStation *station, int platform) {
    if (!checkStation(station, platform)) return;

    TrainCar *leaveCar = station->platforms[platform]->train_cars;
    TrainCar *temp = NULL;
    station->platforms[platform]->locomotive_power = -1;

    while(leaveCar) {
        temp = leaveCar->next;
        free(leaveCar);
        leaveCar = temp;
    }

    station->platforms[platform]->train_cars = NULL;
}


/* Adauga un vagon la capatul unui tren.
 * 
 * station: gara existenta
 * platform: peronul pe care se afla trenul
 * weight: greutatea vagonului adaugat
 */
void add_train_car(TrainStation *station, int platform, int weight) {
    if (!checkStation(station, platform) || station->platforms[platform]->locomotive_power == -1)
        return;

    TrainCar *newCar = malloc(sizeof(TrainCar));
    if (newCar == NULL) return;
    newCar->next = NULL;
    newCar->weight = weight;

    if (station->platforms[platform]->train_cars == NULL)
        station->platforms[platform]->train_cars = newCar;
    else {
        TrainCar *temp = station->platforms[platform]->train_cars;
        while(temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newCar;
    }
}


int getTrainLength(Train *train) {
    TrainCar *temp = train->train_cars;
    if (temp == NULL) return 0;

    int k = 0;
    while(temp != NULL) {
        k++;
        temp = temp->next;
    }
    return k;
}


/* Scoate vagoanele de o anumita greutate dintr-un tren.
 * 
 * station: gara existenta
 * platform: peronul pe care se afla trenul
 * weight: greutatea vagonului scos
 */
void remove_train_cars(TrainStation *station, int platform, int weight) {
    if(!checkStation(station, platform)) return;
    if(station->platforms[platform]->locomotive_power == -1) return;

    TrainCar *temp = station->platforms[platform]->train_cars;
    TrainCar *prev = NULL;
    if (temp == NULL) return;

    while(temp != NULL && temp->weight == weight) {
       station->platforms[platform]->train_cars = temp->next;
       free(temp);
       temp = station->platforms[platform]->train_cars;
    }

    while(temp) {
        while(temp && temp->weight != weight) {
            prev = temp;
            temp = temp->next;
        }

        if (temp == NULL) return;
        prev->next = temp->next;
        free(temp);

        temp = prev->next;
    }

}


/* Muta o secventa de vagoane dintr-un tren in altul.
 * 
 * station: gara existenta
 * platform_a: peronul pe care se afla trenul de unde se scot vagoanele
 * pos_a: pozitia primului vagon din secventa
 * cars_no: lungimea secventei
 * platform_b: peronul pe care se afla trenul unde se adauga vagoanele
 * pos_b: pozitia unde se adauga secventa de vagoane
 */
void move_train_cars(TrainStation *station, int platform_a, int pos_a, 
                                int cars_no, int platform_b, int pos_b) {
    if (!checkStation(station, platform_a) || !checkStation(station,platform_b)) return;
    if (platform_a == platform_b || cars_no <= 0) return;
    if (pos_a <= 0 || pos_b <= 0) return;

    int len_a = getTrainLength(station->platforms[platform_a]);
    int len_b = getTrainLength(station->platforms[platform_b]);
    if (pos_a + cars_no-1> len_a) return;
    if (pos_b > len_b+1) return;

    //printf("Moving %d cars from position %d, station %d to position %d station %d\n",
    //    cars_no, pos_a, platform_a, pos_b, platform_b);

    TrainCar *start_a = NULL;
    TrainCar *end_a = NULL;
    TrainCar *prev_a = station->platforms[platform_a]->train_cars;
    for(int i=1; i<pos_a-1; i++)
        prev_a = prev_a->next;
    if (pos_a == 1)
        start_a = station->platforms[platform_a]->train_cars;
    else
        start_a = prev_a->next;
    end_a = start_a;

    for(int i=1; i<cars_no; i++)
        end_a = end_a->next;

    TrainCar *remaining_a = end_a->next;

    if (prev_a == start_a) {
        end_a->next = NULL;
        station->platforms[platform_a]->train_cars = remaining_a;
    }
    else {
        end_a->next = NULL;
        prev_a->next = remaining_a;
    }



    if (pos_b == 1) {
        TrainCar *start_b = station->platforms[platform_b]->train_cars;
        end_a->next = start_b;
        station->platforms[platform_b]->train_cars = start_a;
    }
    else if (pos_b == len_b + 1) {
        TrainCar *end_b = station->platforms[platform_b]->train_cars;
        for(int i=1; i<len_b; i++)
            end_b = end_b->next;
        end_b->next = start_a;
    }
    else {
        TrainCar *start_b = station->platforms[platform_b]->train_cars;
        for(int i=1; i<pos_b-1; i++)
            start_b = start_b->next;

        TrainCar *remaining_b = start_b->next;
        start_b->next = start_a;
        end_a->next = remaining_b;
    }

}

int getTrainWeight(Train *train) {
    int weight = 0;
    TrainCar *temp = train->train_cars;
    while (temp) {
        weight += temp->weight;
        temp = temp->next;
    }
    return weight;
}

int getTrainPower(Train *train) {
    return train->locomotive_power - getTrainWeight(train);
}

/* Gaseste trenul cel mai rapid.
 * 
 * station: gara existenta
 *
 * return: peronul pe care se afla trenul
 */
int find_express_train(TrainStation *station) {
    if (!station || !station->platforms) return -1;

    Train *temp = NULL;
    int fastestIndex = -1, maxPower = -1;
    for(int i=0; i<station->platforms_no; i++) {
        if (station->platforms[i]->locomotive_power != -1) {
            temp = station->platforms[i];
            fastestIndex = i;
            maxPower = getTrainPower(temp);
            break;
        }
    }
    if (temp == NULL) return -1;

    for(int i=0; i<station->platforms_no; i++) {
        temp = station->platforms[i];
        if (station->platforms[i]->locomotive_power != -1) {
            if (getTrainPower(temp) > maxPower) {
                maxPower = getTrainPower(temp);
                fastestIndex = i;
            }
        }
    }
    return fastestIndex;
}


/* Gaseste trenul supraincarcat.
 * 
 * station: gara existenta
 *
 * return: peronul pe care se afla trenul
 */
int find_overload_train(TrainStation *station) {
    if (!station || !station->platforms) return -1;

    for (int i=0; i<station->platforms_no; i++) {
        if (station->platforms[i]->locomotive_power != -1) {
            if (getTrainPower(station->platforms[i]) < 0)
                return i;
        }
    }
    return -1;
}


/* Gaseste trenul cu incarcatura optima.
 * 
 * station: gara existenta
 *
 * return: peronul pe care se afla trenul
 */
int find_optimal_train(TrainStation *station) {
    if (!station || !station->platforms) return -1;

    Train *temp = NULL;
    int optimalIndex = -1, optimalRatio = -1;
    for(int i=0; i<station->platforms_no; i++) {
        if (station->platforms[i]->locomotive_power != -1) {
            temp = station->platforms[i];
            optimalIndex = i;
            optimalRatio = station->platforms[i]->locomotive_power - getTrainWeight(station->platforms[i]);
            break;
        }
    }
    if (temp == NULL) return -1;

    for(int i=0; i<station->platforms_no; i++) {
        if (station->platforms[i]->locomotive_power != -1) {
            int currentRatio = station->platforms[i]->locomotive_power - getTrainWeight(station->platforms[i]);
            if (currentRatio < optimalRatio) {
                optimalRatio = currentRatio;
                optimalIndex = i;
            }
        }
    }
    return optimalIndex;
}

/* Gaseste trenul cu incarcatura nedistribuita bine.
 * 
 * station: gara existenta
 * cars_no: lungimea secventei de vagoane
 *
 * return: peronul pe care se afla trenul
 */
int find_heaviest_sequence_train(TrainStation *station, int cars_no, TrainCar **start_car) {
    if (!station) return -1;

    int heaviestWeightSum = -1, heaviestIndex = -1;
    *start_car = NULL;
    for(int i=0; i<station->platforms_no; i++) {
        int trainLength = getTrainLength(station->platforms[i]);

        int offset = 0;
        while(offset + cars_no <= trainLength) {

            int currentWeight = 0;
            TrainCar *startPos = station->platforms[i]->train_cars;
            TrainCar *ptr_start_car = station->platforms[i]->train_cars;
            for (int j=0; j<offset; j++) {
                startPos = startPos->next;
                ptr_start_car = ptr_start_car->next;
            }

            for(int j=0; j<cars_no; j++) {
                currentWeight += startPos->weight;
                startPos = startPos->next;
            }

            if (currentWeight > heaviestWeightSum) {
                heaviestWeightSum = currentWeight;
                heaviestIndex = i;
                *start_car = ptr_start_car;
            }
            offset++;
        }
    }
    return heaviestIndex;
}


void swapCars(TrainCar *left, TrainCar *right) {
    int temp = left->weight;
    left->weight = right->weight;
    right->weight = temp;
}

/* Ordoneaza vagoanele dintr-un tren in ordinea descrescatoare a greutatilor.
 * 
 * station: gara existenta
 * platform: peronul pe care se afla trenul
 */
void order_train(TrainStation *station, int platform) {
    if (!checkStation(station, platform)) return;
    if (station->platforms[platform]->locomotive_power == -1) return;

    TrainCar *temp = station->platforms[platform]->train_cars;
    if (!temp) return;

    TrainCar *left = NULL;
    TrainCar *ptr = NULL;
    int ok = 0;
    do {
        ok = 0;
        ptr = temp;

        while(ptr->next != left) {
            if (ptr->weight < ptr->next->weight) {
                swapCars(ptr, ptr->next);
                ok = 1;
            }
            ptr = ptr->next;
        }
        left = ptr;
    }while (ok);
}


/* Scoate un vagon din trenul supraincarcat.
 * 
 * station: gara existenta
 */
void fix_overload_train(TrainStation *station) {
    int overloadIndex = find_overload_train(station);
    if (overloadIndex == -1) return;

    TrainCar *temp = station->platforms[overloadIndex]->train_cars;
    if (temp == NULL) return;

    int removeIndex = 0, ratio = -1;
    for(int i=0; i<getTrainLength(station->platforms[overloadIndex]) && temp; i++) {
        int power = station->platforms[overloadIndex]->locomotive_power;
        int weight = getTrainWeight(station->platforms[overloadIndex]);

        if (ratio == -1 && power-weight+temp->weight >= 0) {
            ratio = power-weight+temp->weight;
            removeIndex = i;
        }

        if (power-weight+temp->weight < ratio && power-weight+temp->weight >= 0) {
            ratio = power-weight+temp->weight;
            removeIndex = i;
        }
        temp = temp->next;
    }

    temp = station->platforms[overloadIndex]->train_cars;
    if (removeIndex == 0) {
        station->platforms[overloadIndex]->train_cars = station->platforms[overloadIndex]->train_cars->next;
        temp->next = NULL;
        free(temp);
    }
    else {
        for(int i=0; i<removeIndex-1; i++) {
            temp = temp->next;
        }
        TrainCar *remove = temp->next;
        temp->next = temp->next->next;
        remove->next = NULL;
        free(remove);
    }
}
