# Tema 3 SDA - Block DAG

# MAIN
Indiferent de cerinta, programul citeste numarul de noduri, graful, lista
sa de adiacenta si asociaza numele nodurilor cu indici pentru a lucra mai usor
cu graful. Este apelata functia createGraph() care aloca memorie si initializeaza
un nou graf. Sunt citite urmatoarele linii din fisier care reprezinta lista de 
adiacenta iar legaturile dintre noduri sunt inserate in structura grafului.
Urmeaza apoi trei if-uri pentru a rezolva fiecare cerinta.

# Cerinta 1
Pentru prima cerinta am folosit o functie de parcurgere DFS usor modificata pentru a 
determina daca graful este sau nu aciclic. Functia foloseste un vector de noduri
vizitate, insa nu marcheaza nodurile fie cu 0 (nevizitat) sau 1 (vizitat) ci cu valorile:
0 (nevizitat), 1 (in curs de vizitare) sau 2 (vizitat). Daca sunt gasite noduri care au 
fost explorate la alte apeluri recursive ale functiei, atunci inseamna ca am gasit un
ciclu in graf, returnand valoarea 1. Daca se termina lantul recursiv cu valoarea 0, atunci
inseamna ca nu avem cicluri in graf. Raspunsul este scris in fisier pe baza valorii 
returnate de functia DFS().

# Cerinta 2
Functiia past() parcurge graful in stil DFS si marcheaza indicii nodurilor vizitate in vectorul
pastVector. Pentru functia future() se procedeaza la fel, numai ca nu iteram recursiv peste
copiii nodului din parametru, ci apelam recurziv future() pentru parintii nodului. Pentru 
functia anticone() am propus un algoritm simplu care determina ce elemente nu se regasesc nici in
past(nod), nici in future(nod). Functia tips() returneaza indicii nodurilor care au ca parinti
NULL. Toate cele 4 functii sunt apelate din main si valorile calculate sunt salvate in vectori
separati. Pentru scriere, se apeleaza functia printVectors(), care mai intai sorteaza lexicografic
fiecare vector si apoi scrie valorile in fisier.

# Cerinta 3
Pentru a treia cerinta, raspunsurile vor fi in vectorul blueSet. Este rulata functia tips() deoarece
avem nevoie de nodurile terminale in functia orderDAG, care este apelata ulterior. Algoritmul pentru
a determina blueSet functioneaza in felul urmator: Primul pas este sa apelam functia
calculateBlockScores(), care determina pentru fiecare nod numarul de copii ai acestuia. Numarul acesta
il vom denumi ca fiind scorul unui nod. Algoritmul parcurge nodurile cu scor maxim, plecand de la
nodul terminal cu scor maxim, pana la Genesis, folosind functia past(). In cazul in care este 
egalitate intre scoruri, se alege arbitrar un nod cu scor maxim. Aceste noduri sunt adaugate in ordinea
parcurgerii in blueSet. Urmatorul pas parcurge in ordine inversa DOAR nodurile din blueSet gasite la
pasul anterior. Pentru fiecare dintre noduri, se determina multimea past(), se itereaza peste elementele
multimii, iar pentru fiecare dintre nodurile din past, mai intai se verifica daca este deja in blueSet, 
iar daca nodul de verificat nu se afla in blueSet, se verifica daca intersectia dintre anticone-ul 
acestui nod si blueSet este mai mic sau egal decat k. Daca este indeplinita conditia, nodul respectiv
este adaugat in blueSet si se repeta procesul pentru restul nodurilor. Ultimul pas face exact acelasi
lucru ca la pasul anterior, insa pentru nodurile din tips(), la care nu se ajunge la pasul anterior.
Vectorul blueSet este apoi sortat lexicografic si valorile sunt scrise in fisier. De mentionat este ca
acest algoritm pare sa functioneze pentru majoritatea seturilor de test, insa unele pica, posibil
din cauza unor edge case-uri netratate.
