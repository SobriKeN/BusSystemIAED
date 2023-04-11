/* iaed-23 - ist1103473 - project1 */
/* 
IAED's first project: made by Fábio Sobrinho, ist1103473.
The objective of this project was to create a bus managment system which
consists of routes (known in the code as Carreira), stops (known as Paragem)
and connections (known as Ligacoes) which are mechanisms to connect stops in 
order to create or expand a route. You also have a function which also gives 
us the stops in which intersections exist (more than one route assoiacted).
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define NUMCARREIRA 200 /*max number of routes*/
#define NUMPARAGEM 10000 /*max number of stops*/
#define NUMLIGACAO 30000 /*max number of connections*/

#define TAMCARREIRA 21 /*max length of route's name: 20 bytes + \0*/
#define TAMPARAGEM 51 /*max length of stop's name: 50 bytes + \0*/
#define TAMINVERSO 8 /*max length of word 'inverso': 50 bytes + \0*/

#define NORMAL 0 /*symbolizing normal way of printing stops in a route*/
#define INVERSO 1 /*symbolizing inverted way of printing stops in a route*/
#define NAOEXISTE -1 /*symbolizing non-existent content inside the system*/

/*----------------------------STRUCTURES------------------------------------*/

/*
Stop - consists of a name, latitude, longitude and the number of routes
associated
*/
typedef struct 
{
    char nome[TAMPARAGEM];
    double latitude;
    double longitude; 
    int numeroCarreiras;
} Paragem;

/*
Route- consists of a name, the stops associated with the route, the number
of stops, the cost and the duration of the route
*/
typedef struct 
{
    char nome[TAMCARREIRA];
    /*Irrealistic having a route with 10000 stops*/
    Paragem parCarreira[NUMPARAGEM / 2]; 
    int numeroParagens;
    double custo;
    double duracao;
} Carreira;

/* Will save the routes and stops inside of the system */
Carreira carreiras[NUMCARREIRA];
Paragem paragens[NUMPARAGEM];

/*----------------------------GLOBAL VARIABLES------------------------------*/

/* 
Number of stops, routes and connections associated in the system
Very helpful while verifying through global lists "paragens" and
"carreiras"
*/
int numParagens = 0;
int numCarreiras = 0;
int numLigacoes = 0;

/*----------------------------AUXILIARY FUNCTIONS---------------------------*/

/*
Function used to read input (normally input of char type) from stdin.
Separates word the have brackets from those that do not and returns 0 if
next character to read is newline.
*/
int readWord(char str[]) {
    int i = 0, c = getchar();
    /*Checks if we have any unnecessary space between inputs*/
    while (c == ' ' || c == '\t'){
        c = getchar();
    }
    /*
    If character starts with brackets, it only finishes when if
    finds the next brackets character, cleans the rest of "str" when
    word is done being read.
    */
    if(c == '"'){
        c = getchar();
        while(c != '"'){
            str[i++] = c;
            c = getchar();
            if(c == '"'){
                str[i] = '\0';
                c = getchar();
                return (c == '\n');
            }
        }
    }
    /*
    Else, it only finishes when it finds the next space/newline, 
    cleans the rest of "str" when word is done being read. Doesn't 
    have else keyword becuase in certain inputs, we are going to use 
    this function when there is nothing to read.
    */
    if(c != '\n'){
        str[i++] = c;
        c = getchar();
        while (c != ' ' && c != '\t' && c != '\n') {
            str[i++] = c;
            c = getchar();
        }
    }
    str[i] = '\0';
    return (c == '\n');
}

/*
Function that swaps two strings in a list. Saves a in a local variable
and we switch that variable with b, changing positions.
*/
void swapStrings(char a[], char b[]) {
   char temp[TAMCARREIRA];
   strcpy(temp, a);
   strcpy(a, b);
   strcpy(b, temp);
}

/*
Bubble sort function. Used to sort routes when discovering intersections
between stops.
*/
void bubbleSort(char strings[][TAMCARREIRA], int numStrings) {
   int i, j;
   for (i = 0; i < numStrings - 1; i++) {
      for (j = 0; j < numStrings - i - 1; j++) {
         if (strcmp(strings[j], strings[j+1]) > 0) {
            swapStrings(strings[j], strings[j+1]);
         }
      }
   }
}

/*----------------------------STOPS---------------------------*/

/*
Function that checks and tries to find a stop in the system.
If it finds the stop in the list "paragens", it returns the index 
of the stop in the list. Else, it doesn't exist, returns NAOEXISTE(-1).
*/
int encontrarParagem(char name[]){
    int i;
    for (i = 0; i < numParagens; i++){
        if(strcmp(name, paragens[i].nome) == 0){
            return i;
        }
    }
    return NAOEXISTE;
}

