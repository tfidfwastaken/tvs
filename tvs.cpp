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
    A_info();
    string get_username() {
        return username;
    }
    string get_email() {
        return email;
    }
    void set_username(char *uname) {
        this->username = uname;
    }
    void set_email(char *email) {
        this->email = email;
    }
};

class Commit {
private:
    A_info info;
    int commit_id;
    // int next_id;
    // int prev_id;
    string commit_msg;

public:
    // Commit *next, *prev;
    Commit();
    A_info get_author_info() {
        return info;        
    }
    int get_commit_id() {
        return commit_id;
    }
    string get_commit_msg() {
        return commit_msg;
    }
    void store_files(char **added_files);
    void display_commit_data();
};

class Branch {
private:

    // This is a linked list of Commit objects
    list<Commit> commits;

    // HEAD is an iterator that points to a commit obj.
    // We will be assigning it to current commit.
    list<Commit>::iterator HEAD;

    string branch_name;

    // Set of strings. Using a set because fast access,
    // which is needed when there are way too many files.
    set<string> added_files;

public:
    Branch();
    int get_node_count() {
        return no_of_nodes;
    }
    string get_branch_name() {
        return branch_name;
    }
    set<string> get_added_file() {
        return added_files;
    }
    void commit_log(int n);
    void add(char **filenames);
    void status();
    void commit(char *msg);
    void checkout(int commit_id);
    ~Branch();
};


// Init function

int tvsinit()
{
    // creates a path object
    fs::path tvspath("./.tvs");

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

// Branch function definitions
Branch::Branch() :
    branch_name("master") {

    fs::path tvspath("./.tvs");
    if(!fs::exists(tvspath)) {
        tvsinit();
    } else {

        // Reading all repo data from files

        // Reading branch_info
        ifstream b_data(tvspath + "/branch_info");
        string head_id, filename;
        getline(b_data, head_id);
        getline(b_data, branch_name);
        while(b_data) {
            getline(b_data, filename);
            added_files.insert(filename);
        }
        b_data.close();

        // Reading binary commit data and loading it onto our list<Commit>
        ifstream c_data(tvspath + "/commit_nodes.bin", ios::binary);
        Commit commit;
        while(c_data.read(&commit, sizeof(Commit))) {
            commits.push_back(commit);
        }
        
        // Setting our HEAD to point to the current commit
        for(HEAD = commits.end(); (*HEAD).commit_id != head_id; HEAD++);
    }
}

Branch::~Branch()
{
    // auto just means infer the type, because I'm too lazy to figure it out myself
    auto commit_file_name = tvspath + "/commit_nodes.bin";
    auto branch_file_name = tvspath + "/branch_info";
    remove(commit_file_name);
    remove(branch_file_name);

    // Writing our data to files

    // Writing commit data to binary format
    ofstream c_data(commit_file_name, ios::binary);
    for(auto cmt = commits.end(); cmt != commits.begin(); cmt--) {
        c_data.write(&(*cmt), sizeof(Commit));
    }
    c_data.close();
    
    // Writing branch_info
    ofstream b_data(commit_file_name);
    b_data << (*HEAD).commit_id << endl;
    b_data << (*HEAD).branch_name << endl;
    for(auto filename : added_files) {
        b_data << filename << endl;
    }
    b_data.close();

    // Cleaning up data
    commits.clear();
    added_files.clear();
}

void Branch::commit_log(int n)
{
    //if(HEAD != NULL) {
    //    int i = 0;
    //    auto tmp = HEAD;
    //    while (i < n || tmp != NULL) {
    //        tmp->display_commit_data();
    //        ++i;
    //        tmp = tmp->prev;
    //    }
    //} else {
    if(!commits.empty()) {
        auto commit = HEAD;
        for(int i = 0; i < n; ++i) {
            (*commit).display_commit_data();
        }
    } else {
        cout << "No commits yet." << endl;
    }
}

// Commit function declarations
void Commit::display_commit_data()
{
    auto auth = get_author_info();
    cout << "Message: " << get_commit_msg() << endl;
    cout << "Author: " << endl;
    cout << "Name: " << auth.get_username() << endl;
    cout << "Email: " << auth.get_email() << endl;
    cout << "ID: " << get_commit_id() << endl; 
}

// main for testing
int main()
{
    tvsinit();
    return 0;
}
