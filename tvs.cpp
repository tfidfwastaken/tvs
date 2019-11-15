#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <list>
#include <cstring>
#include <experimental/filesystem>
using namespace std;
namespace fs = std::experimental::filesystem;

class Commit;

class Branch {
private:
    //Commit *HEAD;
    list<Commit> commits;
    list<Commit>::iterator HEAD;
    //int no_of_nodes;
    string branch_name;
    set<string> added_files;

public:
    Branch();
    int get_node_count() {
        return no_of_nodes;
    }
    string get_branch_name() {
        return branch_name;
    }
    string* get_file_list() {
        return added_files;
    }
    void commit_log(int n);
    void add(char **filenames);
    void status();
    void commit(char *msg);
    void checkout(int commit_id);
    ~Branch();
};

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
    int next_id;
    int prev_id;
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

// Init function

int tvsinit()
{
    fs::path tvspath("./.tvs");
    if(!fs::exists(tvspath)) {
        if(!fs::create_directory(tvspath)) {
            cerr << strerror(errno) << endl;
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
        ifstream b_data(tvspath + "/branch_info");
        string head_id, filename;
        getline(b_data, head_id);
        getline(b_data, branch_name);
        while(b_data) {
            getline(b_data, filename);
            added_files.insert(filename);
        }
        b_data.close();

        ifstream c_data(tvspath + "/commit_nodes.bin", ios::binary);
        Commit commit;
        while(c_data.read(&commit, sizeof(Commit))) {
            commits.push_back(commit);
        }
        for(HEAD = commits.end(); (*HEAD).commit_id != head_id; HEAD++);
    }
}

Branch::~Branch()
{
    auto commit_file_name = tvspath + "/commit_nodes.bin";
    auto branch_file_name = tvspath + "/branch_info";
    remove(commit_file_name);
    remove(branch_file_name);

    ofstream c_data(commit_file_name, ios::binary);
    for(auto cmt = commits.end(); cmt != commits.begin(); cmt--) {
        c_data.write(&(*cmt), sizeof(Commit));
    }
    c_data.close();
    
    ofstream b_data(commit_file_name);
    b_data << (*HEAD).commit_id << endl;
    b_data << (*HEAD).branch_name << endl;
    for(auto filename : added_files) {
        b_data << filename << endl;
    }
    b_data.close();

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
