#include <iostream>
#include <windows.h>
#include <sql.h>
#include <sqltypes.h>
#include <sqlext.h>
#include <string>

using namespace std;

int main() {
    SQLHENV henv;
    SQLHDBC hdbc;
    SQLHSTMT hstmt;
    SQLRETURN retcode;

    SQLCHAR OutConnStr[255];
    SQLSMALLINT OutConnStrLen;
    SQLCHAR* l_pConnectionStr = (SQLCHAR*)"SERVER=JOSEN\\JOSENDB;UID=Josen;PWD=jiang891203;DATABASE=issuelog;DRIVER={SQL Server Native Client 11.0};";

    // Allocate environment handle
    retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);

    // Set the ODBC version environment attribute
    if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
        retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);

        // Allocate connection handle
        if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
            retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);

            // Set login timeout to 5 seconds
            if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
                SQLSetConnectAttr(hdbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0);

                // Connect to data source
                retcode = SQLDriverConnect(
                    hdbc, //ConnectionHandle
                    NULL, // WindowHandle
                    l_pConnectionStr, // InConnectionString
                    strlen((char *)l_pConnectionStr), // StringLength1
                    OutConnStr,     //OutConnectionString
                    255,            //BufferLength
                    &OutConnStrLen, //StringLength2Ptr
                    SQL_DRIVER_COMPLETE); //DriverCompletion

                // Allocate statement handle
                if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
                    retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

                    // Process data
                    retcode = SQLExecDirect(hstmt, (SQLCHAR*)"select idIssue,issuedesc from issues", SQL_NTS);

                    if (retcode == SQL_SUCCESS) {
                        SQLINTEGER sTestInt, cbTestFloat, iCount = 1;
                        SQLLEN cbTestInt, cbTestStr;
                        SQLCHAR szTestStr[200];
                        while (TRUE) {
                            retcode = SQLFetch(hstmt);
                            if (retcode == SQL_ERROR || retcode == SQL_SUCCESS_WITH_INFO) {
                                cout << "An error occurred";
                            }
                            if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
                                SQLGetData(hstmt, 1, SQL_C_ULONG, &sTestInt, 0, &cbTestInt);
                                SQLGetData(hstmt, 2, SQL_C_CHAR, szTestStr, 200, &cbTestStr);

                                /* Print the row of data */
                                cout << "Row " << iCount << ":" << endl;
                                cout << "Column1: " << sTestInt << endl;
                                cout << "Column2: " << szTestStr << endl;

                                iCount++;
                            }
                            else {
                                break;
                            }
                        }
                    }
                    else {
                        cout << "Query execution error." << endl;
                    }

                    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
                    SQLDisconnect(hdbc);
                }
                else {
                    cout << "Connection error" << endl;
                }
                SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
            }
        }
        SQLFreeHandle(SQL_HANDLE_ENV, henv);
    }

    system("pause");
    return 0;
}