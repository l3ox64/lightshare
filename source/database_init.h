#ifndef DATABASEINIT_H
#define DATABASEINIT_H

#include <iostream>
#include <pqxx/pqxx>
#include <filesystem>
#include "env.h"

using namespace std;
namespace fs = filesystem;

bool createDatabaseAndTable(const string& dbName, const string& tableName) {
    try {
        string env_path = fs::absolute(".env").string();
        load_env_file(env_path);

        // Load environment variables
        string DB_HOST = get_env("DB_HOST");
        string DB_PORT = get_env("DB_PORT");
        string DB_NAME = get_env("DB_NAME");
        string DB_USER = get_env("DB_USER");
        string DB_PASSWORD = get_env("DB_PASSWORD");

        // Connect to the default "postgres" database to check/create the target database
        pqxx::connection tempConn(
            "host=" + DB_HOST +
            " port=" + DB_PORT +
            " dbname=" + DB_USER +
            " user=" + DB_USER +
            " password=" + DB_PASSWORD
        );

        if (!tempConn.is_open()) {
            cerr << "Error: Could not connect to PostgreSQL to check/create the database.\n";
            return false;
        }
	
		pqxx::nontransaction txn(tempConn); 

        // Check if the database already exists
        string checkDbQuery = "SELECT 1 FROM pg_database WHERE datname = " + txn.quote(dbName);
        pqxx::result dbResult = txn.exec(checkDbQuery);

        if (dbResult.empty()) {
            // If the database does not exist, create it
            txn.exec("CREATE DATABASE " + dbName);
            cout << "Database '" << dbName << "' created successfully.\n";
        } else {
            cout << "Database '" << dbName << "' already exists.\n";
        }
        txn.commit();

        // tempConn goes out of scope here and automatically disconnects

        // Connect to the newly created (or existing) database
        pqxx::connection conn(
            " host=" + DB_HOST +
            " port=" + DB_PORT +
            " dbname=" + dbName +
            " user=" + DB_USER +
            " password=" + DB_PASSWORD
        );

        if (!conn.is_open()) {
            cerr << "Error: Could not connect to database '" << dbName << "'.\n";
            return false;
        }

        pqxx::work txn2(conn);

        // Check if the table already exists
        string checkTableQuery = "SELECT 1 FROM information_schema.tables WHERE table_name = " + txn2.quote(tableName);
        pqxx::result tableResult = txn2.exec(checkTableQuery);

        if (tableResult.empty()) {
            // If the table does not exist, create it
            string createTableQuery =
                "CREATE TABLE " + tableName + " ("
                "id SERIAL PRIMARY KEY, "
                "username VARCHAR(100), "
                "password TEXT NOT NULL, "
				"created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP"
                ")";
            txn2.exec(createTableQuery);
            cout << "Table '" << tableName << "' created successfully.\n";
        } else {
            cout << "Table '" << tableName << "' already exists.\n";
        }

        txn2.commit();

        return true;

    } catch (const exception& e) {
        cerr << "Error in createDatabaseAndTable: " << e.what() << "\n";
        return false;
    }
}

#endif // DATABASEINIT_H
