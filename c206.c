
/* c206.c **********************************************************}
{* Téma: Dvousměrně vázaný lineární seznam
**
**                   Návrh a referenční implementace: Bohuslav Křena, říjen 2001
**                            Přepracované do jazyka C: Martin Tuček, říjen 2004
**                                            Úpravy: Kamil Jeřábek, září 2018
**
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou
** typu tDLList (DL znamená Double-Linked a slouží pro odlišení
** jmen konstant, typů a funkcí od jmen u jednosměrně vázaného lineárního
** seznamu). Definici konstant a typů naleznete v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu
** s výše uvedenou datovou částí abstrakce tvoří abstraktní datový typ
** obousměrně vázaný lineární seznam:
**
**      DLInitList ...... inicializace seznamu před prvním použitím,
**      DLDisposeList ... zrušení všech prvků seznamu,
**      DLInsertFirst ... vložení prvku na začátek seznamu,
**      DLInsertLast .... vložení prvku na konec seznamu,
**      DLFirst ......... nastavení aktivity na první prvek,
**      DLLast .......... nastavení aktivity na poslední prvek,
**      DLCopyFirst ..... vrací hodnotu prvního prvku,
**      DLCopyLast ...... vrací hodnotu posledního prvku,
**      DLDeleteFirst ... zruší první prvek seznamu,
**      DLDeleteLast .... zruší poslední prvek seznamu,
**      DLPostDelete .... ruší prvek za aktivním prvkem,
**      DLPreDelete ..... ruší prvek před aktivním prvkem,
**      DLPostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      DLPreInsert ..... vloží nový prvek před aktivní prvek seznamu,
**      DLCopy .......... vrací hodnotu aktivního prvku,
**      DLActualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na další prvek seznamu,
**      DLPred .......... posune aktivitu na předchozí prvek seznamu,
**      DLActive ........ zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce
** explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam 
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c206.h"

int errflg;
int solved;

void DLError() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globální proměnná -- příznak ošetření chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/
	L->First = NULL;		/*Provedeme inicializace*/
	L->Act = NULL;
	L->Last = NULL;
    
	
}

void DLDisposeList (tDLList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Rušené prvky seznamu budou korektně
** uvolněny voláním operace free. 
**/
	L->Act = L->First;							/*Nastavime aktivnim prvni prvek*/
	L->Last = NULL;								/*Zrusime ukazatel na prvek last*/

	while(L->First != NULL){					/*Jdeme zrusit seznam od zacatku*/
		tDLElemPtr delete_element = L->First;	/*Ukazatel na prvni prvek ulozime*/
		L->First = L->First->rptr;				/*Ted nasledujici prvek bude prvni*/
		free(delete_element);					/*Uvolnime pamet*/
	}
	
}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vloží nový prvek na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	tDLElemPtr new_element = malloc(sizeof(struct tDLElem));	/*alokace pameti pro novy prvek*/
	if(new_element == NULL){
		DLError();
	}
	else{
		new_element->lptr = NULL;			/*Pokud bude prvni neukazuje nalevo*/
		new_element->data = val;			/*Priradime data pro novy prvek*/
		new_element->rptr = L->First;		/*Novy prvek napravo ukaze na First, a pokud ten neexistuje tak ukaze na NULL*/
		
		if(L->First == NULL){
			L->Last = new_element;			/*Pokud novy prvek je sam v seznamu, tak je posledni taky*/
		}
		else{
			L->First->lptr = new_element;	/*Prvni prvek ukaze nalevo na novy prvek*/
		}
		
		L->First = new_element;				/*Novy prvek je ted prvni*/
	}
	
}

