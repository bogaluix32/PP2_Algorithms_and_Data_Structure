#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <locale.h>

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define CYAN "\x1b[36m"
#define WHITE "\x1b[37m"
#define RESET_COLOR "\x1b[0m"
#define MAXCHAT 1000
#define MAX 256

//----------Funciones Auxiliares----------

void clearBuffer() {
  while((getchar()) != '\n');
}

void removeLineBreak(char string[]){

    string[strlen(string) - 1] = '\0';

}

bool validateIdentificationCard(char identificationCard[]){

    int size = strlen(identificationCard);

    if (size != 9){
        return false;
    }

    for (int i = 0; i < size; i++){

        if (!isdigit(identificationCard[i])){
            return false;
        }
    }

    return true;
}

bool validateMail(char mail[]){
    
    if (!isalpha(mail[0])){
        return false;
    }

    int at = -1, dot = -1;
    int count = 0;

    for (int i = 0; i < strlen(mail); i++){
        
        if (mail[i] == '@'){
            at = i;
            count++;
        }
        else if (mail[i] == '.'){
            dot = i;
        }
    }

    if (at == -1 || dot == -1){
        return false;
    }

    if (count != 1){
        return false;
    }

    if (at > dot){
        return false;
    }

    return !(dot >= (strlen(mail) -1));
}

bool validateDates(char date[]){

    int size = strlen(date);

    if (size != 8){
        return false;
    }

    for (int i = 0; i < size; i++){

        if (i == 2 || i == 5){
            if (date[i] != 47){
                return false;
            }
            else{
                continue;
            }
        }

        if (!isdigit(date[i])){
            return false;
        }
    }

    return true;
}

bool validateIsAlpha(char name[]){
    
    int size = strlen(name);

    for (int i = 0; i < size; i++){

        if (!isalpha(name[i]) && !isspace(name[i])){
            return false;
        }
    }

    return true;
}

bool validateIsNum(char num[]){
    
    int size = strlen(num);

    for (int i = 0; i < size; i++){

        if (!isdigit(num[i])){
            return false;
        }
    }

    return true;
}

bool validatePostalCode(char postalCode[]){

    int size = strlen(postalCode);

    if (size != 5){
        return false;
    }

    for (int i = 0; i < size; i++){

        if (!isdigit(postalCode[i])){
            return false;
        }
    }

    return true;
}

char getYesOrNo(){
    char option;

    while(true){

        option = getchar();

        if (option == 'Y' || option == 'N'){
            break;

        }else{
            printf(RED "Letra invalida\n" CYAN "Por favor ingrese Y o N:" RESET_COLOR "\n");
        }
    }

    return option;
}

//----------Registrar colaboradores----------

/*
Entradas: datos del colaborador: numero de cedula, nombre, primer apellido, segundo apellido, 
          correo, rol en la empresa y fecha de cumpleanios
Salidas: el colaborador registrado en la lista de colaboradores
Restricciones: numero de cedula debe tener 9 digitos, nombre debe componerse de letras, los apellidos deben componerse de letras, 
               el correo debe tener formato ejemplo@dominio.prefijo, el rol debe componerse de letras y la fecha de cumpleanios en
               formato xx/xx/xx
*/

typedef struct collaboratorsList{
    char identificationCard[15];
    char name[50];
    char lastName[50];
    char SecondLastName[50];
    char mail[100];
    char role[100];
    char birthday[15];
    struct collaboratorsList* next;
}collaboratorsNode;

collaboratorsNode* createNode(char id[], char currentName[], char currentLastName[], char currentSecLastName[], char currentMail[], char currentRole[], char currentBirthday[]){

	collaboratorsNode *newNode = (collaboratorsNode *) malloc(sizeof(collaboratorsNode));

	newNode->next = NULL;

	strcpy(newNode->identificationCard, id);
    strcpy(newNode->name, currentName);
    strcpy(newNode->lastName, currentLastName);
    strcpy(newNode->SecondLastName, currentSecLastName);
    strcpy(newNode->mail, currentMail);
    strcpy(newNode->role, currentRole);
    strcpy(newNode->birthday, currentBirthday);
	
	return newNode;
}

void uploadCollaborators(collaboratorsNode** collaboratorsList){

    FILE * ptrFile = fopen("COLABORADORES.csv", "r");

    char data[7][120];
    char line[330];
    int count;

    while (fgets(line, sizeof(line), ptrFile)){

        char *token;

        token = strtok(line, ",");
        count = 0;

        while (token != NULL){

            strcpy(data[count], token);
            token = strtok(NULL, ",");
            count++;
        }

        collaboratorsNode *newNode = createNode(data[0], data[1], data[2], data[3], data[4], data[5], data[6]);

        if (*collaboratorsList == NULL){
            *collaboratorsList = newNode;
        }
        else{
            collaboratorsNode *last = *collaboratorsList;

            while (last->next != NULL){
                last = last->next;
            }

            last->next = newNode;
        }  
    }

    fclose(ptrFile);
}

void appendCollaborators(collaboratorsNode **collaboratorsList){

    char currentID[15];
    bool resID;
    printf(CYAN "Ingrese la cedula del colaborador\n" RESET_COLOR);

    while (true){

        fgets(currentID, sizeof(currentID), stdin);
        removeLineBreak(currentID);

        resID = validateIdentificationCard(currentID);

        if (resID){
            break;
        }
        else{
            printf(RED "El numero de cedula es invalido\n" CYAN "Intente de nuevo:\n" RESET_COLOR);
        }
    }
    
    char currentName[50];
    bool resName;
    printf(CYAN "Ingrese el nombre del colaborador:\n" RESET_COLOR);
    
    while (true){

        fgets(currentName, sizeof(currentName), stdin);
        removeLineBreak(currentName);

        resName = validateIsAlpha(currentName);

        if (resName){
            break;
        }
        else{
            printf(RED "El nombre es invalido\n" CYAN "Intente de nuevo:\n" RESET_COLOR);
        }
    }

    char currentLastName[50];
    bool resLastName;
    printf(CYAN "Ingrese el primer apellido del colaborador:\n" RESET_COLOR);
    
    while (true){

        fgets(currentLastName, sizeof(currentLastName), stdin);
        removeLineBreak(currentLastName);

        resLastName = validateIsAlpha(currentLastName);

        if (resLastName){
            break;
        }
        else{
            printf(RED "El apellido es invalido\n" CYAN "Intente de nuevo:\n" RESET_COLOR);
        }
    }

    char currentSecondLastName[50];
    bool resSecLastName;
    printf(CYAN "Ingrese el segundo apellido del colaborador:\n" RESET_COLOR);
    
    while (true){

        fgets(currentSecondLastName, sizeof(currentSecondLastName), stdin);
        removeLineBreak(currentSecondLastName);

        resSecLastName = validateIsAlpha(currentSecondLastName);

        if (resSecLastName){
            break;
        }
        else{
            printf(RED "El apellido es invalido\n" CYAN "Intente de nuevo:\n" RESET_COLOR);
        }
    }

    char currentMail[100];
    bool resMail;
    printf(CYAN "Ingrese el mail del colaborador:\n" RESET_COLOR);
    
    while (true){

        fgets(currentMail, sizeof(currentMail), stdin);
        removeLineBreak(currentMail);

        resMail = validateMail(currentMail);

        if (resMail){
            break;
        }
        else{
            printf(RED "El mail es invalido\n" CYAN "Intente de nuevo:\n" RESET_COLOR);
        }
    }

    char currentRole[100];

    printf(CYAN "Ingrese el rol del colaborador en la empresa:\n" RESET_COLOR);
    fgets(currentRole, sizeof(currentRole), stdin);
    removeLineBreak(currentRole);

    char currentBirthday[15];
    bool resDate;
    printf(CYAN "Ingrese la fecha de cumpleanios del colaborador en formato xx/xx/xxxx:\n" RESET_COLOR);

    while (true){

        clearBuffer();

        fgets(currentBirthday, sizeof(currentBirthday), stdin);
        removeLineBreak(currentBirthday);
        resDate = validateDates(currentBirthday);

        if (resDate){
            break;
        }
        else{
            printf(RED "La fecha es invalida\n" CYAN "Intente de nuevo:\n" RESET_COLOR);
        }
    }

    collaboratorsNode *newNode = createNode(currentID, currentName, currentLastName, currentSecondLastName, currentMail, currentRole, currentBirthday);
    collaboratorsNode *last = *collaboratorsList;

    if (*collaboratorsList == NULL){
        *collaboratorsList = newNode;
        printf(GREEN "Colaborador registrado exitosamente\n" RESET_COLOR);
        return;
    }

    while (last->next != NULL){
        last = last->next;
    }

    last->next = newNode;
    printf(GREEN "Colaborador registrado exitosamente\n" RESET_COLOR);

    clearBuffer();
    return;
}

void showCollaborators(collaboratorsNode *collaboratorsList){
    collaboratorsNode *i;

    if (collaboratorsList == NULL){
        printf(RED "Lista vacia\n" RESET_COLOR);
        return;
    }

    printf(CYAN "Los colaboradores registrados son:\n" RESET_COLOR);
    
    for (i = collaboratorsList; i != NULL; i = i->next){
        printf(YELLOW "     Cedula: %s     \n" RESET_COLOR, i->identificationCard);
        printf(CYAN "Nombre:" RESET_COLOR " %s\n", i->name);
        printf(CYAN "Primer apellido:" RESET_COLOR " %s\n", i->lastName);
        printf(CYAN "Segundo apellido:" RESET_COLOR " %s\n", i->SecondLastName);
        printf(CYAN "Correo:" RESET_COLOR " %s\n", i->mail);
        printf(CYAN "Rol:" RESET_COLOR " %s\n", i->role);
        printf(CYAN "Fecha de cumpleanios:" RESET_COLOR " %s\n", i->birthday);
    }
    
    printf("\n");
}

//----------Modificar colaboradores----------

/*
Entradas: datos del colaborador: numero de cedula, nombre, primer apellido, segundo apellido, 
          correo, rol en la empresa y fecha de cumpleanios
Salidas: el colaborador modificado en la lista de colaboradores
Restricciones: numero de cedula debe tener 9 digitos, nombre debe componerse de letras, los apellidos deben componerse de letras, 
               el correo debe tener formato ejemplo@dominio.prefijo, el rol debe componerse de letras y la fecha de cumpleanios en
               formato xx/xx/xx
*/

