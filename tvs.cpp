#include <iostream>
#include <fstream>

using namespace std;

class Branch {
private:
    Commit *HEAD;
    int no_of_nodes;
    char *branch_name;
    char **added_files;

public:
    Branch();
    int get_node_count() {
        return no_of_nodes;
    }
    char* get_branch_name() {
        return branch_name;
    }
    char* get_file_list() {
        return added_files;
    }
    void commit_log(int n);
    void add(char **filenames);
    void status();
    void commit(char *msg);
    void checkout(int commit_id);
    ~Branch();
};

class Commit {
private:
    A_info info;
    int commit_id;
    int next_id;
    int prev_id;
    char *commit_msg;
    Commit *next, *prev;

public:
    Commit();
    A_info get_author_info() {
        return info;        
    }
    int get_commit_id() {
        return commit_id;
    }
    void store_files(char **added_files);
    void display_commit_data();
};

class A_info {
private:
    char *username;
    char *email;
public:
    A_info();
    char* get_username() {
        return username;
    }
    char* get_email() {
        return email;
    }
    void set_username(char *uname) {
        this.username = uname;
    }
    void set_email(char *email) {
        this.email = email;
    }
};

