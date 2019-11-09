# Documentation

Running `tvs init` would create a `.tvs` folder, which has subfolders dedicated to each commit. These subfolders contain the changed files since the last commit. It will also ask for author's data.

```
file1.c  
file2.c  
.tvs/  
├── branch_info  
├── HEAD.id  
├── author_info  
├── commit_nodes.bin  
├── commit_1  
│  ├── file1.c // ideally should be compressed but we'll see later  
│  └── file2.c  
├── commit_2  
│  ├── file1.c  
│  └── file2.c  
...
```

The program's interface is given by these classes:

## `Branch`

The `Branch` class contains information about the commit history which is given by a doubly linked list of `Commit` objects.

### Private members

- `Commit *HEAD`: Stores the HEAD commit, ie, the current commit.
- `int no_of_nodes`
- `char branch_name[]`
- `char *added_files[]`

### Public members

- `Branch()`

  This constructor will check if `.tvs/` exists, else will run the `tvs init` function.  
  If `./tvs` is found, it will load data from `./tvs/branch_info` which contains data about the branch, except for the `commit_id` of `HEAD`, which is stored in a separate file called `HEAD.id`. This data is loaded and stored to the private variables above.  
  Then a doubly linked list of `Commit` objects is created by reading the info from `.tvs/commit_nodes.bin` which is a binary file containing node data.
  Then `HEAD.id` is read and `HEAD` of the branch object is made to point to that commit object having that id.

- `void commit_log(int n)`: Displays commit log of the last `n` `Commit` objects. This is done by traversing the list backward from `HEAD` and running each object's `display_commit_data()` function.

- `void add(char **filenames)`: Adds the given `filenames` to `added_files`. This function is initiated by `tvs add` command.

- `void status()`: Shows status of which files have been added to `added_files`.

- `void commit(char *msg)`

  This function will create a new commit ID. Then it will create a new `Commit` object with that ID and `msg`, and make `HEAD` point to that commit. Then it will copy all the contents of `added_files` to `.tvs/commit_id/`, by calling `HEAD`'s `store_files()` function.

- `void checkout(int commit_id)`

  This function will make `HEAD` point to `Commit` object containing `commit_id`. Then it will copy the contents of `.tvs/commit_id` to the present working directory.  
  NOTE: If the staging area isn't clean, ie, there are uncommitted files, this operation will be aborted.  
It will also update the file `.tvs/HEAD.id`.

- `~Branch()`

  This destructor will first write into `.tvs/branch_info` and then will store all the commit objects in a binary file called `.tvs/commit_nodes.bin`.
  Then all the memory is freed.

## `Commit`

### private members

- `A_info info`: Info about the author of commit wrapped an `A_info` class.
- `int commit_id`
- `int next_id`
- `int prev_id`
- `char commit_msg[]`
- `Commit *next, *prev`

### public members

- (All relevant accessors and mutators)
- `void store_files(char **added_files)`: Stores `added_files` to the folder `.tvs/commit_id`.
- `void display_commit_data()`


## `A_info`

Just a class containing info like author's email and name and stuff. The constructor loads info from `.tvs/author_info` which is created on running `tvs init`.
