#include "source/lightshare.h"
#include "crow.h"
#include <string>
#include <iostream>
#include <filesystem>

using namespace std;

int main(){	
	load_env_file(".env");
	
	createDatabaseAndTable("lightshare","lightshare"); //(dbname, tablename)

	Database db;
	db.createUser("test","password");
	//db.createUser("Leo","123456");
	//db.deleteUser("Leo");
	//db.deleteUser("test");
	//db.updateUserPass("Leo","ciao");
	crow::SimpleApp app;

	CROW_ROUTE(app, "/home")([](){
		auto page = crow::mustache::load_text("home.html");
		return page;
	});
		
	CROW_ROUTE(app, "/upload")([](){
		auto page = crow::mustache::load_text("home.html");
		return page;
	});
	
	CROW_ROUTE(app, "/register")([](){
		auto page = crow::mustache::load_text("register.html");
		return page;		
	});

	/*CROW_ROUTE(app, "/<string>")([](const crow::request& req, string name){
		auto page = crow::mustache::load("home.html");
		crow::mustache::context ctx ({{"user", name}});
		return page.render(ctx);
	});*/

	app.port(stoi(get_env("PORT"))).run();

}




