/* 
 * File:   SerialPortReader.cpp
 * Author: haikalpribadi
 * 
 * Created on 10 March 2014, 14:17
 */

#include "SerialPortReader.h"

SerialPortReader::SerialPortReader() {
    ros::NodeHandle nh("~");
    if(nh.getParam("port", port)) {
        ROS_INFO("%s reading serial port at: %s", nh.getNamespace().c_str(), port.c_str());
    }
    else {
        ROS_ERROR("Please set serial port address for %s", nh.getNamespace().c_str());
        ros::requestShutdown();
    }
    
    arduino.Open(port);    
    arduino.SetBaudRate(SerialStreamBuf::BAUD_9600);  
    arduino.SetCharSize(SerialStreamBuf::CHAR_SIZE_8);
    
    int size = 200;
    char delim = '\r';
    char str[size];
    bool start = false;
    while(!start && ros::ok()) {
        arduino.getline(str, size, delim);
        if(str[1]=='s')
            start = true;    
    }
    arduino.getline(str,size,delim);
    
    topic = "serial_data_";
    stringstream ss;
    ss << topic << str[1];
    topic = ss.str();
    
    ROS_INFO("Serial port %s is publishing at %s", port.c_str(), topic.c_str());
    serialdata_pub = node.advertise<std_msgs::Int32MultiArray>(topic,1000);
    
}

void SerialPortReader::readPort() {
    int size = 200;
    char delim = '\r';
    std_msgs::Int32MultiArray msg;
    
    while (ros::ok()) {
        char str[size];
        arduino.getline(str, size, delim);
        vector<int> data = splitStringToInt(str);
        
        for(int i=1; i<data.size(); i++) {
            msg.data.push_back(data[i]);
        }
        serialdata_pub.publish(msg);
        msg.data.clear();
    }
}

vector<int> SerialPortReader::splitStringToInt(string str) {
    istringstream iss(str);
    vector<int> ints;
    int n;
    while( iss >> n ) {
        ints.push_back(n);
    }
    
    return ints;
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "SerialPortReader");
    
    SerialPortReader reader;
    reader.readPort();
    
    return 0;
}