void modifyCollaborator(collaboratorsNode **collaboratorsList){

    collaboratorsNode *ptrCollaborator = *collaboratorsList;

    char currentID[15];
    bool resID;
    printf(CYAN "Ingrese la cedula del colaborador a modificar:\n" RESET_COLOR);

    while (true){

        fgets(currentID, sizeof(currentID), stdin);
        removeLineBreak(currentID);

        resID = validateIdentificationCard(currentID);

        if (resID){
            break;
        }
        else{
            printf(RED "El numero de cedula es invalido\n" CYAN "Intente de nuevo:\n" RESET_COLOR);
        }
    }

    char newName[50], newLastName[50], newSecondLastName[50], newMail[100], newRole[100], newBirthday[15];

    char yesOrNo_1, yesOrNo_2, yesOrNo_3, yesOrNo_4, yesOrNo_5, yesOrNo_6;

    bool resName, resLast, restSecLast, resDate, resMail, resRole;

    while (ptrCollaborator != NULL){

        if (strcmp(ptrCollaborator->identificationCard, currentID) == 0){

            printf(CYAN "Quieres modificar el nombre? Ingrese Y para afirmar o N para negar\n" RESET_COLOR);
            yesOrNo_1 = getYesOrNo();

            if (yesOrNo_1 == 'Y'){

                clearBuffer();
                printf(CYAN "Ingrese el nuevo nombre:\n" RESET_COLOR);
                
                while (true){

                    fgets(newName, sizeof(newName), stdin);
                    removeLineBreak(newName);

                    resName = validateIsAlpha(newName);

                    if (resName){
                        break;
                    }
                    else{
                        printf(RED "El nombre es invalido\n" CYAN "Intente de nuevo:\n" RESET_COLOR);
                    }
                }
                
                strcpy(ptrCollaborator->name, newName);

            }else{

                printf(CYAN "Ok\n" RESET_COLOR);
                clearBuffer();
            }

            printf(CYAN "Quieres modificar el primer apellido? Ingrese Y para afirmar o N para negar:\n" RESET_COLOR);
            yesOrNo_2 = getYesOrNo();

            if (yesOrNo_2 == 'Y'){

                clearBuffer();
                printf(CYAN "Ingrese el nuevo apellido:\n" RESET_COLOR);

                while (true){

                    fgets(newLastName, sizeof(newLastName), stdin);
                    removeLineBreak(newLastName);

                    resLast = validateIsAlpha(newLastName);

                    if (resLast){
                        break;
                    }
                    else{
                        printf(RED "El nombre es invalido\n" CYAN "Intente de nuevo:\n" RESET_COLOR);
                    }
                }

                strcpy(ptrCollaborator->lastName, newLastName);
            }
            else{

                printf(CYAN "Ok\n" RESET_COLOR);
                clearBuffer();
            }

            printf(CYAN "Quieres modificar el segundo apellido? Ingrese Y para afirmar o N para negar:\n" RESET_COLOR);
            yesOrNo_3 = getYesOrNo();

            if (yesOrNo_3 == 'Y'){

                clearBuffer();
                printf(CYAN "Ingrese el nuevo apellido:\n" RESET_COLOR);

                while (true){

                    fgets(newSecondLastName, sizeof(newSecondLastName), stdin);
                    removeLineBreak(newSecondLastName);

                    restSecLast = validateIsAlpha(newSecondLastName);

                    if (restSecLast){
                        break;
                    }
                    else{
                        printf(RED "El nombre es invalido\n" CYAN "Intente de nuevo:\n" RESET_COLOR);
                    }
                }
                
                strcpy(ptrCollaborator->SecondLastName, newSecondLastName);
            }
            else{

                printf(CYAN "Ok\n" RESET_COLOR);
                clearBuffer();
            }

            printf(CYAN "Quieres modificar el mail? Ingrese Y para afirmar o N para negar:\n" RESET_COLOR);
            yesOrNo_4 = getYesOrNo();

            if (yesOrNo_4 == 'Y'){

                printf(CYAN "Ingrese el nuevo mail:\n" RESET_COLOR);

                while (true){

                    fgets(newMail, sizeof(newMail), stdin);
                    removeLineBreak(newMail);

                    resMail = validateMail(newMail);

                    if (resMail){
                        break;
                    }
                    else{
                        printf(RED "El mail es invalido\n" CYAN "Intente de nuevo:\n" RESET_COLOR);
                    }
                }
                
                strcpy(ptrCollaborator->mail, newMail);
            }
            else{

                printf(CYAN "Ok\n" RESET_COLOR);
                clearBuffer();
            }

            printf(CYAN "Quieres modificar el rol? Ingrese Y para afirmar o N para negar:\n" RESET_COLOR);
            yesOrNo_5 = getYesOrNo();

            if (yesOrNo_5 == 'Y'){

                clearBuffer();
                printf(CYAN "Ingrese el nuevo rol:\n" RESET_COLOR);

                while (true){

                    fgets(newRole, sizeof(newRole), stdin);
                    removeLineBreak(newRole);

                    resRole = validateIsAlpha(newRole);

                    if (resRole){
                        break;
                    }
                    else{
                        printf(RED "El rol es invalido\n" CYAN "Intente de nuevo:\n" RESET_COLOR);
                    }
                }
                
                strcpy(ptrCollaborator->role, newRole);
            }
            else{

                printf(CYAN "Ok\n" RESET_COLOR);
                clearBuffer();
            }

            printf(CYAN "Quieres modificar la fecha de cumpleanios? Ingrese Y para afirmar o N para negar:\n" RESET_COLOR);
            yesOrNo_6 = getYesOrNo();

            if (yesOrNo_6 == 'Y'){
                clearBuffer();
                printf(CYAN "Ingrese la nueva fecha en formato xx/xx/xxxx:\n" RESET_COLOR);

                while (true){

                    fgets(newBirthday, sizeof(newBirthday), stdin);
                    removeLineBreak(newBirthday);

                    resDate = validateDates(newBirthday);

                    if (resDate){
                        break;
                    }
                    else{
                        printf(RED "La fecha es invalida\n" CYAN "Intente de nuevo:\n" RESET_COLOR);
                    }
                }
               
                strcpy(ptrCollaborator->birthday, newBirthday);
            }
            else{

                printf(CYAN "Ok\n" RESET_COLOR);
                clearBuffer();
            }

            printf(GREEN "Informacion del colaborador modificada exitosamente\n" RESET_COLOR);

            return;
        }

        ptrCollaborator = ptrCollaborator->next;
    }

    if (ptrCollaborator == NULL){

        printf(RED "Colaborador no encontrado\n" RESET_COLOR);
        return;
    }
}

//----------Eliminar colaboradores----------

/*
Entradas: datos del colaborador: numero de cedula, nombre, primer apellido, segundo apellido, 
          correo, rol en la empresa y fecha de cumpleanios
Salidas: el colaborador eliminado en la lista de colaboradores
Restricciones: numero de cedula debe tener 9 digitos, nombre debe componerse de letras, los apellidos deben componerse de letras, 
               el correo debe tener formato ejemplo@dominio.prefijo, el rol debe componerse de letras y la fecha de cumpleanios en
               formato xx/xx/xx
*/

void deleteCollaborators(collaboratorsNode **collaboratorsList){

    char currentID[15];
    bool resID;

    printf(CYAN "Ingrese la cedula del colaborador a eliminar:\n" RESET_COLOR);
    
    while (true){

        fgets(currentID, sizeof(currentID), stdin);
        removeLineBreak(currentID);

        resID = validateIdentificationCard(currentID);

        if (resID){
            break;
        }
        else{
            printf(RED "El numero de cedula es invalido\n" CYAN "Intente de nuevo:\n" RESET_COLOR);
        }
    }

    collaboratorsNode *currentCollab = *collaboratorsList, *previousCollab;

    if (currentCollab != NULL && strcmp(currentCollab->identificationCard, currentID) == 0){

        *collaboratorsList = currentCollab->next;
        printf(GREEN "Se ha eliminado el colaborador con cedula %s\n" RESET_COLOR, currentCollab->identificationCard);
        free(currentCollab);
        return;
    }

    while (currentCollab != NULL && strcmp(currentCollab->identificationCard, currentID) != 0){

        previousCollab = currentCollab;
        currentCollab = currentCollab->next;
    }

    if (currentCollab == NULL){

        printf(RED "Colaborador no encontrado\n" RESET_COLOR);
        return;
    }

    previousCollab->next = currentCollab->next;

    printf(GREEN "Se ha eliminado el colaborador con cedula %s\n" RESET_COLOR, currentCollab->identificationCard);

    free(currentCollab);
}

//----------Consultar colaboradores----------

/*
Entradas: respuesta de tipo si o no por parte del usuario, en caso de que desee consultar informacion especifica de un colaborador
Salidas: la consulta de los colaboradores registrados en la lista de colaboradores
Restricciones: la entrada del usuario debe ser un caracter Y o N. Y = si o N = no
*/

void consultCollaborators(collaboratorsNode *collaboratorsList){

    collaboratorsNode *i;
    char yesOrNo;

    if (collaboratorsList == NULL){

        printf(RED "Lista vacia\n" RESET_COLOR);
        return;
    }

    printf(CYAN "Los colaboradores registrados son:\n" RESET_COLOR);

    for (i = collaboratorsList; i != NULL; i = i->next){

        printf(YELLOW "     Cedula: %s     \n" RESET_COLOR, i->identificationCard);
        printf(CYAN "Nombre:" RESET_COLOR " %s %s %s\n", i->name, i->lastName, i->SecondLastName);
    }

    printf(CYAN "Desea consultar un colaborador en particular? Ingrese Y para afirmar o N para negar:\n" RESET_COLOR);

    yesOrNo = getYesOrNo();

    if (yesOrNo == 'Y'){

        char currentID[15];
        bool resID;
        printf(CYAN "Ingrese la cedula del colaborador a consultar:\n" RESET_COLOR);

        while (true){

            fgets(currentID, sizeof(currentID), stdin);
            removeLineBreak(currentID);

            resID = validateIdentificationCard(currentID);

            if (resID){
                break;
            }
            else{
                printf(RED "El numero de cedula es invalido\n" CYAN "Intente de nuevo:\n" RESET_COLOR);
            }
        }

        for (i = collaboratorsList; i != NULL; i = i->next){

            if (strcmp(i->identificationCard, currentID) == 0){

                printf(YELLOW "     Cedula: %s     \n" RESET_COLOR, i->identificationCard);
                printf(CYAN "Nombre:" RESET_COLOR " %s\n", i->name);
                printf(CYAN "Primer apellido:" RESET_COLOR " %s\n", i->lastName);
                printf(CYAN "Segundo apellido:" RESET_COLOR " %s\n", i->SecondLastName);
                printf(CYAN "Correo:" RESET_COLOR " %s\n", i->mail);
                printf(CYAN "Rol:" RESET_COLOR " %s\n", i->role);
                printf(CYAN "Fecha de cumpleanios:" RESET_COLOR " %s\n", i->birthday);

                printf("\n");
                return;
            }
        }
    }
    else{

        printf(CYAN "Ok\n" RESET_COLOR);
        clearBuffer();
        return;
    }
}

//----------Registar equipo----------

/*
Entradas: datos del equipo: nombre, descripcion y miembros
Salidas: el equipo registrado en la lista de equipos
Restricciones: numero de cedula de cada miembro debe tener 9 digitos, nombre debe componerse de letras
*/

typedef struct teamList{
    char teamName[50];
    char teamDescription[100];
    int members[1000];
    int quantityMembers;
    struct teamList* next;
}teamNode;

teamNode* createTeamNode(char name[], char description[], int arrayMembers[], int size){

	teamNode *newNode = (teamNode *) malloc(sizeof(teamNode));

	newNode->next = NULL;

	strcpy(newNode->teamName, name);
    strcpy(newNode->teamDescription, description);
    newNode->quantityMembers = size;
    
    for (int i = 0; i < size; i++){

        newNode->members[i] = arrayMembers[i];
    }
	
	return newNode;
}

void appendTeam(teamNode **teamList){

    char currentName[50];
    bool resName;
    printf(CYAN "Ingrese el nombre del equipo:\n" RESET_COLOR);
    
    while (true){

        fgets(currentName, sizeof(currentName), stdin);
        removeLineBreak(currentName);

        resName = validateIsAlpha(currentName);

        if (resName){
            break;
        }
        else{
            printf(RED "El nombre es invalido\n" CYAN "Intente de nuevo:\n" RESET_COLOR);
        }
    }

    char currentDescription[100];
    bool resDesc;
    printf(CYAN "Ingrese la descripcion del equipo:\n" RESET_COLOR);
    
    while (true){

        fgets(currentDescription, sizeof(currentDescription), stdin);
        removeLineBreak(currentDescription);

        resDesc = validateIsAlpha(currentDescription);

        if (resDesc){
            break;
        }
        else{
            printf(RED "La descripcion es invalido\n" CYAN "Intente de nuevo:\n" RESET_COLOR);
        }
    }

    char currentID[15];
    char yesOrNo;
    int membersArray[1000];
    int i = 0;
    bool resID;

    while (true){

        printf(CYAN "Ingrese la cedula del colaborador que pertenece a este equipo:\n" RESET_COLOR);
    
        while (true){

            fgets(currentID, sizeof(currentID), stdin);
            removeLineBreak(currentID);

            resID = validateIdentificationCard(currentID);

            if (resID){
                break;
            }
            else{
                printf(RED "El numero de cedula es invalido\n" CYAN "Intente de nuevo:\n" RESET_COLOR);
            }
        }

        int id = atoi(currentID);
        membersArray[i] = id;
        i++;

        printf(CYAN "Desea agregar otro miembro a este equipo? Ingrese Y para afirmar o N para negar\n" RESET_COLOR);
        yesOrNo = getYesOrNo();

        if (yesOrNo == 'N'){

            break;
        }
    }

    teamNode *newNode = createTeamNode(currentName, currentDescription, membersArray, i);
    teamNode *last = *teamList;

    if (*teamList == NULL){
        *teamList = newNode;
        printf(GREEN "Equipo registrado exitosamente\n" RESET_COLOR);
        return;
    }

    while (last->next != NULL){
        last = last->next;
    }

    last->next = newNode;
    printf(GREEN "Equipo registrado exitosamente\n" RESET_COLOR);

    clearBuffer();
    return;
}

