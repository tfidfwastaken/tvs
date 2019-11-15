#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <experimental/filesystem>
using namespace std;
namespace fs = std::experimental::filesystem;

class Commit;

class Branch {
private:
    Commit *HEAD;
    int no_of_nodes;
    string branch_name;
    string *added_files;

public:
    Branch() :
        HEAD(NULL),
        no_of_nodes(0),
        branch_name("master"),
        added_files(NULL)
    {
    }
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
    Commit *next, *prev;
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
void Branch::commit_log(int n)
{
    if(HEAD != NULL) {
        int i = 0;
        auto tmp = HEAD;
        while (i < n || tmp != NULL) {
            tmp->display_commit_data();
            ++i;
            tmp = tmp->prev;
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
