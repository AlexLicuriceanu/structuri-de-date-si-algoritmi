# Tema 1 PCLP1 - Gara din Hogsmeade

```
Test arrive_train...........................passed
Test leave_train...........................passed
Test add_train_car...........................passed
Test remove_train_cars...........................passed
Test move_train_cars...........................passed
Test find_express_train...........................passed
Test find_overload_train...........................passed
Test find_optimal_train...........................passed
Test find_heaviest_sequence_train...........................passed
Test order_train...........................passed
Test fix_overload_train...........................passed

Valgrind errors...........................failed
                                      Total: 90
```

# OPEN_TRAIN_STATION
Functia aloca memorie pentru variabila newStation, initializeaza campul platforms_no cu variabila primita ca parametru si este alocata memorie pentru platforms_no peroane. Fiecare peron este initializat cu campurile locomotive_power = -1 si train_cars = NULL. Functia ulterior returneaza variabila newStation.

# CLOSE_TRAIN_STATION
Functia itereaza peste peroane, iar pentru fiecare se ia iteratorul remove care initial este un pointer catre primul vagon al peronului respectiv. Variabila temp stocheaza pointerul catre urmatorul vagon, memoria din remove este eliberata, iar remove devine temp. Pentru fiecare peron este eliberata memoria ramasa, iar la finalul functiei este eliberata memoria din variabila station.

# SHOW_EXISTING_TRAINS
Sunt parcurse toate peroanele, pentru fiecare se verifica daca nu are campul locomotive_power egal cu -1 (in caz ca e -1, inseamna ca peronul nu are locomotiva). Se ia un iterator, temp care salveaza pointerul catre primul vagon, iar folosind un while, se printeaza greutatea fiecarui vagon.

# checkStation
Functia primeste un pointer la structura garii, si numarul platformei. Functia verifica numarul platformei exista un tren pe platforma respectiva. Daca sunt indeplinite toate conditiile, functia returneaza valoarea 1, in caz contrar valoarea 0.

# ARRIVE_TRAIN
Este apelata functia checkStation, daca sunt indeplinite conditiile, este alocata memorie pentru un nou tren in variabila newTrain, campul locomotive_power este initializat cu valoarea primita ca parametru, campul train_cars cu NULL iar pointerul catre tren al platformei pointeaza spre variabila newTrain.

# LEAVE_TRAIN
Se apeleaza functia checkStation, variabila leaveCar este pointer catre primul vagon al trenului, campul locomotivul_power devine -1, iar intr-un while, variabila temp este pointer catre leaveCar.next, este eliberata memoria din leaveCar, iar leaveCar devine temp, astfel este eliberata secvential memoria alocata fiecarui vagon.

# ADD_TRAIN_CAR
Este apelata functia checkStation si se verifica daca locomotive_power = -1, daca este -1, inseamna ca nu exista locomotiva pe platforma data, iar functia se inchide. In caz contrar, se aloca memorie unui nou vagon in variabila newCar, newCar.next este setat pe NULL iar campul newCar.weight este setat cu valoarea weight primita ca parametru. Daca pointerul catre vagoane este NULL (trenul nu are vagoane), acesta este initializat direct cu variabila newCar. Altfel, se ia un iterator temp care porneste de la primul vagon si se duce pana la ultimul, iar ultimul temp.next va fi un pointer catre vagonul nou creat, astfel introducandu-se vagonul nou la finalul secventei.

# REMOVE_TRAIN_CARS
Se apeleaza functia checkStation si se verifica daca exista locomotiva pe peronul dat (adica locomotive_power sa fie diferit de -1), daca nu sunt indeplinite conditiile, apelul functiei se inchide. Un prim while trateaza cazul in care se regasesc unul sau mai multe vagoane de greutate weight la inceputul secventei, si sunt eliminate. Urmatoarele doua while-uri itereaza peste toate elementele, iar daca un vagon cu greutatea cautata este gasit, este suprascrisa adresa lui cu adresa urmatorului, iar memoria celui gasit este eliberata.

