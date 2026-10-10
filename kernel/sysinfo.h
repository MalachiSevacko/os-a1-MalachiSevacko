
struct sysinfo {
  uint64 freemem;   // bytes of free physical memory
  uint64 nproc;     // processes whose state is not UNUSED
  uint64 nopenfile; // file-table entries with ref > 0
};
