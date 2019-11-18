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
    Commit() : info(A_info()), commit_id(string()), commit_msg("") {};
    A_info get_author_info() {
        return info;
    }
    string get_commit_id() {
        return "";
    }
    string get_commit_msg() {
        return commit_msg;
    }
    void store_files(string *added_files);
    void display_commit_data();
    void save(ofstream &ofile);
    void open(ifstream &ifile);
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
        return commits.size();
    }
    string get_branch_name() {
        return branch_name;
    }
    set<string> get_added_file() {
        return added_files;
    }
    void commit_log(int n);
    void add(string *filenames);
    void status();
    void commit(char *msg);
    void checkout(string commit_id);
    ~Branch();
};


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

// Branch function definitions
Branch::Branch() : branch_name("master") {

    commits = list<Commit>();
    fs::path tvspath("./.tvs");
    if(!fs::exists(tvspath)) {
        tvsinit();
    } else {

        // Reading all repo data from files

        // Reading branch_info
        ifstream b_data(tvspath.string() + "/branch_info");
        string head_id, filename;
        getline(b_data, head_id);
        getline(b_data, branch_name);
        while(b_data) {
            getline(b_data, filename);
            added_files.insert(filename);
        }
        b_data.close();

        // Reading binary commit data and loading it onto our list<Commit>
        ifstream c_data(tvspath.string() + "/commit_nodes.bin", ios::binary);
        Commit commit;
        // while(c_data.read(reinterpret_cast<char *>(&commit), sizeof(Commit))) {
        //     commits.push_back(commit);
        // }
        
        // Setting our HEAD to point to the current commit
        for(HEAD = commits.begin(); (*HEAD).get_commit_id() != head_id; HEAD++);
    }
}

Branch::~Branch()
{
    // auto just means infer the type, because I'm too lazy to figure it out myself
    fs::path tvspath("./.tvs");
    auto commit_file_name = tvspath.string() + "/commit_nodes.bin";
    auto branch_file_name = tvspath.string() + "/branch_info";
    remove(commit_file_name.c_str());
    remove(branch_file_name.c_str());

    // Writing our data to files

    // Writing commit data to binary format
    ofstream c_data(commit_file_name, ios::binary);
    for(auto cmt = commits.end(); cmt != commits.begin(); cmt--) {
        // c_data.write(reinterpret_cast<char *>(&(*cmt)), sizeof(Commit));
        // cmt.save(c_data);
    }
    c_data.close();
    
    // Writing branch_info
    ofstream b_data(commit_file_name);
    b_data << (*HEAD).get_commit_id() << endl;
    b_data << branch_name << endl;
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

// void save(ofstream &of)
// {
//     of.write((char *)(info

// main for testing
int main()
{
    tvsinit();
    Branch master_branch;
    master_branch.commit_log(2);
    return 0;
}
