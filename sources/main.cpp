#include <iostream>
#include <filesystem>
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>
using namespace std;
using namespace std::filesystem;

const string Flag="HackerSir{Linux_is_awsome}";//The Flag
const string flagPathName = "Desktop/flag.txt";//The path and name after "~/"
path userPath;//The /home/<username>

struct Info {
    bool exist;
    bool typeCheck;
    path Path;
    directory_entry entry;
};

void init();
path takeUserPath(path nowDir);
void writeFlag();
// Info findFileFromRoot(string fileName);
// Info findDirFromRoot(string dirName);
Info findFileFromHome(string fileName);
Info findDirFromHome(string dirName);
perms str2Perms(string permission);
bool checkPermission(path filePath,string strPermission);
bool checkOwner(path filePath,int uid);

int main() {
    init();
    writeFlag();
}


void init() {
    userPath=takeUserPath(current_path());
}

path takeUserPath(const path nowDir) {
    string home;
    int slashCount=0;
    string nowDirStr = string(nowDir);
    for (char i : nowDirStr) {
        home+=i;
        if (i=='/') {
            slashCount++;
        }
        if (slashCount==3) {
            // path=/home/user/...
            break;
        }
    }
    return home;
}

void writeFlag() {
    ofstream os;
    string targetPath = string(userPath)+flagPathName;
    os.open(targetPath);
    os << Flag;
    os.close();
}
Info findFileFromRoot(string fileName) {
    Info result;
    result.exist = true;
    result.typeCheck=true;
    result.Path = path("/");
    result.entry = directory_entry(result.Path);

    path target = "/"+fileName;
    if (!exists(target)) {
        result.exist = false;
        return result;
    }
    directory_entry entry = directory_entry(target);
    if (!is_regular_file(entry)) {
        result.typeCheck = false;
        result.entry = entry;
        return result;
    }
    result.entry = entry;
    return result;
}
Info findDirFromRoot(string dirName) {
    Info result;
    result.exist = true;
    result.typeCheck=true;
    result.Path = path("/");
    result.entry = directory_entry(result.Path);

    path target = "/"+dirName;
    if (!exists(target)) {
        result.exist = false;
        return result;
    }
    directory_entry entry = directory_entry(target);
    if (!is_directory(entry)) {
        result.typeCheck = false;
        result.entry = entry;
        return result;
    }
    result.entry = entry;
    return result;
}
Info findFileFromHome(string fileName) {
    Info result;
    result.exist = true;
    result.typeCheck=true;
    result.Path = userPath;
    result.entry = directory_entry(result.Path);

    path target = userPath;
    target+=fileName;
    if (!exists(target)) {
        result.exist = false;
        return result;
    }
    path filePath = path(target);
    directory_entry entry = directory_entry(target);
    if (!is_regular_file(entry)) {
        result.typeCheck = false;
        result.entry = entry;
        return result;
    }
    result.Path = filePath;
    result.entry = entry;
    return result;
}
Info findDirFromHome(string dirName) {
    Info result;
    result.exist = true;
    result.typeCheck=true;
    result.Path = userPath;
    result.entry = directory_entry(result.Path);

    path target = userPath;
    target+=dirName;
    if (!exists(target)) {
        result.exist = false;
        return result;
    }
    path filePath = path(target);
    directory_entry entry = directory_entry(target);
    if (!is_directory(entry)) {
        result.typeCheck = false;
        result.entry = entry;
        return result;
    }
    result.Path = filePath;
    result.entry = entry;
    return result;
}

perms str2Perms(string permission) {
    perms perm = perms::none;
    int permission_owner = permission[0]-'0';
    int permission_group = permission[1]-'0';
    int permission_others = permission[2]-'0';

    switch (permission_owner) {
        case 7:
            perm^=perms::owner_all;
            break;
        case 6:
            perm^=perms::owner_write;
            perm^=perms::owner_read;
            break;
        case 5:
            perm^=perms::owner_exec;
            perm^=perms::owner_read;
            break;
        case 4:
            perm^=perms::owner_read;
            break;
        case 3:
            perm^=perms::owner_write;
            perm^=perms::owner_exec;
            break;
        case 2:
            perm^=perms::owner_write;
            break;
        case 1:
            perm^=perms::owner_exec;
            break;
    }

    switch (permission_group) {
        case 7:
            perm^=perms::group_all;
            break;
        case 6:
            perm^=perms::group_write;
            perm^=perms::group_read;
            break;
        case 5:
            perm^=perms::group_exec;
            perm^=perms::group_read;
            break;
        case 4:
            perm^=perms::group_read;
            break;
        case 3:
            perm^=perms::group_write;
            perm^=perms::group_exec;
            break;
        case 2:
            perm^=perms::group_write;
            break;
        case 1:
            perm^=perms::group_exec;
            break;
    }

    switch (permission_others) {
        case 7:
            perm^=perms::others_all;
            break;
        case 6:
            perm^=perms::others_write;
            perm^=perms::others_read;
            break;
        case 5:
            perm^=perms::others_exec;
            perm^=perms::others_read;
            break;
        case 4:
            perm^=perms::others_read;
            break;
        case 3:
            perm^=perms::others_write;
            perm^=perms::others_exec;
            break;
        case 2:
            perm^=perms::others_write;
            break;
        case 1:
            perm^=perms::others_exec;
            break;
    }

    return perm;

}


bool checkPermission(path filePath,string strPermission){
    auto permission = status(filePath).permissions();
    if ((permission^str2Perms(strPermission))==perms::none) {
        return true;
    }
    return false;
}

bool checkOwner(path filePath,int uid) {
    struct stat fileStat;
    stat(filePath.c_str(), &fileStat);
    uid_t ownerUid = fileStat.st_uid;
    // cout << ownerUid;
    if (ownerUid==uid) {
        return true;
    }
    return false;
}

