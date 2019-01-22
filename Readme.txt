This is a readme file.

This Assignment consists of the following files
a make file - to execute the whole files with one go in the Linux based system.
type "make" to compile and create object files
type "make clean" to remove all object files and TestAssn1_1 file which is required for execution
type "./TestAssn1_1" and click enter to execute the assignment

storage_mgr.c - which holds the function definitions for all the functions required for the Assignment.

dberror.h - Header file which consists of all the constants RC defined 

dberror.c - file which holds functions defined for printError and errorMessage functions

test_assn1_1.c - The tests required for checking all the functions written for the programming Assignment
***********************************************************************************
createPageFile
Create a new page file fileName. The initial file size should be one page. This method should fill this single page with '\0' bytes.
***********************************************************************************
openPageFile
Opens an existing page file. Should return RC_FILE_NOT_FOUND if the file does not exist. The second parameter is an existing file handle. If opening the file is successful, then the fields of this file handle should be initialized with the information about the opened file. For instance, you would have to read the total number of pages that are stored in the file from disk.
***********************************************************************************
closePageFile, destroyPageFile
Close an open page file or destroy (delete) a page file.
***********************************************************************************
Read and Write Methods
There are two types of read and write methods that have to be implemented: Methods with absolute addressing (e.g., readBlock) and methods that address relative to the current page of a file (e.g., readNextBlock). 
***********************************************************************************
readBlock
The method reads the pageNumth block from a file and stores its content in the memory pointed to by the memPage page handle. If the file has less than pageNum pages, the method should return RC_READ_NON_EXISTING_PAGE.
***********************************************************************************
getBlockPos
Return the current page position in a file 
***********************************************************************************
readFirstBlock, readLastBlock
Read the first respective last page in a file
***********************************************************************************
readPreviousBlock, readCurrentBlock, readNextBlock
Read the current, previous, or next page relative to the curPagePos of the file. The curPagePos should be moved to the page that was read. If the user tries to read a block before the first page of after the last page of the file, the method should return RC_READ_NON_EXISTING_PAGE.
***********************************************************************************
writeBlock, writeCurrentBlock
Write a page to disk using either the current position or an absolute position.
***********************************************************************************
appendEmptyBlock
Increase the number of pages in the file by one. The new last page should be filled with zero bytes.
***********************************************************************************
ensureCapacity
If the file has less than numberOfPages pages then increase the size to numberOfPages.
***********************************************************************************
