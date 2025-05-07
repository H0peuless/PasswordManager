//
// Created by nekoo on 05/12/2024.
//
#ifndef ENCRYPTION_H
#define ENCRYPTION_H
#include <charconv>
#include <iostream>
#include <string>
#include <bitset>
#include <math.h>
#include "sha256.h"
using namespace std;

const string idline = "RX5U6ZIYVF onizeàvuq pfin)h902G";

string generatePassword(int size){
    string alphabet = "azertyuiopmlkjhgfdsqwxcvbnWXCVBNMLKJHGFDSQAZERTYUIOP1234567890)][{#&;,'";
    string password;
    srand(time(0));
    for(int i=0;i<size;i++) {
        password+=alphabet[rand()%(alphabet.length()-1)];
    }
    return password;
}

//--------Hashing function-----------------
string hashing(const string& s) {
    char hex[SHA256_HEX_SIZE + 1];
    sha256_hex(s.c_str(), s.length(), hex);
    hex[SHA256_HEX_SIZE] = '\0';

    return string(hex);  
}



//--------Decoding/Encodeing bit string----
string string_to_bit(string s) {
    string res = "";
    for (int i = 0; i < s.length(); i++) {
        bitset <8> x(s[i]);
        res+= x.to_string();
    }
    return res;
}
string bit_to_string(string binary) {
    string res = "";
    for (int i = 0; i < binary.length(); i+=8) {
        int b = stoi(binary.substr(i,8),nullptr,2);
        bitset <8> x(b);
        res += char(x.to_ulong());
    }
    return res;
}
//--------XOR function---------------------
string XOR(string key, string value) {
    string result = "";
    for (int i = 0; i < value.length(); i++) {
        result += key[i%key.length()] ^ value[i];
    }
    return result;
}
//--------Obfuscation----------------------
string obfuscation(const string s) {
    string obfuscated = "s2#Hfd13>;Djh"; //Base of the obfuscated string
    for (int i = 0; i < 12; i++) {
        obfuscated.push_back((char)(i));
        obfuscated += s[i];
    }
    for (int i = 12; i < s.length(); i++) {
        obfuscated += s[i];
    }
    return obfuscated;
}
string deobfuscation(string s) {
    string deobfucated;
    s = s.substr(13, s.length() - 13);
    for (int i = 0; i < 24; i++) {
        if (i%2){
            deobfucated += s[i];
        }
    }
    s = s.substr(24, s.length() - 24);
    return deobfucated+s;
}
#endif //ENCRYPTION_H
