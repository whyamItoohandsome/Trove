#include "trove.h"
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>

void scan_directory_remove (char *dirname){
    HASHTABLE *hashtable = hashtable_new();
    char pathname[MAXPATHLEN]; //name of path REGARDLESS of whether its full or relative
    char absolute_path[MAXPATHLEN];//absolute pathname of a file

    DIR* dir= opendir(dirname);
    struct dirent* dp;

    //report error if directory cannot be opened
    if (dir == NULL){
        perror("no directory");
        exit(EXIT_FAILURE);
    }

    //read directory until we reach singular file / end of directory    
    while((dp = readdir(dir)) != NULL){  
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) {
            continue;
        }


        struct stat  stat_buffer; //stat for info about directories
        struct stat  *stat_pointer = &stat_buffer;

        // //if a directory contain a fullpath name then convert it to absolute pathnames
        sprintf(pathname, "%s/%s", dirname, dp->d_name); 
        realpath(pathname, absolute_path);




        //ensures path can be accessed
        if (stat(absolute_path, stat_pointer) != 0){
            perror(absolute_path);
            exit(EXIT_FAILURE);
        }

        //if the "file" is a directory, recursively search that directory again
        else if(S_ISDIR(stat_pointer->st_mode)){
            printf("reading directory: %s \n", pathname);
            scan_directory(pathname); //recrusive
        }

        //if file is reg file, pass that to file_reader function for processing
        else if(S_ISREG(stat_pointer->st_mode)){
            printf("finding word in: %s\n", pathname);
            hashtable_remove(hashtable, pathname);;
        }
        else{
            printf("\t%s is unknown\n", pathname);
        }
    }
    
    closedir(dir);
}
