#include "tvs.h"

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
