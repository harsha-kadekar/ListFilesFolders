#ifndef SEARCHCRITERIA_H
#define SEARCHCRITERIA_H

#include<Windows.h>

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


typedef struct lstFileInfo
{
	DWORD dwFileSize;
	WCHAR* strFileName;
	WCHAR* location;
	DWORD dwFileAttributes;
	bool bHidden;
	bool bReadonly;
	FILETIME ftCreationTime;
	FILETIME ftModificationTime;
	bool bIsFile;
	struct lstFileInfo* ParentFolder;
	struct lstFileInfo* ChildElements;
	PWIN32_FIND_DATA actualData;
}LSTFILEINFO;

#endif