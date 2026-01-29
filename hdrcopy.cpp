#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ftw.h>
#include <libgen.h>
#include <sys/stat.h>
#include <unistd.h>        // getopt()
#include <ctype.h>

#define MAX_FILES  10000   // Real data amount is small so we can use fixed size table
#define LEN_FNAME  128
#define LEN_NAME   64
#define SRC_DIRS   8


typedef struct
{
    char fpath[LEN_FNAME];
    char name[LEN_NAME];
    int  duplicate;
} db_record_t;


typedef struct
{
    int          records;
    db_record_t  file[MAX_FILES];
} dbase_t;


typedef struct {
    int  dryrun;
    int  verbose;
    int  csource;
    int  src_dirs;
    char src_dir[SRC_DIRS][LEN_FNAME];
    char dest_dir[LEN_FNAME];
} conf_t;


dbase_t dbase;
conf_t  conf =
{
    .dryrun    = 1,
    .verbose   = 0,
    .csource   = 0,
    .src_dirs  = 0
};

//-------------------------------------------------------------------------

int strcmp_ignore_case(const char *s1, const char *s2)
{
    while (*s1 && *s2) {
        int diff = tolower((unsigned char)*s1) - tolower((unsigned char)*s2);
        if (diff != 0) {
            return diff;
        }
        s1++;
        s2++;
    }
    return tolower((unsigned char)*s1) - tolower((unsigned char)*s2);
}


//-------------------------------------------------------------------------

int db_add_record( const char *fpath, const char *name )
{
    if ( dbase.records >= MAX_FILES ) {
        return -1;
    }
    int ix = dbase.records;

    strncpy( dbase.file[ix].fpath, fpath, LEN_FNAME );
    strncpy( dbase.file[ix].name,  name,  LEN_NAME  );
    return ++dbase.records;
}


int db_mark_duplicates( void )
{
    int duplicates = 0;

    for ( int i = 0; i < (dbase.records - 2); i++ ) {
        for ( int j = (i + 1); j < (dbase.records - 1); j++ ) {
        //  if ( strncmp( dbase.file[i].name, dbase.file[i].name, LEN_NAME  ) ) {
        //  Windows is case insensitive
            if ( strcmp_ignore_case( dbase.file[i].name, dbase.file[i].name ) ) {
                continue;
            }
            dbase.file[i].duplicate = 1;
            dbase.file[j].duplicate = 1;
        }
    }
    return duplicates;
}

//-------------------------------------------------------------------------

// Function to copy a file from source to destination
void copy_file(const char *src, const char *dest)
{
    FILE *source = fopen(src, "rb");
    FILE *target = fopen(dest, "wb");
    if (!source || !target) {
        if (source) fclose(source);
        return;
    }
    char buf[8192];
    size_t size;
    while ((size = fread(buf, 1, sizeof(buf), source)) > 0) {
        fwrite(buf, 1, size, target);
    }
    fclose(source);
    fclose(target);
}


// Callback for nftw to process each file in the tree
int process_file(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf )
{
    // Only process regular files
    if (tflag == FTW_F) {
        const char *ext = strrchr(fpath, '.');
        if ( conf.csource ) {
            if ( ! (ext && (strcmp(ext, ".h") == 0 || strcmp(ext, ".hpp") == 0 || strcmp(ext, ".c") == 0 || strcmp(ext, ".cpp") == 0)) ) {
                return 0;
            }
        }
        else {
            if ( ! (ext && (strcmp(ext, ".h") == 0 || strcmp(ext, ".hpp") == 0)) ) {
                return 0;
            }
        }
        //if (ext && (strcmp(ext, ".h") == 0 || strcmp(ext, ".hpp") == 0))
        {
            char const *filename = basename( const_cast<char*>(fpath) );
            char        target_path[1024];

            snprintf(target_path, sizeof(target_path), "%s/%s", conf.dest_dir, filename);

            if ( conf.verbose ) {
                printf("Copying: %s -> %s\n", fpath, target_path);
            }

            db_add_record( fpath, filename );

            if ( ! conf.dryrun ) {
                copy_file(fpath, target_path);
            }
        }
    }
    return 0; // Continue walking
}


void help( const char *argv0 )
{
    fprintf(stderr, "\n");
    fprintf(stderr, "Usage: %s [-v] [-x] [-c] -d dstDir  srcDir1  srcDir2  ...\n", argv0);
    exit(EXIT_FAILURE);
}


int parse_arguments( int argc, char *argv[] )
{
    char options[] = "?hxvcd:s:";
    int  opt;

    while ((opt = getopt(argc, argv, options)) != -1)
    {
        switch (opt)
        {
            case '?':
                help( argv[0] );
                break;
            case 'h':
                help( argv[0] );
                break;
            case 'x':
                conf.dryrun   = 0;
                break;
            case 'v':
                conf.verbose += 1;
                break;
            case 'c':
                conf.csource  = 1;
                break;
            case 'd':
                strncpy( conf.dest_dir, optarg, LEN_FNAME );
                break;
            case 's':
                if ( conf.src_dirs < SRC_DIRS ) {
                    strncpy( conf.src_dir[conf.src_dirs++], optarg, LEN_FNAME );
                }
                break;

            /// ToDo: Fix me....
            default: /* '?' */
                if ( conf.src_dirs < SRC_DIRS ) {
                    strncpy( conf.src_dir[conf.src_dirs++], argv[optind], LEN_FNAME );
                }
                break;
        }
    }
    if (optind >= argc) {
       fprintf(stderr, "Expected argument after options\n");
       exit(EXIT_FAILURE);
    }
    if ( !strlen(conf.dest_dir) ) {
        printf("ERROR: missing destination directory\n");
        exit(EXIT_FAILURE);
    }
    // Parse rest of source directory arguments from command line tail
    for ( int i = optind; i < argc; i++ ) {
        if ( conf.src_dirs  < SRC_DIRS  ) {
            strncpy( conf.src_dir[conf.src_dirs++], argv[i], LEN_FNAME );
        }
    }
    if ( !conf.src_dirs ) {
        printf("ERROR: missing source directory\n");
        exit(EXIT_FAILURE);
    }
    //  "Debug stuff"
    if ( conf.verbose > 1 ) {
        printf("destination argument  = %s\n", conf.dest_dir);
        for ( int ix = 0; ix < conf.src_dirs; ix++ ) {
            printf("source argument (i=%d) = %s\n", ix, conf.src_dir[ix]);
        }
    }
    return optind + 1;  // Return count of parsed command line arguments
}


int main( int argc, char *argv[] )
{
    parse_arguments( argc, argv );

    // Create destination directory if it doesn't exist
    #ifdef  __linux__
    mkdir(conf.dest_dir, 0755);
    #else  // Windows
    mkdir(conf.dest_dir);
    #endif // __linux__

    for ( int i = 0; i < conf.src_dirs; i++ ) {
        // nftw(path, callback, max_open_fds, flags)
        if (nftw(conf.src_dir[i], process_file, 20, FTW_PHYS) == -1) {
            perror("nftw");
            return 1;
        }
    }

    if ( conf.verbose ) {
        printf("\n----------------------------------------------\n");
        printf("Files processed: %d\n", dbase.records);
        printf("Duplicates:      %d\n", db_mark_duplicates() );
    }
    printf("Done.\n");
    return 0;
}
