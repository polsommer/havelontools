#ifdef RCSID
static char *RCSid =
   "$Header: oci11.c 14-jul-99.13:27:25 mjaeger Exp $ ";
#endif /* RCSID */

/* Copyright (c) 1995, 1999, Oracle Corporation.  All rights reserved.
*/

/*
   NAME
     oci11.c
   DESCRIPTION
     Demo program for A22400 OCI Techniques White Paper
     Tests array fetch using PL/SQL procedures via cursor variables
   MODIFIED   (MM/DD/YY)
    mjaeger    07/14/99 -  bug 808870: OCCS: convert tabs, no long lines
    ehayes     05/22/97 -  Fix VMS porting exceptions
    dchatter   07/15/96 -  hda is a ub4 array to prevent bus error
    vraghuna   03/01/95 -  Creation
*/
/*
 * Name:        oci11.c
 *
 * Description: This program retrieves the column information for a set of
 *              tables called TESTxx where xx is a number.
 *
 *              It illustrates getting data back using ARRAY FETCH
 *              through a PLSQL PROCEDURE using a RESULT SET
 *              It also illustrates re-executing the same query again without
 *              having to rebind
 *              The user can create multiple TESTxx tables
 *              The user can enter the number of rows to be fetched on each
 *              cycle
 *
 * Based on:    oci08.c
 * Changes:     second cursor now returned from within plsql procedure
 *              define calls needed based on second cursor
 *              fetch cyle on second cursor, exec on first cursor
 *
 * Setup:       Run corresponding ociXX.sql before running this program.
 *              Link program and run it.
 *              Program requires TWO arguments:
 *                          - the number of tables
 *                          - the number of rows to fetch per cycle
 *
 *                           oci11 4 3 <return>
 *
 * OCI Calls used:
 *
 *      Phase         OCI Call           Notes
 *      ------------------------------------------------------------------
 *      Login     -   olog               Use instead of orlon as of 7.2
 *      Open      -   oopen
 *      Parse     -   oparse
 *      Bind      -   obndra
 *      Describe  -   none               Hard-coded query
 *      Define    -   odefin             This is for the returned cursor
 *      Execute   -   oexec              For the plsql procedure
 *      Fetch     -   ofen               Fetch for second cursor.
 *      Close     -   oclose
 *      Logoff    -   olof
 *
 * This program is for educational purposes.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include <oratypes.h>
/* LDA and CDA struct declarations */
#include <ocidfn.h>
#ifdef __STDC__
#include <ociapr.h>
#else
#include <ocikpr.h>
#endif
/* demo constants and structs */
#include <ocidem.h>

#define MAX_NAME_LENGTH  30               /* Maximum length of a column name */

/* oparse flags */
#define  DEFER_PARSE        1
#define  NATIVE             1
#define  VERSION_7          2

/* exit flags */
#define OCI_EXIT_FAILURE 1
#define OCI_EXIT_SUCCESS 0

Lda_Def lda;                                                   /* login area */
ub4     hda[HDA_SIZE/(sizeof(ub4))];                            /* host area */
Cda_Def cda;                                                  /* cursor area */
Cda_Def cursor2;

/* Globals needed */
text tabname [MAX_NAME_LENGTH+1];       /* table name - generated on the fly */
uword tabnum = 0;                                            /* table number */
uword row_count = 0;                       /* number of rows returned so far */
uword numrows;                                            /* get it as input */

/* Function prototypes */
void gentable();
void logon ();
void logoff ();
void setup();
void err_report();
void get_data();
void dump_data();
void do_exit();

/* SQL statement used in this program */

text *sqlstmt = (text *)"\
                 begin \
                   oci11pkg.oci11proc(:curs,:tabname);\
                 end;";

#define NUMCOLS 3               /* number of columns returned by above query */
sword num_ret;            /* number of rows ACTUALLY returned from procedure */
sword done_fetch;                  /* any more rows to fetch for this table? */

/* array fetch definitions */
struct  col_struct{
         sword size;                                 /* length of the column */
         sword type;                                   /* type of the column */
       } ;

struct col_struct coltable [] =    /* query returns one char and two numbers */
       {  {MAX_NAME_LENGTH+1, SQLT_STR},
          {sizeof(eword), SQLT_INT},
          {sizeof(eword), SQLT_INT}
        };

