/*******************************************************************************
 *
 * Copyright (c) 2000 Easysoft Ltd
 *
 * The contents of this file are subject to the Easysoft Public License 
 * Version 1.0 (the "License"); you may not use this file except in compliance 
 * with the License. 
 *
 * You may obtain a copy of the License at http://www.easysoft.org/EPL.html 
 *
 * Software distributed under the License is distributed on an "AS IS" basis, 
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License for 
 * the specific language governing rights and limitations under the License. 
 *
 * The Original Code was created by Easysoft Limited and its successors. 
 *
 * Contributor(s): Tom Fosdick (Easysoft) 
 *
 *******************************************************************************
 *
 * $Id: SQLGetTypeInfo.c,v 1.4 2004/08/27 19:45:40 dbox Exp $
 *
 * $Log: SQLGetTypeInfo.c,v $
 * Revision 1.4  2004/08/27 19:45:40  dbox
 * correct some bad behavior in ar/ir handles wrt number of records in re-used handles
 *
 * Revision 1.3  2004/08/06 20:43:23  dbox
 * comment out line stmt->current_ir->num_recs=0;
 *
 * Revision 1.2  2002/06/26 21:02:23  dbox
 * changed trace functions, setenv DEBUG 2 traces through SQLxxx functions
 * setenv DEBUG 3 traces through OCIxxx functions
 *
 *
 * VS: ----------------------------------------------------------------------
 *
 * Revision 1.1.1.1  2002/02/11 19:48:07  dbox
 * second try, importing code into directories
 *
 * Revision 1.14  2000/07/21 10:12:37  tom
 * lots of minor tweaks
 *
 * Revision 1.13  2000/07/10 08:24:35  tom
 * tweaks for less tolerant compilers
 *
 * Revision 1.12  2000/07/07 08:08:06  tom
 * initial implementation
 *
 * Revision 1.11  2000/06/05 16:05:37  tom
 * Hacks for Perl DBD::ODBC - nameley VARCHAR is apparently long :)
 *
 * Revision 1.10  2000/05/23 16:09:38  tom
 * Added LONG and tweaked a few others
 *
 * Revision 1.9  2000/05/12 10:59:50  tom
 * Checking for release candidate
 *
 * Revision 1.8  2000/05/11 13:17:32  tom
 * *** empty log message ***
 *
 * Revision 1.7  2000/05/08 16:21:00  tom
 * General tidyness mods and clean up
 *
 * Revision 1.6  2000/05/04 14:56:31  tom
 * diagnostics now cleared down (almost) properly
 * local functions renamed to make clashes less likely
 *
 * Revision 1.5  2000/05/03 16:00:02  tom
 * initial tracing implementation
 *
 * Revision 1.4  2000/05/02 14:29:01  tom
 * initial thread safety measures
 *
 * Revision 1.3  2000/04/20 10:50:31  nick
 * Add to CVS and tidy up
 *
 * Revision 1.2  2000/04/19 15:27:47  tom
 * First Functional Checkin
 *
 * Revision 1.1  2000/04/13 11:44:22  tom
 * Added files
 *
 ******************************************************************************/

#include "common.h"

static char const rcsid[]= "$RCSfile: SQLGetTypeInfo.c,v $ $Revision: 1.4 $";

/*
 * The information required for this isn't stored in Oracle, so
 * we have to invent it from thin air.
 */
/*
"TYPE_NAME", "DATA_TYPE","COLUMN_SIZE","LITERAL_PREFIX","LITERAL_SUFFIX","CREATE_PARAMS", "NULLABLE", "CASE_SENSITIVE", "SEARCHABLE", "UNSIGNED_ATTRIBUTE", "FIXED_PREC_SCALE", "AUTO_UNIQUE_VALUE", "LOCAL_TYPE_NAME", "MINIMUM_SCALE", "MAXIMUM_SCALE"
"BFILE",            -4, 2147483647, "'",    "'",    <Null>,            1, 1, 0, <Null>, 0, <Null>, "BFILE",            <Null>, <Null>
"BLOB",             -4, 2147483647, "'",    "'",    <Null>,            1, 1, 0, <Null>, 0, <Null>, "BLOB",             <Null>, <Null>
"CHAR",              1, 2000,       "'",    "'",    "max length",      1, 1, 3, <Null>, 0, 0,      "CHAR",             <Null>, <Null>
"CLOB",             -1, 2147483647, "'",    "'",    <Null>,            1, 1, 0, <Null>, 0, <Null>, "CLOB",             <Null>, <Null>
"DATE",             11, 19,         "'",    "'",    <Null>,            1, 0, 3, <Null>, 0, 0,      "DATE",             0,      0
"DOUBLE PRECISION",  8, 15,         <Null>, <Null>, <Null>,            1, 0, 3, 0,      0, 0,      "DOUBLE PRECISION", <Null>, <Null>
"LONG",             -1, 2147483647, "'",    "'",    <Null>,            1, 1, 0, <Null>, 0, <Null>, "LONG",             <Null>, <Null>
"LONG RAW",         -4, 2147483647, "'",    "'",    <Null>,            1, 0, 0, <Null>, 0, <Null>, "LONG RAW",         <Null>, <Null>
"DECIMAL",           3, 38,         <Null>, <Null>, "precision,scale", 1, 0, 3, 0,      0, 0,      "DECIMAL",          0,      38
"RAW",              -3, 2000,       "'",    "'",    "max length",      1, 0, 3, <Null>, 0, <Null>, "RAW",              <Null>, <Null>
"VARCHAR2",         12, 4000,       "'",    "'",    "max length",      1, 1, 3, <Null>, 0, 0,       "VARCHAR2",        <Null>, <Null>
*/

