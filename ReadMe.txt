Name of the Project:ListFilesFolders
Purpose: It should list all the files and folders based on certain parameters. Usually whenever one of the drives starts getting filled i try to search each directory and try to find the files and folders which are taking 
too much space and try to delete it. This task is little bit boring as well as time consuming. I need to check each folder its size and if it is taking much space then go inside that folder and try to see which file or folder is taking. If
it is a file and it is not relavant then delete it else if its folder then again follow same steps.

Requirments:
1. List the files and folders based on a searching criteria.
	Searching criteria may be
	1. Name of the file/folder
	2. Atleast size
	3. Atmax size
	4. Date Modified/Date Created
	5. User/Creator of the file
	6. Combination of all these
	7. Hidden folders
	8. Readonly files
	9. Shared Folder/Files
	10. Type of the file
2. Copy the file/Folder to a new location
3. Delete the file/Folder
4. A descent UI


Design:
1. Need a Structure to hold the search criteria
2. Need a structure to hold the file information
3. Need a structure to hold the folder information

	