# MOVE_TRAIN_CARS
Se apeleaza checkStation pentru platformele platform_a si platform_b, in caz ca platform_a este egal cu platform_b, sau numarul de vagoane de mutat este nul sau negativ, sau oricare dintre pos_a si pos_b este nula sau negativa, apelul functiei se inchide. Variabilele len_a si len_b stocheaza lungimile trenurilor de pe platformele platform_a, respectiv platform_b. Daca pos_a + cars_no – 1 > len_a (daca nu exista cars_no vagoane de la pozitia pos_a) functia se inchide. Daca pos_b > len_b + 1 (daca pozitia pos_b depaseste numarul de vagoane+1) functia se inchide. Variabila start_a este pointer catre primul dintre vagoanele care trebuie mutate, iar end_a catre ultimul vagon. Prev_a este un pointer catre vagonul de dinainte de primul vagon al secventei care trebuie mutata. Daca pos_a = 1 (daca se doreste mutarea unei secvente incepand cu primul vagon) start_a devine pointer catre primul vagon al trenului. Altfel, start_a este pointer catre prev_a.next. Pentru a gasi capatul secventei de mutat, pointerul end_a este initializat cu start_a si folosind un for, pointerul este mutat catre ultimul vagon dorit. Pointerul remaining_a arata catre vagoanele ramase de dupa secventa ce trebuie mutata. Daca secventa ce trebuie mutata incepe de la primul vagon, end_a.next devine NULL, iar pointerul catre primul vagon al trenului de pe platform_a arata catre remaining_a. Altfel, end_a.next devine NULL, iar prev_a.next pointeaza catre remaining_a (se leaga vagoanele care erau inaintea celor de mutat, cu cele de dupa). Acum programul are o variabila pointer catre inceputul si sfarsitul secventei de mutat, iar legaturile sirului de vagoane de pe platform_a sunt refacute. Daca se doreste inserarea incepand cu prima pozitie in trenul de pe platform_b, variabila start_b este pointer catre primul vagon al trenului, end_a.next devine start_b, iar pointerul catre primul vagon devine start_a (in final, head: start_a -> (…) -> end_a - > start_b -> (…)). Daca secventa trebuie inserata la finalul trenului de pe platform_b, pointerul end_b gaseste adresa ultimului vagon si introduce in campul next al acestuia, adresa primului vagon din secventa de mutat (in final, head: (…) -> end_b -> start_a -> (…)). Altfel, start_b salveaza pozitia de pe care se va insera secventa, iar remaining_b salveaza vagoanele care vor fi adaugate la finalul secventei extrase. Start_b.next stocheaza start_a, iar end_a.next pe remaining_b (In final, head: (…) -> start_b -> start_a -> (…) -> end_a -> remaining_b -> (…)).

# FIND_EXPRESS_TRAIN
Se verifica daca gara si platformele exista, se itereaza peste fiecare platforma pentru a gasi-o pe prima care are o locomotiva, se salveaza pointerul catre tren, indicele peronului si puterea trenului in variabilele temp, fastestIndex si respectiv maxPower (calculata cu functia getTrainPower care scade din valoarea campului locomotive_power suma greutatilor vagoanelor) si se da break. Daca temp este NULL (daca nu s-a gasit niciun tren) apelul se inchide. Se itereaza din nou peste toate platformele care au trenuri, iar daca getTrainPower() intoarce o valoare mai mare decat maxPower se actualizeaza maxPower si fastestIndex. In final functia intoarce variabila fastestIndex, indicele peronului.

# FIND_OVERLOAD_TRAIN
Se verifica existenta garii si a platformelor, se itereaza peste toate platformele, daca pe platforma se afla un tren, rezultatul functiei getTrainPower() este comparat cu 0, iar in cazul in care valoarea intoarsa de functia getTrainPower este negativa, find_overload_train intoarce indicele la care s-a gasit trenul supraincarcat.