struct   colp   {                                       /* Buffer descriptor */
         sword    c_type;             /* What type is this column fetched as */
         ub1     *c_buf;                     /* The area to store the column */
         ub2      c_size;                        /* Size of the storage area */
         sb2     *c_indp;              /* Indicator variable for this column */
         ub2     *c_rlen;                    /* Fetched length of the column */
         ub2     *c_rcode;                          /* array of return codes */
         ub4      c_curlen;                                /* current length */
  };/* COLUMN STRUCTURE */
typedef  struct   colp colp;
#define  malloc_col(n) (colp *)malloc((size_t)(n * sizeof(colp)));

colp *cols;                                              /* column structure */

main(argc, argv)
eword argc;
text **argv;
{

  uword numtabs;                               /* number of tables requested */
  uword tabindex;                                          /* for loop index */

  if (argc != 3)
  {
     printf("\n\n Usage: <prog> <numtabs> <numrows> \n");
     printf(" where numtabs is the number of tables with name TESTxx\n" );
     printf(" and numrows is number of rows per fetch cycle\n\n\n");
     do_exit(OCI_EXIT_FAILURE);
  }

  numtabs = atoi((char *)argv[1]);
  printf("\n Number of tables is %d\n", numtabs);

  numrows = atoi((char *)argv[2]);
  printf("\n Number of rows to be fetched per cycle is %d\n", numrows);

  logon();                                       /* logon to Oracle database */

  setup();                                          /* prepare sql statement */

  for (tabindex = 0; tabindex < numtabs; tabindex++)
  {
    gentable();

    if (oexec(&cda))
      {
        err_report(&cda);
        do_exit(OCI_EXIT_FAILURE);
      }

    get_data();
  }

  logoff();                                        /* logoff Oracle database */

  do_exit(OCI_EXIT_SUCCESS);

}

/*
 * Function: setup
 *
 * Description: This routine does the necessary setup to execute the SQL
 *              statement. Specifically, it does the open, parse, bind and
 *              define phases as needed.
 *
 */
void setup()
{

  memset((dvoid *)&cursor2, 0, sizeof(Cda_Def));

  if (oopen(&cda, &lda, (text *) 0, -1, -1, (text *) 0, -1))         /* open */
  {
    err_report(&cda);
    do_exit(OCI_EXIT_FAILURE);
  }

  if (oparse(&cda, sqlstmt, (sb4) -1, DEFER_PARSE,                  /* parse */
               (ub4) VERSION_7))
  {
    err_report(&cda);
    do_exit(OCI_EXIT_FAILURE);
  }

                                               /* bind all the scalar values */
  if (obndra(&cda, (text *)":curs", -1,
             (ub1 *)&cursor2, -1,
             SQLT_CUR, -1, (sb2 *) 0, (ub2 *)0, (ub2 *)0,
             (ub4)0, (ub4 *)0, (text *)0, -1, -1))                   /* bind */
  {
    err_report(&cda);
    do_exit(OCI_EXIT_FAILURE);
  }

  if (obndra(&cda, (text *)":tabname", -1,
             (ub1 *)tabname, 31,
             SQLT_STR, -1, (sb2 *) 0, (ub2 *)0, (ub2 *)0,
             (ub4)0, (ub4 *)0, (text *)0, -1, -1))                   /* bind */
  {
    err_report(&cda);
    do_exit(OCI_EXIT_FAILURE);
  }

}

/*
 * Function: get_data
 *
 * Description: This routine actually executes the cursor and fetches the
 *              data. It calls dump_data to print the data
 *
 */
