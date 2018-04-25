/*
 *
 *
 *	Copyright (c) 1990 by   Institut fuer Kernphysik,    H.Kramer
 *				Johannes Gutenberg-Universitaet Mainz
 *
 *	All rights reserved.
 *
 *	@(#)thtrees.c	1.8	KPH	95/10/20	95/10/20
 *
 *
 */

#ifdef sccs
static char SccsId[] = "@(#)thtrees.c	1.8\t10/20/95 (kramer/kph)";
#endif

#include <stdio.h>
#include "thtrees.h"

#ifdef __STDC__
extern void *malloc(size_t size);
extern void free(void *ptr);
#else
extern char * malloc() ;
extern int free() ;
#endif


#define _info(p) ((p) ->info)
#define l(p) ((p) ->_l)
#define rtag(p) ((p) ->r_tag)
#define r(p) ((p) ->_r)

#define _diff(a,b) (compar ? (*compar)(a,b) :((* (int*)(a)) - (* (int*)(b))) )

#ifdef __STDC__
static THNode **find(
	char * k,	/* Information */
	register THNode ** rootp, /* ->->auf Wurzel */
	int (*compar)(char *a, char *b), /* Vergleichsfunktion */
	THNode **parent ,
	int *c /* Ergebnis */
	)
#else
static THNode **find(k,rootp,compar,parent,c)
	char * k;	/* Information */
	register THNode ** rootp; /* ->->auf Wurzel */
	int (*compar)(); /* Vergleichsfunktion */
	THNode **parent ;
	int *c; /* Ergebnis */
#endif

/*

Resultate der Funktion find:

	find()  *find() *parent *c	Resultat
	0	?	0	?	Fehler kein Baum
		0	0	?	leerer Baum
	!=0	0	!=0	<0	Element gehoert nach links
		thread  !=0	>0	Element gehoert nach rechts
			0	0	Element ist Wurzel
	!=0	!=0	!=0	0 	Element sitzt tiefer

*/
{	


	*parent = (THNode *) NULL ;
	if (rootp && *rootp)
		for (;;) {
			if ((*c = _diff(k,_info(*rootp))) == 0 )
				break;
			*parent = *rootp;

			if (*c <0) {
				rootp = & l(*parent);
				if (! *rootp)
					break;
			}
			
			else {
				rootp = & r(*parent);
				if (rtag(*parent))
					break;
			}
		}
								
	return(rootp);
	
}

#ifdef __STDC__
char ** thtfind(
	char *k,	/* Info */
	register THNode **rootp,
	int (*compar) (char *a, char * b)
	)
#else
char ** thtfind(k,rootp,compar)
	char *k;	/* Info */
	register THNode **rootp;
	int (*compar) ();
#endif

/*
	sucht den Schluessel k im Baum rootp mittels der
	Vergleichsfunktion compar
*/
{
	THNode *parent;
	int c;
	
	if ((rootp=find(k,rootp,compar,& parent, & c)) && * rootp && c== 0 )
		return (& _info(* rootp)) ;
		
	return( (char **) NULL );
}

#ifdef __STDC__
char ** thtsearch(
	char *k,	/* Info */
	register THNode **rootp,
	int (*compar)(char *a, char *b)
	)
#else
char ** thtsearch(k,rootp,compar)
	char *k;	/* Info */
	register THNode **rootp;
	int (*compar) ();
#endif
/*
	sucht den Schluessel k im Baum rootp mittels der
	Vergleichsfunktion compar, falls dieser nicht existiert wird er
	eingetragen.
	
*/
{
  register THNode  * p;
  THNode *parent;
  int c;
	

  if ((rootp = find(k,rootp,compar, &parent, &c))) {
    if (*rootp && c == 0 ) {
      return( & _info(* rootp));
    } else if ((p = (THNode *) malloc(sizeof (THNode)))) {
      _info(p) = k;
      l(p) = (THNode *) NULL;
      if(parent && c > 0 ) {
	r(p) = r(parent);
	r(parent) = p; 
	rtag(parent) = 0;
      } else {
	r(p) = parent ;
	* rootp  = p;
      }

      rtag(p) = 1;
      return( & _info(p) );
    }
  }

  return( (char * *) NULL);
}



/*
	Definiere  (siehe Wirth)
		preorder : Wurzel, links, rechts
		postorder :links, Wurzel, rechts
		endorder: links,rechts,Wurzel
		
*/


