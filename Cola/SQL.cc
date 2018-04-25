#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "rundatabase.h"
#include "Luminosity.h"

#ifndef HAVE_PGSQL
void updateSQLdatabase(const char *, luminosity *, const char *) {
  std::cerr << "WARNING: No support for SQL database access." << std::endl;
}
void readSQLdatabase(const char *, const char *, const char *, const char *) {
  std::cerr << "WARNING: No support for SQL database access." << std::endl;
}
#else
# ifdef HAVE_LIBPQPP_HH
#  include "libpq++.H"
static const char dummy = rcsid[0]; // this line to avoid warnings.
                                    // remove, if rcsid not defined
# elif defined(HAVE_LIBPQPP_H)
#ifdef HAVE_SQLCA_H
#include "sqlca.h" /* DLLIMPORT is defined here, no need for line below */
#endif
//#define DLLIMPORT /* fix PostgreSQL 7.4.2-1 nonsense on Fedora Core 2 */
#  ifdef HAVE_PG_CONFIG_H
#   undef PACKAGE_BUGREPORT
#   undef PACKAGE_NAME
#   undef PACKAGE_STRING
#   undef PACKAGE_TARNAME
#   undef PACKAGE_VERSION
#   include "pg_config.h"
#   define HAVE_NAMESPACE_STD 1
#  endif
#  include "libpq++.h"
# else
#  include "pgdatabase.h"
# endif

static const int maxsql = 8192; // libpq++ limit!

struct mytable { char *name, *rdbname, *sequence, query[maxsql]; 
  int  record;
} table[] = {
  {(char *)"lumi",         (char *)"Lumi_A",   (char *)"seq", "", 0},
  {(char *)"lumi",         (char *)"Lumi_B",   (char *)"seq", "", 0},
  {(char *)"lumi",         (char *)"Lumi_C",   (char *)"seq", "", 0},
  {(char *)"lumi",         (char *)"Lumi_AB",  (char *)"seq", "", 0},
  {(char *)"lumi",         (char *)"Lumi_BC",  (char *)"seq", "", 0},
  {(char *)"lumi",         (char *)"Lumi_AC",  (char *)"seq", "", 0},
  {(char *)"lumi",         (char *)"Lumi_ABC", (char *)"seq", "", 0},
  {(char *)"spectrometer", (char *)"A",        (char *)"seq", "", 0},
  {(char *)"spectrometer", (char *)"B",        (char *)"seq", "", 0},
  {(char *)"spectrometer", (char *)"C",        (char *)"seq", "", 0},
  {(char *)"spectrometer", (char *)"D",        (char *)"seq", "", 0},
  {(char *)"hdca",         (char *)"HDCA",     (char *)"seq", "", 0},
  {(char *)"target",       (char *)"Target",   (char *)"seq", "", 0},
  {(char *)"simul",        (char *)"Simul",    (char *)"seq", "", 0},
  {(char *)"setup",        (char *)"Setup",    (char *)"seq", "", 0},
  {(char *)"nnpol",        (char *)"nPol",     (char *)"seq", "", 0},
  {(char *)"tof",          (char *)"ToF",      (char *)"seq", "", 0},
  {(char *)"tof2",         (char *)"ToF2",     (char *)"seq", "", 0},
  {(char *)"tof3",         (char *)"ToF3",     (char *)"seq", "", 0},
  {(char *)"h1",           (char *)"H1",       (char *)"seq", "", 0},
  {(char *)"h2",           (char *)"H2",       (char *)"seq", "", 0},
  {(char *)"h3",           (char *)"H3",       (char *)"seq", "", 0},
  {(char *)"h4",           (char *)"H4",       (char *)"seq", "", 0},
      
  {NULL, NULL, NULL, "",0}
};

static inline int exec(PgDatabase &db, char *sql)
{
  if (db.Exec(sql) != PGRES_COMMAND_OK) {
    std::cerr<<"Can't execute SQL "<<sql<<std::endl;
    return 1;
  }
  return 0;
}