/*
Function that creates the stop in the system. It first finds if the
stop is already in the system. If so, sends a message to user. Else, 
creates a new stop (Paragem) and places it in the list.
*/
void criarParagem(char name[], double lat, double longi){
    int possivel = encontrarParagem(name);
    if (possivel >= 0){
        printf("%s: stop already exists.\n", name);
    }
    else {
        Paragem p;
        strcpy(p.nome, name);
        p.latitude = lat;
        p.longitude = longi;
        p.numeroCarreiras = 0;
        paragens[numParagens] = p;
        numParagens++;
    }
}

/*
Function that prints the latitude and longitude of a certain stop.
It first checks if stop is already in the system. If not, sends error
message to user. Else, prints out the latitude and longitude with 16
spaces and 12 characters.
*/
void printLatLong(char name[]){
    int possivel = encontrarParagem(name);
    if (possivel < 0){
        printf("%s: no such stop.\n", name);
    }
    else{
        Paragem p = paragens[possivel];
        printf("%16.12f %16.12f\n", p.latitude, p.longitude);
    }
}

/*
Function that prints the existing stops in the system. Goes through list
"paragens" and prints every list one by one. 
*/
void printParagens(){
    int i;
    for (i = 0; i < numParagens; i++){
        Paragem p = paragens[i];
        printf("%s: %16.12f %16.12f %d\n", 
        p.nome, 
        p.latitude, 
        p.longitude, 
        p.numeroCarreiras);
    }
}

/*
Stop menu - where input is read to verify what command has been brought to the
system.  
*/
void menuParagens(){
    int c;
    char name[TAMPARAGEM];
    double latitude, longitude;
    c = getchar(); /*To check if we need to use readWord*/
    if (c == '\n'){
        printParagens();
        return;
    }
    if (readWord(name)){ /*If returned 0, does the function*/
        printLatLong(name);
        return;
    }
    else{
        /*Always true*/
        if(scanf("%lf %lf", &latitude, &longitude) == 2){;
            criarParagem(name, latitude, longitude);
            return;
        }
    }
}

/*----------------------------ROUTES---------------------------*/

/*
Function that checks and tries to find a route in the system.
If it finds the stop in the list "carreiras", it returns the index 
of the stop in the list. Else, it doesn't exist, returns NAOEXISTE(-1).
*/
int encontrarCarreira(char name[]){
    int i;
    for (i = 0; i < numCarreiras; i++){
        if(strcmp(name, carreiras[i].nome) == 0){
            return i;
        }
    }
    return NAOEXISTE;
}

/*
Function that creates the route in the system. Creates a new route 
(Carreira) and places it in the list.
*/
void criarCarreira(char name[]){
    Carreira a;
    strcpy(a.nome, name);
    a.numeroParagens = 0;
    a.custo = 0;
    a.duracao = 0;
    carreiras[numCarreiras] = a;
    numCarreiras++;
}

/*
Function that prints the existing stops in the route. It checks if we
print the normal or the inverted way. After that, we find the route in
"carreiras" and we go to the list of stops associated with the route 
(parCarreira) printing every stop inside that list one by one until
we reach the number of stops associated (numeroParagens).
*/
void printParagensCarreira(char name[], int character){
    int i, j;
    if (character == NORMAL) /*Normal way*/{
        for (i = 0; i < numCarreiras; i++){
            if(strcmp(name, carreiras[i].nome) == 0) /*Check route name*/{
                for(j = 0; j < carreiras[i].numeroParagens; j++){
                    /*Goes through list from left to right*/
                    if (j == carreiras[i].numeroParagens -1){
                        printf("%s\n",carreiras[i].parCarreira[j].nome);
                        break;
                    }
                    printf("%s, ", carreiras[i].parCarreira[j].nome);
                }
            }
        }
        return;
    }
    else /*Inverted way*/{
        for (i = 0; i < numCarreiras; i++){
            if(strcmp(name, carreiras[i].nome) == 0) /*Check route name*/{
                for(j = carreiras[i].numeroParagens-1; j >=0; j--){
                    /*Goes through list from left to right*/
                    if (j == 0){
                        printf("%s\n",carreiras[i].parCarreira[j].nome);
                        break;
                    }
                    printf("%s, ",carreiras[i].parCarreira[j].nome);
                }
            }
        }
        return;
    }
}

/*
Function that prints the existing routes in the system.Goes through list
"paragens" and prints every list one by one. If route doesn't have any stops
associated, we don't print any stops, else, print everyting about the stop.
*/
void printCarreiras(){
    int i;
    for (i = 0; i < numCarreiras; i++){
        Carreira c = carreiras[i];
        if(c.numeroParagens == 0){
            printf("%s %d %.2f %.2f\n",
            c.nome, 
            c.numeroParagens, 
            c.custo, 
            c.duracao);
        }
        else{
            printf("%s %s %s %d %.2f %.2f\n",  
            c.nome, 
            c.parCarreira[0].nome, 
            c.parCarreira[c.numeroParagens - 1].nome, 
            c.numeroParagens, 
            c.custo, 
            c.duracao);
        }
    }
}