//----------Eliminar equipo----------

/*
Entradas: datos del equipo: nombre, descripcion y miembros
Salidas: el equipo eliminado en la lista de equipos
Restricciones: numero de cedula de cada miembro debe tener 9 digitos, nombre debe componerse de letras,la descripcion componerse de letras 
*/

void deleteTeam(teamNode **teamList){

    teamNode *currentTeam = *teamList, *previous;

    char currentName[50];
    bool resName;
    printf(CYAN "Ingrese el nombre del equipo a eliminar:\n" RESET_COLOR);
    
    while (true){

        fgets(currentName, sizeof(currentName), stdin);
        removeLineBreak(currentName);

        resName = validateIsAlpha(currentName);

        if (resName){
            break;
        }
        else{
            printf(RED "El nombre es invalido\n" CYAN "Intente de nuevo:\n" RESET_COLOR);
        }
    }

    if (currentTeam != NULL && strcmp(currentTeam->teamName, currentName) == 0){

        *teamList = currentTeam->next;
        printf(GREEN "Se ha eliminado el equipo %s\n" RESET_COLOR, currentTeam->teamName);
        free(currentTeam);
        return;
    }

    while (currentTeam != NULL && strcmp(currentTeam->teamName, currentName) != 0){
        
        previous = currentTeam;
        currentTeam = currentTeam->next;
    }

    if (currentTeam == NULL){

        printf(RED "Equipo no encontrado\n" RESET_COLOR);
        return;
    }

    previous->next = currentTeam->next;

    printf(GREEN "Se ha eliminado el equipo %s\n" RESET_COLOR, currentTeam->teamName);

    free(currentTeam);
}

//----------Consultar equipo----------

/*
Entradas: la lista de equipos y la lista de colaboradores
Salidas: informacion de los equipos registrados en la lista de equipos
Restricciones: la lista de equipos y la lista de colaboradores no deben estar vacias
*/

void consultTeam(teamNode *teamList, collaboratorsNode *collaboratorsList){

    teamNode *i;
    collaboratorsNode *k;
    int size; 
    char id[15];

    for (i = teamList; i != NULL; i = i->next){

        size = i->quantityMembers;

        printf(YELLOW "     Nombre del equipo: %s     \n" RESET_COLOR, i->teamName);
        printf(CYAN "Descripcion:" RESET_COLOR " %s\n", i->teamDescription);
        printf(YELLOW "Miembros:\n" RESET_COLOR);

        for (int j = 0; j < size; j++){

            itoa(i->members[j], id, 10);

            for (k = collaboratorsList; k != NULL; k = k->next){

                if (strcmp(k->identificationCard, id) == 0){

                    break;
                }
            }

            printf(CYAN "-%d | %s %s %s\n" RESET_COLOR, i->members[j], k->name, k->lastName, k->SecondLastName);
        }
    }
}

//----------Registrar bitacora----------

/*
Entradas: numero de cedula del colaborador al que pertenece la bitacora y descripcion o observacion de rendimiento
Salidas: la bitacora registrada en la lista de bitacoras 
Restricciones: numero de cedula de cada miembro debe tener 9 digitos
*/

typedef struct binnacle{
    char identificationCard[15];
    char description[500];
    struct binnacle* next;
}binnacleNode;

binnacleNode* createbinnacleNode(char id[], char description[]){

	binnacleNode *newNode = (binnacleNode *) malloc(sizeof(binnacleNode));

	newNode->next = NULL;

	strcpy(newNode->identificationCard, id);
    strcpy(newNode->description, description);
	
	return newNode;
}

void uploadBinnacle(binnacleNode** binnacle){

    FILE * ptrFile = fopen("BINNACLES.csv", "r");

    char data[2][120];
    char line[330];
    int count;

    while (fgets(line, sizeof(line), ptrFile)){

        char *token;

        token = strtok(line, ",");
        count = 0;

        while (token != NULL){

            strcpy(data[count], token);
            token = strtok(NULL, ",");
            count++;
        }

        binnacleNode *newNode = createbinnacleNode(data[0], data[1]);

        if (*binnacle == NULL){
            *binnacle = newNode;
        }
        else{
            binnacleNode *last = *binnacle;

            while (last->next != NULL){
                last = last->next;
            }

            last->next = newNode;
        }  
    }

    fclose(ptrFile);
}

void appendBinnacle(binnacleNode **binnacle){

    char currentID[15];
    bool resID;
    printf(CYAN "Ingrese la cedula del colaborador\n" RESET_COLOR);

    while (true){

        fgets(currentID, sizeof(currentID), stdin);
        removeLineBreak(currentID);

        resID = validateIdentificationCard(currentID);

        if (resID){
            break;
        }
        else{
            printf(RED "El numero de cedula es invalido\n" CYAN "Intente de nuevo:\n" RESET_COLOR);
        }
    }

    char currentDescription[500];

    printf(CYAN "Ingrese la observacion de rendimiento:\n" RESET_COLOR);
    fgets(currentDescription, sizeof(currentDescription), stdin);
    removeLineBreak(currentDescription);

    binnacleNode *newNode = createbinnacleNode(currentID, currentDescription);
    binnacleNode *last = *binnacle;

    if (*binnacle == NULL){
        *binnacle = newNode;
        printf(GREEN "Bitacora registrada exitosamente\n" RESET_COLOR);
        return;
    }

    while (last->next != NULL){
        last = last->next;
    }

    last->next = newNode;
    printf(GREEN "Bitacora registrada exitosamente\n" RESET_COLOR);

    clearBuffer();
    return;
}

//----------Consultar bitacora----------

/*
Entradas: la lista de bitacoras
Salidas: todas las bitacoras registradas en la lista de bitacoras 
Restricciones: la lista de bitacoras no debe estar vacia
*/

void consultBinnacle(binnacleNode *binnacle){

    binnacleNode *i;

    if (binnacle == NULL){

        printf(RED "No hay bitacoras registradas\n" RESET_COLOR);
        return;
    }

    printf(CYAN "Las bitacoras registradas son:\n" RESET_COLOR);
    
    for (i = binnacle; i != NULL; i = i->next){

        printf(YELLOW "     Cedula: %s     \n" RESET_COLOR, i->identificationCard);
        printf(CYAN "Observaciones:" RESET_COLOR " %s\n", i->description);

    }
    
    printf("\n");
}

//----------Eliminar bitacora----------

/*
Entradas: numero de cedula del colaborador al que pertenece la bitacora
Salidas: la bitacora eliminada en la lista de bitacoras 
Restricciones: numero de cedula del colaborador debe tener 9 digitos
*/

void deleteBinnacle(binnacleNode **binnacle){

    binnacleNode *currentBinnacle = *binnacle, *previous;

    char currentID[15];
    bool resID;
    printf(CYAN "Ingrese la cedula del colaborador cuya bitacora desea eliminar\n" RESET_COLOR);

    while (true){

        fgets(currentID, sizeof(currentID), stdin);
        removeLineBreak(currentID);

        resID = validateIdentificationCard(currentID);

        if (resID){
            break;
        }
        else{
            printf(RED "El numero de cedula es invalido\n" CYAN "Intente de nuevo:\n" RESET_COLOR);
        }
    }

    if (currentBinnacle != NULL && strcmp(currentBinnacle->identificationCard, currentID) == 0){

        *binnacle = currentBinnacle->next;
        printf(GREEN "Se ha eliminado la bitacora del colaborador %s\n" RESET_COLOR, currentBinnacle->identificationCard);
        free(currentBinnacle);
        return;
    }

    while (currentBinnacle != NULL && strcmp(currentBinnacle->identificationCard, currentID) != 0){
        
        previous = currentBinnacle;
        currentBinnacle = currentBinnacle->next;
    }

    if (currentBinnacle == NULL){

        printf(RED "Bitacora no encontrada\n" RESET_COLOR);
        return;
    }

    previous->next = currentBinnacle->next;

    printf(GREEN "Se ha eliminado la bitacora del colaborador %s\n" RESET_COLOR, currentBinnacle->identificationCard);

    free(currentBinnacle);
}

//----------Operaciones con archivos----------

//comandos

void cdCommand(){

    char basePath[MAX];
    char yesOrNo;

    printf(YELLOW "Direccion en la que se encuentra el programa:" RESET_COLOR " %s\n", getcwd(basePath, MAX));

    printf(CYAN "Desea cambiar de ruta? Ingrese Y para afirmar o N para negar\n" RESET_COLOR);
    clearBuffer();
    yesOrNo = getYesOrNo();

    if (yesOrNo == 'Y'){

        clearBuffer();
        printf(CYAN "Ingrese la nueva ruta:\n" RESET_COLOR);
        fgets(basePath, sizeof(basePath), stdin);
        removeLineBreak(basePath);

        printf(GREEN "Ruta modificada exitosamente\n" RESET_COLOR);
    }
    else{

        printf(CYAN "Ok\n" RESET_COLOR);
    }
}

void mkdirCommand(){

    char nameDirectory[30];
    int check;

    printf(CYAN "Ingrese el nombre del directorio\n" RESET_COLOR);
    clearBuffer();
    fgets(nameDirectory, sizeof(nameDirectory), stdin);
    removeLineBreak(nameDirectory);

    check = mkdir(nameDirectory);

    if (!check){

        printf(GREEN "Directorio creado exitosamente\n" RESET_COLOR);
    }
    else{

        printf(RED "Error al crear el directorio\n" RESET_COLOR);
    }
}

void copyCommand(bool flag){

    int ch;
    FILE *fp1, *fp2;
    char source[MAX], target[MAX];

    clearBuffer();
    printf(CYAN "Ingrese la ruta donde se encuentra archivo:\n" RESET_COLOR);
    fgets(source, MAX, stdin);
    source[strlen(source) - 1] = '\0';

    printf(CYAN "Ingrese la ruta de destino:\n" RESET_COLOR);
    fgets(target, MAX, stdin);
    target[strlen(target) - 1] = '\0';

    fp1 = fopen(source, "r");
    fp2 = fopen(target, "w");

    if (!fp1){

        printf(RED "Imposible abrir el archivo fuente\n" RESET_COLOR);
        fclose(fp2);
        return;
    }

    if (!fp2) {

        printf(RED "Imposible abrir el directorio de destino\n" RESET_COLOR);
        return;
    }

    while ((ch = fgetc(fp1)) != EOF) {

        fputc(ch, fp2);
    }

    fclose(fp1);
    fclose(fp2);

    if (flag){

        remove(source);
        printf(GREEN "Operacion realizada exitosamente\n" RESET_COLOR);
        return;
    }

    printf(GREEN "Operacion realizada exitosamente\n" RESET_COLOR);
}

void rmdirCommand(){

    char source[MAX];
    char sourceAux[MAX];
    char sourceAuxTwo[MAX];

    clearBuffer();
    printf(CYAN "Ingrese la ruta del directorio:\n" RESET_COLOR);
    fgets(source, MAX, stdin);
    source[strlen(source) - 1] = '\0';

    strcpy(sourceAuxTwo, source);

    struct dirent *de;

	DIR *dr = opendir(source);

	if (dr == NULL){

		printf(RED "Error al abrir el directorio\n" RESET_COLOR);
		return;
	}

    strcat(source, "\\");
    strcpy(sourceAux, source);

	while ((de = readdir(dr)) != NULL){

        strcat(source, de->d_name);
        remove(source);
        strcpy(source, sourceAux);
    }

    rmdir(sourceAuxTwo);

	closedir(dr);

    printf(GREEN "Operacion realizada exitosamente\n" RESET_COLOR);
}

void treeCommandAux(char *basePath, const int root){

    int i;
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);

    if (!dir){

        return;
    }

    while ((dp = readdir(dir)) != NULL){

        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0){

            for (i = 0; i < root; i++){

                if (i%2 == 0 || i == 0){

                    printf("%c", 179);
                }
                else{

                    printf(" ");
                }
            }

            printf("%c%c%s\n", 195, 196, dp->d_name);

            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);
            treeCommandAux(path, root + 2);
        }
    }

    closedir(dir);
}

