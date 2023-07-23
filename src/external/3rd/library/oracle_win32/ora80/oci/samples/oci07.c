#ifdef RCSID
static char *RCSid =
   "$Header: oci07.c 14-jul-99.13:25:05 mjaeger Exp $ ";
#endif /* RCSID */

/* Copyright (c) 1995, 1999, Oracle Corporation.  All rights reserved.
*/

/*
   NAME
     oci07.c
   DESCRIPTION
     Demo program for A22400 OCI Techniques White Paper
     Tests array fetch with piggyback of first fetch and execute
   MODIFIED   (MM/DD/YY)
    mjaeger    07/14/99 -  bug 808870: OCCS: convert tabs, no long lines
    ehayes     05/29/97 -  Fix olint errors
    dchatter   07/15/96 -  hda is a ub4 array to prevent bus error
    vraghuna   03/01/95 -  Creation
*/

/*
 * Name:        oci07.c
 *
 * Description: This program retrieves the column information for a single
 *              table called TEST1
 *
 *              It illustrates getting data back using ARRAY FETCH
 *              via a SELECT statement.
 *              The number of columns being returned is hard coded
 *              at 4 columns. The table has 4 columns for simplicity
 *              This program piggy backs the first fetch on the execute
 *              call
 *
 * Based on:    oci06.c
 * Changes:     changed oexec to oexfet
 *
 * Setup:       Run corresponding ociXX.sql before running this program.
 *              Link program and run it.
 *              Program requires NO arguments
 *
 *                           oci07 <return>
 *
 * OCI Calls used:
 *
 *      Phase         OCI Call           Notes
 *      ------------------------------------------------------------------
 *      Login     -   olog               Use instead of orlon as of 7.2
 *      Open      -   oopen
 *      Parse     -   oparse
 *      Bind      -   obndra             array bind by name
 *      Describe  -   none               Hard-coded query
 *      Define    -   odefin
 *      Execute   -   oexfet             Includes first fetch
 *      Fetch     -   none               Included in execute
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

/* Globals needed */
text tabname [MAX_NAME_LENGTH+1];       /* table name - generated on the fly */
uword tabnum = 0;                                            /* table number */
uword row_count = 0;                       /* number of rows returned so far */
uword numrows = 4;                                              /* HARDCODED */

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

text *sqlstmt = (text *)"SELECT cname, clength, colid \
                 FROM ocicolu\
                 WHERE tname = :tabname";

#define NUMCOLS 3               /* number of columns returned by above query */

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

  numtabs = 1;                                                  /* hardcoded */
  printf("\n Number of tables is %d\n", numtabs);

  logon();                                       /* logon to Oracle database */

  setup();                                          /* prepare sql statement */

  for (tabindex = 0; tabindex < numtabs; tabindex++)
  {
    gentable();
    get_data();                                             /* retrieve data */
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

  colp *colsptr;                                        /* temporary pointer */
  sword colindex;

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

  if (obndra(&cda, (text *)":tabname", -1,
             (ub1 *)tabname, (sword)sizeof(tabname),
             SQLT_STR, -1, (sb2 *) 0, (ub2 *)0, (ub2 *)0,
             (ub4)0, (ub4 *)0, (text *)0, -1, -1))                   /* bind */
  {
    err_report(&cda);
    do_exit(OCI_EXIT_FAILURE);
  }


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

    if (odefin(&cda, colindex + 1,
               colsptr->c_buf, colsptr->c_size,
               (sword)colsptr->c_type, (sword) -1,
               (sb2 *)colsptr->c_indp, (text *)0, -1, -1,
               colsptr->c_rlen, colsptr->c_rcode))
    {
      err_report(&cda);
      do_exit(OCI_EXIT_FAILURE);
    }

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

  printf("\n printing column information for table %s :\n\n", tabname);

                                                 /* expecting an EXACT match */
                                               /* cancel cursor after oexfet */
  if (oexfet(&cda, (ub4)numrows, -1, -1) == 0)
  {
    dump_data();
  }
  else
  {
    err_report(&cda);
    do_exit(OCI_EXIT_FAILURE);
  }


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
  for (index = 0; index < numrows; index++)
  {

    printf(" column %d name is %s\n", index + 1 ,
             cols[0].c_buf + index * cols[0].c_size);

    memcpy((dvoid *)&value, (dvoid *)(cols[1].c_buf + index * cols[1].c_size),
            *(cols[1].c_rlen + index));
    printf(" column %d length is %d \n", index + 1 , value);

    memcpy((dvoid *)&value, (dvoid *)(cols[2].c_buf + index * cols[2].c_size),
            *(cols[2].c_rlen + index));
    printf(" column %d id is %d \n", index + 1 , value);

  }
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
