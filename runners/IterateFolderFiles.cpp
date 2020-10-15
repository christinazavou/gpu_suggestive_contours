//
// Created by christina on 15-10-20.
//

#include <stdio.h>
#include <dirent.h>
#include <iostream>
#include <cstring>
#include <fcntl.h>
#include <zconf.h>
#include <sys/stat.h>

int listdir(const char *path) {
    struct dirent *entry;
    DIR *dp;

    dp = opendir(path);
    if (dp == NULL) {
        perror("opendir: Path does not exist or could not be read.");
        return -1;
    }

    int objects = 0;
    while ((entry = readdir(dp))){
        if ( !strcmp( entry->d_name, "."  )) continue;
        if ( !strcmp( entry->d_name, ".." )) continue;
        if ( entry->d_name[0] == '.' ) continue;

        std::cout<<entry->d_name<<std::endl;
        objects += 1;
        if (objects >= 100)
            break;
    }

    closedir(dp);
    return 0;
}

void str_concat(){
    const char* the_folder = "/media/christina/Elements/ANNFASS_SOLUTION/proj_style_data/rtsc_in/buildnet";
    char* kati = "kai";
    char* newname;

    newname = (char*) malloc(100);
    strcpy(newname, the_folder);
    strcat(newname, kati);
    std::cout<<newname;
};

void make_dir(){
    if (access("/media/christina/Elements/ANNFASS_SOLUTION/proj_style_out/rtsc_out/buildnet/kati", F_OK) == -1)
    {
        mkdir("/media/christina/Elements/ANNFASS_SOLUTION/proj_style_out/rtsc_out/buildnet/kati",0777);
    }
}

int main(){

    char*kati="ewks.ply";
    std::cout<<std::string (kati).substr(0, strlen(kati)-4);
//    make_dir();
//    str_concat();
//    listdir("/media/christina/Elements/ANNFASS_SOLUTION/proj_style_data/rtsc_in/buildnet");

}