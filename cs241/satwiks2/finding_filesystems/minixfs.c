/**
 * Finding Filesystems
 * CS 241 - Spring 2019
 */
#include "minixfs.h"
#include "minixfs_utils.h"
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

/**
 * Virtual paths:
 *  Add your new virtual endpoint to minixfs_virtual_path_names
 */
char *minixfs_virtual_path_names[] = {"info", /* add your paths here*/};

/**
 * Forward declaring block_info_string so that we can attach unused on it
 * This prevents a compiler warning if you haven't used it yet.
 *
 * This function generates the info string that the virtual endpoint info should
 * emit when read
 */
static char *block_info_string(ssize_t num_used_blocks) __attribute__((unused));
static char *block_info_string(ssize_t num_used_blocks) {
    char *block_string = NULL;
    ssize_t curr_free_blocks = DATA_NUMBER - num_used_blocks;
    asprintf(&block_string, "Free blocks: %zd\n"
                            "Used blocks: %zd\n",
             curr_free_blocks, num_used_blocks);
    return block_string;
}

// Don't modify this line unless you know what you're doing
int minixfs_virtual_path_count =
    sizeof(minixfs_virtual_path_names) / sizeof(minixfs_virtual_path_names[0]);

int minixfs_chmod(file_system *fs, char *path, int new_permissions) {
    if (fs == NULL || path == NULL)
        return -1;
    inode *modify = get_inode(fs, path);
    if(modify == NULL) {
        errno = ENOENT;
        return -1;
    }
    int type = (modify->mode >> RWX_BITS_NUMBER);
    type = type << RWX_BITS_NUMBER;
    modify->mode = new_permissions;
    modify->mode |= type;
    clock_gettime(CLOCK_REALTIME, &(modify->ctim));
    // Thar she blows!
    return 0;
}

int minixfs_chown(file_system *fs, char *path, uid_t owner, gid_t group) {
    if (fs == NULL || path == NULL)
        return -1;
    inode *modify = get_inode(fs, path);
    if(modify == NULL) {
        errno = ENOENT;
        return -1;
    }
    if (owner != ((uid_t)-1))
        modify->uid = owner;
    if (group != ((gid_t)-1))
        modify->gid = group;
    clock_gettime(CLOCK_REALTIME, &(modify->ctim));
    // Land ahoy!
    return 0;
}

inode *minixfs_create_inode_for_path(file_system *fs, const char *path) {
    inode *exists = get_inode(fs, path);
    if(exists != NULL) {
        return NULL;
    }
    char *location = strdup(path);
    const char *file = NULL;
    inode_number new_inode = first_unused_inode(fs);
    inode *parent_dir = parent_directory(fs, path, &file);
    if (parent_dir != NULL && is_directory(parent_dir) && valid_filename(file)) {
        inode *new = fs->inode_root + new_inode;
        init_inode(parent_dir, new);
        minixfs_dirent write_dirent;
        write_dirent.name = strdup(file);
        write_dirent.inode_num = new_inode;

        *(location + (file-location) -1) = '\0';
        char buf[256];
        buf[255] = '\0';
        make_string_from_dirent(buf, write_dirent);
        minixfs_write(fs, location, buf, 256, (long*)&parent_dir->size);
        free(location);
        free(write_dirent.name);
        return new;
    }
    free(location);
    // Land ahoy!
    return NULL;
}

ssize_t minixfs_virtual_read(file_system *fs, const char *path, void *buf,
                             size_t count, off_t *off) {
    if (!strcmp(path, "info")) {
        int used = 0;
        for (unsigned i = 0; i < fs->meta->dblock_count; i++) {
            if (GET_DATA_MAP(fs->meta)[i] == 1) {
                used++;
            }
        }
        char *block_info = block_info_string(used);
        int len = strlen(block_info);
        if (*off >= len) {
            free(block_info);
            return 0;
        }

        if ((*off + count) >(size_t) len) {
            count = len-*off;
        }
        memcpy(buf, block_info+(*off),count);
        *off += count;
        free(block_info);
        return count;

        // TODO implement the "info" virtual file here
    }
    // TODO implement your own virtual file here
    errno = ENOENT;
    return -1;
}

ssize_t minixfs_write(file_system *fs, const char *path, const void *buf,
                      size_t count, off_t *off) {
    inode *modify = get_inode(fs, path);
    if (modify == NULL) {
        modify = minixfs_create_inode_for_path(fs, path);
    }
    int inode_count = NUM_DIRECT_INODES + NUM_INDIRECT_INODES;
    if (*off + count > inode_count * sizeof(data_block)) {
        errno = ENOSPC;
        return -1;
    }
    uint64_t temp = *off+count;
    int status = minixfs_min_blockcount(fs, path, temp/sizeof(data_block) + 1);
    if (status == -1) {
        errno = ENOSPC;
        return -1;
    }
    int num_blocks = *off/sizeof(data_block);
    int offset = 0;
    for (size_t i = 0; i < count; i++) {
        if ( offset == sizeof(data_block) ) {
            offset = 0;
            num_blocks++;
        }
        if (num_blocks < NUM_DIRECT_INODES) {
            fs->data_root[modify->direct[num_blocks]].data[offset] = ((char*)buf)[i];
        }
        else {
            fs->data_root[((data_block_number*)fs->data_root[modify->indirect].data)[num_blocks-NUM_DIRECT_INODES]].data[offset] = ((char*)buf)[i];
        }
        offset++;
    }

    modify->size = *off + count;
    clock_gettime(CLOCK_REALTIME, &(modify->atim));
    clock_gettime(CLOCK_REALTIME, &(modify->mtim));
    // X marks the spot
    return count;
}

ssize_t minixfs_read(file_system *fs, const char *path, void *buf, size_t count,
                     off_t *off) {
    const char *virtual_path = is_virtual_path(path);
    if (virtual_path)
        return minixfs_virtual_read(fs, virtual_path, buf, count, off);
    // 'ere be treasure!
    inode *modify = get_inode(fs, path);
    if(modify == NULL) {
        errno = ENOENT;
        return -1;
    }

    if ((size_t)*off > modify->size)
        return 0;

    if (count > modify->size - *off)
        count = modify->size - *off;

    int num_blocks = (count)/sizeof(data_block);

    int offset = *off + count - num_blocks*sizeof(data_block) - 1;

    for(size_t i = 0; i < count; i++) {
        if (offset == -1) {
            offset += sizeof(data_block);
            num_blocks--;
        }
        if (num_blocks < NUM_DIRECT_INODES)
            ((char*)buf)[count-i-1] = fs->data_root[modify->direct[num_blocks]].data[offset--];
        else
            ((char*)buf)[count-i-1] = fs->data_root[((data_block_number*)fs->data_root[modify->indirect].data)[num_blocks-NUM_DIRECT_INODES]].data[offset--];
    }
    *off += count;
    clock_gettime(CLOCK_REALTIME, &(modify->atim));
    return count;
}
