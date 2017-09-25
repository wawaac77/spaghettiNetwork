//
//  main.cpp
//  spaghettiNetwork
//
//  Created by Alice Jin on 25/9/2017.
//  Copyright © 2017 Alice Jin. All rights reserved.
//

#include <iostream>
#include<cstdlib>
#include<stdio.h>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

vector<long> getParameters(string rule);
vector<bitset<8>> getBinaryNums (string rule, bool isRule);
long findRouterNum (string rule);
int spaghettiNetwork(std::vector<int> interfaces, std::vector<int> network, std::vector<std::string> routingTable, std::string packet);
int isMatch (string rule, vector<bitset<8>> packetArray);
int searchFinalRouter(int start, std::vector<bool> used, std::vector<int> network, std::vector<std::string> routingTable, std::vector<bitset<8>> packetArray, std::vector<int> interfaces);


//***************** on codefights *****************//
int spaghettiNetwork(std::vector<int> interfaces, std::vector<int> network, std::vector<std::string> routingTable, std::string packet) {
    
    // insert code here...
    
    std::vector<bitset<8>> packetArray = getBinaryNums(packet, 0);
    cout << "after getBinaryNums(packet, 0) *******" << endl;
    for (int i = 0; i < packetArray.end() - packetArray.begin(); i++) {
        cout << "packetArray " << i << "  " << packetArray[i]  << endl;
    }
    
    int start = findRouterNum(packet);
    cout << "originalStart" << start << endl;
    std::vector<bool> usedRouter;
    //int totalNum = network[network.end() - 1];
    for (int i = 0; i < 200; i++) {
        usedRouter.push_back(0);
    }
    
    int ans = searchFinalRouter(start, usedRouter, network, routingTable, packetArray, interfaces);
    
    cout << "ans " <<ans <<endl;
    return ans;
}


std::vector<long> getParameters(string rule) {
    
    vector<long> ans;
    
    rule = rule.erase(0, rule.find_first_of(",") + 1);
    rule = rule.erase(0, rule.find_first_of(",") + 1);
    
    unsigned long pos = rule.find_first_of(",");
    string part = rule.substr(0, pos);
    long num = strtol(part.c_str(), NULL, 10);
    ans.push_back(num);
    
    rule = rule.erase(0, pos + 1);
    pos = rule.find_first_of(",");
    part = rule.substr(0, pos);
    num = strtol(part.c_str(), NULL, 10);
    ans.push_back(num);
    
    rule = rule.erase(0, pos + 1);
    part = rule;
    num = strtol(part.c_str(), NULL, 10);
    ans.push_back(num);
    
    for (int i = 0; i < 3; i++) {
        cout << " i " << ans[i] << endl;
    }
    return ans;
}

vector<bitset<8>> getBinaryNums (string rule, bool isRule) {
    vector<bitset<8>> ruleArray;
    
    cout << "rule original ----"<< rule << endl;
    
    rule = rule.erase(0, rule.find_first_of(",") + 1);
    
    //** deal with rule
    while (rule.find_first_of(".") != -1) {
        unsigned long pos = rule.find_first_of(".");
        string part = rule.substr(0, pos);
        long num = strtol(part.c_str(), NULL, 10);
        std::bitset<8> binaryNum = std::bitset<8>(num);
        cout <<"rule "  << rule << "  binaryNum "  << binaryNum << endl;
        ruleArray.push_back(binaryNum);
        
        rule = rule.erase(0, pos + 1);
    }
    string part;
    if (isRule) {
        part = rule.substr(0, rule.find_first_of(","));
    } else {
        part = rule;
    }
    
    long num = strtol(part.c_str(), NULL, 10);
    std::bitset<8> binaryNum = std::bitset<8>(num);
    cout << "binaryNum " << binaryNum << endl;
    ruleArray.push_back(binaryNum);
    
    
    for (int i = 0; i < ruleArray.end() - ruleArray.begin(); i++) {
        cout << "ruleArray " << i  << " "<< ruleArray[i] << endl;
    }
    return ruleArray;
}

long findRouterNum (string rule) {
    string part = rule.substr(0, rule.find_first_of(","));
    long num = strtol(part.c_str(), NULL, 10);
    
    return num;
}

int isMatch (string rule, vector<bitset<8>> packetArray) {
    vector<long> parameters = getParameters(rule);
    vector<bitset<8>> binaryNums = getBinaryNums(rule, 1);
    
    int groupNum = parameters[0] / 8;
    int oneByOneNum = parameters[0] % 8;
    
    cout << "groupNum" << groupNum << endl;
    cout << "oneByOneNum" << oneByOneNum << endl;
    
    for (int i = 0; i < groupNum; i++) {
        if (binaryNums[i] != packetArray[i]) {
            return -1;
        }
    }
    
    for (int i = 0; i < oneByOneNum; i++) {
        if (binaryNums[groupNum][7 - i] != packetArray[groupNum][7 - i]) {
            return -1;
        }
    }
    
    return parameters[0];
    
}