/*
	Iterativer Besuch der Knoten im Baum in postorder Reihenfolge

*/
void 
thtpost(
	  register THNode *root ,
	  int (*action) (char ** info, enum VISIT order, void *),
	  void * client_data
	  )

{
  register THNode * p;
  
  if (!root)
    return;
  
  for(;;) {
    while((p = l(root))) 
      root = p;
    
    if (rtag(root)) {
      if ((*action)( & _info(root), leaf, client_data) == -1)
	return;
      do {
	root = r(root);
	if (! root)
	  return;
	if ((* action) ( & _info(root), postorder, client_data) == -1) return;
      } while (rtag(root));
    } else  {
      if ((* action)(& _info(root), postorder, client_data) == -1) return;
    }
    
    root = r(root);
  }
} 


void thtpre(register THNode *root, 
	    int (*action)(char ** info, enum VISIT order, void *),
	    void * client_data
	    )
{
  register THNode * p;
  
  if ( ! root )
    return ;
  
  for (;;) {
    while ((p = l(root))){
      if (( * action)(& _info(root), preorder, client_data) == -1) return;
      root = p;
    }
    
    if (rtag(root))
      { if ((* action)(& _info(root),leaf, client_data) == -1) return;
	do {
	  root = r(root);
	  if (! root )
	    return;
	} while (rtag(root));
      } else {
	if ((* action)( & _info(root), preorder, client_data) == -1) return;
      }
    root = r(root);
  }
}


void 
thtend(
       register THNode * root,
       int (* action)(char ** info, enum VISIT order, void *) ,
       void * client_data    
       )
{
  register int link;
  
  if (root) {
    thtend(l(root), action, client_data);
    if ((link = ! rtag(root)))
      thtend(r(root), action, client_data);
    
    if ((* action)(& _info(root), (l(root) || link? endorder : leaf), 
		   client_data ) == -1)
      return;
  }
  return;
} 	

#ifdef __STDC__
char * thtdelete(
	char *k, /* Info */
	register THNode ** rootp, /* --> Wurzel des Baums */
	int (* compar)(char *a, char * b) /* Vergleichsfunktion */
	)
#else
char * thtdelete(k,rootp,compar)
	char *k; /* Info */
	register THNode ** rootp; /* --> Wurzel des Baums */
	int (* compar)(); /* Vergleichsfunktion */
#endif

/*

	Loeschen eines Eintrags mit dem Schluesel k aus
	dem Baum
	
	int (* compar)(char * a, char * b); 
*/ 
{
	register THNode * p,* R;
	char * erg;
	THNode *parent;
	int c;
	
	if (! (rootp = find(k,rootp,compar, & parent, & c)) || ! * rootp 
	    || c )
		return( (char *) NULL );
	
	erg = _info(*rootp)  ;
	
	if (! rtag(*rootp) ) {
		R = r(*rootp);
		
		if (! l(*rootp)) {
			p = *rootp; *rootp = R;
			free((char*)p);
			return (erg);
		}
		
		if (! l(R) ) {
			l(R) = l(*rootp);
			free((char*)*rootp);
			*rootp = R;
		} else {
			p = R;
			while(l(l(p)))
				p = l(p);
				
			l(l(p)) = l(*rootp);
			free((char*) *rootp);
			*rootp = l(p);
			l(p) = rtag(l(p)) ? (THNode *) NULL : r(l(p)) ;
			r(*rootp) = R, rtag(*rootp) = 0;
		}
		if ((p = l(*rootp))) { /* ueberfluessig */
			while (! rtag(p))
				p = r(p);
			r(p) = *rootp ,rtag(p) = 1;
		}
	} 	
	else if ((p = l(*rootp))) { 
		/* es existiert ein linker kein rechter Unterbaum */
		while(! rtag(p))
			p =r(p);	
			
		r(p) = r(*rootp),rtag(p) = 1; /* thread kopieren */
		p = *rootp , *rootp = l(p);
		free((char*)p);
	}
	else if  (parent && r(parent) == *rootp) {
		p = *rootp;
		r(parent) = r(p);
		rtag(parent) = 1;
		free((char*)p);
	}

	else /* einziges linkes Blatt kan geloescht werden */
		free((char*)*rootp ), * rootp = (THNode *) NULL;
		
	return (erg);	   
}
	    

