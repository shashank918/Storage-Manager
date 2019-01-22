#include "storage_mgr.h"
#include "dberror.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>



void initStorageManager(void){
printf("Storage manager has been initialised\n\n");
//We don't have any global data structure to initialise
}

/*createPageFile method used to Create file if not exists*/

RC createPageFile(char *fileName)
{
FILE *filepointer=NULL;
//check if file exists or not
struct stat check;
int exist = stat(fileName,&check);
if(exist == 0)
 {
 printf("File, %s already exists \n\n", fileName);
 return RC_OK;
 }

OPEN(filepointer,fileName,"w+");

CHECK_DO((filepointer==NULL),printf("File creation failed\n\n");return RC_FILE_CREATION_FAILED);
 printf("File, %s has been created\n\n", fileName);
 char *input = (char*) malloc(PAGE_SIZE);
 int i=0;
for(i=0; i<PAGE_SIZE; i++)
 {
 input[i]='\0';
 }
fwrite(input,sizeof(char),PAGE_SIZE,filepointer);
free(input);
CLOSE(filepointer);
return RC_OK;

}

/*openPageFile method used open file to read or write*/
RC openPageFile(char *fileName, SM_FileHandle *fHandle)
{
FILE *filepointer=NULL;
//check if file exists or not
 struct stat check;
 int exist = stat(fileName,&check);
 if(exist != 0)
 {
 printf("File, %s do not exists \n\n", fileName);
 return RC_FILE_NOT_FOUND;
 }

OPEN(filepointer,fileName,"r+");

 CHECK_DO((filepointer==NULL),printf("File, %s do not exists  \n\n", fileName);return RC_FILE_NOT_FOUND);

 struct stat total;
//count the total number of page in a file
 if(stat(fileName, &total) != -1 )
 {
 int numberOfPages;
 numberOfPages = (total.st_size)/PAGE_SIZE;

 CHECK_DO(((total.st_size)%PAGE_SIZE > 0),MOVE(fHandle->totalNumPages,numberOfPages+1);)
 else
MOVE(fHandle->totalNumPages,numberOfPages);

 }
 MOVE(fHandle->fileName,fileName);
 MOVE(fHandle->curPagePos,0);
 MOVE(fHandle->mgmtInfo,filepointer);
 return RC_OK;

}

/* closePageFile method closes the file and frees the memory*/
RC closePageFile(SM_FileHandle *fHandle)
{

FILE *filepointer=NULL;
int fileclosed;
int a;
a=0;

CHECK_DO(fHandle == NULL,return RC_FILE_HANDLE_NOT_INIT;)
 else
 filepointer=fHandle->mgmtInfo;
 (CLOSE(filepointer)==0) ? (printf("file, %s has been closed \n\n",fHandle->fileName), a=1) : a++ ;
  if(a=1)
  return RC_OK;
  else
  return RC_FILE_NOT_CLOSED;
}

/*destroyPageFile method deletes the file from memory*/

RC destroyPageFile(char *fileName)
{
int filedestroyed;
int a;
a=0;
filedestroyed = remove(fileName);
(filedestroyed==0) ? (printf("file, %s has been Destroyed \n\n",fileName), a=1) : a++ ;
 if(a=1)
 return RC_OK;
 else
 return RC_FILE_NOT_FOUND;
}


/*readBlock method reads the block from file and loads the data into memory*/

RC readBlock(int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage){

  int seeker;
  int a;
  FILE *filepointer = fopen(fHandle->fileName,"r+");
  fHandle->mgmtInfo = filepointer;
  if(fHandle==NULL)//check if the filepointer is NULL
  return RC_FILE_HANDLE_NOT_INIT;
  if(pageNum-fHandle->totalNumPages>0 || pageNum <0)//check the range of pages within the total PAGE_SIZE
  return RC_READ_NON_EXISTING_PAGE;
  //open the file if incase it is closed
   CHECK_DO(!filepointer,filepointer = fopen(fHandle->fileName,"r+");fHandle->mgmtInfo = filepointer;);
   seeker = fseek(filepointer,PAGE_SIZE*(pageNum),SEEK_SET);
  if(seeker!=0)
  return RC_FILE_READ_FAILED;
  else{//Read the page and store it inside memPage pointer
    fread(memPage,sizeof(char),PAGE_SIZE,filepointer);
    fHandle->curPagePos = pageNum;
    return RC_OK;
  }
}

//getBlockPos method returns the poistion at which the fHandle is present
  int getBlockPos (SM_FileHandle *fHandle){
    int pos = fHandle->curPagePos;
    return pos;
  }