# FIND_OPTIMAL_TRAIN
Se verifica existenta garii si a platformelor, se itereaza peste toate platformele, se verifica daca pe platforma este un tren, in caz afirmativ, variabila temp ia adresa trenului, optimalIndex salveaza indicele la care s-a gasit, iar optimalRatio calculeaza diferenta dintre puterea locomotivei si suma greutatilor vagoanelor, folosing functia getTrainWeight() si se da break. Daca temp este NULL (nu s-a gasit un tren), functia intoarce -1. Se itereaza din nou toate platformele, se verifica daca pe platforma este un tren, variabila currentRatio calculeaza diferenta dintre puterea locomotivei si suma greutatilor vagoanelor si compara currentRatio cu optimalRatio. Daca optimalRatio > currentRatio, optimalRatio salveaza valoarea din currentRatio si optimalIndex salveaza indicele peronului la care a fost gasit. In final functia intoarce variabila optimalIndex.

# FIND_HEAVIEST_SEQUENCE_TRAIN
Functia itereaza peste toate trenurile, pentru fiecare se salveaza lungimea in variabila trainLength. Variabila offset pleaca de la 0 si cat timp offset + cars_no <= trainLength, se calculeaza suma greutatilor a cars_no vagoane plecand de la pozitia offset. Pointerul ptr_start_car ia adresa de unde incepe secventa curenta care se testeaza. Daca greutatea secventei actuale este mai mare decat cea mai mare gasita de pana atunci, cea record devine cea curenta, variabila heaviestIndex salveaza indicele platformei la care s-a gasit secventa, iar *start_car ia adresa lui ptr_start_car. Variabila offset este incrementata cu 1 la fiecare cicleare a while-ului, pana cand se iese din conditie. In final, functia returneaza variabila heaviestIndex.

# ORDER_TRAIN
Se apeleaza checkStation si se verifica daca exista un tren pe platforma data. Pointerul temp ia adresa primului vagon, daca temp este NULL, trenul nu are vagoane, deci nu avem ce sa ordonam, functia se inchide. Altfel, se face bubblesort pe vagoanele trenului, si se face interschimbarea pe pointeri folosind functia swapCars().

# FIX_OVERLOAD_TRAIN
Folosind functia find_overload_train, se stocheaza in variabila overloadIndex valoarea intoarsa de aceasta. Daca overloadIndex = -1, inseamnca ca nu s-a gasit un tren supraincarcat si se iese din apel. Altfel, folosind functia getTrainLength(), se itereaza peste toate vagoanele trenului, pentru fiecare ciclu power salveaza puterea locomotivei, iar weight salveaza suma greutatilor tuturor vagoanelor. Pe primul if se va intra numai o data, pentru a calcula prima valoare cu care se vor compara restul, in ratio se salveaza power – weight + temp.weight, care trebuie sa fie >= 0, iar in removeIndex se salveaza indicele la care s-ar putea gasi vagonul ce trebuie eliminat. Cum se doreste ca incarcatura trenului sa fie optima, al doilea if verifica daca vagonul curent poate fi eliminat pentru a putea trage mai multa incarcatura decat daca l-am fi eliminat pe cel anterior, apoi se calculeaza ratio si se salveaza indicele. Pointerul temp este resetat la primul vagon si se iau doua cazuri. Daca removeIndex = 0 atunci pointerul catre primul vagon arata catre cel de-al doilea vagon, temp.next devine NULL pentru a se inlatura vagonul din lista, iar memoria este eliberata prin free(temp). In orice alt caz, un for itereaza cu temp peste vagoane pana la cel ce trebuie eliminat, pointerul remove arata catre vagonul din temp.next, temp.next devine temp.next.next, remove.next devine NULL, astfel se taie legatura din lista, iar memoria vagonului este eliberata.