void get_data()
{

  colp *colsptr;
  uword colindex;
  printf("\n printing column information for table %s :\n\n", tabname);

  cols = malloc_col(NUMCOLS);
  colsptr = cols;
  for (colindex = 0; colindex < NUMCOLS; colindex++, colsptr++)
  {

    colsptr->c_size  = coltable[colindex].size;
    colsptr->c_type  = coltable[colindex].type;
    colsptr->c_indp  = (sb2*)malloc((ub4)(numrows*sizeof(sb2)));
    colsptr->c_rlen  = (ub2*)malloc((ub4)(numrows*sizeof(ub2)));
    colsptr->c_rcode = (ub2*)malloc((ub4)(numrows*sizeof(ub2)));
    colsptr->c_buf   = (ub1 *)malloc((ub4)(numrows*colsptr->c_size));
    colsptr->c_curlen = 0;

    if (odefin(&cursor2, colindex + 1,
               colsptr->c_buf, colsptr->c_size,
               (sword)colsptr->c_type, (sword) -1,
               (sb2 *)colsptr->c_indp, (text *)0, -1, -1,
               colsptr->c_rlen, colsptr->c_rcode))
    {
      err_report(&cursor2);
      do_exit(OCI_EXIT_FAILURE);
    }
  }


  while(1)                          /* we definitely have more rows to fetch */
  {

    ofen(&cursor2, numrows);                             /* next fetch cycle */

    if ((cursor2.rc == NO_DATA_FOUND) && ((cursor2.rpc - row_count) == 0))
      break;

    if ((cursor2.rc == 0) ||
        (cursor2.rc == NO_DATA_FOUND) && (cursor2.rpc - row_count)> 0)
    {
     dump_data();
     if ((cursor2.rc == NO_DATA_FOUND) && (cursor2.rpc == row_count))
       break;
    }

    else
    {
      err_report(&cursor2);
      do_exit(OCI_EXIT_FAILURE);
    }

  }
  row_count = 0;                     /* reset running counter for next table */
}


/*
 * Function: dump_data
 *
 * Description: This routine prints out the row/s from each fetch
 *
 */
void dump_data()
{

  uword index;
  ub4 value;
  for (index = 0; index < cursor2.rpc - row_count; index++)
  {

    printf(" column %d name is %s\n", row_count + index + 1 ,
             cols[0].c_buf + index * cols[0].c_size);

    memcpy((dvoid *)&value, (dvoid *)(cols[1].c_buf + index * cols[1].c_size),
            *(cols[1].c_rlen + index));
    printf(" column %d length is %d \n", row_count + index + 1 , value);

    memcpy((dvoid *)&value, (dvoid *)(cols[2].c_buf + index * cols[2].c_size),
            *(cols[2].c_rlen + index));
    printf(" column %d id is %d \n", row_count + index + 1 , value);

  }

  row_count += cursor2.rpc - row_count;        /* we only need the increment */

  printf(" ==> Total rows so far is %d\n", row_count);
}

/*
 * Function: err_report
 *
 * Description: This routine prints out the most recent OCI error
 *
 */
void err_report(cursor)
Cda_Def *cursor;
{
    sword n;
    text msg[512];                      /* message buffer to hold error text */

    if (cursor->fc > 0)
      printf("\n-- ORACLE error when processing OCI function %s \n\n",
            oci_func_tab[cursor->fc]);
    else
      printf("\n-- ORACLE error\n");

    n = (sword)oerhms(&lda, cursor->rc, msg, (sword) sizeof msg);
    fprintf(stderr, "%s\n", msg);

}

/*
 * Function: gentable
 *
 * Description: This routine generates the next table name of the form TESTxx
 *
 */
void gentable()
{

  tabnum++;                                  /* generate the next table name */
  sprintf((char *)tabname, "TEST%d", tabnum);

}

/*
 * Function: do_exit
 *
 * Description: This routine exits with a status
 *
 */
void do_exit(status)
eword status;
{

  if (status == OCI_EXIT_FAILURE)
     printf("\n Exiting with FAILURE status %d\n", status);
  else
     printf("\n Exiting with SUCCESS status %d\n", status);

  exit(status);
}

/*
 * Function: login
 *
 * Description: This routine logs on onto the database as OCITEST/OCITEST
 *
 */
void logon()
{

  if (olog(&lda, (ub1 *)hda, (text *)"OCITEST", -1, (text *)"OCITEST", -1,
           (text *)0, -1, (ub4)OCI_LM_DEF))
  {
    err_report((Cda_Def *)&lda);
    exit(OCI_EXIT_FAILURE);
  }

  printf("\n Connected to ORACLE as ocitest\n");

}

/*
 * Function: logoff
 *
 * Description: This routine closes out any cursors and logs off the database
 *
 */
void logoff()
{

  if (oclose(&cda))                                          /* close cursor */
  {
    fprintf(stderr, "Error closing cursor 1.\n");
    do_exit(OCI_EXIT_FAILURE);
  }

  if (ologof(&lda))                                  /* log off the database */
  {
    fprintf(stderr, "Error on disconnect.\n");
    do_exit(OCI_EXIT_FAILURE);
  }

}