void treeCommand(){

    char path[100];

    printf(CYAN "Ingrese la ruta:\n" RESET_COLOR);
    scanf("%s", path);

    treeCommandAux(path, 0);
}

void dirCommand(){

    char source[MAX];

    printf(CYAN "Ingrese la ruta del directorio:\n" RESET_COLOR);
    clearBuffer();
    fgets(source, MAX, stdin);
    source[strlen(source) - 1] = '\0';

    struct dirent *de;

	DIR *dr = opendir(source);

	if (dr == NULL){

		printf(RED "Error al abrir el directorio\n" RESET_COLOR);
		return;
	}

	while ((de = readdir(dr)) != NULL){

        printf("%s\n", de->d_name);
    }

	closedir(dr);
}

void crfileCommand(){

    char source[MAX];

    FILE *ptrFile;

    printf(CYAN "Ingrese la ruta en donde desea alamcenar el archivo:\n" RESET_COLOR);
    clearBuffer();
    fgets(source, MAX, stdin);
    source[strlen(source) - 1] = '\0';

    ptrFile = fopen(source, "a+");

    if (ptrFile == NULL){

        printf(RED "Error al crear el archivo\n" RESET_COLOR);
        return;
    }

    fclose(ptrFile);

    printf(GREEN "Archivo creado exitosamente\n" RESET_COLOR);
}

void rmfileCommand(){

    char source[MAX];

    printf(CYAN "Ingrese la ruta del archivo:\n" RESET_COLOR);
    clearBuffer();
    fgets(source, MAX, stdin);
    source[strlen(source) - 1] = '\0';

    if (remove(source) == 0){

        printf(GREEN "Archivo eliminado exitosamente\n" RESET_COLOR);
    }
    else{

        printf(RED "Error al eliminar el archivo\n" RESET_COLOR);
    }
}

void renameCommand(){

    char source[MAX];

    printf(CYAN "Ingrese la ruta del archivo a renombrar:\n" RESET_COLOR);
    clearBuffer();
    fgets(source, MAX, stdin);
    source[strlen(source) - 1] = '\0';

    char newName[50];

    printf(CYAN "Ingrese la ruta con el nuevo nombre y extension del archivo:\n" RESET_COLOR);
    fgets(newName, sizeof(newName), stdin);
    newName[strlen(newName) - 1] = '\0';

    if (rename(source, newName) == 0){

        printf(GREEN "Archivo renombrado exitosamente\n" RESET_COLOR);
    }
    else{

        printf(RED "Error al renombrar el archivo\n" RESET_COLOR);
    }
}

/*
Entradas: el numero de opcion del menu asociado al comando y la ruta del directorio o archivo
Salidas: el resultado de la operacion realizada por el comando ejecutado
Restricciones: la ruta ingresada debe ser valida
*/

void fileOperations(){

    int command;

    do{
        printf(YELLOW "Operaciones disponibles\n" RESET_COLOR);
        printf(CYAN "1.  Cd\n" RESET_COLOR);
        printf(CYAN "2.  Mkdir\n" RESET_COLOR);
        printf(CYAN "3.  Copy\n" RESET_COLOR);
        printf(CYAN "4.  Move\n" RESET_COLOR);
        printf(CYAN "5.  Rmdir\n" RESET_COLOR);
        printf(CYAN "6.  Tree\n" RESET_COLOR);
        printf(CYAN "7.  Dir\n" RESET_COLOR);
        printf(CYAN "8.  Crfile\n" RESET_COLOR);
        printf(CYAN "9. Rmfile\n" RESET_COLOR);
        printf(CYAN "10. Rename\n" RESET_COLOR);
        printf(CYAN "11. Call\n" RESET_COLOR);
        printf(CYAN "12. Salir\n" RESET_COLOR);

        scanf("%d", &command); 

        switch (command){

            case 1:
                cdCommand();
                break;
            case 2:
                mkdirCommand();
                break;
            case 3:
                copyCommand(false);
                break;
            case 4:
                copyCommand(true);
                break;
            case 5:
                rmdirCommand();
                break;
            case 6:
                treeCommand();
                break;
            case 7:
                dirCommand();
                break;
            case 8:
                crfileCommand();
                break;
            case 9:
                rmfileCommand();
                break;
            case 10:
                renameCommand();
                break;
            case 11:
                crfileCommand();
                break;
            case 12:
                printf(YELLOW "Saliendo...\n" RESET_COLOR);
                break;
            default:
                printf(RED "Operacion incorrecta" CYAN "Intente de nuevo\n" RESET_COLOR);
                break;
        }

    } while (command != 12 && command != EOF);
    
}

//----------Registrar domicilo de colaboradores----------

/*
Entradas: numero de cedula del colaborador, lugar de domicilio y codigo postal del lugar de domicilio
Salidas: el lugar de domicilio registrado en la lista de lugares de residencia
Restricciones: numero de cedula de cada miembro debe tener 9 digitos, el domicilio debe conformarse de letras 
               y el codigo postal debe estar compuesto por 5 digitos
*/

typedef struct homeList{
    char identificationCard[15];
    char place[100];
    int postalCode;
    struct homeList* next;
}homeNode;

homeNode* createHomeNode(char newID[], char newPlace[], int newPostalCode){

	homeNode *newNode = (homeNode *) malloc(sizeof(homeNode));

	newNode->next = NULL;

    strcpy(newNode->identificationCard, newID);
	strcpy(newNode->place, newPlace);
    newNode->postalCode = newPostalCode;
	
	return newNode;
}

void uploadHome(homeNode** homeList){

    FILE * ptrFile = fopen("HOME.csv", "r");

    char data[3][120];
    char line[330];
    int count;

    while (fgets(line, sizeof(line), ptrFile)){

        char *token;

        token = strtok(line, ",");
        count = 0;

        while (token != NULL){

            strcpy(data[count], token);
            token = strtok(NULL, ",");
            count++;
        }

        homeNode *newNode = createHomeNode(data[0], data[1], atoi(data[2]));

        if (*homeList == NULL){
            *homeList = newNode;
        }
        else{
            homeNode *last = *homeList;

            while (last->next != NULL){
                last = last->next;
            }

            last->next = newNode;
        }  
    }

    fclose(ptrFile);
}

void appendHome(homeNode **homeList){

    char currentID[15];
    bool resID;
    printf(CYAN "Ingrese la cedula del colaborador\n" RESET_COLOR);

    while (true){

        fgets(currentID, sizeof(currentID), stdin);
        removeLineBreak(currentID);

        resID = validateIdentificationCard(currentID);

        if (resID){
            break;
        }
        else{
            printf(RED "El numero de cedula es invalido\n" CYAN "Intente de nuevo:\n" RESET_COLOR);
        }
    }
    
    char currentPlace[100];
    bool resPlace;
    printf(CYAN "Ingrese el lugar de residencia:\n" RESET_COLOR);
    
    while (true){

        fgets(currentPlace, sizeof(currentPlace), stdin);
        removeLineBreak(currentPlace);

        resPlace = validateIsAlpha(currentPlace);

        if (resPlace){
            break;
        }
        else{
            printf(RED "El lugar de residencia es invalido\n" CYAN "Intente de nuevo:\n" RESET_COLOR);
        }
    }

    char currentPostalCode[10];
    bool resPostalCode;
    printf(CYAN "Ingrese el codigo postal del lugar de residencia\n" RESET_COLOR);

    while (true){

        fgets(currentPostalCode, sizeof(currentPostalCode), stdin);
        removeLineBreak(currentPostalCode);

        resPostalCode = validatePostalCode(currentPostalCode);

        if (resPostalCode){
            break;
        }
        else{
            printf(RED "El codigo postal es invalido\n" CYAN "Intente de nuevo:\n" RESET_COLOR);
        }
    }

    homeNode *newNode = createHomeNode(currentID, currentPlace, atoi(currentPostalCode));
    homeNode *last = *homeList;

    if (*homeList == NULL){
        *homeList = newNode;
        printf(GREEN "Lugar de residencia registrado exitosamente\n" RESET_COLOR);
        return;
    }

    while (last->next != NULL){
        last = last->next;
    }

    last->next = newNode;
    printf(GREEN "Lugar de residencia registrado exitosamente\n" RESET_COLOR);

    clearBuffer();
    return;
}

//----------Modificar domicilo de colaboradores----------

/*
Entradas: numero de cedula del colaborador, nuevo lugar de domicilio y nuevo codigo postal del lugar de domicilio
Salidas: el lugar de domicilio modificado en la lista de lugares de residencia
Restricciones: numero de cedula de cada miembro debe tener 9 digitos, el domicilio debe conformarse de letras 
               y el codigo postal debe estar compuesto por 5 digitos
*/

void modifyHome(homeNode **homeList){

    homeNode *ptrHome = *homeList;

    char currentID[15];
    bool resID;
    printf(CYAN "Ingrese la cedula del colaborador cuya residencia desea modificar:\n" RESET_COLOR);

    while (true){

        fgets(currentID, sizeof(currentID), stdin);
        removeLineBreak(currentID);

        resID = validateIdentificationCard(currentID);

        if (resID){
            break;
        }
        else{
            printf(RED "El numero de cedula es invalido\n" CYAN "Intente de nuevo:\n" RESET_COLOR);
        }
    }

    char newPlace[50], strPostalCode[10];
    char yesOrNo_1, yesOrNo_2;
    bool resPlace, resPostalCode;

    while (ptrHome != NULL){

        if (strcmp(ptrHome->identificationCard, currentID) == 0){

            printf(CYAN "Quieres modificar la residencia? Ingrese Y para afirmar o N para negar\n" RESET_COLOR);
            yesOrNo_1 = getYesOrNo();

            if (yesOrNo_1 == 'Y'){

                clearBuffer();
                printf(CYAN "Ingrese el nuevo lugar de residencia:\n" RESET_COLOR);
                
                while (true){

                    fgets(newPlace, sizeof(newPlace), stdin);
                    removeLineBreak(newPlace);

                    resPlace = validateIsAlpha(newPlace);

                    if (resPlace){
                        break;
                    }
                    else{
                        printf(RED "El lugar de residencia es invalido\n" CYAN "Intente de nuevo:\n" RESET_COLOR);
                    }
                }
                
                strcpy(ptrHome->place, newPlace);

            }else{

                printf(CYAN "Ok\n" RESET_COLOR);
                clearBuffer();
            }

            printf(CYAN "Quieres modificar el codigo postal? Ingrese Y para afirmar o N para negar:\n" RESET_COLOR);
            yesOrNo_2 = getYesOrNo();

            if (yesOrNo_2 == 'Y'){

                clearBuffer();
                printf(CYAN "Ingrese el nuevo codigo postal:\n" RESET_COLOR);

                while (true){

                    fgets(strPostalCode, sizeof(strPostalCode), stdin);
                    removeLineBreak(strPostalCode);

                    resPostalCode = validatePostalCode(strPostalCode);

                    if (resPostalCode){
                        break;
                    }
                    else{
                        printf(RED "El codigo postal es invalido\n" CYAN "Intente de nuevo:\n" RESET_COLOR);
                    }
                }

                ptrHome->postalCode = atoi(strPostalCode);
            }
            else{

                printf(CYAN "Ok\n" RESET_COLOR);
                clearBuffer();
            }

            printf(GREEN "Residencia del colaborador modificada exitosamente\n" RESET_COLOR);

            return;
        }

        ptrHome = ptrHome->next;
    }

    if (ptrHome == NULL){

        printf(RED "Residencia del colaborador no encontrada\n" RESET_COLOR);
        return;
    }
}

//----------Consultar domicilo de colaboradores----------

/*
Entradas: la lista de lugares de domicilio
Salidas: los domicilios registrados en la lista de lugares de residencia
Restricciones: la lista de lugares de domicilio no debe estar vacia
*/

void consultHome(homeNode *homeList){

    homeNode *i;

    if (homeList == NULL){

        printf(RED "No hay residencias registradas\n" RESET_COLOR);
        return;
    }

    for (i = homeList; i != NULL; i = i->next){

        printf(YELLOW "     Cedula: %s     \n" RESET_COLOR, i->identificationCard);
        printf(CYAN "Lugar de residencia:" RESET_COLOR " %s\n", i->place);
        printf(CYAN "Codigo postal:" RESET_COLOR " %d\n", i->postalCode);
    }

    printf("\n");
}

