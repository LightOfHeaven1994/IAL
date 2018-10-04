
/* c201.c *********************************************************************}
{* Téma: Jednosměrný lineární seznam
**
**                     Návrh a referenční implementace: Petr Přikryl, říjen 1994
**                                          Úpravy: Andrea Němcová listopad 1996
**                                                   Petr Přikryl, listopad 1997
**                                Přepracované zadání: Petr Přikryl, březen 1998
**                                  Přepis do jazyka C: Martin Tuček, říjen 2004
**                                            Úpravy: Kamil Jeřábek, září 2018
**
** Implementujte abstraktní datový typ jednosměrný lineární seznam.
** Užitečným obsahem prvku seznamu je celé číslo typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou typu tList.
** Definici konstant a typů naleznete v hlavičkovém souboru c201.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ tList:
**
**      InitList ...... inicializace seznamu před prvním použitím,
**      DisposeList ... zrušení všech prvků seznamu,
**      InsertFirst ... vložení prvku na začátek seznamu,
**      First ......... nastavení aktivity na první prvek,
**      CopyFirst ..... vrací hodnotu prvního prvku,
**      DeleteFirst ... zruší první prvek seznamu,
**      PostDelete .... ruší prvek za aktivním prvkem,
**      PostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      Copy .......... vrací hodnotu aktivního prvku,
**      Actualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      Succ .......... posune aktivitu na další prvek seznamu,
**      Active ........ zjišťuje aktivitu seznamu.
**
** Při implementaci funkcí nevolejte žádnou z funkcí implementovaných v rámci
** tohoto příkladu, není-li u dané funkce explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam předá
** někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c201.h"

int errflg;
int solved;

void Error() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;                      /* globální proměnná -- příznak chyby */
}

void InitList (tList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/
	L->First = NULL;		/*Provedeme inicializace*/
	L->Act = NULL;
	
}

void DisposeList (tList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam L do stavu, v jakém se nacházel
** po inicializaci. Veškerá paměť používaná prvky seznamu L bude korektně
** uvolněna voláním operace free.
***/
	L->Act = L->First;						/*Nastavime aktivnim prvni prvek*/
	
	while(L->First != NULL){				/*Jdeme zrusit seznam od zacatku*/
		tElemPtr delete_element = L->First;	/*Ukazatel na prvni prvek ulozime*/
		L->First = L->First->ptr;
		free(delete_element);
	}

}

void InsertFirst (tList *L, int val) {
/*
** Vloží prvek s hodnotou val na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci Error().
**/
	tElemPtr new_element = malloc(sizeof(struct tElem));		/*Alokace pameti pro novy prvek*/
    
    if(new_element == NULL){									/*Overeni ze alokace byla provedena uspesne*/
    	Error();
    }
    else{
    	new_element->ptr = L->First;							/*Vlozime novy prvek do seznamu a ukazeme ze je prvni*/
    	new_element->data = val;
    	L->First = new_element;
    }
    
}

void First (tList *L) {
/*
** Nastaví aktivitu seznamu L na jeho první prvek.
** Funkci implementujte jako jediný příkaz, aniž byste testovali,
** zda je seznam L prázdný.
**/
	L->Act = L->First;	/*Nastavime aktivitu seznamu L na jeho prvni prvek*/

}

void CopyFirst (tList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci Error().
**/
	if(L->First == NULL){		/*Pokud je seznam L prazdny*/
		Error();
	}
	else{
		*val = L->First->data;	/*vratime hodnotu prvniho prvku seznamu L*/
	}

}

