/* 
 * File:   SerialPortReader.h
 * Author: haikalpribadi
 *
 * Created on 10 March 2014, 14:17
 */

#ifndef SERIALPORTREADER_H
#define	SERIALPORTREADER_H

#include <iostream>
#include <ros/ros.h>
#include <SerialStream.h>
#include <sstream>
#include <stdio.h>
#include <std_msgs/Int32MultiArray.h>
#include <vector>

using namespace std;
using namespace LibSerial;

class SerialPortReader {
public:
    SerialPortReader();
    
    void readPort();
    
private:
    ros::NodeHandle node;
    ros::Publisher serialdata_pub;
    
    string port;
    string topic;
    SerialStream arduino; 
    
    vector<int> splitStringToInt(string str);
    
};

#endif	/* SERIALPORTREADER_H */

