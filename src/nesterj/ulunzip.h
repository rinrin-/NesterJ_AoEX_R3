
int UseUnzipLibExeLoad(char *fn, unsigned char **buff, unsigned long *ucsize, const char **szExtensions);
int UseUnzipLibExeLoad_P(char *fn, unsigned char *buff, unsigned long msize, unsigned long *ucsize, const char **szExtensions);
int UsezipLibMemToZip(unsigned char *memp, unsigned int csize, char *outfn, char *infn);
int UsezipLibFindFileFromExtention(char *fn, const char **szExtensions);