/*
Create route menu - it verifies if the route is in the system or not.
If so, if prints the stops associated with the route.
Else, it creates the route.
*/
void menuCriarCarreira(char name[]){
    int i = encontrarCarreira(name);
    if (i >= 0){
        printParagensCarreira(name, NORMAL);
        return;
    }
    criarCarreira(name);
}

/*
Route menu - where input is read to verify what command has been brought 
to the system.  
*/
void menuCarreiras(){
    int c;
    char name[TAMCARREIRA], inverso[TAMINVERSO];
    c = getchar(); /*To check if we need to use readWord*/
    if (c == '\n'){
        printCarreiras();
        return;
    }
    if (readWord(name)){ /*If returned 0, does the function*/
        menuCriarCarreira(name);
        return;
    }
    readWord(inverso);
    /*
    In order to print route's stops the imverted way, we need to
    use word 'inverso', which can be abbreviated to a minimum of
    3 characters, to check if valid, compare string inverso with
    every valid word that can be used. If word is none of those,
    send error message to user.
    */
    if ((strcmp(inverso, "inverso") == 0) ||
        (strcmp(inverso, "invers") == 0) ||
        (strcmp(inverso, "inver") == 0) ||
        (strcmp(inverso, "inve") == 0) ||
        (strcmp(inverso, "inv") == 0)){
        printParagensCarreira(name, INVERSO);
        return;
    }
    else{
        printf("incorrect sort option.\n");
        return;
    }
}

/*----------------------------CONNECTIONS---------------------------*/

/*
Function that creates the connection when there are no stops associated
with the route- in other words- we are starting the route.
Stop of index indOri -> first stop; stop of index indDest -> last stop
Associates cost and duration, increments number of routes associated with
both stops and increments number of connections.
*/
void comecarCarreira(int indCar, int indDest, int indOri, 
                    double custo, double duracao){
        carreiras[indCar].parCarreira[0] = paragens[indOri];
        carreiras[indCar].parCarreira[1] = paragens[indDest];
        carreiras[indCar].custo = custo;
        carreiras[indCar].duracao = duracao;
        carreiras[indCar].numeroParagens += 2;
        paragens[indOri].numeroCarreiras++;
        paragens[indDest].numeroCarreiras++;
        numLigacoes++;
}

/*
Function that creates the connection when we want to create a circular route 
(origin = destination): stop of index indDes(first stop) -> last stop
Associates cost and duration, increments number of routes associated with
the stop and increments number of connections.
*/
void ligacaoCasoCircular(int indDes, int indCar, double custo, double duracao){
        carreiras[indCar].parCarreira
            [carreiras[indCar].numeroParagens] = paragens[indDes];
        carreiras[indCar].custo += custo;
        carreiras[indCar].duracao += duracao;
        carreiras[indCar].numeroParagens++;
        numLigacoes++;
}

/*
Function that creates the connection when we want to create before the origin: 
stop of index indOri -> first stop. Associates cost and duration, increments 
number of routes associated with the stop and increments number of connections.
*/
void ligacaoAntesOrigem(int indOri, int indCar, double custo, double duracao){
        int i;
        for (i = carreiras[indCar].numeroParagens - 1; i >= 0; i--) {
            carreiras[indCar].parCarreira[i+1] = carreiras[indCar].parCarreira[i];
        }
        carreiras[indCar].parCarreira[0] = paragens[indOri];
        paragens[indOri].numeroCarreiras++; 
        carreiras[indCar].custo += custo;
        carreiras[indCar].duracao += duracao;
        carreiras[indCar].numeroParagens++;
        numLigacoes++;
}

/*
Function that creates the connection when we want to create after the 
destination: stop of index indDes -> last stop. Associates cost and duration, 
increments number of routes associated with the stop and increments number of 
connections.
*/
void ligacaoDepoisDes(int indDes, int indCar, double custo, double duracao){
        carreiras[indCar].parCarreira
         [carreiras[indCar].numeroParagens] = paragens[indDes];
        paragens[indDes].numeroCarreiras++; 
        carreiras[indCar].custo += custo;
        carreiras[indCar].duracao += duracao;
        carreiras[indCar].numeroParagens++;
        numLigacoes++;
}