sword ood_fetch_sqlgettypeinfo(struct hStmt_TAG* stmt)
{
	/*const char *type_name[]={      "VARCHAR2",       "BFILE",            "BLOB",            "CHAR",         "CLOB",          "DATE",             "DOUBLE",       "LONG",          "LONG RAW",        "NUMBER",          "RAW",           };*/
	/*const int data_type[]={        SQL_VARCHAR,      SQL_LONGVARBINARY,  SQL_LONGVARBINARY, SQL_CHAR,       SQL_LONGVARCHAR, SQL_TIMESTAMP,      SQL_DOUBLE,     SQL_LONGVARCHAR, SQL_LONGVARBINARY, SQL_NUMERIC,       SQL_BINARY,      };*/
	/*const int column_size[]={      4000,             2147483647,         2147483647,        2000,           2147483647,      19,                 23,             2147483647,      2147483647,        38,                2000,            };*/
	/*const char *literal_fix[]={    "'",              "'",                "'",               "'",            "'",             "'",                "",             "'",             "'",               "",                "'",             }; */
	/*const char *create_params[]={  "max_length",     "",                 "",                "max length",   "",              "",                 "",             "",              "",                "precision,scale", "max_length",    };*/
	/*const int nullable[]={         1,                1,                  1,                 1,              1,               1,                  1,              1,               1,                 1,                 1,               };*/
	/*const int case_sensitive[]={   1,                1,                  1,                 1,              1,               0,                  0,              1,               0,                 0,                 0,               };*/
	/*const int searchable[]={       SQL_SEARCHABLE,   SQL_PRED_NONE,      SQL_PRED_NONE,     SQL_SEARCHABLE, SQL_PRED_NONE,   SQL_SEARCHABLE,     SQL_SEARCHABLE, SQL_PRED_NONE,   SQL_PRED_NONE,     SQL_SEARCHABLE,    SQL_PRED_NONE,   };*/
	/*const int unsigned_attr[]={    0,                0,                  0,                 0,              0,               0,                  SQL_FALSE,      0,               0,                 SQL_FALSE,         0,               };*/
	/*const int fixed_prec_scale[]={ 0,                0,                  0,                 0,              0,               0,                  0,              0,               0,                 0,                 0,               };*/
	/*const int auto_unique_val[]={  SQL_FALSE,        SQL_FALSE,          SQL_FALSE,         SQL_FALSE,      SQL_FALSE,       SQL_FALSE,          SQL_FALSE,      SQL_FALSE,       SQL_FALSE,         SQL_FALSE,         SQL_FALSE,       };*/
	/*local type name is the same as type_name */
	/*const int minimum_scale[]={    0,                0,                  0,                 0,              0,               0,                  0,              0,               0,                 0,                 0,               };*/
	/*const int maximum_scale[]={    0,                0,                  0,                 0,              0,               0,                  0,              0,               0,                 38,                0,               };*/
	/*const int sql_data_type[]={    SQL_VARCHAR,      SQL_LONGVARBINARY,  SQL_LONGVARBINARY, SQL_CHAR,       SQL_LONGVARCHAR, SQL_DATETIME,       SQL_DOUBLE,     SQL_LONGVARCHAR, SQL_LONGVARBINARY, SQL_NUMERIC,       SQL_BINARY,      };*/
	/*const int datetime_sub[]={     0,                0,                  0,                 0,              0,               SQL_CODE_TIMESTAMP, 0,              0,               0,                 0,                 0,               };*/
	/*const int num_prec_radix[]={   0,                0,                  0,                 0,              0,               0,                  10,             0,               0,                 10,                0,               };*/
	/*leading precision is always NULL*/

	const char *type_name[]={      "VARCHAR2",     "CHAR",        "NUMBER",           "DATE",             "DOUBLE",          "RAW",         "CLOB",           "LONG",             "BLOB",            "BFILE",           "LONG RAW"        };
    const int data_type[]={        SQL_VARCHAR,    SQL_CHAR,       SQL_NUMERIC,       SQL_TIMESTAMP,      SQL_DOUBLE,        SQL_BINARY,     SQL_LONGVARCHAR, SQL_LONGVARCHAR,    SQL_LONGVARBINARY, SQL_LONGVARBINARY, SQL_LONGVARBINARY };
    const int column_size[]={      4000,           2000,           38,                19,                 23,                2000,           2147483647,      2147483647,         2147483647,        2147483647,        2147483647        };
    const char *literal_fix[]={    "'",            "'",            "",                "'",                "",                "'",            "'",             "'",                "'",               "'",               "'"               }; 
    const char *create_params[]={  "max_length",   "max length",   "precision,scale", "",                 "",                "",             "",              "",                 "",                "",                ""                };
    const int nullable[]={         1,              1,              1,                 1,                  1,                 1,              1,               1,                  1,                 1,                 1                 };
    const int case_sensitive[]={   1,              1,              0,                 0,                  0,                 0,              1,               1,                  1,                 1,                 0                 };
    const int searchable[]={       SQL_SEARCHABLE, SQL_SEARCHABLE, SQL_SEARCHABLE,    SQL_SEARCHABLE,     SQL_SEARCHABLE,    SQL_PRED_NONE,  SQL_PRED_NONE,   SQL_PRED_NONE,      SQL_PRED_NONE,     SQL_PRED_NONE,     SQL_PRED_NONE     };
    const int unsigned_attr[]={    0,              0,              SQL_FALSE,         0,                  SQL_FALSE,         0,              0,               0,                  0,                 0,                 0                 };
    const int fixed_prec_scale[]={ 0,              0,              0,                 0,                  0,                 0,              0,               0,                  0,                 0,                 0                 };
    const int auto_unique_val[]={  SQL_FALSE,      SQL_FALSE,      SQL_FALSE,         SQL_FALSE,          SQL_FALSE,         SQL_FALSE,      SQL_FALSE,       SQL_FALSE,          SQL_FALSE,         SQL_FALSE,         SQL_FALSE         };
    /*local type name is the same as type_name */
    const int minimum_scale[]={    0,              0,              0,                 0,                  0,                 0,              0,               0,                  0,                 0,                 0                 };
    const int maximum_scale[]={    0,              0,              38,                0,                  0,                 0,              0,               0,                  0,                 0,                 0                 };
    const int sql_data_type[]={    SQL_VARCHAR,    SQL_CHAR,       SQL_NUMERIC,       SQL_DATETIME,       SQL_DOUBLE,        SQL_BINARY,     SQL_LONGVARCHAR, SQL_LONGVARCHAR,    SQL_LONGVARBINARY, SQL_LONGVARBINARY, SQL_LONGVARBINARY };
    const int datetime_sub[]={     0,              0,              0,                 SQL_CODE_TIMESTAMP, 0,                 0,              0,               0,                  0,                 0,                 0                 };
    const int num_prec_radix[]={   0,              0,              10,                0,                  10,                0,              0,               0,                  0,                 0,                 0                 };


	/* null_map[row_number][attribute_number]*/
	/*const char null_map[20][11]={ {0,               0,               0,                0,       0,              0,            0,          0,             0,                 0,         0},*/
	/* data_type */               /*{0,               0,               0,                0,       0,              0,            0,          0,             0,                 0,         0},*/
	/* column_size */             /*{0,               0,               0,                0,       0,              0,            0,          0,             0,                 0,         0},*/
	/* literal prefix */          /*{0,               0,               0,                0,       0,              0,            -1,         0,             0,                 -1,        0},*/
	/* literal suffix */          /*{0,               0,               0,                0,       0,              0,            -1,         0,             0,                 -1,        0},*/
	/* create params */           /*{0,              -1,              -1,                0,      -1,             -1,            -1,        -1,            -1,                 0,         0},*/
	/* nullable */                /*{0,               0,               0,                0,       0,              0,            0,          0,             0,                 0,         0},*/
	/* case sensitive */          /*{0,               0,               0,                0,       0,              0,            0,          0,             0,                 0,         0},*/
	/* searchable */              /*{0,               0,               0,                0,       0,              0,            0,          0,             0,                 0,         0},*/
	/* unsigned */                /*{-1,             -1,              -1,               -1,      -1,             -1,           -1,         0,             -1,                0,         -1},*/
	/* fixed_prec_scale */        /*{0,               0,               0,                0,       0,              0,            0,          0,             0,                 0,         0},*/
	/* auto_unique */             /*{0,              -1,              -1,                0,       -1,             0,            0,          -1,            -1,                0,         -1},*/
	/* local type name */         /*{0,               0,               0,                0,       0,              0,            0,          0,             0,                 0,         0},*/
	/* minimum_scale */           /*{-1,             -1,              -1,               -1,      -1,             -1,           0,          -1,            -1,                0,         -1},*/
	/* maximum_scale */           /*{-1,             -1,              -1,               -1,      -1,             -1,           0,          -1,            -1,                0,         -1},*/
	/* sql data type  */          /*{0,               0,               0,                0,       0,              0,            0,          0,             0,                 0,         0},*/
	/* datetime sub */            /*{-1,             -1,              -1,               -1,      -1,             0,            -1,         -1,            -1,                -1,        -1},*/
	/* num_prec_radix */          /*{-1,             -1,              -1,               -1,      -1,             -1,           0,          -1,            -1,                0,         -1},*/
	/* interval precision */      /*{-1,             -1,              -1,               -1,      -1,             -1,           -1,         -1,            -1,                -1,        -1} };*/

    const char null_map[20][11]={ {0,                0,                 0,              0,            0,         0,       0,          0,               0,               0,             0},
    /* data_type */               {0,                0,                 0,              0,            0,         0,       0,          0,               0,               0,             0},
    /* column_size */             {0,                0,                 0,              0,            0,         0,       0,          0,               0,               0,             0},
    /* literal prefix */          {0,                0,                 -1,              0,            -1,        0,       0,         0,               0,               0,             0},
    /* literal suffix */          {0,                0,                 -1,              0,            -1,        0,       0,         0,               0,               0,             0},
    /* create params */           {0,                0,                 0,             -1,            -1,         0,      -1,        -1,              -1,              -1,            -1},
    /* nullable */                {0,                0,                 0,              0,            0,         0,       0,          0,               0,               0,             0},
    /* case sensitive */          {0,                0,                 0,              0,            0,         0,       0,          0,               0,               0,             0},
    /* searchable */              {0,                0,                 0,              0,            0,         0,       0,          0,               0,               0,             0},
    /* unsigned */                {-1,               -1,                0,             -1,           -1,         -1,      -1,         0,              -1,             -1,             -1},
    /* fixed_prec_scale */        {0,                0,                 0,              0,            0,         0,       0,          0,               0,               0,             0},
    /* auto_unique */             {0,                0,                0,             0,            0,         -1,       -1,          -1,              -1,              -1,            -1},
    /* local type name */         {0,                0,                 0,              0,            0,         0,       0,          0,               0,               0,             0},
    /* minimum_scale */           {-1,               -1,                0,             -1,           0,         -1,      -1,          -1,              -1,             -1,            -1},
    /* maximum_scale */           {-1,               -1,                0,             -1,           0,         -1,      -1,          -1,              -1,             -1,            -1},
    /* sql data type  */          {0,                0,                 0,              0,            0,         0,       0,          0,               0,               0,             0},
    /* datetime sub */            {-1,               -1,                -1,             0,            -1,        -1,      -1,         -1,              -1,             -1,            -1},
    /* num_prec_radix */          {-1,               -1,                0,             -1,           0,         -1,      -1,          -1,              -1,             -1,            -1},
    /* interval precision */      {-1,               -1,                -1,             -1,           -1,        -1,      -1,         -1,              -1,             -1,            -1}};


    int ctr,fetch_row;
	unsigned int app_row;
	signed char *cptr;

	stmt->num_fetched_rows=0;
	for(app_row=0;app_row<stmt->row_array_size;app_row++)
	{
		stmt->current_row=app_row+1;
	    /*
	     * stmt->alt_fetch_data is a map of thirteen signed chars indicating
	     * which rows we should return.
	     *
	     * stmt->alt_fetch_data[0] is the row number we returned last time
	     * stmt->alt_fetch_data[12] is -1, indicating we should return
	     * OCI_NO_DATA
	     */
	    cptr=((signed char*)stmt->alt_fetch_data);
	    fetch_row=((signed char*)cptr)[0];
	    cptr++;
	    while(!cptr[fetch_row])
	    {
		    fetch_row++;
	    }
	    if(cptr[fetch_row]==-1)
	    {
			if(stmt->row_status_ptr)
			{
				stmt->row_status_ptr[app_row]=(SQLUSMALLINT)SQL_NO_DATA;
			}
			if(!app_row)
			{
				return OCI_NO_DATA;
			}
			continue;
	    }
		else
		{
			stmt->num_fetched_rows++;
			if(stmt->row_status_ptr)
			{
				stmt->row_status_ptr[app_row]=(SQLUSMALLINT)SQL_ROW_SUCCESS;
			}
    
	        for(ctr=0;ctr<19;ctr++)
	        {
		        stmt->current_ir->recs.ir[ctr+1].ind_arr[app_row]=
			        (sb2)null_map[ctr][fetch_row];
	        }
	        strcpy((char*)(stmt->current_ir->recs.ir[1].data_ptr)
				    +(app_row*stmt->current_ir->recs.ir[1].data_size),
			        type_name[fetch_row]);

            ((SQLINTEGER*)
			 stmt->current_ir->recs.ir[2].data_ptr)[app_row]=
		        data_type[fetch_row];

            ((SQLINTEGER*)
			 stmt->current_ir->recs.ir[3].data_ptr)[app_row]=
		        column_size[fetch_row];

	        strcpy((char*)(stmt->current_ir->recs.ir[4].data_ptr)
				+(app_row*stmt->current_ir->recs.ir[4].data_size),
			        literal_fix[fetch_row]);

	        strcpy((char*)(stmt->current_ir->recs.ir[5].data_ptr)
				+(app_row*stmt->current_ir->recs.ir[5].data_size),
			        literal_fix[fetch_row]);

	        strcpy((char*)(stmt->current_ir->recs.ir[6].data_ptr)
				+(app_row*stmt->current_ir->recs.ir[6].data_size),
			        create_params[fetch_row]);

            ((SQLINTEGER*)
			 stmt->current_ir->recs.ir[7].data_ptr)[app_row]=
		        nullable[fetch_row];

            ((SQLINTEGER*)
			 stmt->current_ir->recs.ir[8].data_ptr)[app_row]=
		        case_sensitive[fetch_row];

            ((SQLINTEGER*)
			 stmt->current_ir->recs.ir[9].data_ptr)[app_row]=
		        searchable[fetch_row];

            ((SQLINTEGER*)
			 stmt->current_ir->recs.ir[10].data_ptr)[app_row]=
		        unsigned_attr[fetch_row];

            ((SQLINTEGER*)
			 stmt->current_ir->recs.ir[11].data_ptr)[app_row]=
		        fixed_prec_scale[fetch_row];

            ((SQLINTEGER*)
			 stmt->current_ir->recs.ir[12].data_ptr)[app_row]=
		        auto_unique_val[fetch_row];
			
	        strcpy((char*)(stmt->current_ir->recs.ir[13].data_ptr)
				+(app_row*stmt->current_ir->recs.ir[13].data_size),
			        type_name[fetch_row]);

            ((SQLINTEGER*)
			 stmt->current_ir->recs.ir[14].data_ptr)[app_row]=
		        minimum_scale[fetch_row];

            ((SQLINTEGER*)
			 stmt->current_ir->recs.ir[15].data_ptr)[app_row]=
		        maximum_scale[fetch_row];

            ((SQLINTEGER*)
			 stmt->current_ir->recs.ir[16].data_ptr)[app_row]=
		        sql_data_type[fetch_row];

            ((SQLINTEGER*)
			 stmt->current_ir->recs.ir[17].data_ptr)[app_row]=
		        datetime_sub[fetch_row];

            ((SQLINTEGER*)
			 stmt->current_ir->recs.ir[18].data_ptr)[app_row]=
		        num_prec_radix[fetch_row];

            ((SQLINTEGER*)
			 stmt->current_ir->recs.ir[19].data_ptr)[app_row]=0;
    
#if defined(UNIX_DEBUG) && defined (ENABLE_TRACE)
	    ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
            (SQLHANDLE)stmt->dbc,0,"isiisssiiiiiisiiiiii",
			"FETCH ROW",app_row,
			"TYPE_NAME",stmt->current_ir->recs.ir[1].data_ptr,
			"DATATYPE",
			((SQLINTEGER*)
			 stmt->current_ir->recs.ir[2].data_ptr)[app_row],
			"COLUMN_SIZE",
			((SQLINTEGER*)
			 stmt->current_ir->recs.ir[3].data_ptr)[app_row],
			"LITERAL_PREFIX",(char*)stmt->current_ir->recs.ir[4].data_ptr
			+(app_row*stmt->current_ir->recs.ir[4].data_size),
			"LITERAL_SUFFIX",(char*)stmt->current_ir->recs.ir[5].data_ptr
			+(app_row*stmt->current_ir->recs.ir[5].data_size),
			"CREATE PARAMS",(char*)stmt->current_ir->recs.ir[6].data_ptr
			+(app_row*stmt->current_ir->recs.ir[6].data_size),
			"NULLABLE",
			((SQLINTEGER*)
			 stmt->current_ir->recs.ir[7].data_ptr)[app_row],
			"CASE_SENSITIVE",
			((SQLINTEGER*)
			 stmt->current_ir->recs.ir[8].data_ptr)[app_row],
			"SEARCHABLE",
			((SQLINTEGER*)
			 stmt->current_ir->recs.ir[9].data_ptr)[app_row],
			"UNSIGNED",
			((SQLINTEGER*)
			 stmt->current_ir->recs.ir[10].data_ptr)[app_row],
			"FIXED_PREC_SCALE",
			((SQLINTEGER*)
			 stmt->current_ir->recs.ir[11].data_ptr)[app_row],
			"AUTO_UNIQUE",
			((SQLINTEGER*)
			 stmt->current_ir->recs.ir[12].data_ptr)[app_row],
			"LOCAL_TYPE_NAME",(char*)stmt->current_ir->recs.ir[13].data_ptr
				+(app_row*stmt->current_ir->recs.ir[7].data_size),
			"MINIMUM_SCALE",
			((SQLINTEGER*)
			 stmt->current_ir->recs.ir[14].data_ptr)[app_row],
			"MAXIMUM_SCALE",
			((SQLINTEGER*)
			 stmt->current_ir->recs.ir[15].data_ptr)[app_row],
			"SQL_DATA_TYPE",
			((SQLINTEGER*)
			 stmt->current_ir->recs.ir[16].data_ptr)[app_row],
			"SQL_DATETIME_SUB",
			((SQLINTEGER*)
			 stmt->current_ir->recs.ir[17].data_ptr)[app_row],
			"NUM_PREC_RADIX",
			((SQLINTEGER*)
			 stmt->current_ir->recs.ir[18].data_ptr)[app_row],
			"INTERVAL_PREC",
			((SQLINTEGER*)
			 stmt->current_ir->recs.ir[19].data_ptr)[app_row]);
#endif


	        stmt->num_result_rows++;
	        ((unsigned char*)stmt->alt_fetch_data)[0]=fetch_row+1;
		}
	}
	return OCI_SUCCESS;
}

