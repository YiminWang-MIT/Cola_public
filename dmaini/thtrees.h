#ifndef  __thtrees_H__
#define __thtrees_H__
#ifdef __cplusplus
extern "C" {
#endif

/*
 *
 *	1990 H.Kramer, Institut fuer Kernphysik,
 *	Johannes Gutenberg-Universitaet Mainz
 *
 *
 *	@(#)thtrees.h	1.8	KPH	92/05/09	92/05/09
 *
 */


enum VISIT	{leaf,postorder,preorder,endorder} ;

typedef struct thnode {
	char *info;
	struct thnode *_l,*_r;
	char r_tag;
	} THNode;


#ifdef __STDC__
typedef int (* THT_ACTION) (char **, enum VISIT r, void *);

char ** thtfind(
	char *k,	/* Info */
	register THNode **rootp,
	int (*compar) (char *a, char * b)
	);
char ** thtsearch(
	char *k,	/* Info */
	register THNode **rootp,
	int (*compar)(char *a, char *b)
	);
void thtpost( /* left root right */
	register THNode *root ,
	int (*action) (char **, enum VISIT r, void *),
	void *     
	);
void thtpre(  /* root left right */
	    register THNode *root,
	    int (*action)(char ** info, enum VISIT order, void *),
	    void * );
void thtend(
	register THNode * root,
	int (* action)(char ** info,enum VISIT order, void *), void * 
	);

char * thtdelete(
	char *k, /* Info */
	register THNode ** rootp, /* --> Wurzel des Baums */
	int (* compar)(char *a, char * b) /* Vergleichsfunktion */
	);
char ** thtload(
	char * (* getinfo)(void),
	register THNode ** rootp
	);
void thtclear(
	THNode ** rootp,
	void (*action)(char ** info,enum  VISIT order) 
	);


#else
	char ** thtfind();
	char ** thtsearch();
	void thtpost();
	void thtpre();
	void thtend();
	char * thtdelete();
	char ** thtload();
	void thtclear();
#endif

#ifdef __cplusplus
}
#endif



#ifdef __cplusplus

class ThtIterator {
  THNode * root;
  enum { START, LEFT, THREAD}    direction;

public:
  ThtIterator(THNode *p) { root = p; direction = START;}
  char * post();
};

#endif


#endif /* __thtrees_H__ */