//----------Eliminar domicilo de colaboradores----------

/*
Entradas: numero de cedula del colaborador cuya residencia se desea eliminar
Salidas: el lugar de domicilio eliminado en la lista de lugares de residencia
Restricciones: numero de cedula de cada miembro debe tener 9 digitos
*/

void deleteHome(homeNode **homeList){

    char currentID[15];
    bool resID;

    printf(CYAN "Ingrese la cedula del colaborador cuya residencia eliminar:\n" RESET_COLOR);
    
    while (true){

        fgets(currentID, sizeof(currentID), stdin);
        removeLineBreak(currentID);

        resID = validateIdentificationCard(currentID);

        if (resID){
            break;
        }
        else{
            printf(RED "El numero de cedula es invalido\n" CYAN "Intente de nuevo:\n" RESET_COLOR);
        }
    }

    homeNode *currentHome = *homeList, *previous;

    if (currentHome != NULL && strcmp(currentHome->identificationCard, currentID) == 0){

        *homeList = currentHome->next;
        printf(GREEN "Se ha eliminado la residencia del colaborador con cedula %s\n" RESET_COLOR, currentHome->identificationCard);
        free(currentHome);
        return;
    }

    while (currentHome != NULL && strcmp(currentHome->identificationCard, currentID) != 0){

        previous = currentHome;
        currentHome = currentHome->next;
    }

    if (currentHome == NULL){

        printf(RED "Residencia del colaborador no encontrada\n" RESET_COLOR);
        return;
    }

    previous->next = currentHome->next;

    printf(GREEN "Se ha eliminado la residencia del colaborador con cedula %s\n" RESET_COLOR, currentHome->identificationCard);

    free(currentHome);
}

//----------Registrar nueva ruta para el paseo----------

/*
Entradas: origen, destino, tiempo, distancia y tipo de ruta
Salidas: la ruta registrada en la lista de rutas del paseo
Restricciones: el origen debe conformase de letras, el destino debe conformase de letras, el tiempo debe ser un numero, 
               la distancia debe ser un numero y el tipo de ruta debe ser un caracter T o A o M segun corresponda
*/

typedef struct routeList{
    int origin;
    int destination;
    int time;
    int distance;
    char routeType;
    struct routeList* next;
}routeNode;

routeNode* createRouteNode(int newOrigin, int newDestination, int newTime, int newDistance, char newType){

	routeNode *newNode = (routeNode *) malloc(sizeof(routeNode));

	newNode->next = NULL;

    newNode->origin = newOrigin;
    newNode->destination = newDestination;
    newNode->time = newTime;
    newNode->distance = newDistance;
    newNode->routeType = newType;
	
	return newNode;
}

void uploadRoute(routeNode** routeList){

    FILE * ptrFile = fopen("ROUTES.csv", "r");

    char data[5][120];
    char line[330];
    int count;

    while (fgets(line, sizeof(line), ptrFile)){

        char *token;

        token = strtok(line, ",");
        count = 0;

        while (token != NULL){

            strcpy(data[count], token);
            token = strtok(NULL, ",");
            count++;
        }
        char aux[1];
        
        strcpy(aux, data[4]);
        routeNode *newNode = createRouteNode(atoi(data[0]), atoi(data[1]), atoi(data[2]), atoi(data[3]), aux[0]);

        if (*routeList == NULL){
            *routeList = newNode;
        }
        else{
            routeNode *last = *routeList;

            while (last->next != NULL){
                last = last->next;
            }

            last->next = newNode;
        }  
    }

    fclose(ptrFile);
}

int convertPlaceToPostalCode(homeNode **homeList, char placeToConvert[]){

    homeNode *i;
    int result;

    for (i = *homeList; i != NULL; i = i->next){

        if (strcmp(i->place, placeToConvert) == 0){

            result = i->postalCode;
            return result;
        }
    }

    return -1;
}

char * convertPostalCodeToPlace(homeNode **homeList, int postalCodeToConvert){

    homeNode *i;
    char* result;
    char* fail = "-1";

    for (i = *homeList; i != NULL; i = i->next){

        if (i->postalCode == postalCodeToConvert){

            result = i->place;
            return result;
        }
    }

    return fail;
}

int getOrigin(homeNode **homeList){

    char strOrigin[100];
    int origin;
    bool resOrigin;

    printf(CYAN "Ingrese el lugar de origen:\n" RESET_COLOR);
    
    while (true){

        fgets(strOrigin, sizeof(strOrigin), stdin);
        removeLineBreak(strOrigin);

        resOrigin = validateIsAlpha(strOrigin);

        if (resOrigin){
            break;
        }
        else{
            printf(RED "El lugar de origen es invalido\n" CYAN "Intente de nuevo:\n" RESET_COLOR);
        }
    }

    origin = convertPlaceToPostalCode(homeList, strOrigin);

    return origin;
}

int getDestination(homeNode **homeList){

    char strDestination[100];
    int destination;
    bool resDestination;

    printf(CYAN "Ingrese el lugar de destino:\n" RESET_COLOR);
    
    while (true){

        fgets(strDestination, sizeof(strDestination), stdin);
        removeLineBreak(strDestination);

        resDestination = validateIsAlpha(strDestination);

        if (resDestination){
            break;
        }
        else{
            printf(RED "El lugar de destino es invalido\n" CYAN "Intente de nuevo:\n" RESET_COLOR);
        }
    }

    destination = convertPlaceToPostalCode(homeList, strDestination);

    return destination;
}

int getEstimatedTime(){

    char strTime[5];
    int time;
    bool resTime;

    printf(CYAN "Ingrese el tiempo en minutos que se tarda en llegar al destino:\n" RESET_COLOR);

    while (true){

        fgets(strTime, sizeof(strTime), stdin);
        removeLineBreak(strTime);

        resTime = validateIsNum(strTime);

        if (resTime){
            break;
        }
        else{
            printf(RED "Los minutos indicados son invalidos\n" CYAN "Intente de nuevo:\n" RESET_COLOR);
        }
    }

    time = atoi(strTime);

    return time;
}

int getEstimatedDistance(){

    char strDistance[10];
    int distance;
    bool resDistance;

    printf(CYAN "Ingrese la distancia en kilometros de la ruta:\n" RESET_COLOR);

    while (true){

        fgets(strDistance, sizeof(strDistance), stdin);
        removeLineBreak(strDistance);

        resDistance = validateIsNum(strDistance);

        if (resDistance){
            break;
        }
        else{
            printf(RED "La distancia es invalida\n" CYAN "Intente de nuevo:\n" RESET_COLOR);
        }
    }

    distance = atoi(strDistance);

    return distance;
}

char getTypeRoute(){

    char typeRoute;

    printf(YELLOW "Tipos de ruta disponibles: T -> Terrestre / A -> Aerea / M -> Maritima\n" RESET_COLOR);
    printf(CYAN "Ingrese la letra correspondiente segun el tipo de ruta:\n" RESET_COLOR);
    
    while(true){

        typeRoute = getchar();

        if (typeRoute == 'T' || typeRoute == 'A' || typeRoute == 'M'){

            break;

        }else{

            printf(RED "Letra invalida\n" CYAN "Por favor ingrese T o A o M:" RESET_COLOR "\n");
        }
    }

    return typeRoute;
}

void appendRoute(routeNode **routeList, homeNode **homeList){

    int origin = getOrigin(homeList);
    int destination = getDestination(homeList);
    int estimatedTime = getEstimatedTime();
    int estimatedDistance = getEstimatedDistance();
    char typeRoute = getTypeRoute();

    routeNode *newNode = createRouteNode(origin, destination, estimatedTime, estimatedDistance, typeRoute);
    routeNode *last = *routeList;

    if (*routeList == NULL){
        *routeList = newNode;
        printf(GREEN "Ruta registrada exitosamente\n" RESET_COLOR);
        return;
    }

    while (last->next != NULL){
        last = last->next;
    }

    last->next = newNode;
    printf(GREEN "Ruta registrada exitosamente\n" RESET_COLOR);

    clearBuffer();
    return;
}

//----------Modificar nueva ruta para el paseo----------

/*
Entradas: nuevo origen,  nuevo destino, nuevo tiempo, nueva distancia y nuevo tipo de ruta
Salidas: la ruta modificada en la lista de rutas del paseo
Restricciones: el origen debe conformase de letras, el destino debe conformase de letras, el tiempo debe ser un numero, 
               la distancia debe ser un numero y el tipo de ruta debe ser un caracter T o A o M segun corresponda
*/

void modifyRoutes(routeNode **routeList, homeNode **homeList){

    int originToModify = getOrigin(homeList);
    int destinationToModify = getDestination(homeList);
    char typeRouteToModify = getTypeRoute();

    int newOrigin, newDestination, newTime, newDistance;
    char newTypeRoute;

    routeNode *ptrRoute = *routeList;
    char yesOrNo_1, yesOrNo_2, yesOrNo_3, yesOrNo_4, yesOrNo_5;

    while (ptrRoute != NULL){

        if ((ptrRoute->origin == originToModify) && (ptrRoute->destination == destinationToModify) && (ptrRoute->routeType == typeRouteToModify)){

            printf(CYAN "Quieres modificar el origen? Ingrese Y para afirmar o N para negar\n" RESET_COLOR);
            yesOrNo_1 = getYesOrNo();

            if (yesOrNo_1 == 'Y'){

                clearBuffer();
                newOrigin = getOrigin(homeList);
                ptrRoute->origin = newOrigin;
            }
            else{

                printf(CYAN "Ok\n" RESET_COLOR);
                clearBuffer();
            }
            
            printf(CYAN "Quieres modificar el destino? Ingrese Y para afirmar o N para negar\n" RESET_COLOR);
            yesOrNo_2 = getYesOrNo();

            if (yesOrNo_2 == 'Y'){

                clearBuffer();
                newDestination = getDestination(homeList);
                ptrRoute->destination = newDestination;
            }
            else{

                printf(CYAN "Ok\n" RESET_COLOR);
                clearBuffer();
            }
            
            printf(CYAN "Quieres modificar el tiempo estimado? Ingrese Y para afirmar o N para negar\n" RESET_COLOR);
            yesOrNo_3 = getYesOrNo();

            if (yesOrNo_3 == 'Y'){

                clearBuffer();
                newTime = getEstimatedTime();
                ptrRoute->time = newTime;
            }
            else{

                printf(CYAN "Ok\n" RESET_COLOR);
                clearBuffer();
            }
            
            printf(CYAN "Quieres modificar la distancia estimada? Ingrese Y para afirmar o N para negar\n" RESET_COLOR);
            yesOrNo_4 = getYesOrNo();

            if (yesOrNo_4 == 'Y'){

                clearBuffer();
                newDistance = getEstimatedDistance();
                ptrRoute->distance = newDistance;
            }
            else{

                printf(CYAN "Ok\n" RESET_COLOR);
                clearBuffer();
            }
            
            printf(CYAN "Quieres modificar el tipo de ruta? Ingrese Y para afirmar o N para negar\n" RESET_COLOR);
            yesOrNo_5 = getYesOrNo();

            if (yesOrNo_5 == 'Y'){

                clearBuffer();
                newTypeRoute = getTypeRoute();
                ptrRoute->routeType = newTypeRoute;
            }
            else{

                printf(CYAN "Ok\n" RESET_COLOR);
                clearBuffer();
            }

            printf(GREEN "Una de las rutas del paseo ha sido modificada exitosamente\n" RESET_COLOR);

            return;
        }

        ptrRoute = ptrRoute->next;
    }
    
    if (ptrRoute == NULL){

        printf(RED "Ruta no encontrada\n" RESET_COLOR);
        return;
    }
}

//----------Consultar nueva ruta para el paseo----------

/*
Entradas: la lista de rutas de paseo y la lista de domicilios
Salidas: muestra las rutas registradas en la lista de rutas del paseo
Restricciones: las listas recibidas como entrada no deben estar vacias
*/

