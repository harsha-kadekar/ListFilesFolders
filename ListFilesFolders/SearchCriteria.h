#ifndef SEARCHCRITERIA_H
#define SEARCHCRITERIA_H

#include<Windows.h>

typedef struct SearchFilesFolders
{
	bool bHiddenFiles;
	bool bReadOnlyFiles;
	__int64 nMaxSize;
	__int64 nMinSize;
	bool bSearchForMaxSize;
	bool bSearchForMinSize;
	bool bCreationDateGreater;
	bool bCreationDateLesser;
	bool bModificationGreater;
	bool bModificationLesser;
	WCHAR* strCreationDate;
	WCHAR* strModificationDate;
	WCHAR* location;
	

}SEARCHFILEFOLDER;


typedef struct FileInfo
{
	__int64 dwFileSize;
	WCHAR* strFileName;
	WCHAR* location;
	DWORD dwFileAttributes;
	bool bHidden;
	bool bReadonly;
	FILETIME ftCreationTime;
	FILETIME ftModificationTime;
	bool bIsFile;
	bool bError;
	struct lstFileInfo* ParentFolder;
	struct LstFileInSubFolder* ChildElements;
	PWIN32_FIND_DATA actualData;
}FILEINFO;

typedef struct LstFileInSubFolder
{
	FILEINFO* fileFolder;
	FILEINFO* nextFileFolder;
}LSTFILEINSUBFOLDER;



#endif