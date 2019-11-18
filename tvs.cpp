#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <list>
#include <cstring>
#include <experimental/filesystem>
using namespace std;
namespace fs = std::experimental::filesystem;

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

public:
    // Commit *next, *prev;
    Commit();
    // ~Commit();
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
    void store_files(string *added_files);
    void display_commit_data();
    void save(ofstream &ofile);
    void open(ifstream &ifile);
};

Commit::Commit() : info(A_info())
{
    ifstream ifile("./.tvs/branch_info");
    if(ifile) {
        getline(ifile, commit_id);
        getline(ifile, commit_msg);
    } else {
        ifile.close();
        ofstream ofile("./.tvs/branch_info");
        ofile << "0" << endl;
        ofile << "init" << endl;
        ofile.close();
        cout << "No commits yet" << endl;
    }
    ifile.close();
}

void Commit::make_commit(string msg)
{
    int id = stoi(commit_id);
    commit_id = to_string(++id);
    commit_msg = msg;
    remove("./.tvs/branch_info");
    ofstream ofile("./.tvs/branch_info");
    ofile << commit_id << endl;
    ofile << commit_msg << endl;
}

// Commit::~Commit() : info(A_info())
// {
//     ofstream ifile("./.tvs/" + "/branch_info");
//     if(ifile) {
//         getline(ifile, commit_id);
//         getline(ifile, commit_msg);
//     } else {
//         cout << "No commits yet" << endl;
//     }
//     ifile.close();
// }

// Init function

int tvsinit()
{
    // creates a path object
    string tvspath("./.tvs");

    if(!fs::exists(tvspath)) {
        if(!fs::create_directory(tvspath)) { // this if is just a failure check
            cerr << strerror(errno) << endl;
            return -2;
        }
        return 0;
    }
    cerr << "Already a tvs repo" << endl;
    return -1;
}

// main for testing
int main()
{
    tvsinit();
    Commit c;
    c.make_commit("henlo");
    cout << c.get_commit_id();
    // master_branch.commit_log(2);
    return 0;
}