//***************** deep search *****************//
int searchFinalRouter(int start, std::vector<bool> used, std::vector<int> network, std::vector<std::string> routingTable, std::vector<bitset<8>> packetArray, std::vector<int> interfaces) {
    cout <<"start ****** " << start << endl;
    
    for (int i = 0; i < used.end() - used.begin(); i++) {
        cout << "used[i]" << i << " " << used[i] << endl;
    }
    
    if (used[start]) {
        return -1;
    }
    
    int max = -1; //try to find max
    int maxi = -1;
    int maxWeight = -1;
    vector<long> maxParameters;

  
    for (int i = 0; i < routingTable.end() - routingTable.begin() ; i++ ) {
        cout << i << endl;
        cout << "findRouterNum(routingTable[i])  " << i << " " << findRouterNum(routingTable[i]) << endl;
        // if router correct
        if (start == findRouterNum(routingTable[i])) {
        
            int matchNum = isMatch(routingTable[i], packetArray); //find match or not
        
            vector<long> currentParameters = getParameters(routingTable[i]);
            
            cout << i <<" matchNum " << matchNum <<endl;
            if (matchNum != -1 && network[currentParameters[2]] != -1) { // if find match
                
                if (maxWeight < currentParameters[1]) {
                    maxWeight = currentParameters[1];
                    
                    max = matchNum;
                    maxi = i;
                    maxParameters = currentParameters;
                } else if (maxWeight == currentParameters[1]) {
                    
                    if (matchNum > max) {
                        max = matchNum;
                        maxi = i;
                        maxParameters = currentParameters;
                    } else if (matchNum == max) {
                        if (currentParameters[2] < maxParameters[2]) {
                            max = matchNum;
                            maxi = i;
                            maxParameters = currentParameters;
                        }
                    }
                    
                }

                
            }
            
        }
    }
    
    vector<bool> newUsed = used;
    newUsed[start] = 1;
    cout << "newUsed[start]  " << newUsed[start] << "  start "<< start << endl;
    
    if (max == -1) {
        return start;
    } else {
        int newStart = network[maxParameters[2]];
        cout << "max !!!!new!!!!!!! " << max <<endl;
        cout << "maxi !!!!new!!!!!!! " << maxParameters[2] <<endl;
        cout << "newStart !!!!!!!!!!! " << newStart <<endl;
        
        int newRouterNum;
        if (newStart > routingTable.end() - routingTable.begin() - 1) {
            newRouterNum = interfaces[newStart];
        } else {
            newRouterNum = findRouterNum(routingTable[newStart]);
        }
        
        /*
        //int newRouterNum = interfaces[newStart];
        if (newRouterNum == NULL) {
            return start;
        }
         */
        
        
        cout << "newRouterNum !!!!!!!!!!!" << newRouterNum <<endl;
        cout <<"new end~~~~~~~~~~~~~~" <<endl;
        return searchFinalRouter(newRouterNum, newUsed, network, routingTable, packetArray, interfaces);
    }
    
    
}

int main(int argc, const char * argv[]) {
    // insert code here...
   
    
    /*
    std::vector<int> interfaces = {0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3};
    std::vector<int> network = {12, 4, -1, -1, 1, 13, 8, 9, 6, 7, 14, -1, 0, 5, 10, -1};
    std::vector<std::string> routingTable =
                                            {"0,192.168.1.0,24,50,0",
                                             "0,10.12.7.0,24,50,2",
                                             "0,0.0.0.0,0,10,1",
                                             "0,0.0.0.0,0,5,0",
                                             "0,10.12.7.0,28,50,3",
                                             "1,0.0.0.0,0,50,4",
                                             "1,10.12.0.0,16,75,4",
                                             "1,10.12.0.0,16,80,6",
                                             "1,10.12.7.3,32,75,5",
                                             "2,192.168.0.0,16,20,10",
                                             "2,10.12.1.0,24,20,10",
                                             "2,10.12.7.0,24,20,11",
                                             "2,0.0.0.0,0,50,11",
                                             "3,0.0.0.0,0,10,12"};
    
    std::string packet = "0,10.12.7.3";
    */
    
    /*
    std::vector<int> interfaces = {0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 5};
    std::vector<int> network = {4, 20, -1, -1, 0, 9, 21, -1, 12, 5, 22, -1, 8, 17, 23, -1, 24, 13, -1, -1, 1, 6, 10, 14, 16};
    std::vector<std::string> routingTable =
    {"0,192.168.2.0,24,20,1",
        "0,192.168.0.0,16,10,0",
        "0,192.168.3.0,24,20,1",
        "1,192.168.1.0,24,10,6",
        "1,192.168.1.0,24,20,5",
        "1,192.168.1.0,24,15,6",
        "2,192.168.1.0,24,5,10",
        "2,192.168.1.0,26,5,10",
        "2,192.168.1.5,32,5,8",
        "2,192.168.0.0,16,5,10",
        "2,192.168.1.0,29,5,10",
        "3,192.168.1.0,24,10,14",
        "3,192.168.1.0,24,10,13",
        "3,192.168.1.0,24,10,14", 
        "4,192.168.1.4,32,10,16", 
        "4,192.168.1.8,29,10,16"};
    
    std::string packet = "0,192.168.1.5";

     */
    
    std::vector<int> interfaces = {0, 0, 1, 1, 2, 2};
    std::vector<int> network = {2, 4, 0, 5, 1, 3};
    std::vector<std::string> routingTable =
    {"0,10.20.30.0,24,10,0",
        "1,10.20.30.0,24,10,3",
        "2,10.20.30.0,24,10,4"};
    
    std::string packet = "2,10.20.30.40";

    
    
    cout << "***********" << endl;
    return spaghettiNetwork(interfaces, network, routingTable, packet);
}


