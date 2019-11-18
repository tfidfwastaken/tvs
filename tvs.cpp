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
    vector<string> added_files;
    int num_added;

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
    void add_file(string filename);
    void store_files();
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
        commit_id = "-1";
        ofstream ofile("./.tvs/branch_info");
        ofile << "0" << endl;
        ofile << "init" << endl;
        ofile.close();
        num_added = 0;
        cout << "No commits yet. Making first commit." << endl;
    }
    ifile.close();
}

void Commit::make_commit(string msg)
{
    if(!added_files.empty()) {
        int id = stoi(commit_id);
        commit_id = to_string(++id);
        commit_msg = msg;
        remove("./.tvs/added_files");
        remove("./.tvs/branch_info");
        ofstream ofile("./.tvs/branch_info");
        ofile << commit_id << endl;
        ofile << commit_msg << endl;
        fs::path dst("./.tvs/" + commit_id + "/");
        fs::create_directory("./.tvs/" + commit_id);
        for(auto file : added_files) {
            fs::copy("./" + file, dst);
        }
    } else {
        cout << "Please add files to commit." << endl;
    }
}

void Commit::add_file(string filename)
{
    added_files.push_back(filename);
}

void Commit::store_files()
{
    ofstream ofile("./.tvs/added_files");
    for(auto it : added_files) {
        ofile << it << endl;
    }
    ofile.close();
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
    c.add_file("tvs.cpp");
    c.add_file("DOCS.md");
    c.make_commit("henlo");
    cout << c.get_commit_id();
    // master_branch.commit_log(2);
    return 0;
}
