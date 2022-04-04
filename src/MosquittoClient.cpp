#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include "MosquittoClient.h"

using namespace std;

MosquittoClient::MosquittoClient(){
	this->connect(this->mqtt_host, this->mqtt_port, 1000, 1000, 1000);
	this->set_max_seconds_without_messages(2147483647); // Max long value
        this->loop_thread = thread([this] {this->loop();});
}

MosquittoClient::~MosquittoClient(){

}

void MosquittoClient::on_message(const string& topic, const string& message) {
	std::cout << "Message recieved BASE" << std::endl;
}