SQLRETURN SQL_API SQLGetTypeInfo(
    SQLHSTMT        StatementHandle,
    SQLSMALLINT        DataType )
{
    SQLRETURN status=SQL_SUCCESS;
    hStmt_T* stmt=(hStmt_T*)StatementHandle;
    ir_T* ir;
    ar_T* ar;
	
if(ENABLE_TRACE){
    ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_ENTRY,
            (SQLHANDLE)stmt,0,"ii",
			"DataType",DataType,
			"stmt->row_array_size",stmt->row_array_size);
}
    ood_clear_diag((hgeneric*)stmt);
    ood_mutex_lock_stmt(stmt);

    /* Clear old data out of stmt->current_ir so it can be rebound.
       The data in stmt->current_ar must not be touched, since it
       may contain already bound ODBC columns. */

    ood_ir_array_reset (stmt->current_ir->recs.ir, stmt->current_ir->num_recs,
			stmt->current_ir);

    if(SQL_SUCCESS!=ood_alloc_col_desc(stmt,19,stmt->current_ir,
				       stmt->current_ar))
    {
if(ENABLE_TRACE){
        ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
                (SQLHANDLE)NULL,SQL_ERROR,"");
}
        ood_mutex_unlock_stmt(stmt);
        return SQL_ERROR;
    }

	/*
	 * Assign the alternative fetch function
	 */
	stmt->alt_fetch=ood_fetch_sqlgettypeinfo;

	stmt->alt_fetch_data=ORAMALLOC(14*sizeof(signed char));
	((signed char*)stmt->alt_fetch_data)[0]=0;
	((signed char*)stmt->alt_fetch_data)[12]=-1;

	switch(DataType)
	{
		case SQL_ALL_TYPES:
			((signed char*)stmt->alt_fetch_data)[1]=1;
			((signed char*)stmt->alt_fetch_data)[2]=1;
			((signed char*)stmt->alt_fetch_data)[3]=1;
			((signed char*)stmt->alt_fetch_data)[4]=1;
			((signed char*)stmt->alt_fetch_data)[5]=1;
			((signed char*)stmt->alt_fetch_data)[6]=1;
			((signed char*)stmt->alt_fetch_data)[7]=1;
			((signed char*)stmt->alt_fetch_data)[8]=1;
			((signed char*)stmt->alt_fetch_data)[9]=1;
			((signed char*)stmt->alt_fetch_data)[10]=1;
			((signed char*)stmt->alt_fetch_data)[11]=1;
			break;
	    
	    case SQL_VARCHAR:
			((signed char*)stmt->alt_fetch_data)[1]=1;
			((signed char*)stmt->alt_fetch_data)[2]=0;
			((signed char*)stmt->alt_fetch_data)[3]=0;
			((signed char*)stmt->alt_fetch_data)[4]=0;
			((signed char*)stmt->alt_fetch_data)[5]=0;
			((signed char*)stmt->alt_fetch_data)[6]=0;
			((signed char*)stmt->alt_fetch_data)[7]=1;
			((signed char*)stmt->alt_fetch_data)[8]=1;
			((signed char*)stmt->alt_fetch_data)[9]=0;
			((signed char*)stmt->alt_fetch_data)[10]=0;
			((signed char*)stmt->alt_fetch_data)[11]=0;
			break;
        
		case SQL_CHAR:
			((signed char*)stmt->alt_fetch_data)[1]=0;
			((signed char*)stmt->alt_fetch_data)[2]=1;
			((signed char*)stmt->alt_fetch_data)[3]=0;
			((signed char*)stmt->alt_fetch_data)[4]=0;
			((signed char*)stmt->alt_fetch_data)[5]=0;
			((signed char*)stmt->alt_fetch_data)[6]=0;
			((signed char*)stmt->alt_fetch_data)[7]=0;
			((signed char*)stmt->alt_fetch_data)[8]=0;
			((signed char*)stmt->alt_fetch_data)[9]=0;
			((signed char*)stmt->alt_fetch_data)[10]=0;
			((signed char*)stmt->alt_fetch_data)[11]=0;
			break;

	    case SQL_DECIMAL:
	    case SQL_NUMERIC:
			((signed char*)stmt->alt_fetch_data)[1]=0;
			((signed char*)stmt->alt_fetch_data)[2]=0;
			((signed char*)stmt->alt_fetch_data)[3]=1;
			((signed char*)stmt->alt_fetch_data)[4]=0;
			((signed char*)stmt->alt_fetch_data)[5]=0;
			((signed char*)stmt->alt_fetch_data)[6]=0;
			((signed char*)stmt->alt_fetch_data)[7]=0;
			((signed char*)stmt->alt_fetch_data)[8]=0;
			((signed char*)stmt->alt_fetch_data)[9]=0;
			((signed char*)stmt->alt_fetch_data)[10]=0;
			((signed char*)stmt->alt_fetch_data)[11]=0;
			break;

	    
		case SQL_DOUBLE:
			((signed char*)stmt->alt_fetch_data)[1]=0;
			((signed char*)stmt->alt_fetch_data)[2]=0;
			((signed char*)stmt->alt_fetch_data)[3]=0;
			((signed char*)stmt->alt_fetch_data)[4]=0;
			((signed char*)stmt->alt_fetch_data)[5]=1;
			((signed char*)stmt->alt_fetch_data)[6]=0;
			((signed char*)stmt->alt_fetch_data)[7]=0;
			((signed char*)stmt->alt_fetch_data)[8]=0;
			((signed char*)stmt->alt_fetch_data)[9]=0;
			((signed char*)stmt->alt_fetch_data)[10]=0;
			((signed char*)stmt->alt_fetch_data)[11]=0;
			break;

		case SQL_VARBINARY:
			((signed char*)stmt->alt_fetch_data)[1]=0;
			((signed char*)stmt->alt_fetch_data)[2]=0;
			((signed char*)stmt->alt_fetch_data)[3]=0;
			((signed char*)stmt->alt_fetch_data)[4]=0;
			((signed char*)stmt->alt_fetch_data)[5]=0;
			((signed char*)stmt->alt_fetch_data)[6]=1;
			((signed char*)stmt->alt_fetch_data)[7]=0;
			((signed char*)stmt->alt_fetch_data)[8]=0;
			((signed char*)stmt->alt_fetch_data)[9]=1;
			((signed char*)stmt->alt_fetch_data)[10]=1;
			((signed char*)stmt->alt_fetch_data)[11]=1;
			break;

		case SQL_LONGVARCHAR:
			((signed char*)stmt->alt_fetch_data)[1]=0;
			((signed char*)stmt->alt_fetch_data)[2]=0;
			((signed char*)stmt->alt_fetch_data)[3]=0;
			((signed char*)stmt->alt_fetch_data)[4]=0;
			((signed char*)stmt->alt_fetch_data)[5]=0;
			((signed char*)stmt->alt_fetch_data)[6]=0;
			((signed char*)stmt->alt_fetch_data)[7]=1; /* TODO this is CLOB */
			((signed char*)stmt->alt_fetch_data)[8]=1; /* TODO this is LONG */
			((signed char*)stmt->alt_fetch_data)[9]=0;
			((signed char*)stmt->alt_fetch_data)[10]=0;
			((signed char*)stmt->alt_fetch_data)[11]=0;
			break;

		case SQL_LONGVARBINARY:
			((signed char*)stmt->alt_fetch_data)[1]=0;
			((signed char*)stmt->alt_fetch_data)[2]=0;
			((signed char*)stmt->alt_fetch_data)[3]=0;
			((signed char*)stmt->alt_fetch_data)[4]=0;
			((signed char*)stmt->alt_fetch_data)[5]=0;
			((signed char*)stmt->alt_fetch_data)[6]=0;
			((signed char*)stmt->alt_fetch_data)[7]=0;
			((signed char*)stmt->alt_fetch_data)[8]=0;
			((signed char*)stmt->alt_fetch_data)[9]=1;
			((signed char*)stmt->alt_fetch_data)[10]=1;
			((signed char*)stmt->alt_fetch_data)[11]=1;
			break;

		case SQL_TYPE_TIMESTAMP:
		case SQL_TIMESTAMP:
			((signed char*)stmt->alt_fetch_data)[1]=0;
			((signed char*)stmt->alt_fetch_data)[2]=0;
			((signed char*)stmt->alt_fetch_data)[3]=0;
			((signed char*)stmt->alt_fetch_data)[4]=1;
			((signed char*)stmt->alt_fetch_data)[5]=0;
			((signed char*)stmt->alt_fetch_data)[6]=0;
			((signed char*)stmt->alt_fetch_data)[7]=0;
			((signed char*)stmt->alt_fetch_data)[8]=0;
			((signed char*)stmt->alt_fetch_data)[9]=0;
			((signed char*)stmt->alt_fetch_data)[10]=0;
			((signed char*)stmt->alt_fetch_data)[11]=0;
			break;
			
        case SQL_SMALLINT:
		case SQL_TINYINT:
		case SQL_INTEGER:
		case SQL_REAL:
		case SQL_FLOAT:
		case SQL_BIT:
		case SQL_BIGINT:
		case SQL_BINARY:
		case SQL_TYPE_DATE:
		case SQL_TYPE_TIME:
		default:
			((signed char*)stmt->alt_fetch_data)[1]=0;
			((signed char*)stmt->alt_fetch_data)[2]=0;
			((signed char*)stmt->alt_fetch_data)[3]=0;
			((signed char*)stmt->alt_fetch_data)[4]=0;
			((signed char*)stmt->alt_fetch_data)[5]=0;
			((signed char*)stmt->alt_fetch_data)[6]=0;
			((signed char*)stmt->alt_fetch_data)[7]=0;
			((signed char*)stmt->alt_fetch_data)[8]=0;
			((signed char*)stmt->alt_fetch_data)[9]=0;
			((signed char*)stmt->alt_fetch_data)[10]=0;
			((signed char*)stmt->alt_fetch_data)[11]=0;
			break;
	}

    ir=stmt->current_ir->recs.ir;
    ar=stmt->current_ar->recs.ar;

    /* stmt->current_ir->num_recs is equal to the allocated size of the
       ir and ar arrays. Shouldn't expect it to record the number of
       bound parameters.  
       stmt->current_ir->num_recs=19; */

	/*
     * Col 0 is bookmark, not implemented yet
     */
    ir++,ar++;
    /*
     * Col 1 is TYPE_NAME, VARCHAR
     */
	ir->ind_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(sb2));
	memset(ir->ind_arr,0,ir->desc->stmt->row_array_size*sizeof(sb2));
	ir->length_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(ub2));
	ir->rcode_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(ub2));
    status|=ood_assign_ir(ir,SQLT_STR,16,0,
            ocistr_sqlnts,ocistr_sqlnts);
    ar->concise_type=ar->data_type=SQL_C_CHAR;
    ar->display_size=15;
    ar->octet_length=ar->length=16;
    strcpy((char*)ar->column_name,"TYPE_NAME");
	if(!ir->data_ptr)
        ir->data_ptr=ORAMALLOC(ir->data_size*stmt->row_array_size);

    ir++,ar++;

	/*
     * Col 2 is DATA_TYPE, SQL_SMALLINT
     */
	ir->ind_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(sb2));
	memset(ir->ind_arr,0,ir->desc->stmt->row_array_size*sizeof(sb2));
	ir->length_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(ub2));
	ir->rcode_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(ub2));
    status|=ood_assign_ir(ir,SQLT_INT,sizeof(long),0,
            ociint_sqlslong,ociint_sqlnts);
    ar->concise_type=ar->data_type=SQL_C_SSHORT;
    ar->display_size=sqltype_display_size(SQL_C_SSHORT,0);
    ar->octet_length=ar->length=sizeof(long);
    strcpy((char*)ar->column_name,"DATA_TYPE");
	if(!ir->data_ptr)
        ir->data_ptr=ORAMALLOC(ir->data_size*stmt->row_array_size*stmt->row_array_size);

    ir++,ar++;

	/*
	 * Col 3 is column size, integer
	 */
	ir->ind_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(sb2));
	memset(ir->ind_arr,0,ir->desc->stmt->row_array_size*sizeof(sb2));
	ir->length_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(ub2));
	ir->rcode_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(ub2));
    status|=ood_assign_ir(ir,SQLT_INT,sizeof(long),0,
            ociint_sqlslong,ociint_sqlnts);
    ar->concise_type=ar->data_type=SQL_C_SLONG;
    ar->display_size=sqltype_display_size(SQL_C_SLONG,0);
    ar->octet_length=ar->length=sizeof(long);
    strcpy((char*)ar->column_name,"COLUMN_SIZE");
	if(!ir->data_ptr)
        ir->data_ptr=ORAMALLOC(ir->data_size*stmt->row_array_size);

    ir++,ar++;

	/*
	 * Col 4 is LITERAL_PREFIX, varchar(3)
	 */
	ir->ind_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(sb2));
	memset(ir->ind_arr,0,ir->desc->stmt->row_array_size*sizeof(sb2));
	ir->length_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(ub2));
	ir->rcode_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(ub2));
    status|=ood_assign_ir(ir,SQLT_STR,3,0,
            ocistr_sqlnts,ocistr_sqlnts);
    ar->concise_type=ar->data_type=SQL_C_CHAR;
    ar->display_size=2;
    ar->octet_length=ar->length=3;
    strcpy((char*)ar->column_name,"LITERAL_PREFIX");
	if(!ir->data_ptr)
        ir->data_ptr=ORAMALLOC(ir->data_size*stmt->row_array_size);

    ir++,ar++;

	/*
	 * Col 5 is LITERAL_SUFFIX
	 */
	ir->ind_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(sb2));
	memset(ir->ind_arr,0,ir->desc->stmt->row_array_size*sizeof(sb2));
	ir->length_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(ub2));
	ir->rcode_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(ub2));
    status|=ood_assign_ir(ir,SQLT_STR,3,0,
            ocistr_sqlnts,ocistr_sqlnts);
    ar->concise_type=ar->data_type=SQL_C_CHAR;
    ar->display_size=2;
    ar->octet_length=ar->length=3;
    strcpy((char*)ar->column_name,"LITERAL_SUFFIX");
	if(!ir->data_ptr)
        ir->data_ptr=ORAMALLOC(ir->data_size*stmt->row_array_size);

    ir++,ar++;

	/*
	 * Col 6 is CREATE_PARAMS
	 */
	ir->ind_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(sb2));
	memset(ir->ind_arr,0,ir->desc->stmt->row_array_size*sizeof(sb2));
	ir->length_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(ub2));
	ir->rcode_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(ub2));
    status|=ood_assign_ir(ir,SQLT_STR,32,0,
            ocistr_sqlnts,ocistr_sqlnts);
    ar->concise_type=ar->data_type=SQL_C_CHAR;
    ar->display_size=31;
    ar->octet_length=ar->length=32;
    strcpy((char*)ar->column_name,"CREATE_PARAMS");
	if(!ir->data_ptr)
        ir->data_ptr=ORAMALLOC(ir->data_size*stmt->row_array_size);

    ir++,ar++;

	/*
	 * Col 7 is the NULLABLE contingent
	 */
	ir->ind_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(sb2));
	memset(ir->ind_arr,0,ir->desc->stmt->row_array_size*sizeof(sb2));
	ir->length_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(ub2));
	ir->rcode_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(ub2));
    status|=ood_assign_ir(ir,SQLT_INT,sizeof(long),0,
            ociint_sqlslong,ociint_sqlnts);
    ar->concise_type=ar->data_type=SQL_C_SSHORT;
    ar->display_size=sqltype_display_size(SQL_C_SSHORT,0);
    ar->octet_length=ar->length=sizeof(long);
    strcpy((char*)ar->column_name,"NULLABLE");
	if(!ir->data_ptr)
        ir->data_ptr=ORAMALLOC(ir->data_size*stmt->row_array_size);

    ir++,ar++;

	/*
	 * Col 8 is CASE_SENSITIVE
	 */
	ir->ind_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(sb2));
	memset(ir->ind_arr,0,ir->desc->stmt->row_array_size*sizeof(sb2));
	ir->length_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(ub2));
	ir->rcode_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(ub2));
    status|=ood_assign_ir(ir,SQLT_INT,sizeof(long),0,
            ociint_sqlslong,ociint_sqlnts);
    ar->concise_type=ar->data_type=SQL_C_SSHORT;
    ar->display_size=sqltype_display_size(SQL_C_SSHORT,0);
    ar->octet_length=ar->length=sizeof(long);
    strcpy((char*)ar->column_name,"CASE_SENSITIVE");
	if(!ir->data_ptr)
        ir->data_ptr=ORAMALLOC(ir->data_size*stmt->row_array_size);

    ir++,ar++;
    
	/*
	 * Col 9 is SEARCHABLE
	 */
	ir->ind_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(sb2));
	memset(ir->ind_arr,0,ir->desc->stmt->row_array_size*sizeof(sb2));
	ir->length_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(ub2));
	ir->rcode_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(ub2));
    status|=ood_assign_ir(ir,SQLT_INT,sizeof(long),0,
            ociint_sqlslong,ociint_sqlnts);
    ar->concise_type=ar->data_type=SQL_C_SSHORT;
    ar->display_size=sqltype_display_size(SQL_C_SSHORT,0);
    ar->octet_length=ar->length=sizeof(long);
    strcpy((char*)ar->column_name,"SEARCHABLE");
	if(!ir->data_ptr)
        ir->data_ptr=ORAMALLOC(ir->data_size*stmt->row_array_size);

    ir++,ar++;

	/*
	 * Col 10 is UNSIGN_ATTRIBUTE
	 */
	ir->ind_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(sb2));
	memset(ir->ind_arr,0,ir->desc->stmt->row_array_size*sizeof(sb2));
	ir->length_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(ub2));
	ir->rcode_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(ub2));
    status|=ood_assign_ir(ir,SQLT_INT,sizeof(long),0,
            ociint_sqlslong,ociint_sqlnts);
    ar->concise_type=ar->data_type=SQL_C_SSHORT;
    ar->display_size=sqltype_display_size(SQL_C_SSHORT,0);
    ar->octet_length=ar->length=sizeof(long);
    strcpy((char*)ar->column_name,"UNSIGNED_ATTRIBUTE");
	if(!ir->data_ptr)
        ir->data_ptr=ORAMALLOC(ir->data_size*stmt->row_array_size);

    ir++,ar++;

	/*
	 * Col 11 is FIXED_PREC_SCALE
	 */
	ir->ind_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(sb2));
	memset(ir->ind_arr,0,ir->desc->stmt->row_array_size*sizeof(sb2));
	ir->length_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(ub2));
	ir->rcode_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(ub2));
    status|=ood_assign_ir(ir,SQLT_INT,sizeof(long),0,
            ociint_sqlslong,ociint_sqlnts);
    ar->concise_type=ar->data_type=SQL_C_SSHORT;
    ar->display_size=sqltype_display_size(SQL_C_SSHORT,0);
    ar->octet_length=ar->length=sizeof(long);
    strcpy((char*)ar->column_name,"FIXED_PREC_SCALE");
	if(!ir->data_ptr)
        ir->data_ptr=ORAMALLOC(ir->data_size*stmt->row_array_size);

    ir++,ar++;

	/*
	 * Col 12 is AUTO_UNIQUE
	 */
	ir->ind_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(sb2));
	memset(ir->ind_arr,0,ir->desc->stmt->row_array_size*sizeof(sb2));
	ir->length_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(ub2));
	ir->rcode_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(ub2));
    status|=ood_assign_ir(ir,SQLT_INT,sizeof(long),0,
            ociint_sqlslong,ociint_sqlnts);
    ar->concise_type=ar->data_type=SQL_C_SSHORT;
    ar->display_size=sqltype_display_size(SQL_C_SSHORT,0);
    ar->octet_length=ar->length=sizeof(long);
    strcpy((char*)ar->column_name,"AUTO_UNIQUE_VALUE");
	if(!ir->data_ptr)
        ir->data_ptr=ORAMALLOC(ir->data_size*stmt->row_array_size);

    ir++,ar++;

	/*
	 * Col 13 is the LOCAL_TYPE_NAME
	 */
	ir->ind_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(sb2));
	memset(ir->ind_arr,0,ir->desc->stmt->row_array_size*sizeof(sb2));
	ir->length_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(ub2));
	ir->rcode_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(ub2));
    status|=ood_assign_ir(ir,SQLT_STR,16,0,
            ocistr_sqlnts,ocistr_sqlnts);
    ar->concise_type=ar->data_type=SQL_C_CHAR;
    ar->display_size=15;
    ar->octet_length=ar->length=16;
    strcpy((char*)ar->column_name,"LOCAL_TYPE_NAME");
	if(!ir->data_ptr)
        ir->data_ptr=ORAMALLOC(ir->data_size*stmt->row_array_size*2);

    ir++,ar++;

	/* 
	 * Col 14 is the MINIMUM SCALE
	 */
	ir->ind_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(sb2));
	memset(ir->ind_arr,0,ir->desc->stmt->row_array_size*sizeof(sb2));
	ir->length_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(ub2));
	ir->rcode_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(ub2));
    status|=ood_assign_ir(ir,SQLT_INT,sizeof(long),0,
            ociint_sqlslong,ociint_sqlnts);
    ar->concise_type=ar->data_type=SQL_C_SSHORT;
    ar->display_size=sqltype_display_size(SQL_C_SSHORT,0);
    ar->octet_length=ar->length=sizeof(long);
    strcpy((char*)ar->column_name,"MINIMUM_SCALE");
	if(!ir->data_ptr)
        ir->data_ptr=ORAMALLOC(ir->data_size*stmt->row_array_size);

    ir++,ar++;

	/*
	 * Col 15 is the MAXIMUM SCALE
	 */
	ir->ind_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(sb2));
	memset(ir->ind_arr,0,ir->desc->stmt->row_array_size*sizeof(sb2));
	ir->length_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(ub2));
	ir->rcode_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(ub2));
    status|=ood_assign_ir(ir,SQLT_INT,sizeof(long),0,
            ociint_sqlslong,ociint_sqlnts);
    ar->concise_type=ar->data_type=SQL_C_SSHORT;
    ar->display_size=sqltype_display_size(SQL_C_SSHORT,0);
    ar->octet_length=ar->length=sizeof(long);
    strcpy((char*)ar->column_name,"MAXIMUM_SCALE");
	if(!ir->data_ptr)
        ir->data_ptr=ORAMALLOC(ir->data_size*stmt->row_array_size);

    ir++,ar++;

	/* 
	 * Col 16 is the SQL_DATA_TYPE
	 */
	ir->ind_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(sb2));
	memset(ir->ind_arr,0,ir->desc->stmt->row_array_size*sizeof(sb2));
	ir->length_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(ub2));
	ir->rcode_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(ub2));
    status|=ood_assign_ir(ir,SQLT_INT,sizeof(long),0,
            ociint_sqlslong,ociint_sqlnts);
    ar->concise_type=ar->data_type=SQL_C_SSHORT;
    ar->display_size=sqltype_display_size(SQL_C_SSHORT,0);
    ar->octet_length=ar->length=sizeof(long);
    strcpy((char*)ar->column_name,"SQL_DATA_TYPE");
	if(!ir->data_ptr)
        ir->data_ptr=ORAMALLOC(ir->data_size*stmt->row_array_size);

    ir++,ar++;

	/*
	 * Col 17 is SQL_DATETIME_SUB
	 */
	ir->ind_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(sb2));
	memset(ir->ind_arr,0,ir->desc->stmt->row_array_size*sizeof(sb2));
	ir->length_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(ub2));
	ir->rcode_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(ub2));
    status|=ood_assign_ir(ir,SQLT_INT,sizeof(long),0,
            ociint_sqlslong,ociint_sqlnts);
    ar->concise_type=ar->data_type=SQL_C_SSHORT;
    ar->display_size=sqltype_display_size(SQL_C_SSHORT,0);
    ar->octet_length=ar->length=sizeof(long);
    strcpy((char*)ar->column_name,"SQL_DATETIME_SUB");
	if(!ir->data_ptr)
        ir->data_ptr=ORAMALLOC(ir->data_size*stmt->row_array_size);

    ir++,ar++;

	/*
	 * Col 18 is NUM_PREC_RADIX
	 */
	ir->ind_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(sb2));
	memset(ir->ind_arr,0,ir->desc->stmt->row_array_size*sizeof(sb2));
	ir->length_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(ub2));
	ir->rcode_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(ub2));
    status|=ood_assign_ir(ir,SQLT_INT,sizeof(long),0,
            ociint_sqlslong,ociint_sqlnts);
    ar->concise_type=ar->data_type=SQL_C_SLONG;
    ar->display_size=sqltype_display_size(SQL_C_SLONG,0);
    ar->octet_length=ar->length=sizeof(long);
    strcpy((char*)ar->column_name,"NUM_PREC_RADIX");
	if(!ir->data_ptr)
        ir->data_ptr=ORAMALLOC(ir->data_size*stmt->row_array_size);

    ir++,ar++;

	/*
	 * Col 19 is INTERVAL_PRECISION
	 */
	ir->ind_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(sb2));
	memset(ir->ind_arr,0,ir->desc->stmt->row_array_size*sizeof(sb2));
	ir->length_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(ub2));
	ir->rcode_arr=ORAMALLOC(ir->desc->stmt->row_array_size*sizeof(ub2));
    status|=ood_assign_ir(ir,SQLT_INT,sizeof(long),0,
            ociint_sqlslong,ociint_sqlnts);
    ar->concise_type=ar->data_type=SQL_C_SSHORT;
    ar->display_size=sqltype_display_size(SQL_C_SSHORT,0);
    ar->octet_length=ar->length=sizeof(long);
    strcpy((char*)ar->column_name,"INTERVAL_PRECISION");
	if(!ir->data_ptr)
        ir->data_ptr=ORAMALLOC(ir->data_size*stmt->row_array_size);

    ood_mutex_unlock_stmt(stmt);
if(ENABLE_TRACE){
    ood_log_message(stmt->dbc,__FILE__,__LINE__,TRACE_FUNCTION_EXIT,
            (SQLHANDLE)NULL,status,"");
}
    return SQL_SUCCESS;
}
