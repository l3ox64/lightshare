#include <iostream>
#include <sql.h>
#include <sqlext.h>


// Funzione per mostrare errori ODBCvoid printSQLError(SQLHANDLE handle, SQLSMALLINT type) {
    SQLCHAR sqlState[6], message[256];
    SQLINTEGER nativeError;
    SQLSMALLINT messageLength;
    
    if (SQLGetDiagRec(type, handle, 1, sqlState, &nativeError, message, sizeof(message), &messageLength) == SQL_SUCCESS) {
        std::cerr << "ODBC Error: " << message << " (SQLState: " << sqlState << ")\n";
    }
}

int main() {
    SQLHENV env;
    SQLHDBC dbc;
    SQLHSTMT stmt;
    SQLRETURN ret;

    // 1. Inizializza il driver ODBC
    ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);
    if (ret != SQL_SUCCESS) {
        std::cerr << "Errore: impossibile allocare l'ambiente ODBC.\n";
        return 1;
    }
    SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);

    // 2. Alloca un handle di connessione
    ret = SQLAllocHandle(SQL_HANDLE_DBC, env, &dbc);
    if (ret != SQL_SUCCESS) {
        std::cerr << "Errore: impossibile allocare la connessione ODBC.\n";
        return 1;
    }

    // 3. Connessione al database (usa un DSN configurato o una stringa di connessione)
    std::string connStr = "DRIVER={ODBC Driver 17 for SQL Server};SERVER=your_server_address;DATABASE=your_database;UID=your_username;PWD=your_password;";
    ret = SQLDriverConnect(dbc, NULL, (SQLCHAR*)connStr.c_str(), SQL_NTS, NULL, 0, NULL, SQL_DRIVER_NOPROMPT);
    
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
        std::cerr << "Errore di connessione al database.\n";
        printSQLError(dbc, SQL_HANDLE_DBC);
        SQLFreeHandle(SQL_HANDLE_DBC, dbc);
        SQLFreeHandle(SQL_HANDLE_ENV, env);
        return 1;
    }

    std::cout << "Connessione al database MSSQL riuscita!\n";

    // 4. Eseguire una query SQL
    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    if (ret != SQL_SUCCESS) {
        std::cerr << "Errore nell'allocazione dello statement.\n";
        return 1;
    }

    // Esempio di query
    ret = SQLExecDirect(stmt, (SQLCHAR*)"SELECT TOP 5 id, name FROM your_table;", SQL_NTS);
    if (ret != SQL_SUCCESS) {
        std::cerr << "Errore nell'esecuzione della query.\n";
        printSQLError(stmt, SQL_HANDLE_STMT);
    } else {
        // Recupero risultati
        SQLCHAR name[100];
        SQLINTEGER id;
        while (SQLFetch(stmt) == SQL_SUCCESS) {
            SQLGetData(stmt, 1, SQL_C_LONG, &id, 0, NULL);
            SQLGetData(stmt, 2, SQL_C_CHAR, name, sizeof(name), NULL);
            std::cout << "ID: " << id << " - Nome: " << name << std::endl;
        }
    }

    // 5. Cleanup
    SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    SQLDisconnect(dbc);
    SQLFreeHandle(SQL_HANDLE_DBC, dbc);
    SQLFreeHandle(SQL_HANDLE_ENV, env);

    return 0;
}
