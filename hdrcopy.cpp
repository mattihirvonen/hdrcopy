#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ftw.h>
#include <libgen.h>
#include <sys/stat.h>

#define MAX_FILES  100000  // Real data amount is small so we can use fixed size table
#define LEN_FNAME  128
#define LEN_NAME   64


typedef struct {
    int dryrun;
} conf_t;


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


conf_t conf =
{
    .dryrun = 1
};

const char  *dest_dir;
dbase_t      dbase;

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
            if ( strncmp( dbase.file[i].name, dbase.file[i].name, LEN_NAME ) ) {
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
        if (ext && (strcmp(ext, ".h") == 0 || strcmp(ext, ".hpp") == 0)) {
            char target_path[1024];
            char *filename = basename((char *)fpath);
            snprintf(target_path, sizeof(target_path), "%s/%s", dest_dir, filename);

            printf("Copying: %s -> %s\n", fpath, target_path);

            db_add_record( fpath, filename );

            if ( ! conf.dryrun ) {
                copy_file(fpath, target_path);
            }
        }
    }
    return 0; // Continue walking
}


int main(int argc, char *argv[])
{
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <source_dir> <dest_dir> [-x]\n", argv[0]);
        return 1;
    }
    if (argc > 3) {
        conf.dryrun = 0;
    }

    const char *src_dir = argv[1];
    dest_dir = argv[2];

    // Create destination directory if it doesn't exist
    #ifdef LINUX
    mkdir(dest_dir, 0755);
    #else
    mkdir(dest_dir);
    #endif // LINUX

    // nftw(path, callback, max_open_fds, flags)
    if (nftw(src_dir, process_file, 20, FTW_PHYS) == -1) {
        perror("nftw");
        return 1;
    }

    printf("\n----------------------------------------------\n");
    printf("Files processed: %d\n", dbase.records);
    printf("Duplicates:      %d\n", db_mark_duplicates() );
    printf("Done.\n");
    return 0;
}