void DLInsertLast(tDLList *L, int val) {
/*
** Vloží nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/ 	
	tDLElemPtr new_element = malloc(sizeof(struct tDLElem));	/*alokace pameti pro novy prvek*/
	if(new_element == NULL){
		DLError();
	}
	else{
		new_element->rptr = NULL;			/*Pokud bude posledni neukazuje napravo*/
		new_element->data = val;			/*Priradime data pro novy prvek*/
		new_element->lptr = L->Last;		/*Novy prvek nalevo ukaze na Last, a pokud ten neexistuje tak ukaze na NULL*/

		if(L->Last == NULL){
			L->First = new_element;			/*Pokud novy prvek je sam v seznamu, tak je prvni taky*/
		}
		else{
			L->Last->rptr = new_element;	/*Posledni prvek ukaze napravo na novy prvek*/
		}

		L->Last = new_element;				/*Novy prvek je ted posledni*/
	}
	
}

void DLFirst (tDLList *L) {
/*
** Nastaví aktivitu na první prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
	L->Act = L->First; 	/*Nastavime aktivitu na prvni prvek seznamu L*/

}

void DLLast (tDLList *L) {
/*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
	L->Act = L->Last;	/*Nastavime aktivitu na posledni prvek seznamu L*/
	
}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
	if(L->First == NULL){		/*Pokud seznam je prazdny, volame Err*/
		DLError();
	}
	else{
		*val = L->First->data; /*Vratime hodnotu prvniho prvku seznamu*/
	}
	
}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
	if(L->First == NULL){		/*Pokud seznam je prazdny, volame Err*/
		DLError();
	}
	else{
		*val = L->Last->data;	/*Vratime hodnotu posledniho prvku seznamu*/
	}
	
}

void DLDeleteFirst (tDLList *L) {
/*
** Zruší první prvek seznamu L. Pokud byl první prvek aktivní, aktivita 
** se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/
	if(L->First != NULL){			/*Pokud seznam neni prazdny*/
		if(L->First == L->Act){		/*Pokud prvni prvek je aktivni*/
			L->Act = NULL;			/*Aktivita se ztraci*/
		}

		tDLElemPtr del_first_element = L->First;	/*Ukazatel na prvni prvek ulozime*/

		if(L->First->rptr != NULL){					/*Pokud v seznamu existuji dalsi prvky*/
			L->First = L->First->rptr;				/*Dalsi prvek nastavime jako prvni*/
			L->First->lptr = NULL;					/*Zrusime ukazatel na minuly prvek*/
		}
		else{										/*Pokud v seznamu je zadny prvkek*/
			L->First = NULL;						/*Zrusime ukazatele na prvni a posledni prvky*/
			L->Last = NULL;
		}
		free(del_first_element);					/*Uvolnime pamet*/

	}
	
}	

void DLDeleteLast (tDLList *L) {
/*
** Zruší poslední prvek seznamu L. Pokud byl poslední prvek aktivní,
** aktivita seznamu se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/ 
	if(L->First != NULL){			/*Pokud seznam neni prazdny*/
		if(L->Last == L->Act){		/*Pokud prvni prvek je aktivni*/
			L->Act = NULL;			/*Aktivita se ztraci*/
		}

		tDLElemPtr del_last_element = L->Last;		/*Ukazatel na posledni prvek ulozime*/

		if(L->Last->lptr != NULL){					/*Pokud v seznamu existuji dalsi prvky*/
			L->Last = L->Last->lptr;				/*Dalsi prvek nastavime jako posledni*/
			L->Last->rptr = NULL;					/*Zrusime ukazatel na minuly prvek*/
		}
		else{										/*Pokud v seznamu je zadny prvkek*/
			L->First = NULL;						/*Zrusime ukazatele na prvni a posledni prvky*/
			L->Last = NULL;
		}
		free(del_last_element);						/*Uvolnime pamet*/
	}
	
}

