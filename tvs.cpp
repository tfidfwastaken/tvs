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
    int get_node_count();
    char* get_branch_name();
    char* get_file_list();
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
    A_info get_author_info();
    void set_author_info();
    int get_commit_id();
    void store_files(char **added_files);
    void display_commit_data();
};

class A_info {
private:
    char *username;
    char *email;
public:
    char* get_username();
    char* get_email();
    char* set_username();
    char* set_email();
};
