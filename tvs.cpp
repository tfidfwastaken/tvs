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

Commit::Commit() : info(A_info())
{
    ifstream ifile("./.tvs/branch_info");
    if(ifile) {
        getline(ifile, commit_id);
        getline(ifile, commit_msg);
        load_files();
    } else {
        ifile.close();
        commit_id = "0";
        ofstream ofile("./.tvs/branch_info");
        ofile << "0" << endl;
        ofile << "init" << endl;
        ofile.close();
        cout << "No commits yet. Making first commit." << endl;
    }
    ifile.close();
}

void Commit::make_commit(string msg)
{
    if(!added_files.empty() && commit_id != "-1") {
        int id = stoi(commit_id);
        prev_id = id - 1;
        while(fs::exists(".tvs/" + to_string(id))) {
            id++;
        }
        commit_id = to_string(id);
        commit_msg = msg;
        remove("./.tvs/added_files");
        remove("./.tvs/branch_info");
        ofstream ofile("./.tvs/branch_info");
        ofile << commit_id << endl;
        ofile << commit_msg << endl;
        fs::path dst("./.tvs/" + commit_id + "/");
        if(prev_id != -1) {
            fs::path prev_commit_dir("./.tvs/" + to_string(prev_id) + "/");
            fs::copy(prev_commit_dir, dst, fs::copy_options::recursive|fs::copy_options::update_existing);
        }
        fs::create_directory("./.tvs/" + commit_id);
        for(auto file : added_files) {
            fs::copy("./" + file, dst, fs::copy_options::update_existing);
        }
        added_files.clear();
        prev_id = id - 1;
    } else {
        cout << "Please add files to commit." << endl;
    }
}

void Commit::checkout(string id)
{
    if(added_files.empty()) {
        commit_id = id;
        fs::path src(".tvs/" + id);
        if(!fs::exists(src)) {
            cout << "Invalid commit ID. Aborting." << endl;
            return;
        }
        fs::path dst(".");
        fs::copy(src, dst, fs::copy_options::recursive|fs::copy_options::overwrite_existing);
        remove("./.tvs/branch_info");
        ofstream ofile("./.tvs/branch_info");
        ofile << commit_id << endl;
        ofile << commit_msg << endl;
    } else {
        cout << "Working directory not clean.\nPlease commit your changes before checkout." <<endl;
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

void Commit::load_files()
{
    ifstream ifile("./.tvs/added_files");
    string fn;
    while(ifile) {
        getline(ifile, fn);
        added_files.push_back(fn);
    }
    ifile.close();
}

Commit::~Commit()
{
    store_files();
}

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
    cout << "---------------" << endl;
    c.add_file("tvs.cpp");
    c.make_commit("henlo");
    cout << c.get_commit_id() << endl;
    cout << "---------------" << endl;
    c.add_file("DOCS.md");
    c.make_commit("henlo2");
    ofstream("DOCS.md").put('a');
    c.add_file("DOCS.md");
    c.make_commit("henlo3");
    cout << "---------------" << endl;
    cout << c.get_commit_id() << endl;
    c.checkout("1");
    cout << c.get_commit_id() << endl;
    // master_branch.commit_log(2);
    return 0;
}