/*
Connections menu- where input is read to verify what command has been brought 
to the system.  
*/
void menuLigacoes(){
    int indCar, indOri, indDest;
    char nomeCarreira[TAMCARREIRA], nomeOrigem[TAMPARAGEM];
    char nomeDestino[TAMPARAGEM];
    double custo, duracao;

    /* reads input */
    readWord(nomeCarreira);
    readWord(nomeOrigem);
    readWord(nomeDestino);
    /* always true */
    if (scanf("%lf %lf", &custo, &duracao) == 2){};
    
    /* 
    checks input is right:
    - if route of index indCar exists.
    - if stop of index indOri exists.
    - if stop of index indDest exists.
    - if cost and duration are positive numbers.
    */
    if((indCar = encontrarCarreira(nomeCarreira)) == NAOEXISTE){
        printf("%s: no such line.\n", nomeCarreira);
        return;
    }
    if((indOri = encontrarParagem(nomeOrigem)) == NAOEXISTE){
        printf("%s: no such stop.\n", nomeOrigem);
        return;
    }
    if((indDest = encontrarParagem(nomeDestino)) == NAOEXISTE){
        printf("%s: no such stop.\n", nomeDestino);
        return;
    }
    if (custo < 0 || duracao < 0){
        printf("negative cost or duration.\n");
        return;
    }

    /* 
    Checks if if it is one of these cases:
    - no stops.
    - destination = first stop AND origin = last stop.
    - destination = first stop.
    - origin = last stop.
    */
    if (carreiras[indCar].numeroParagens == 0){
        comecarCarreira(indCar, indDest, indOri, custo, duracao);
        return;
    }
    else if (strcmp(nomeDestino,carreiras[indCar].parCarreira[0].nome) == 0 &&
            strcmp(nomeOrigem,carreiras[indCar].parCarreira  
                [carreiras[indCar].numeroParagens -1].nome) == 0){
        ligacaoCasoCircular(indDest, indCar, custo, duracao);
        return;
    }
    
    else if (strcmp(nomeDestino,carreiras[indCar].parCarreira[0].nome) == 0){
        ligacaoAntesOrigem(indOri, indCar, custo, duracao);
        return;
    }
    else if (strcmp(nomeOrigem, carreiras[indCar].parCarreira  
                [carreiras[indCar].numeroParagens -1].nome) == 0){
        ligacaoDepoisDes(indDest, indCar, custo, duracao);
        return;
    }
    /*If none of them, send error message to user*/
    else{
        printf("link cannot be associated with bus line.\n");
    }
}

/*
Intersections menu.
*/
void listaInterseccao(){
    int i, j, k, l = 0, m, n;
    char carrInter[NUMCARREIRA][TAMCARREIRA];

    /*
    For every stop in system we check if we have number of routes bigger 
    than one.
    */
    for (i = 0; i < numParagens; i++){
        if (paragens[i].numeroCarreiras > 1){
            /*
            If so, we check through every list of stops associated with the 
            route to see if stop is inside of that list, checking through
            every route in the system.
            */
            for (j = 0; j < numCarreiras; j++){
                for (k = 0; k < carreiras[j].numeroParagens; k++){
                    if (strcmp(paragens[i].nome, 
                                carreiras[j].parCarreira[k].nome) == 0){
                        /*
                        If found, send route name to new list and move to the
                        next route, incrementing l (size of list after
                        checking).
                        */
                        strcpy(carrInter[l], carreiras[j].nome);
                        l++;
                        break;
                    }
                }
            }
            /*Sort the list alfabetically*/
            bubbleSort(carrInter, l);
            /*
            Print every route one by one:
            First argument prints the stop, the number of routes and the first
            route's name.
            The others only print the route's name.
            */
            for(m = 0; m < l; m++){
                if (m == 0){
                    printf("%s %d: %s ",
                    paragens[i].nome,
                    paragens[i].numeroCarreiras, 
                    carrInter[m]);
                }
                else if(m == l - 1) {
                    printf("%s\n",carrInter[m]);
                    for (n = 0; n < l; n++) {
                        /*
                        Used in order to clean the list after everything has
                        been printed, so that when we check next stop, we 
                        don't have random routes that are not associated with
                        the stop we are checking.
                        */
                       memset(carrInter[n], '\0', TAMCARREIRA);
                    }
                    l = 0;
                    break;
                }
                else{
                    printf("%s ", carrInter[m]);
                }
            }
        }
    }
    return;
}

/*
Main menu.
*/
int main() {
    int c; 

 	while ((c = getchar()) != EOF) {
		switch (c) {
		case 'q': return 0;
		case 'p': menuParagens(); /*Stop menu*/
			break;
        case 'c': menuCarreiras(); /*Route menu*/
			break;
		case 'l': menuLigacoes(); /*Connection menu*/
			break;
		case 'i': listaInterseccao(); /*Intersection menu*/
			break;
		}
	}
	return 0;
}