#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <pqxx/pqxx>

constexpr auto DB_HOST = "localhost";
constexpr auto DB_PORT = "5432";
constexpr auto DB_NAME = "l3ox";
constexpr auto DB_USER = "l3ox";
constexpr auto DB_PASSWORD = "l3ox";
constexpr auto DB_TABLE = "crowapp";

using namespace std;

class Database {
public:
		
	Database() {
        try {
            //PostgreSQL parameters
            string connStr = "host=" + string(DB_HOST) +
                                  " port=" + string(DB_PORT) +
                                  " dbname=" + string(DB_NAME) +
                                  " user=" + string(DB_USER) +
                                  " password=" + string(DB_PASSWORD);

            //String Connection
            conn = make_unique<pqxx::connection>(connStr);

            if (conn->is_open()) {
                cout << "Connected to Postgres: "<<DB_HOST<<endl;
            } else {
                throw runtime_error("Connection error to Postgres");
            }
        } catch (const exception& e) {
            cerr << "Connection error: Possible wrong parameters: " << e.what() << "\n";
        }
    }

    ~Database() {
        if (conn && conn->is_open()) {
            cout << "Connection Closed.\n";
			conn = nullptr;
		}
    }

 	//CREATE
    bool createUser(const string& username, const string& password) {
        return executeQuery("INSERT INTO " + string(DB_TABLE) +
                            " (username, password) VALUES (" +
                            conn->quote(username) + ", " + conn->quote(password) + ")",
                            "user created: " + username);
    }
	
	//READ
    bool readUser(const string& username) {
        try {
            pqxx::nontransaction txn(*conn);
            string query = "SELECT username FROM " + string(DB_TABLE) +
                                " WHERE username = " + txn.quote(username);
            pqxx::result result = txn.exec(query);
			return true;
        } catch (const exception& e) {
            cerr << "Error read to DB: " << e.what() << "\n";
            return false;
        }
    }

    //UPDATE
    bool updateUser(const string& username, const string& newPassword) {
        return executeQuery("UPDATE " + std::string(DB_TABLE) +
                            " SET password = " + conn->quote(newPassword) +
                            " WHERE username = " + conn->quote(username),
                            "password updated: " + username);
    }

	//DELETE
    bool deleteUser(const string& username) {
        return executeQuery("DELETE FROM " + string(DB_TABLE) +
                            " WHERE username = " + conn->quote(username),
                            "user removed: " + username);
    }

private:
    unique_ptr<pqxx::connection> conn;

    //create custom query
    bool executeQuery(const string& query, const string& successMessage) {
        try {
            pqxx::work txn(*conn);
            txn.exec(query);
            txn.commit();
            cout << successMessage << "\n";
            return true;
        } catch (const std::exception& e) {
            cerr << "❌ Errore esecuzione query: " << e.what() << "\n";
            return false;
        }
    }
};

#endif // DATABASE_H

