#ifndef SEARCHCRITERIA_H
#define SEARCHCRITERIA_H

typedef struct SearchFilesFolders
{
	bool bHiddenFiles;
	bool bReadOnlyFiles;
	int nMaxSize;
	int nMinSize;
	bool bCreationDateGreater;
	bool bCreationDateLesser;
	bool bModificationGreater;
	bool bModificationLesser;
	WCHAR* strCreationDate;
	WCHAR* strModificationDate;
	

}SEARCHFILEFOLDER;

#endif