void consultRoutes(routeNode **routeList, homeNode **homeList){

    routeNode *i;
    char* strOrigin; 
    char* strDestination;

    for (i = *routeList; i != NULL; i = i->next){

        strOrigin = convertPostalCodeToPlace(homeList, i->origin);
        printf(CYAN "Origen:" RESET_COLOR " %s\n", strOrigin);

        strDestination = convertPostalCodeToPlace(homeList, i->destination);
        printf(CYAN "Destino:" RESET_COLOR " %s\n", strDestination);

        printf(CYAN "Tiempo:" RESET_COLOR " %d minutos\n", i->time);
        printf(CYAN "Distancia:" RESET_COLOR " %d km\n", i->distance);

        if (i->routeType == 'T'){

            printf(CYAN "Tipo:" RESET_COLOR " Terrestre\n");
        }
        else if (i->routeType == 'A'){

            printf(CYAN "Tipo:" RESET_COLOR " Aerea\n");
        }
        else if (i->routeType == 'M'){

            printf(CYAN "Tipo:" RESET_COLOR " Maritima\n");
        }

        printf("\n");
    }
}

//----------Eliminar nueva ruta para el paseo----------

/*
Entradas: origen, destino y tipo de ruta de la ruta que se desea eliminar
Salidas: la ruta eliminada en la lista de rutas del paseo
Restricciones: el origen debe conformase de letras, el destino debe conformase de letras
               y el tipo de ruta debe ser un caracter T o A o M segun corresponda
*/

void deleteRoutes(routeNode **routeList, homeNode **homeList){

    int originToDelete = getOrigin(homeList);
    int destinationToDelete = getDestination(homeList);
    char typeRouteToDelete = getTypeRoute();

    routeNode *currentRoute = *routeList, *previous;

    if ((currentRoute != NULL) && (currentRoute->origin == originToDelete) && (currentRoute->destination == destinationToDelete) && (currentRoute->routeType == typeRouteToDelete)){

        *routeList = currentRoute->next;
        printf(GREEN "Se ha eliminado la ruta indicada\n" RESET_COLOR);
        free(currentRoute);
        return;
    }

    while ((currentRoute != NULL) && (currentRoute->origin != originToDelete) && (currentRoute->destination != destinationToDelete) && (currentRoute->routeType != typeRouteToDelete)){

        previous = currentRoute;
        currentRoute = currentRoute->next;
    }

    if (currentRoute == NULL){

        printf(RED "Ruta no encontrada\n" RESET_COLOR);
        return;
    }

    previous->next = currentRoute->next;

    printf(GREEN "Se ha eliminado la ruta indicada\n" RESET_COLOR);

    free(currentRoute);
}

int getRoutesQuantity(routeNode** routeList){

    int size = 0;
    routeNode* current = *routeList;

    while (current != NULL){
        size++;
        current = current->next;
    }

    return size;
}

int getQuantityRoutesByClasification(routeNode** routeList, char clasification){

    int size = 0;
    routeNode* current = *routeList;

    while (current != NULL){

        if (current->routeType == clasification){

            size++;
        }

        current = current->next;
    }

    return size;
}

typedef struct Graph{
    struct Node* head[MAX];
}Graph;

typedef struct Node{
    int src, dest, weight;
    struct Node* next;
}Node;

typedef struct Edge{
    int src, dest, weight, position;
}Edge;

Graph* createGraph(Edge edges[], int n, routeNode **routeList, int routesQuantity){
    
    Graph* graph = (Graph*) malloc(sizeof(Graph));
 
    for (int i = 0; i < routesQuantity; i++){

        graph->head[i] = NULL;
    }

    for (int i = 0; i < n; i++){

        int src = edges[i].src;
        int dest = edges[i].dest;
        int weight = edges[i].weight;
        int position = edges[i].position;

        Node* newNode = (Node*) malloc(sizeof(Node));

        newNode->src = src;
        newNode->dest = dest;
        newNode->weight = weight;

        newNode->next = graph->head[position];

        graph->head[position] = newNode;
    }

    return graph;
}

void auxPrintGraph(routeNode *routeList, homeNode **homeList, int source, int destination, int weight){

    routeNode *i;

    char* strOriginPlace;
    char* strDestinationPlace;

    for (i = routeList; i != NULL; i = i->next){

        if (i->origin == source && i->destination == destination && i->distance == weight){

            strOriginPlace = convertPostalCodeToPlace(homeList, i->origin);
            strDestinationPlace = convertPostalCodeToPlace(homeList, i->destination);

            printf("%s -> %s -> %d km\t", strOriginPlace, strDestinationPlace, i->distance);
            break;
        }
    }
}

int printGraph(Graph* graph, routeNode *routeList, homeNode **homeList, int routesQuantity){

    int count = 1;

    for (int i = 0; i < routesQuantity; i++){
        
        Node* ptr = graph->head[i];

        while (ptr != NULL){

            auxPrintGraph(routeList, homeList, ptr->src, ptr->dest, ptr->weight);
            count++;
            ptr = ptr->next;
        }
 
        printf("\n");
    }

    return count;
}

//----------Simular ruta para el paseo / funciones auxiliares----------

typedef struct AdjListNode{
	int dest;
	int weight;
	struct AdjListNode* next;
}AdjListNode;

typedef struct AdjList{
    struct AdjListNode *head;
}AdjList;

typedef struct dijkstraGraph{
	int V; //V = vertices 
	struct AdjList* array;
}dijkstraGraph;

AdjListNode* newAdjListNode(int dest, int weight){

	AdjListNode* newNode = (AdjListNode*) malloc(sizeof(AdjListNode));

	newNode->dest = dest;
	newNode->weight = weight;
	newNode->next = NULL;

	return newNode;
}

dijkstraGraph* createDijkstraGraph(int V){

	dijkstraGraph* graph = (dijkstraGraph*) malloc(sizeof(dijkstraGraph));
	graph->V = V;

	graph->array = (AdjList*) malloc(V * sizeof(AdjList));

	for (int i = 0; i < V; ++i)
		graph->array[i].head = NULL;

	return graph;
}

void addDijkstraEdge(dijkstraGraph* graph, int src, int dest, int weight){

	AdjListNode* newNode = newAdjListNode(dest, weight);

	newNode->next = graph->array[src].head;
	graph->array[src].head = newNode;

	newNode = newAdjListNode(src, weight);

	newNode->next = graph->array[dest].head;
	graph->array[dest].head = newNode;
}

typedef struct MinHeapNode{
	int v; //v = vertices
	int dist;
}MinHeapNode;

typedef struct MinHeap{
	int size;	
	int capacity;
	int *pos; //pos = posicion
	struct MinHeapNode **array;
}MinHeap;

MinHeapNode* newMinHeapNode(int v, int dist){

	MinHeapNode* minHeapNode = ( MinHeapNode*) malloc(sizeof( MinHeapNode));

	minHeapNode->v = v;
	minHeapNode->dist = dist;
	return minHeapNode;
}

MinHeap* createMinHeap(int capacity){
	MinHeap* minHeap = ( MinHeap*) malloc(sizeof( MinHeap));
	minHeap->pos = (int *)malloc(capacity * sizeof(int));

	minHeap->size = 0;
	minHeap->capacity = capacity;
	minHeap->array = ( MinHeapNode**) malloc(capacity *sizeof( MinHeapNode*));

	return minHeap;
}

void swapMinHeapNode( MinHeapNode** a, MinHeapNode** b){
	MinHeapNode* t = *a;
	*a = *b;
	*b = t;
}

void minHeapify(MinHeap* minHeap, int idx){

	int smallest, left, right;

	smallest = idx;
	left = 2 * idx + 1;
	right = 2 * idx + 2;

	if (left < minHeap->size && minHeap->array[left]->dist < minHeap->array[smallest]->dist){

        smallest = left;
    }
	
	if (right < minHeap->size && minHeap->array[right]->dist < minHeap->array[smallest]->dist){

        smallest = right;
    }
	
	if (smallest != idx){
		
		MinHeapNode *smallestNode = minHeap->array[smallest];
		MinHeapNode *idxNode = minHeap->array[idx];

		minHeap->pos[smallestNode->v] = idx;
		minHeap->pos[idxNode->v] = smallest;

		swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);

		minHeapify(minHeap, smallest);
	}
}

int isEmpty(MinHeap* minHeap){

	return minHeap->size == 0;
}

MinHeapNode* extractMin(MinHeap* minHeap){

	if (isEmpty(minHeap)){

        return NULL;
    }

	MinHeapNode* root = minHeap->array[0];

	MinHeapNode* lastNode = minHeap->array[minHeap->size - 1];
	minHeap->array[0] = lastNode;

	minHeap->pos[root->v] = minHeap->size-1;
	minHeap->pos[lastNode->v] = 0;

	--minHeap->size;

	minHeapify(minHeap, 0);

	return root;
}

void decreaseKey(MinHeap* minHeap, int v, int dist){
	
	int i = minHeap->pos[v];
	
	minHeap->array[i]->dist = dist;

	while (i && minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist){
		
		minHeap->pos[minHeap->array[i]->v] = (i-1)/2;

		minHeap->pos[minHeap->array[(i-1)/2]->v] = i;

		swapMinHeapNode(&minHeap->array[i],&minHeap->array[(i - 1) / 2]);

		i = (i - 1) / 2;
	}
}

bool isInMinHeap(MinHeap *minHeap, int v){

    if (minHeap->pos[v] < minHeap->size){

        return true;
    }
	
    return false;
}

void printArr(int dist[], int n){

	printf(CYAN "Punto \t" WHITE "|" CYAN "   Distancia en km\n" RESET_COLOR);

	for (int i = 0; i < n; ++i){

        printf("  %d \t|\t %d\n", i, dist[i]);
    }
}

void dijkstra(dijkstraGraph* graph, int src){
	
	int V = graph->V; //v = vertices

	int dist[V];	

	MinHeap* minHeap = createMinHeap(V);

	for (int v = 0; v < V; ++v){

		dist[v] = INT_MAX;
		minHeap->array[v] = newMinHeapNode(v,dist[v]);
		minHeap->pos[v] = v;
	}

	minHeap->array[src] = newMinHeapNode(src, dist[src]);
	minHeap->pos[src] = src;
	dist[src] = 0;

	decreaseKey(minHeap, src, dist[src]);

	minHeap->size = V;

	while (!isEmpty(minHeap)){
		
		MinHeapNode* minHeapNode = extractMin(minHeap);
	
		int u = minHeapNode->v;

		AdjListNode* pCrawl = graph->array[u].head;

		while (pCrawl != NULL){

			int v = pCrawl->dest;
            
			if (isInMinHeap(minHeap, v) && dist[u] != INT_MAX && pCrawl->weight + dist[u] < dist[v]){

				dist[v] = dist[u] + pCrawl->weight;
				decreaseKey(minHeap, v, dist[v]);
			}
			pCrawl = pCrawl->next;
		}
	}

	printArr(dist, V);
}

void printColaboratorInPlace(homeNode *homeList, routeNode *routeList ,collaboratorsNode *collaboratorsList, char type){

    homeNode *k;
    routeNode *i;
    collaboratorsNode *j;

    char* strPlace;

    printf(CYAN "Lugar \t" WHITE "|" CYAN "   Colaborador visitado\n" RESET_COLOR);

    if (type == 'N'){

        for (i = routeList; i != NULL; i = i->next){

            strPlace = convertPostalCodeToPlace(&homeList ,i->origin);

            for (k = homeList; k != NULL; k = k->next){

                if (strcmp(k->place, strPlace) == 0){

                    for (j = collaboratorsList; j != NULL; j = j->next){

                        if (strcmp(j->identificationCard, k->identificationCard) == 0){

                            printf("%s \t|\t %s %s %s\n", strPlace, j->name, j->lastName, j->SecondLastName);
                        }
                    }
                }
            }
        }
    }
    else{
        for (i = routeList; i != NULL; i = i->next){

            strPlace = convertPostalCodeToPlace(&homeList ,i->origin);

            for (k = homeList; k != NULL; k = k->next){

                if (strcmp(k->place, strPlace) == 0 && i->routeType == type){

                    for (j = collaboratorsList; j != NULL; j = j->next){

                        if (strcmp(j->identificationCard, k->identificationCard) == 0){

                            printf("%s \t|\t %s %s %s\n", strPlace, j->name, j->lastName, j->SecondLastName);
                        }
                    }
                }
            }
        }
    }
}

