#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <vector>

#include "Widget.h"

using namespace std;

Widget::Widget(string type) : MosquittoClient(){
	this->type = type;

	this->LoadConfig();
	this->ParseConfig();
	this->Subscribe();
}

Widget::~Widget(){

}

void Widget::LoadConfig(){
	fstream stream("config/config.json");
        string hold((istreambuf_iterator<char>(stream)),
                 istreambuf_iterator<char>());
	try{
        	this->configuration = nlohmann::json::parse(hold)[this->type];
	}
	catch(exception e){
		std::cout << "Unable to load configuration for type: " << this->type << std::endl;
	}
}

void Widget::ParseConfig(){
	if(this->configuration.contains("topics")){
		this->topics = this->configuration["topics"].get<vector<string>>();
	}
	else{
		std::cout << "Configuration missing topics list for type: " << this->type << std::endl; 
		std::cout << this->configuration.dump() << std::endl;
	}
}

void Widget::Subscribe(){
	for(string topic : this->topics){
		this->subscribe(topic);
	}	
}
