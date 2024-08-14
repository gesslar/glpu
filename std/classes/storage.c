#ifndef __CLASS_STORAGE_OPTIONS__
#define __CLASS_STORAGE_OPTIONS__

class StorageOptions {
    string storage_type;  // "shared" (default), "private", or "custom"
    mixed storage_id;    // Used only for private storage
    string storage_org;   // Organization name (required for all types)
    string storage_directory;
    int clean_on_empty;
    int restore_on_load;
}

#endif // __CLASS_STORAGE_OPTIONS__
