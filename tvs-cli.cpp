#include <string_view>
#include "tvs.h"

// main for testing
int main(int argc, char *argv[])
{
    using namespace literals;
    if(argc > 1) {
        if(argv[1] == "init"sv) {
            tvsinit();
        } else if(argv[1] == "status"sv) {
            Commit c;
            c.status();
        } else if(argv[1] == "add"sv) {
            Commit c;
            for(int i = 2; i < argc; i++) {
                c.add_file(string(argv[i]));
            }
        } else if(argv[1] == "commit"sv) {
            Commit c;
            c.make_commit(string(argv[2]));
        } else if(argv[1] == "checkout"sv) {
            Commit c;
            c.checkout(string(argv[2]));
        }
    } else {
        cout << "Usage:" << endl;
        cout << "  tvs init" << endl;
        cout << "  tvs status" << endl;
        cout << "  tvs add [filename] ..." << endl;
        cout << "  tvs commit \"your commit message here\"" << endl;
        cout << "  tvs checkout {commit-id}" << endl;
        return 1;
    }
    return 0;
    // tvsinit();
    // Commit c;
    // cout << "---------------" << endl;
    // c.add_file("tvs.cpp");
    // c.make_commit("henlo");
    // cout << c.get_commit_id() << endl;
    // cout << "---------------" << endl;
    // c.add_file("DOCS.md");
    // c.make_commit("henlo2");
    // ofstream("DOCS.md").put('a');
    // c.add_file("DOCS.md");
    // c.make_commit("henlo3");
    // cout << "---------------" << endl;
    // cout << c.get_commit_id() << endl;
    // c.checkout("1");
    // cout << c.get_commit_id() << endl;
    // return 0;
}
