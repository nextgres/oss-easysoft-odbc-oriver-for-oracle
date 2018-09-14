lib_LTLIBRARIES = liboraodbc.la

INCLUDES = -I${ODBCIDIR} -I${ORAIDIR} -I${ORANDIR} -I${ORAPDIR} -D_REENTRANT

EXTRA_DIST = \
	common.h \
	diagnostics.h \
	oracle_functions.h \
	string_functions.h \
	common_functions.h \
	mem_functions.h \
	ocitrace.h \
	odbcdriver.exp

liboraodbc_la_LIBADD =  @LD_ODBCINSTLIB@

liboraodbc_la_LDFLAGS = \
	-version-info @LIB_VERSION@ \
    -export-dynamic \
	-export-symbols odbcdriver.exp \
	-L${ORALDIR} ${ORALIB}

liboraodbc_la_DEPENDENCIES =

liboraodbc_la_SOURCES = \
	SQLAllocHandle.c\
	SQLBindCol.c\
	SQLBindParameter.c\
	SQLBrowseConnect.c\
	SQLBulkOperations.c\
	SQLCancel.c\
	SQLCloseCursor.c\
	SQLColAttribute.c\
	SQLColumnPrivileges.c\
	SQLColumns.c\
	SQLConnect.c\
	SQLCopyDesc.c\
	SQLDescribeCol.c\
	SQLDescribeParam.c\
	SQLDisconnect.c\
	SQLDriverConnect.c\
	SQLEndTran.c\
	SQLExecDirect.c\
	SQLExecute.c\
	SQLFetch.c\
	SQLFetchScroll.c\
	SQLForeignKeys.c\
	SQLFreeHandle.c\
	SQLFreeStmt.c\
	SQLGetConnectAttr.c\
	SQLGetCursorName.c\
	SQLGetData.c\
	SQLGetDescField.c\
	SQLGetDescRec.c\
	SQLGetDiagField.c\
	SQLGetDiagRec.c\
	SQLGetEnvAttr.c\
	SQLGetFunctions.c\
	SQLGetInfo.c\
	SQLGetStmtAttr.c\
	SQLGetTypeInfo.c\
	SQLMoreResults.c\
	SQLNativeSql.c\
	SQLNumParams.c\
	SQLNumResultCols.c\
	SQLParamData.c\
	SQLPrepare.c\
	SQLPrimaryKeys.c\
	SQLProcedureColumns.c\
	SQLProcedures.c\
	SQLPutData.c\
	SQLRowCount.c\
	SQLSetConnectAttr.c\
	SQLSetCursorName.c\
	SQLSetDescField.c\
	SQLSetDescRec.c\
	SQLSetEnvAttr.c\
	SQLSetPos.c\
	SQLSetStmtAttr.c\
	SQLSpecialColumns.c\
	SQLStatistics.c\
	SQLTablePrivileges.c\
	SQLTables.c\
	SQLExtendedFetch.c\
	diagnostics.c \
	string_functions.c \
	common_functions.c \
	oracle_functions.c \
	sqllex.l \
	mem_functions.c \
	common.c \
	ocitrace.c

CLEANFILES= .deps/*.P* *.lo core core.* *~