//----------Simular ruta para el paseo----------

/*
Entradas: el tipo de ruta -> terrestre, aerea, maritima o todas las anteriores
Salidas: la ruta mas corta desde el primer lugar registrado, los colaboradores que se visitan en cada punto y el mapa de la ruta. 
Restricciones: el tipo de ruta debe ser un caracter: T -> terrestre / A -> Aerea / M -> Maritima / N -> Todas las anteriores
*/

int simulateRoute(routeNode **routeList, homeNode **homeList, collaboratorsNode **collaboratorsList, char clasification){

    int routesQuantity, count;

    if (clasification == 'N'){

        routesQuantity = getRoutesQuantity(routeList);
    }
    else{

        routesQuantity = getQuantityRoutesByClasification(routeList, clasification);
    }
    
    Edge edges[routesQuantity];
    routeNode *i;
    int j = 0, k, size;
    Graph *graph;

    if (clasification == 'N'){  

        for (i = *routeList; i != NULL; i = i->next){

            edges[j].src = i->origin;
            edges[j].dest = i->destination;
            edges[j].weight = i->distance;
            edges[j].position = j;

            j++;
        }
    }
    else{

        for (i = *routeList; i != NULL; i = i->next){

            if (i->routeType == clasification){

                edges[j].src = i->origin;
                edges[j].dest = i->destination;
                edges[j].weight = i->distance;
                edges[j].position = j;

                j++;
            }
        }
    }

    size = sizeof(edges)/sizeof(edges[0]);

    graph = createGraph(edges, size, routeList, routesQuantity);
                    
    dijkstraGraph* dijkstraGraph = createDijkstraGraph(routesQuantity);

    Node *ptr;

    for (k = 0; k < routesQuantity; k++){

        ptr = graph->head[k];

        addDijkstraEdge(dijkstraGraph, k, k + 1, ptr->weight);
    }  

    dijkstra(dijkstraGraph, 0);

    printColaboratorInPlace(*homeList, *routeList , *collaboratorsList, clasification);

    printf(YELLOW "\nMapa de la ruta del paseo:\n\n" RESET_COLOR);
    count = printGraph(graph, *routeList, homeList, routesQuantity);

    return count;
}

//----------chat----------

int chatCounter = 0;

void chatSystem(char senderName[], char senderLastName[], char senderSecLastname[], char receiverName[], char receiverLastName[], char receiverSecLastname[]){

    char message[MAXCHAT];

    printf(YELLOW "\t\tSistema de chat entre colaboradores\n\n" RESET_COLOR);
    
    printf(CYAN "Presione enter para insertar un salto de linea\n" RESET_COLOR);
    printf(CYAN "Al finalizar el mensaje escriba - en una nueva linea\n" RESET_COLOR);
    printf(CYAN "Ingrese // para cerrar el chat\n\n" RESET_COLOR);

    while (strcmp(message, "//") != 0){

		strcpy(message, "");

		printf(CYAN "Remitente:" RESET_COLOR " %s %s %s\n", senderName, senderLastName, senderSecLastname);

		while (strcmp(message, "-") != 0){

			printf (YELLOW "\n  --> " RESET_COLOR);

			fflush(stdin);
			fgets(message, sizeof(message), stdin);
			strtok(message, "\n");

			chatCounter++;

			if (strcmp(message, "-") == 0){

                chatCounter--;
            }
			if (strcmp(message, "//") == 0){

                break; 
            }	
		}
		
		if (strcmp(message, "//") == 0){

            break;
        }
		
		strcpy(message, "");
		
		printf(CYAN "Destinatario:" RESET_COLOR " %s %s %s\n", receiverName, receiverLastName, receiverSecLastname);

		while (strcmp(message, "-") != 0){

			printf (YELLOW "\n  --> " RESET_COLOR);

			fflush(stdin);
			fgets(message, sizeof(message), stdin);
			strtok(message, "\n");

			chatCounter++;

			if (strcmp(message, "-") == 0){

                chatCounter--;
            }
			if (strcmp(message, "//") == 0){

                break;
            }
		}
	}

	chatCounter--; 
}

/*
Entradas: numero de cedula del emisor, numero de cedula del receptor y nombre del equipo al que pertenecen
Salidas: sistema de chat en enjecucion
Restricciones: el numero de cedula debe conformarse de 9 digitos y el equipo debe estar registrado en la lista de equipos
*/

void chat(teamNode *teamList, collaboratorsNode *collaboratorsList){

    teamNode *i;
    bool senderFlag = false, receiverFlag = false;

    printf(YELLOW "Para el sistema de chat se necesita conocer el numero de cedula del emisor y el receptor junto con el nombre del equipo al que pertenecen\n" RESET_COLOR);

    char currentName[50];
    bool resName;
    printf(CYAN "Ingrese el nombre del equipo:\n" RESET_COLOR);
    
    while (true){

        fgets(currentName, sizeof(currentName), stdin);
        removeLineBreak(currentName);

        resName = validateIsAlpha(currentName);

        if (resName){
            break;
        }
        else{
            printf(RED "El nombre es invalido\n" CYAN "Intente de nuevo:\n" RESET_COLOR);
        }
    }

    char currentSenderID[15];
    bool resSenderID;

    printf(CYAN "Ingrese la cedula del colaborador remitente:\n" RESET_COLOR);
    
    while (true){

        fgets(currentSenderID, sizeof(currentSenderID), stdin);
        removeLineBreak(currentSenderID);

        resSenderID = validateIdentificationCard(currentSenderID);

        if (resSenderID){
            break;
        }
        else{
            printf(RED "El numero de cedula es invalido\n" CYAN "Intente de nuevo:\n" RESET_COLOR);
        }
    }

    int senderID = atoi(currentSenderID);

    char currentReceiverID[15];
    bool resReceiverID;

    printf(CYAN "Ingrese la cedula del colaborador destinatario:\n" RESET_COLOR);
    
    while (true){

        fgets(currentReceiverID, sizeof(currentReceiverID), stdin);
        removeLineBreak(currentReceiverID);

        resReceiverID = validateIdentificationCard(currentReceiverID);

        if (resReceiverID){
            break;
        }
        else{
            printf(RED "El numero de cedula es invalido\n" CYAN "Intente de nuevo:\n" RESET_COLOR);
        }
    }

    int receiverID = atoi(currentReceiverID);
    bool teamFlag = false;

    for (i = teamList; i != NULL; i = i->next){

        if (strcmp(i->teamName, currentName) == 0){

            teamFlag = true;

            for (int j = 0; j < i->quantityMembers; j++){

                if (i->members[j] == senderID){

                    senderFlag = true;
                }
            }

            for (int k = 0; k < i->quantityMembers; k++){

                if (i->members[k] == receiverID){

                    receiverFlag = true;
                }
            }

            if (senderFlag == true && receiverFlag == true){

                printf(GREEN "Colaborares encontrados\n" RESET_COLOR);

                break;
            }
            else{

                printf(RED "Colaborares no encontrados en el equipo ingresado\n" RESET_COLOR);

                return;
            }
        }
    }

    if (teamFlag == false){

        printf(RED "Equipo no encontrado\n" RESET_COLOR);

        return;
    }

    char senderName[50], senderLastName[50], senderSecLastname[50]; 
    char receiverName[50], receiverLastName[50], receiverSecLastname[50];
    bool a = false, b = false;
    collaboratorsNode *x;

    for (x = collaboratorsList; x != NULL; x = x->next){

        if (strcmp(x->identificationCard, currentSenderID) == 0){

            strcpy(senderName, x->name);
            strcpy(senderLastName, x->lastName);
            strcpy(senderSecLastname, x->SecondLastName);

            a = true;
        }

        if (strcmp(x->identificationCard, currentReceiverID) == 0){

            strcpy(receiverName, x->name);
            strcpy(receiverLastName, x->lastName);
            strcpy(receiverSecLastname, x->SecondLastName);

            b = true;
        }

        if (a == true && b == true){

            break;
        }
    }

    setlocale(LC_ALL, "");
    chatSystem(senderName, senderLastName, senderSecLastname, receiverName, receiverLastName, receiverSecLastname);

    return;
}

//----------Analisis de datos----------

//---Cantidad de mensaje enviados---

/*
Entradas: los mensajes de enviados en el chat
Salidas: el numero total de mensajes enviados
Restricciones: no hay
*/

void sentMessage(){

    printf(CYAN "Cantidad de mensajes enviados:" RESET_COLOR " %d\n", chatCounter);
}

//---Promedio de lugares que se visitan en las rutas de paseo---

//auxiliares

typedef struct placesCounterList{
    int counter;
    struct placesCounterList* next;
}placesCounterNode;

placesCounterNode* createPlaceCounterNode(int num){

	placesCounterNode *newNode = (placesCounterNode *) malloc(sizeof(placesCounterNode));

	newNode->next = NULL;

	newNode->counter = num;
	
	return newNode;
}

void uploadPlacesCounter(placesCounterNode** placesCounterList){

    FILE * ptrFile = fopen("PLACESCOUNTER.csv", "r");

    char data[1][120];
    char line[330];
    int count;

    while (fgets(line, sizeof(line), ptrFile)){

        char *token;

        token = strtok(line, ",");
        count = 0;

        while (token != NULL){

            strcpy(data[count], token);
            token = strtok(NULL, ",");
            count++;
        }

        placesCounterNode *newNode = createPlaceCounterNode(atoi(data[0]));

        if (*placesCounterList == NULL){
            *placesCounterList = newNode;
        }
        else{
            placesCounterNode *last = *placesCounterList;

            while (last->next != NULL){
                last = last->next;
            }

            last->next = newNode;
        }  
    }

    fclose(ptrFile);
}

void appendPlacesCounter(placesCounterNode **placesCounterList, int count){

    placesCounterNode *newNode = createPlaceCounterNode(count);
    placesCounterNode *last = *placesCounterList;

    if (*placesCounterList == NULL){
        *placesCounterList = newNode;
        return;
    }

    while (last->next != NULL){
        last = last->next;
    }

    last->next = newNode;

    return;
}

/*
Entradas: lugares registrados en la ruta del paseo simulada
Salidas: el promedio de lugares que se visitan 
Restricciones: no hay
*/

void getPlacesAverage(placesCounterNode *placesCounterList){

    int sum = 0, quantity = 0;
    float average;
    placesCounterNode *i;

    for (i = placesCounterList; i != NULL; i = i->next){

        sum = sum + i->counter;
        quantity++;
    }

    average = sum / quantity;

    printf(CYAN "Promedio de lugares visitados:" RESET_COLOR " %.1f\n", average);
}

//---Cantidad de archivos en total y cantidad de archivos en promedio---

//auxiliares

int sumFiles = 0, quantityCalls = 0;

void countFiles(char *basePath, const int root){

    int i;
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);

    if (!dir){

        return;
    }

    while ((dp = readdir(dir)) != NULL){

        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0){

            sumFiles++;

            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);
            countFiles(path, root + 2);
        }
    }

    quantityCalls++;

    closedir(dir);
}

/*
Entradas: archivos registrados
Salidas: cantidad de archivos en total y cantidad de archivos en promedio
Restricciones: deben haber archivos registrados
*/

void getFileStats(){

    char path[100] = "colabFiles";
    float filesAverage;

    countFiles(path, 0);

    filesAverage = sumFiles / quantityCalls;

    printf(CYAN "Cantidad de archivos en total:" RESET_COLOR " %d\n", sumFiles);
    printf(CYAN "Cantidad de archivos en promedio:" RESET_COLOR " %.1f\n", filesAverage);

    sumFiles = 0;
    quantityCalls = 0;
}

//---Promedio de colaboradores por equipo---

/*
Entradas: numero de colaboradores por equipo y cantidad total de equipos
Salidas: promedio de colaboradores por equipo
Restricciones: deben haber equipos registrados
*/

void getTeamAverage(teamNode *teamList){

    int colabsCount = 0, teamsQuantity = 0;
    float average;
    teamNode *i;

    if (teamList == NULL){

        printf(RED "No hay equipos registrados\n" RESET_COLOR);

        return;
    }

    for (i = teamList; i != NULL; i = i->next){

        colabsCount = colabsCount + i->quantityMembers;
        teamsQuantity++;
    }

    average = colabsCount / teamsQuantity;

    printf(CYAN "Promedio de colaboradores por equipo:" RESET_COLOR " %.1f\n", average);
}