static void readfields(PgDatabase &db, const char *prefix) 
{
  char name[maxsql];

  for (int i=0; i<db.Fields(); i++) {
    const char *value = db.GetValue(0,i);
    if (value) 
      for (int j=0; lname[j].sql; j++) {
	if (!strncmp(lname[j].sql, prefix, strlen(prefix))) {
	  strncpy(name, lname[j].sql,maxsql-1);
	  for(int k=0;name[k];k++) name[k]=(name[k]=='.'?'_':tolower(name[k]));
	  if (!strcmp(name+strlen(prefix), db.FieldName(i))) {
	    union {double *d; char **s;} addr; addr.d=(double *)lname[j].addr;
	    switch(lname[j].type) {
	    case  1 :
	      *addr.d = atof(value); 
	      break;
	    case -1 : 	  
	      strcpy(*addr.s=new char[strlen(value)+1], value);
	      for (int k=0; (*addr.s)[k];k++) 
		if ((*addr.s)[k] == '`') (*addr.s)[k] = '\'';
	      break;
	    case 4711 : 
	      std::cerr << "DATABASE contains invalid entry "
			<< name << std::endl; 
	      break;
	    default :
	      char *cursor = (char *) index(value,'\"')+1; //"
	      if (cursor!=(char *) 1)
		for(int k=0;k<lname[j].type;k++) {
		  char *end = index(cursor,'\"'); //"
		  *end = 0;
		  addr.d[k] = atof(cursor);
		  cursor = index(end+1,'\"')+1; //"
		}
	    }
	  }
	}
      }
  }
}

static int update(PgDatabase &db, char *table, char *where,
	    char *field, int type, void *addr) 
{
  char sql[maxsql];

  switch (type) {
  case 1: 
    sprintf(sql, "UPDATE %s SET %s = %f WHERE %s;",
	    table, field, *(double *) addr, where);
    break;
  case -1:
    if (!*(char  **) addr) return 0; 
    char strng[maxsql];    
    strcpy(strng, *(char  **) addr);

    { 
      char *pos = index(strng, '\'');    
      while (pos) {
	char tmp[maxsql];
	strcpy(tmp, pos);
	strcpy(&pos[1], tmp);
	*pos = '\\';
	pos = index(&pos[2], '\'');
      }
    }
    sprintf(sql, "UPDATE %s SET %s = '%s' WHERE %s;",table,field,strng,where);
    break;
  case 4711: break;
  default:
    sprintf(sql, "UPDATE %s SET %s = '{" ,table, field);
    for(int j=0; j<type; j++) {
      char value[255];
      sprintf(value, j<type-1?"%g,":"%g", ((double *) addr)[j]); 
      strcat (sql,value);
    }
    strcat(sql, "}' WHERE ");
    strcat(sql, where);
    strcat(sql,";");
  }
  return !exec(db, sql);
}

void readSQLdatabase(const char *runname, const char *setup,
		     const char *host, const char *dbname) 
{
  char conninfo[255];
  sprintf(conninfo, "host=%s dbname=%s", host, dbname);
  PgDatabase db(conninfo);

  if (db.Status()!=CONNECTION_OK) {
    std::cerr << "ERROR: No connection to database /""/"
	      << host << "/" << dbname << std::endl;
    return;
  }
  
  char sql[maxsql];

  if (runname) {
    sprintf(sql, "select * from run where name = '%s'", runname);
    if (db.Exec(sql)!=PGRES_TUPLES_OK) return;
    if (db.Tuples() != 1) return;
  } else if (setup) {
    sprintf(sql, "select r.* from run r, setup s where "
	    "s.setup='%s' AND r.id_setup=s.id ORDER BY r.start_time", setup);
    if (db.Exec(sql)!=PGRES_TUPLES_OK) return;
    if (db.Tuples() == 0) return;
  } else {
    std::cerr << "I need setup or runname to access database" << std::endl;
    return;
  }

  char run[18];
  strncpy(run, db.GetValue(0,0), 17); 
  if (runname && strcmp(run,runname))
    std::cerr << "WARNING: Wrong run returned!";


  for (int i=0; table[i].name;i++) {
    sprintf(sql, "select x.* from run r, %s x WHERE r.id_%s=x.id "
	    "AND r.name='%s';", table[i].name, table[i].rdbname, run);

    if (db.Exec(sql)!=PGRES_TUPLES_OK)
      std::cerr << "Can't execute SQL " << sql << std::endl;
    if (db.Tuples() != 1) {
      std::cerr << "Table " << table[i].name << " not found" ; 
      return;
    }

    char prefix[255];
    strcpy(prefix,table[i].rdbname);
    strcat(prefix,".");
    readfields(db,prefix);
  }
}