//readFirstBlock method reads the First block of the File
// and stores content to memPage pointer
RC readFirstBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){
//We shall now call the readblock method to read the first block
//while passing first argument as zero
return readBlock(0,fHandle,memPage);
}

//readLastBlock method reads the Last block of the File
// and stores content to memPage pointer
RC readLastBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){
  //We shall now call the readblock method to read the first block
  //while passing first argument as totalNumPages-1
return readBlock(((fHandle->totalNumPages)-1),fHandle,memPage);
}

//readPreviousBlock method reads previous block while having the current position-1
RC readPreviousBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){
FILE *filepointer;
int num;
filepointer = fHandle->mgmtInfo;
num=(fHandle->curPagePos)-1;

return readBlock(num, fHandle, memPage);
}

//readCurrentBlock method reads previous block while having the current position
RC readCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){
  FILE *filepointer;
  int num;
  filepointer = fHandle->mgmtInfo;
  num=(fHandle->curPagePos);

  return readBlock(num, fHandle, memPage);
}
//readNextBlock method reads previous block while having the current position+1
RC readNextBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){
  FILE *filepointer;
  int num;
  filepointer = fHandle->mgmtInfo;
  num=(fHandle->curPagePos)+1;
  return readBlock(num, fHandle, memPage);
}

/*writeBlock method writes a data from memory to page file that is present on disk and page number is passed in the parameter*/
RC writeBlock(int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage)
{

	FILE *Fptr;
	CHECK_RC(fHandle==NULL,RC_FILE_HANDLE_NOT_INIT);
        OPEN_RC(Fptr,fHandle->fileName,"r+",RC_FILE_NOT_FOUND);
 	MOVE(fHandle->mgmtInfo,Fptr);
        CHECK_RC(pageNum>(fHandle->totalNumPages),RC_WRITE_OUT_OF_BOUND_INDEX);
	CHECK_RC(fseek(Fptr , pageNum*PAGE_SIZE , SEEK_SET) != 0,RC_FILE_OFFSET_FAILED);
        CHECK_RC(fwrite(memPage,sizeof(char),PAGE_SIZE,Fptr)!= PAGE_SIZE,RC_WRITE_FAILED);
	MOVE(fHandle->curPagePos, pageNum);
	MOVE(fHandle->totalNumPages,(ftell(Fptr)/PAGE_SIZE));
 	CLOSE(Fptr);
	return RC_OK;
}


/*writeCurrentBlock method writes a data from memory to current page file*/
RC writeCurrentBlock(SM_FileHandle *fHandle, SM_PageHandle memPage){
	int pageNum;

		MOVE(pageNum,fHandle->curPagePos);
		return writeBlock(pageNum, fHandle, memPage);
}

/*appendEmptyBlock method appends a new page at the end of the file*/
RC appendEmptyBlock(SM_FileHandle *fHandle){

		FILE *Fptr;
                OPEN(Fptr,fHandle->fileName,"a");
		ALLOC(input,PAGE_SIZE);
		FOR_LOOP(i,i<PAGE_SIZE,MOVE(input[i],'\0');)
	        CHECK_RC(fwrite(input,sizeof(char),PAGE_SIZE,Fptr) != PAGE_SIZE,RC_WRITE_FAILED)
		free(input);
		INCR(fHandle->totalNumPages,1);
		CHECK_RC(CLOSE(Fptr) != 0,RC_FILE_NOT_CLOSED)
		return RC_OK;
}


/*ensureCapacity method ensures the number of pages in the file*/
RC ensureCapacity(int numberOfPages, SM_FileHandle *fHandle){
	FILE *Fptr;
	CHECK_RC(fHandle==NULL,RC_FILE_HANDLE_NOT_INIT);
	MOVE(Fptr,fHandle->mgmtInfo);
	CHECK_DO(!Fptr,OPEN(Fptr,fHandle->fileName,"r+"));
	fseek(Fptr, 0L, SEEK_END);
	int pagesInFile = ftell(Fptr)/PAGE_SIZE;
	CHECK_DO(ftell(Fptr)%PAGE_SIZE > 0,pagesInFile=pagesInFile+1);
	CHECK_DO(pagesInFile < numberOfPages,
                 int pagesToAdd = numberOfPages - pagesInFile;
		 int totalSize = (pagesToAdd)*PAGE_SIZE;
		 FOR_LOOP(i,i<totalSize,fprintf(Fptr, "%c", '\0'););
		 fHandle->totalNumPages = (fHandle->totalNumPages)+pagesToAdd);
	printf("This file contains %i pages",fHandle->totalNumPages);
	return RC_OK;
}