void DeleteFirst (tList *L) {
/*
** Zruší první prvek seznamu L a uvolní jím používanou paměť.
** Pokud byl rušený prvek aktivní, aktivita seznamu se ztrácí.
** Pokud byl seznam L prázdný, nic se neděje.
**/
	if(L->First != NULL){		/*Pokud seznam neni prazdny*/
		if(L->First == L->Act){	/*Pokud prvni prvek je aktivni*/
			L->Act = NULL;		/*Ztraceni aktivity seznamu*/
		}
		
		tElemPtr del_first_element = L->First;	/*Ulozime ukazatel na prvnĂi prvek*/

		if(L->First->ptr != NULL){		/*Overime zda existuje dalsi prvek*/
			L->First = L->First->ptr;	/*Pokud existuje, oznacime prvek jako prvni*/
		}
		else{
			L->First = NULL;
		}
		free(del_first_element);		/*Uvolnime pamet*/
	}
	
}	

void PostDelete (tList *L) {
/* 
** Zruší prvek seznamu L za aktivním prvkem a uvolní jím používanou paměť.
** Pokud není seznam L aktivní nebo pokud je aktivní poslední prvek seznamu L,
** nic se neděje.
**/
	if((L->Act != NULL) && (L->Act->ptr != NULL)){		/*Overime zda aktivni­ prvek existuje a že aktivnĂi prvek neni posledni*/
		tElemPtr post_del_element = L->Act->ptr;		/*Ulozime ukazatel na prvek ktery chceme zrusit*/
		
		if(L->Act->ptr->ptr != NULL){					/*Pokud existuje jeste prvek za aktivnim*/
			L->Act->ptr = L->Act->ptr->ptr;				/*Ulozime aktivnĂ­mu prvku na dalsi element v seznamu*/
		}
		else{											/*Pokud nasledujici za aktivnim prvek je poslednĂ­ v seznamu*/
			L->Act->ptr = NULL;
		}
		free(post_del_element);			/*Uvolnime pamet*/
	}
	
}

void PostInsert (tList *L, int val) {
/*
** Vloží prvek s hodnotou val za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje!
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** zavolá funkci Error().
**/
	if(L->Act != NULL){
		tElemPtr new_element = malloc(sizeof(struct tElem));	/*Alokace pameti pro novy prvek*/
		if(new_element == NULL){								/*Overime ze alokace byla provedena uspesne*/
    		Error();
   		}
   		else{
   			new_element->data = val;							/*Vlozime hodnotu val do new_element*/
   			if(L->Act->ptr != NULL){							/*Pokud nasledujici element po Active nenĂi posledni*/	
   				new_element->ptr = L->Act->ptr;					/*Ulozime ukazatel new_element do seznamu*/
   			}
   			else{
   				new_element->ptr = NULL;						/*Pokud dalsi prvky v seznamu neexistuji*/
   			}
   			L->Act->ptr = new_element;							/*Ukazatel aktivniho bude ukazovat na new_element*/
   		}
	}
	
}

void Copy (tList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam není aktivní, zavolá funkci Error().
**/
	if(L->Act != NULL){
		*val = L->Act->data;
	}
	else{
		Error();
	}
	
}

void Actualize (tList *L, int val) {
/*
** Přepíše data aktivního prvku seznamu L hodnotou val.
** Pokud seznam L není aktivní, nedělá nic!
**/
	if(L->Act != NULL){			/*Pokud seznam je aktivni*/
		L->Act->data = val;		/*Ulozime data*/
	}
	
}

void Succ (tList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Všimněte si, že touto operací se může aktivní seznam stát neaktivním.
** Pokud není předaný seznam L aktivní, nedělá funkce nic.
**/
	if(L->Act != NULL){				/*Pokud seznam je aktivni*/
		if(L->Act->ptr != NULL){
			L->Act = L->Act->ptr;	/*Posuneme aktivnĂi prvek dopredu*/
		}
		else{
			L->Act = NULL;			/*Pokud dalsi prvek neexistuje = seznam bude neaktivni*/
		}
	}
	
}

int Active (tList *L) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Tuto funkci je vhodné implementovat jedním příkazem return. 
**/
	return (L->Act != NULL)? 1 : 0;
	
}

/* Konec c201.c */
