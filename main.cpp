#include "crow.h"
#include <string>
#include <iostream>
#include <filesystem>
#include <unordered_map>

using namespace std;
using namespace std::filesystem;


//load .env file
void load_env_file(const string& filename) {
    ifstream file(filename);

    if (!file) {
        cerr << "Error reading file" << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
	
		if (line.empty() || line[0] == '#') {
        continue;
    	}

        istringstream is_line(line);
        string key;
        if (getline(is_line, key, '=')) {
            string value;
            if (getline(is_line, value)) {
                setenv(key.c_str(), value.c_str(), 1);
            }
        }
    }

}


//function to call env variable
const char* get_env(const char* var_name) {
	if(getenv(var_name)==nullptr){	
		cerr<<"Missing env value: "<<var_name<<" (default 0) "<<endl;
		setenv(var_name,"0",1);
	}
	const char* value = getenv(var_name);
    return value;
}


int main(){	
	load_env_file(".env");

	crow::SimpleApp app;

	CROW_ROUTE(app, "/<string>")([](const crow::request& req, string name){
		auto page = crow::mustache::load("home.html");
		crow::mustache::context ctx ({{"user", name}});
		return page.render(ctx);
	});

	CROW_ROUTE(app, "/home")([](){
		auto page = crow::mustache::load_text("home.html");
		return page;
	});
		
	CROW_ROUTE(app, "/upload")([](){
		auto page = crow::mustache::load_text("home.html");
		return page;
	});

	app.port(stoi(get_env("PORT"))).run();

}