//---Top 5 de equipos con mayor cantidad de colaboradores---

//auxiliar

void swap(int *x, int *y){
    int aux;

    aux = *x;
    *x = *y;
    *y = aux;
}

void bubble_Sort(int array[], int size){
    int i, j; 

    for (i = 0; i < size - 1; i++){
        for (j = 0; j < size - i - 1; j++){
            if (array[j] < array[j+1]){
                swap(&array[j], &array[j+1]);
            }
        }
    }
}

/*
Entradas: numero de colaboradores por equipo
Salidas: top 5 de equipos con mas colaboradores
Restricciones: deben haber equipos registrados
*/

void getTopFiveTeams(teamNode *teamList){

    int teamsQuantity = 0, j = 0;
    teamNode *i;

    if (teamList == NULL){

        printf(RED "No hay equipos registrados\n" RESET_COLOR);

        return;
    }

    for (i = teamList; i != NULL; i = i->next){

        teamsQuantity++;
    }

    int topFive[teamsQuantity];


    for (i = teamList; i != NULL; i = i->next){

        topFive[j] = i->quantityMembers;
        j++;
    }


    bubble_Sort(topFive, teamsQuantity);

    printf(YELLOW "Top 5 de equipos con mayor cantidad de colaboradores\n" RESET_COLOR);

    for (int k = 0; k < 5; k++){

        for (i = teamList; i != NULL; i = i->next){

            if (i->quantityMembers == topFive[k]){

                printf(CYAN "Nombre del equipo:" RESET_COLOR " %s" YELLOW " -> " CYAN "Cantidad de miembros:" RESET_COLOR " %d\n", i->teamName, i->quantityMembers);
            }
        }
    }
}

//---Funciones auxiliares para la persistencia de datos en archivos .csv---

void addCollaboratorsFile(char id[], char name[], char lastName[], char secondLastName[], char mail[], char role[], char birthdayDate[]){

    FILE* ptrFile = fopen("COLABORADORES.csv", "a+");

    if (!ptrFile){
        printf(RED "Error al abrir el archivo\n" RESET_COLOR);
        return;
    }

    fprintf(ptrFile, "%s, %s, %s, %s, %s, %s, %s\n", id, name, lastName, secondLastName, mail, role, birthdayDate);

    fclose(ptrFile);
}

void addBinnacleFile(char id[], char desc[]){

    FILE* ptrFile = fopen("BINNACLES.csv", "a+");

    if (!ptrFile){
        printf(RED "Error al abrir el archivo\n" RESET_COLOR);
        return;
    }

    fprintf(ptrFile, "%s, %s\n", id, desc);

    fclose(ptrFile);
}

void addHomeFile(char id[], char place[], int postalCode){

    FILE* ptrFile = fopen("HOME.csv", "a+");

    if (!ptrFile){
        printf(RED "Error al abrir el archivo\n" RESET_COLOR);
        return;
    }

    fprintf(ptrFile, "%s,%s, %d\n", id, place, postalCode);

    fclose(ptrFile);
}

void addRouteFile(int origin, int destination, int time, int distance, char typeRoute){

    FILE* ptrFile = fopen("ROUTES.csv", "a+");

    if (!ptrFile){
        printf(RED "Error al abrir el archivo\n" RESET_COLOR);
        return;
    }

    fprintf(ptrFile, "%d,%d,%d,%d,%c\n", origin, destination, time, distance, typeRoute);

    fclose(ptrFile);
}

void addPlacesCounterFile(int data){

    FILE* ptrFile = fopen("PLACESCOUNTER.csv", "a+");

    if (!ptrFile){
        printf(RED "Error al abrir el archivo\n" RESET_COLOR);
        return;
    }

    fprintf(ptrFile, "%d\n", data);

    fclose(ptrFile);
}

void saveCollaborators(collaboratorsNode **collaboratorsList){

    remove("COLABORADORES.csv");

    collaboratorsNode *aux;

    char id[15], name[50], lastName[50], secLastName[50], mail[100], role[100], birthdayDate[15];

    for (aux = *collaboratorsList; aux != NULL; aux = aux->next){

        strcpy(id, aux->identificationCard);
        strcpy(name, aux->name);
        strcpy(lastName, aux->lastName);
        strcpy(secLastName, aux->SecondLastName);
        strcpy(mail, aux->mail);
        strcpy(role, aux->role);
        strcpy(birthdayDate, aux->birthday);

        addCollaboratorsFile(id, name, lastName, secLastName, mail, role, birthdayDate);
    }
}

void saveBinnacle(binnacleNode **binnacle){

    remove("BINNACLES.csv");

    binnacleNode *aux;

    char id[15], opinion[500];

    for (aux = *binnacle; aux != NULL; aux = aux->next){

        strcpy(id, aux->identificationCard);
        strcpy(opinion, aux->description);

        addBinnacleFile(id, opinion);
    }
}

void saveHome(homeNode **homeList){

    remove("HOME.csv");

    homeNode *aux;

    char id[15], home[100];
    int postal;

    for (aux = *homeList; aux != NULL; aux = aux->next){

        strcpy(id, aux->identificationCard);
        strcpy(home, aux->place);
        aux->postalCode = postal;

        addHomeFile(id, home, postal);
    }
}

void saveRoute(routeNode **routeList){

    remove("ROUTES.csv");

    routeNode *aux;

    int originPlace, destinationPlace, estimatedTime, estimatedDistance;
    char routeType;

    for (aux = *routeList; aux != NULL; aux = aux->next){

        originPlace = aux->origin;
        destinationPlace = aux->destination;
        estimatedTime = aux->time;
        estimatedDistance = aux->distance;
        routeType = aux->routeType;

        addRouteFile(originPlace, destinationPlace, estimatedTime, estimatedDistance, routeType);
    }
}

void savePlacesCounter(placesCounterNode **placesCounterList){

    remove("PLACESCOUNTER.csv");

    placesCounterNode *aux;

    int counterPlace;

    for (aux = *placesCounterList; aux != NULL; aux = aux->next){

        counterPlace = aux->counter;

        addPlacesCounterFile(counterPlace);
    }
}

//----------Función Main----------

int main(){

    //declaración e inicialización de variables y estructuras de datos.
    int choice = 0, placeCounter;
    char option;
    char route;
    char yesOrNo;
    
    collaboratorsNode *collaboratorsList = NULL;
    teamNode *teamList = NULL;
    binnacleNode *binnacle = NULL;
    homeNode *homeList = NULL;
    routeNode *routeList = NULL;
    placesCounterNode *placesCounterList = NULL;

    //Cargar los datos de los archivos .csv a las estructuras de datos
    uploadCollaborators(&collaboratorsList);
    uploadBinnacle(&binnacle);
    uploadHome(&homeList);
    uploadRoute(&routeList);
    uploadPlacesCounter(&placesCounterList);

    //Menú principal del programa
    do{
        printf("\n\t\t\t Bienvenido a ATIWORKS\n");
        printf(YELLOW "Seleccione una opcion\n" RESET_COLOR);
        printf(CYAN "1.  Registrar colaborador\n" RESET_COLOR);
        printf(CYAN "2.  Modificar colaborador\n" RESET_COLOR);
        printf(CYAN "3.  Eliminar colaborador\n" RESET_COLOR);
        printf(CYAN "4.  Consultar colaborador\n" RESET_COLOR);
        printf(CYAN "5.  Registrar equipo\n" RESET_COLOR);
        printf(CYAN "6.  Eliminar equipo\n" RESET_COLOR);
        printf(CYAN "7.  Consultar equipo\n" RESET_COLOR);
        printf(CYAN "8.  Operaciones con archivos\n" RESET_COLOR);
        printf(CYAN "9.  Registrar domicilo de colaboradores\n" RESET_COLOR);
        printf(CYAN "10. Modificar domicilo de colaboradores\n" RESET_COLOR);
        printf(CYAN "11. Consultar domicilo de colaboradores\n" RESET_COLOR);
        printf(CYAN "12. Eliminar domicilo de colaboradores\n" RESET_COLOR);
        printf(CYAN "13. Registrar nueva ruta para el paseo\n" RESET_COLOR);
        printf(CYAN "14. Modificar ruta especifica del paseo\n" RESET_COLOR);
        printf(CYAN "15. Consultar rutas registradas del paseo\n" RESET_COLOR);
        printf(CYAN "16. Eliminar ruta especifica del paseo\n" RESET_COLOR);
        printf(CYAN "17. Simular ruta del paseo\n" RESET_COLOR);
        printf(CYAN "18. Registrar bitacora de trabajo\n" RESET_COLOR);
        printf(CYAN "19. Consultar bitacora de trabajo\n" RESET_COLOR);
        printf(CYAN "20. Eliminar bitacora de trabajo\n" RESET_COLOR);
        printf(CYAN "21. Chat\n" RESET_COLOR);
        printf(CYAN "22. Analisis de datos\n" RESET_COLOR);
        printf(CYAN "23. Salir\n" RESET_COLOR);
        
        scanf("%d",&choice); 

        switch(choice){
            case 1:
                clearBuffer();
                appendCollaborators(&collaboratorsList);
                break;
            case 2:
                clearBuffer();
                modifyCollaborator(&collaboratorsList);
                break;
            case 3:
                clearBuffer();
                deleteCollaborators(&collaboratorsList);
                break;
            case 4:
                clearBuffer();
                consultCollaborators(collaboratorsList);
                break;
            case 5:
                clearBuffer();
                appendTeam(&teamList);
                break;
            case 6:
                clearBuffer();
                deleteTeam(&teamList);
                break;
            case 7:
                consultTeam(teamList, collaboratorsList);
                break;
            case 8:
                fileOperations();
                break;
            case 9:
                clearBuffer();
                appendHome(&homeList);
                break;
            case 10:
                clearBuffer();
                modifyHome(&homeList);
                break;
            case 11:
                consultHome(homeList);
                break;
            case 12:
                clearBuffer();
                deleteHome(&homeList);
                break;
            case 13:
                clearBuffer();
                appendRoute(&routeList, &homeList);
                break;
            case 14:
                clearBuffer();
                modifyRoutes(&routeList, &homeList);
                break;
            case 15:
                consultRoutes(&routeList, &homeList);
                break;
            case 16:
                clearBuffer();
                deleteRoutes(&routeList, &homeList);
                break;
            case 17:
                printf(CYAN "La simulacion de la ruta del paseo se puede realizar clasificando las rutas de acuerdo al tipo\n" RESET_COLOR);
                printf(CYAN "Ingrese Y si desea clasificar la simulacion o ingrese N si no desea clasificar la simulacion\n" RESET_COLOR);
                clearBuffer();
                yesOrNo = getYesOrNo();

                if (yesOrNo == 'Y'){

                    route = getTypeRoute();
                    placeCounter = simulateRoute(&routeList, &homeList, &collaboratorsList, route);
                }
                else{
                    placeCounter = simulateRoute(&routeList, &homeList, &collaboratorsList, yesOrNo);
                }

                appendPlacesCounter(&placesCounterList, placeCounter);

                break;
            case 18:
                clearBuffer();
                appendBinnacle(&binnacle);
                break;
            case 19:
                consultBinnacle(binnacle);
                break;
            case 20:
                clearBuffer();
                deleteBinnacle(&binnacle);
                break;
            case 21:
                clearBuffer();
                chat(teamList, collaboratorsList);
                break;
            case 22:
                printf(YELLOW "\t\tAnalisis de datos\n\n" RESET_COLOR);
                sentMessage();
                getPlacesAverage(placesCounterList);
                getFileStats();
                getTeamAverage(teamList);
                getTopFiveTeams(teamList);
                break;
            case 23:   

                saveCollaborators(&collaboratorsList);
                saveBinnacle(&binnacle);
                saveHome(&homeList);
                saveRoute(&routeList);
                savePlacesCounter(&placesCounterList);

                printf(YELLOW "Hasta pronto" RESET_COLOR "\n");
                break;
            default:
                printf(RED "Incorrect option" CYAN "Intente de nuevo\n" RESET_COLOR);
                break;
        } 
    } while (choice != 23 && choice != EOF);

    return 0;
}