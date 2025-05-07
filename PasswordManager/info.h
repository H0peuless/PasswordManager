#ifndef INFORMATIONS_H
#define INFORMATIONS_H
#include <string>
#include <random>
#include <fstream>
#include <sstream>
#include "encryption.h"

using namespace std;

class Informations {
    string entity;
    string password;
    string email;
    string username;
    int order;

public:
    Informations(const string infos, int index) {
        this->order = index;
        stringstream ss(infos);
        string token;
        vector<string> tokens;
        char delimiter = '\4';
        while (getline(ss, token, delimiter)) {
            string temp(token.substr(0, 4));
            if (temp == "ent:") {
                this->entity = token.substr(4, token.size());
            }
            if (temp == "pas:") {
                string pass = token.substr(4, token.size());
                if (pass.length() > 120) {
                    wxMessageBox("Max password size should be less than 120", "Error", wxICON_ERROR);
                    return;
                }
                if (pass.length() < 12) {
                    wxMessageBox("Min password size should be 12", "Error", wxICON_ERROR);
                    return;
                }
                this->password = token.substr(4, token.size());
            }
            if (temp == "ema:") {
                this->email = token.substr(4, token.size());
            }
            if (temp == "usr:") {
                this->username = token.substr(4, token.size());
            }
        }
    }

    Informations(const string& entity, const string& password, const string& email, const string& username, const int index) {
        if (password.length() > 120) {
            wxMessageBox("Max password size should be less than 120", "Error", wxICON_ERROR);
            return;
        }
        if (password.length() < 12) {
            wxMessageBox("Min password size should be 12", "Error", wxICON_ERROR);
            return;
        }
        this->entity = entity;
        this->password = password;
        this->email = email;
        this->username = username;
        this->order = index;
    }

    Informations(const string infos) {
        stringstream ss(infos);
        string token;
        vector<string> tokens;
        char delimiter = '\4';
        while (getline(ss, token, delimiter)) {
            string temp(token.substr(0, 4));
            if (temp == "ent:") {
                this->entity = token.substr(4, token.size());
            }
            if (temp == "pas:") {
                string pass = token.substr(4, token.size());
                if (pass.length() > 120) {
                    wxMessageBox("Max password size should be less than 120", "Error", wxICON_ERROR);
                    return;
                }
                if (pass.length() < 12) {
                    wxMessageBox("Min password size should be 12", "Error", wxICON_ERROR);
                    return;
                }
                this->password = token.substr(4, token.size());
            }
            if (temp == "ema:") {
                this->email = token.substr(4, token.size());
            }
            if (temp == "usr:") {
                this->username = token.substr(4, token.size());
            }
        }
    }

    string getEntity() const { return entity; }
    string getPassword() const { return password; }
    string getEmail() const { return email; }
    string getUsername() const { return username; }
    int getOrder() const { return order; }

    void setIndex(int index) { this->order = index; }
    void setPassword(string password) { this->password = password; }
    void setEmail(string email) { this->email = email; }
    void setUsername(string username) { this->username = username; }
    void setEntity(string entity) { this->entity = entity; }

    string to_string() const {
        string s = "";
        if (!entity.empty()) {
            s += "ent:" + entity + '\4';
        }
        if (!password.empty()) {
            s += "pas:" + password + '\4';
        }
        if (!email.empty()) {
            s += "ema:" + email + '\4';
        }
        if (!username.empty()) {
            s += "usr:" + username;
        }
        return s;
    }
};

// File operations
vector<Informations> getInformations(string password) {
    vector<Informations> infos;
    ifstream file("password.dat", ios::app);
    string res;
    getline(file, res, '\n');
    file.close();

    res = bit_to_string(res);
    string temp;
    int index = 0;
    int i = 0;
    int j = 0;
    while (i != -1) {
        i = res.find(XOR(to_string(index), idline));
        if (i == -1) break;
        temp = res.substr(j, i - j);
        temp = XOR(obfuscation(password), temp);
        temp = XOR(password, temp);
        Informations elem(temp, index);
        infos.emplace_back(elem);
        index++;
        j = i + idline.length();
    }
    return infos;
}

void addInformations(string password, Informations& informations) {
    string s = XOR(obfuscation(password), informations.to_string());
    s = XOR(password, s);

    fstream file("password.dat", ios::app);
    string endln = XOR(to_string(informations.getOrder()), idline);
    file << string_to_bit(s) << string_to_bit(endln);
}


class Data {
public:
    vector<Informations> infos;
    Data(){}
    Data(vector<Informations> infos) { this->infos = infos; }
    Data(string password) { this->infos = getInformations(password); }
    ~Data() { infos.clear(); }

    void addInformations(string password, Informations& informations) {
        informations.setIndex(infos.size());
        infos.push_back(informations);
        ::addInformations(password, informations);
    }

    void removeInformations(string password, int index) {
        if (index > infos.size() - 1) {
            wxMessageBox("Index is out of bounds", "Error", wxICON_ERROR);
            return;
        }
        infos.erase(infos.begin() + index);
        remove("password.dat");
        for (int i = 0; i < infos.size(); i++) {
            infos[i].setIndex(i);
            ::addInformations(password, infos[i]);
        }
    }

    void eraseAll() {
        infos.clear();
        remove("password.dat");
    }
};



#endif //INFORMATIONS_H

