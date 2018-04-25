//
// Ntuple in Cola++, mode in which ntuple will work in Cola++
//
// @(#) NT mode 
// @(#) Pablo Barneo
// @(#) NIKHEF, Amsterdam
//
// $Id: ntmode.h 2216 2008-06-13 21:13:47Z distler $
//
#ifndef __NT_MODE_H__
#define __NT_MODE_H__

#define MAX_NT_VAR      200    // max no of variables in ntuple
#define NT_VAR_LEN	8      // max lenght of variable name in ntuple
#define	VARNAMELEN	256    // max length of PVS variable names
#define MAX_TITLE_LEN   32
#define	BLOCKNAMELEN	8

#define NTUPLE_ID       10     // default ntuple id
#define NTUPLE_TIT      "COLA++ ntuple"

const int HBOOKMEM = 2000000;
const char NTNAME[8] = "NTUPLE";
const int LUN = 67;
const int LREC = 1024;

#define CHTOP           "COLA++\0\0\0"
#define UNDEFRANGE	(-1)

#define MAX_STR_LEN     2048

// He hecho una copia de esto en Cola_Ntuple porque creo que ese es el sitio
// correcto, pero hay que comprobarlo
//
// I have made a copy of this in Cola_Ntuple because I believe that that
// is the correct site, but it is necessary to verify it
//
// void    get_pvs_var(void *addr, int type, char *out);
// int     open_ntuple(int ntid, char tit[], int nvar, nt_t arr[]);
// int     dump_ntuple(int ntid);
// int     close_ntuple(void);

#endif // __NT_MODE_H__