void DLPostDelete (tDLList *L) {
/*
** Zruší prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se neděje.
**/
	if((L->Act != NULL) && (L->Act->rptr != NULL)){		/*Overime zda aktivni prvek existuje a neni posledni*/
		tDLElemPtr post_del_element = L->Act->rptr;		/*Ukazatel na prvek ktery sledue po aktivnimu ulozime*/

		if(L->Act->rptr->rptr != NULL){					/*Pokud existuje nasledujici prvek za aktivnim*/
			L->Act->rptr = L->Act->rptr->rptr;			/*Ukazatel aktiniho prvku ukaze na nasledijici*/
			L->Act->rptr->rptr->lptr = L->Act;			/*Ukazatel nasledujiciho prvku ukaze nalevo na aktivni*/
		}
		else{
			L->Act->rptr = NULL;						/*Pokud nasledujici prvek neexistuje nastavime ukazatel vpravo na NULL*/
			L->Last = L->Act;							/*Ted posledni prvek je aktivni*/
		}
		free(post_del_element);							/*Uvolnime pamet*/
	}

		
}

void DLPreDelete (tDLList *L) {
/*
** Zruší prvek před aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se neděje.
**/
	if((L->Act != NULL) && (L->Act->lptr != NULL)){		/*Overime zda aktivni prvek existuje a neni posledni*/
		tDLElemPtr pre_del_element = L->Act->lptr;		/*Ukazatel na prvek ktery nachazi pred aktivnim ulozime*/

		if(L->Act->lptr->lptr != NULL){					/*Pokud existuje predchazejici prvek za aktivnim*/
			L->Act->lptr = L->Act->lptr->lptr;			/*Ukazatel aktiniho prvku ukaze na predchazejici*/
			L->Act->lptr->lptr->rptr = L->Act;			/*Ukazatel predchazejiciho prvku ukaze napravo na aktivni*/
		}
		else{
			L->Act->lptr = NULL;						/*Pokud predchazejici prvek neexistuje nastavime ukazatel vpravo na NULL*/
			L->First = L->Act;							/*Ted prvni prvek je aktivni*/
		}
		free(pre_del_element);							/*Uvolnime pamet*/
	}
			
}

void DLPostInsert (tDLList *L, int val) {
/*
** Vloží prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	if(L->Act != NULL){
		tDLElemPtr new_element = malloc(sizeof(struct tDLElem));
		if(new_element == NULL){
			DLError();
		}
		else{
			new_element->data = val;
			if(L->Act->rptr != NULL){
				new_element->rptr = L->Act->rptr;
				L->Act->rptr->lptr = new_element;
			}
			else{
				new_element->rptr = NULL;
				new_element->lptr = L->Act;
				L->Last = new_element;
				
			}
			L->Act->rptr = new_element;
		}
	}
	
}

void DLPreInsert (tDLList *L, int val) {
/*
** Vloží prvek před aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	if(L->Act != NULL){
		tDLElemPtr new_element = malloc(sizeof(struct tDLElem));
		if(new_element == NULL){
			DLError();
		}
		else{
			new_element->data = val;
			if(L->Act->lptr != NULL){
				new_element->lptr = L->Act->lptr;
				L->Act->lptr->rptr = new_element;
			}
			else{
				new_element->lptr = NULL;
				new_element->rptr = L->Act;
				L->First = new_element;
			}
			L->Act->lptr = new_element;
		}

	}
	
	
}

void DLCopy (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam L není aktivní, volá funkci DLError ().
**/
	if(L->Act != NULL){
		*val = L->Act->data;
	}
	else{
		DLError();
	}
	
}

void DLActualize (tDLList *L, int val) {
/*
** Přepíše obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedělá nic.
**/
	if(L->Act != NULL){
		L->Act->data = val;
	}
	
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
**/
	if(L->Act != NULL){
		if(L->Act->rptr != NULL){
			L->Act = L->Act->rptr;
		}
		else{
			L->Act = NULL;
		}
	}
	
}

void DLPred (tDLList *L) {
/*
** Posune aktivitu na předchozí prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
**/
	if(L->Act != NULL){
		if(L->Act->lptr != NULL){
			L->Act = L->Act->lptr;
		}
		else{
			L->Act = NULL;
		}
	}
	
}

int DLActive (tDLList *L) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Funkci je vhodné implementovat jedním příkazem return.
**/
	return (L->Act != NULL)? 1 : 0;
	
}

/* Konec c206.c*/