void updateSQLdatabase(const char *runname, luminosity *lumi, 
		       const char *database) 
{
  if (strncmp(database, "/""/",2)) {
    std::cerr << "No database:" << database << "\n";
    return;
  }
  char *dbhost = new char[strlen(database)-1];
  strcpy(dbhost, database+2);
  char *dbname = index(dbhost,'/')+1;
  if (!dbname) {std::cerr<<"No database:"<<database<<"\n";return;}
  if (index(dbname,'/')) *index(dbname,'/') = '\0';
  *index(dbhost,'/') = '\0';

  char conninfo[255];
  sprintf(conninfo, "host=%s dbname=%s", dbhost, dbname);
  PgDatabase db(conninfo);

  if (db.Status()!=CONNECTION_OK) {
    std::cerr<< "Connect to " << database << " failed" << std::endl;
    return;
  }
  
  char sql[maxsql];
  char test[100000];
  // Create new run (if neccessary)
 
  sprintf(sql,"INSERT INTO run (name) VALUES ('%s');",runname);
  db.Exec(sql);

  // create new records in subtables

  for (int i=0; table[i].name; i++) {
    sprintf(sql, "SELECT NEXTVAL('%s');",table[i].sequence);
    if (db.Exec(sql)!=PGRES_TUPLES_OK) std::cerr<<"Can't execute SQL "<<sql<<std::endl;
    table[i].record = atoi(db.GetValue(0, 0));
    sprintf(sql, "INSERT INTO %s(id) VALUES (%d);",
	    table[i].name, table[i].record);
    if (db.Exec(sql)!=PGRES_COMMAND_OK) std::cerr<<"Can't execute SQL "<<sql<<std::endl;
    sprintf(sql, "UPDATE run SET Id_%s = %d where Name = '%s';",
	    table[i].rdbname, table[i].record, runname);
    if (db.Exec(sql)!=PGRES_COMMAND_OK) std::cerr<<"Can't execute SQL "<<sql<<std::endl;
    sprintf(test, "SELECT DISTINCT x.id FROM %s x, %s y "
	    "WHERE x.id != y.id AND y.id = %d",
	    table[i].name, table[i].name, table[i].record);
    if ((unsigned int)strlen(test)<(unsigned int)maxsql) 
      strcpy(table[i].query,test); 
    else
      std::cerr << "Query to long: "<<test<<std::endl;
  }

  for(int i=0; lname[i].sql; i++) {
    char field[255], where[255];
    strcpy (field, lname[i].sql);
    while (index(field,'.')) *(index(field,'.')) = '_';
    int done=0;
    for (int j=0; table[j].name; j++) {
      char comp[255];
      strcpy(comp,table[j].rdbname);
      strcat(comp,"_");
      if (strlen(field)>strlen(comp)) 
	if (!strncmp(field, comp, strlen(comp))) {
	  char *subfield = field + strlen(table[j].rdbname)+1;
	  sprintf(where,"id = %d", table[j].record);
	  if (update(db, table[j].name,where,subfield,
		     lname[i].type,lname[i].addr)) {
	    sprintf(where, " AND x.%s = y.%s", subfield, subfield);
	    strcat(table[j].query, where);
	  }
	  done=1;break;
	}
    }
    if (!done) {
      sprintf(where,"name = '%s'",runname);
      update(db, (char *)"run", where, field, lname[i].type, lname[i].addr);
    }
  }


  sprintf(sql, "update setup set username = '%s' from run "
	  "where id=run.id_setup AND run.name = '%s'", getenv("USER"),runname);
  if (db.Exec(sql) != PGRES_COMMAND_OK) 
    std::cerr<<"Can't execute SQL "<<sql<<std::endl;
  
  char *starttime, *stoptime;
  int events;
  double A[8], B[8], C[8], AB[8], BC[8], AC[8], ABC[8];
  lumi->output(starttime, stoptime, events, A,B,C,AB,BC,AC,ABC);
  if (!A[0]  ) A[1]   = A[2]   = 0;
  if (!B[0]  ) B[1]   = B[2]   = 0;
  if (!C[0]  ) C[1]   = C[2]   = 0;
  if (!AB[0] ) AB[1]  = AB[2]  = 0;
  if (!BC[0] ) BC[1]  = BC[2]  = 0;
  if (!AC[0] ) AC[1]  = AC[2]  = 0;
  if (!ABC[0]) ABC[1] = ABC[2] = 0;
  sprintf(sql, "update run set start_time = '%s', stop_time='%s', events=%d "
	  "where run.name = '%s'",starttime,stoptime,events,runname);
  if (db.Exec(sql) != PGRES_COMMAND_OK) 
    std::cerr<<"Can't execute SQL "<<sql<<std::endl;

  char *format=(char *)"update lumi SET Events='%.0f', Start_Event= '%.0f', "
	  "End_Event='%.0f', Run_Time='%g', Dead_time='%g', Charge='%g', "
	  "Avg_Current='%g', Corr_Luminosity='%g' WHERE id=%d;";

  sprintf(sql, format,
	  A[0],A[1],A[2],A[3],A[4],A[5],A[6],A[7],table[0].record);
  exec(db,sql);
  sprintf(sql, format,
	  B[0],B[1],B[2],B[3],B[4],B[5],B[6],B[7],table[1].record);
  exec(db,sql);
  sprintf(sql, format,
	  C[0],C[1],C[2],C[3],C[4],C[5],C[6],C[7],table[2].record);
  exec(db,sql);
  sprintf(sql, format,
	  AB[0],AB[1],AB[2],AB[3],AB[4],AB[5],AB[6],AB[7],table[3].record);
  exec(db,sql);
  sprintf(sql, format,
	  BC[0],BC[1],BC[2],BC[3],BC[4],BC[5],BC[6],BC[7],table[4].record);
  exec(db,sql);
  sprintf(sql, format,
	  AC[0],AC[1],AC[2],AC[3],AC[4],AC[5],AC[6],AC[7],table[5].record);
  exec(db,sql);
  sprintf(sql, format, ABC[0],ABC[1],ABC[2],ABC[3],ABC[4],ABC[5],ABC[6],ABC[7],
	  table[6].record);
  exec(db,sql);

  char *queryformat = (char *)
    "SELECT DISTINCT x.id FROM lumi x, lumi y WHERE x.id != y.id AND y.id = %d"
    " AND x.Events     =y.Events    AND x.Start_Event    =y.Start_Event"
    " AND x.End_Event  =y.End_Event AND x.Run_Time       =y.Run_Time"
    " AND x.Dead_time  =y.Dead_Time AND X.Charge         =y.charge"
    " AND x.Avg_Current=y.Avg_Current AND x.Corr_Luminosity=y.Corr_Luminosity";
    
  for(int i=0;i<7;i++) sprintf(table[i].query, queryformat, table[i].record);

  /////////////// Drop duplicate entries

  for (int i=0; table[i].name; i++) {
    strcat(table[i].query, " ORDER BY x.id;");
    int result = db.Exec(table[i].query);
    if (result == PGRES_TUPLES_OK) {
      if (db.Tuples() >= 1) {
	int newid = atoi(db.GetValue(0,0));
	sprintf(sql, "UPDATE run SET Id_%s = %d where Name = '%s';",
		table[i].rdbname, newid, runname);
	exec(db,sql);
	sprintf(sql, "DELETE FROM %s WHERE id=%d;",
		table[i].name, table[i].record);
	exec(db,sql);
      }
    } else if (result != PGRES_EMPTY_QUERY) 
      std::cerr<<"Can't execute QUERY "<<table[i].query<<std::endl;
  }
}

#endif