/*
	Aufbau eines Baumes, falls die eingabe sortiert ist 
*/

#ifdef __STDC__
static THNode *load(
	char * (*getinfo)(void),
	register THNode * corner,
	int level
	)
#else
static THNode *load(getinfo,corner,level)
	char * (*getinfo)();
	register THNode * corner;
	int level;
#endif

/*
	getinfo() Funktion, die bei jedem Aufruf Zeiger auf naechste Info 
	liefert. Null falls keine Information mehr da.
	
	getinfo() == NULL 	-> load() = NULL
	Speicherplatz alle	->
	
*/
{
	register THNode * p, * root;
	int tag,h = -1;

	if ( ! (root = (THNode *) malloc(sizeof(THNode))))
		return( (THNode *) NULL );
		
	if ( ! (_info(root) = (* getinfo)()))
		return( (THNode *) NULL );
		
	if (corner) {
		l(root) = r(corner);
		r(corner) = root, rtag(corner) = 1;
		corner = (THNode *) NULL ;
	}
	
	while (++ h < level  && (p = load(getinfo, corner, h))) {
		corner = p;
		if (rtag(corner))
			break;
			
	}
	if (corner)
		r(root) = r(corner), rtag(root) = 0;
		
	else
		corner = root;
		
	tag = h < level || rtag(corner);
	r(corner) = root, rtag(corner) = tag;
	return(corner);
} 			


#ifdef __STDC__
char ** thtload(
	char * (* getinfo)(),
	register THNode ** rootp
	)
#else
char ** thtload(getinfo, rootp)
	char * (* getinfo)();
	register THNode ** rootp;
#endif	
{
	int level = 0;
	register THNode * p;
	
	if (rootp) {
		*rootp = (THNode *) NULL;
		while ((p = load(getinfo, *rootp, level ++))) {
			*rootp = p;
			if (rtag(p))
				break;
		}
		if ((p = *rootp)) {
			*rootp = r(p);
			r(p) = (THNode *) NULL, rtag(p) = 1;
			return(& _info(p));
		}
	}
	return((char **) NULL );
}

#ifdef __STDC__
void thtclear(
	THNode ** rootp,
	void (*action)(char ** info,enum  VISIT order) 
	)
#else
void thtclear(rootp,action)
	THNode ** rootp;
	void (*action)() ;
#endif

/*
	Schneller Abbau eines Suchbaums
	
	rootp:	Adresse bei der sich zeiger auf Baum befindet.
	action:	Um Speicherplatz der Information freizugeben.
*/

{
	register THNode * root, *p;
	if (! rootp || !(root = *rootp))
		return;
		
	*rootp = (THNode *) NULL ;
	
	for (;;) {
		while ((p = l(root)))
			root = p;
			
		if (rtag(root)) {
			if (action)
				(* action)(& _info(root), leaf);
				
			do {
				p = r(root), free((char*)root), root = p;
				if (! root)
					return;
					
				if (action)
					(* action)(& _info(root), postorder);
				
			} while (rtag(root)) ;
		} 
		else 
		{	if (action)
				(* action)( & _info(root), postorder) ;
		}
		p = r(root), free((char*)root), root = p;
	}
}
					

#ifdef TESTcompile
/*
	TEST-FUNKTION t_thtsearch.c
*/
static char **keys ;
static THNode *root;

static char *getkey(void)
{
        
	return	*keys ++ ;
}

void
static show(char **pp, enum VISIT order)
{
	printf("%s ",*pp);
}


int 
cmp(char *a ,char *b) 
{
  return(strcmp((char*) a, (char *) b)) ;
}
int
main(int argc, char **argv)
{

  char s[20] ;
  char * t ;

  if (argc > 1) 
    keys = ++ argv ;


  thtload(getkey, & root);
	
  printf("pre \t "), thtpre(root,show), putchar('\n') ;
  printf("end \t "), thtend(root,show), putchar('\n') ;
  printf("post \t "), thtpost(root,show), putchar('\n') ;

  
  while (1) {
    t = (char * ) malloc(4) ;
    printf("Enter string to enter\n") ;
    scanf("%s", t) ;
    thtsearch(t, &root, cmp) ;
    printf("Enter string to delete\n") ;
    scanf("%s", s) ;
    thtdelete((char *) s, &root, cmp) ;
    printf("post \t "), thtpost(root,show), putchar('\n') ;


  }

  return(0);	
}
#endif

