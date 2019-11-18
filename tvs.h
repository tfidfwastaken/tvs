#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <list>
#include <cstring>
#include <experimental/filesystem>
using namespace std;
namespace fs = std::experimental::filesystem;

int tvsinit();

class A_info {
private:
    string username;
    string email;
public:
    // A_info();
    string get_username() {
        return username;
    }
    string get_email() {
        return email;
    }
    void set_username(string uname) {
        this->username = uname;
    }
    void set_email(string email) {
        this->email = email;
    }
};

class Commit {
private:
    A_info info;
    string commit_id;
    // int next_id;
    // int prev_id;
    string commit_msg;
    int prev_id;
    vector<string> added_files;

public:
    // Commit *next, *prev;
    Commit();
    ~Commit();
    A_info get_author_info() {
        return info;
    }
    string get_commit_id() {
        return commit_id;
    }
    string get_commit_msg() {
        return commit_msg;
    }
    void make_commit(string msg);
    void add_file(string filename);
    void store_files();
    void load_files();
    void display_commit_data();
    void checkout(string id);
};
