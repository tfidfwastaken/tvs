#include "tvs.h"

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
