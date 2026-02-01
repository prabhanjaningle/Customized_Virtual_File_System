////////////////////////////////
//
//  Header Files
//
///////////////////////////////////////////////////////////

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<stdbool.h>

///////////////////////////////////////////////////////////
//
//  User defined Macros
//
///////////////////////////////////////////////////////////

//Max file size
#define MAXFILESIZE 100

// Maximum number of files that we can opened
#define MAXOPENEDFILES 20

//Maximum number of file that we can create
#define MAXINODE 5

#define READ 1
#define WRITE 2
#define EXECUTE 4

#define REGULARFILE 1
#define SPECIALFILE 2

#define START  0
#define CURRENT 1
#define END 2

#define EXECUTE_SUCCESS 0

///////////////////////////////////////////////////////////
//
//  User defined Macros for ERROR Handling
//
///////////////////////////////////////////////////////////

#define ERR_INVALID_PARAMETER -1
#define ERR_NO_INODES -2
#define ERR_FILE_ALREADY_EXIST -3
#define ERR_FILE_NOT_EXIST -4
#define ERR_PERMISSION_DENIED -5
#define ERR_INSUFFICIENT_SPACE -6
#define ERR_INSUFFICIENT_DATA -7

///////////////////////////////////////////////////////////
//
//  Structure Name: BootBlock
//  Description   : Hold information to boot the OS
//
///////////////////////////////////////////////////////////

struct BootBlock
{
    char Information[100];
};

///////////////////////////////////////////////////////////
//
//  Structure Name: SuperBlock
//  Description   : Hold information about file system
//
///////////////////////////////////////////////////////////

struct SuperBlock
{
    int TotalInodes;
    int FreeInodes;
};

///////////////////////////////////////////////////////////
//
//  Structure Name: Inode
//  Description   : Hold information about file
//
///////////////////////////////////////////////////////////

typedef struct Inode
{
    char FileName[50];
    int InodeNumber;
    int FileSize;
    int ActualFileSize;
    int FileType;
    int RefferenceCount;
    char *Buffer;
    int Permission;
    int LinkCount;
    struct Inode *next;
} INODE, *PINODE, **PPINODE;

///////////////////////////////////////////////////////////
//
//  Structure Name: FileTable
//  Description   : Hold information about opened file
//
///////////////////////////////////////////////////////////

typedef struct FileTable
{
    int ReadOffset;
    int WriteOffset;
    int Count;
    int Mode;
    PINODE ptrinode;
} FILETABLE, *PFILETABLE;

///////////////////////////////////////////////////////////
//
//  Structure Name: UAREA
//  Description   : Hold information about process
//
///////////////////////////////////////////////////////////

struct UAREA
{
    char ProcessName[50];
    PFILETABLE UFDT[MAXOPENEDFILES];
};

///////////////////////////////////////////////////////////
//
//  Global variables
//
///////////////////////////////////////////////////////////

struct BootBlock bootobj;
struct SuperBlock superobj;
PINODE head = NULL;
struct UAREA uareaobj;

///////////////////////////////////////////////////////////
//
//  Function Name : InitialiseUAREA
//  Author        : Prabhanjan Sanjay Ingle
//
///////////////////////////////////////////////////////////

void InitialiseUAREA()
{
    strcpy(uareaobj.ProcessName,"Myexe");

    for(int i = 0; i < MAXOPENEDFILES; i++)
    {
        uareaobj.UFDT[i] = NULL;
    }

    printf("Marvellous CVFS : UAREA initialised successfully\n");
}

///////////////////////////////////////////////////////////
//
//  Function Name : InitialiseSuperblock
//  Author        : Prabhanjan Sanjay Ingle
//
///////////////////////////////////////////////////////////

void InitialiseSupeblock()
{
    superobj.TotalInodes = MAXINODE;
    superobj.FreeInodes = MAXINODE;

    printf("Marvellous CVFS : Super Block initialised successfully\n");
}

///////////////////////////////////////////////////////////
//
//  Function Name : CreateDILB
//  Author        : Prabhanjan Sanjay Ingle
//
///////////////////////////////////////////////////////////

void CreateDILB()
{
    PINODE newn = NULL, temp = NULL;

    for(int i = 1; i <= MAXINODE; i++)
    {
        newn = (PINODE)malloc(sizeof(INODE));

        newn->InodeNumber = i;
        newn->FileSize = 0;
        newn->ActualFileSize = 0;
        newn->LinkCount = 0;
        newn->Permission = 0;
        newn->FileType = 0;
        newn->RefferenceCount = 0;
        newn->Buffer = NULL;
        newn->next = NULL;

        if(temp == NULL)
        {
            head = newn;
            temp = head;
        }
        else
        {
            temp->next = newn;
            temp = temp->next;
        }
    }

    printf("Marvellous CVFS : DILB created successfully\n");
}

///////////////////////////////////////////////////////////
//
//  Function Name : StartAuxilaryDataInitialisation
//  Author        : Prabhanjan Sanjay Ingle
//
///////////////////////////////////////////////////////////

void StartAuxilaryDataInitialisation()
{
    strcpy(bootobj.Information,"Boot Process of Operating System done\n");
    printf("%s",bootobj.Information);

    InitialiseSupeblock();
    CreateDILB();
    InitialiseUAREA();

    printf("Marvellous CVFS : Auxiliary Data initialised successfully\n");
}
///////////////////////////////////////////////////////////
//
//  Function Name : DisplayHelp
//  Discription :   It is used to Display the information about Commands
//  Author :        Prabhanjan Sanjay Ingle
//  Date :          11/08/2025 
//
///////////////////////////////////////////////////////////

void DisplayHelp()
{
    printf("--------------------------------------------------------------------\n");
    printf("------------------Command Manual of Marvellous CVFS-----------------\n");
    printf("--------------------------------------------------------------------\n");

    printf("man : It is used to Display the specific manual page of command\n");
    printf("exit : It is used to Terminate the shell of Marvellous CVFS\n");
    printf("clear : It is used to clear the console of Marvellous CVFS\n");
    printf("creat : It is used to create new file\n");
    printf("unlink : It is used to delete existing file\n");
    printf("stat : It is used to display statiscal information about file\n");
    printf("ls : It is used to lis out all files from the Dorectory\n");
    printf("write : It is used to write the data into file\n");
    printf("read : it is used to read data from file \n");

    //Add More Option here
    
    printf("--------------------------------------------------------------------\n");

}

///////////////////////////////////////////////////////////
//
//  Function Name : ManPage
//  Discription :   It is used to Display Manual Page of the Command
//  Input :         It Accepts the command name
//  Output :        It Displays the Manual details of the command
//  Author :        Prabhanjan Sanjay Ingle
//  Date :          11/08/2025 
//
///////////////////////////////////////////////////////////

void ManPage(
            char *name      //Name of Command
        )
{
    if(strcmp(name,"creat") == 0)
    {
        printf("Dispcription : This command is used to create new regular on our file system\n");
        printf("Usage : creat File_Name Permisssioin\n");
        printf("File_Name : File that you want create\n");
        printf("Permissioin : \n1 : Read  \n2 : Write \n3 : Read + AWrite\n");
    }
    else if(strcmp(name,"exit") == 0)
    {
        printf("Dispcription : This command is used to Terminate the Marvellous CVFS \n");
        printf("Usage : exit\n");
    }
    else if(strcmp(name,"unlink") == 0)
    {
        printf("Dispcription : This command is used to delete regular file system\n");
        
        printf("Usage : unlink File_Name \n");
        printf("File_Name : File that you want delete\n");
    }
    else if(strcmp(name,"stat") == 0)
    {
        printf("Dispcription : This command is used to Display statistical about file\n");
        
        printf("Usage : stat File_Name \n");
        printf("File_Name : The nme of the file whose inforamtion you want to display\n");
    }
    else if(strcmp(name,"ls") == 0)
    {
        printf("Dispcription : This command is used to list all file name foirm diorectory\n");
        
        printf("Usage : ls \n");
    }
    else if(strcmp(name,"write") == 0)
    {
        printf("Dispcription : This command is used to write the data into the file\n");
        
        printf("Usage : write_file_Descriptor \n");
    }
    else if(strcmp(name,"read") == 0)
    {
        printf("Dispcription : This command is used to read the data from the file\n");
        
        printf("Usage : read File_Descriptor size \n");
        printf("File_Descriptor :  Its a value returned by create system call\n");
        printf("Size : Number of bytes that you wanr to read\n");
    }
    //Add More Option here
    else
    {
        printf("No manual entry for %s\n",name);
    }
}

///////////////////////////////////////////////////////////
//
//  Function Name : isFileExists
//  Discription :   It is used to check whether the given file exists or not
//  Input :         File_Name 
//  Output :        It return boolean Value(True : if present, Flase : If not present) 
//  Author :        Prabhanjan Sanjay Ingle
//  Date :          11/08/2025 
//
///////////////////////////////////////////////////////////

bool IsFileExists(
                char *name      //Name of File that we want to check
            )
{
    PINODE temp = head;
    bool bFlag = false;

    while(temp != NULL)             //loop for DILB INodes 
    {
        if((strcmp(name, temp -> FileName) == 0) && (temp -> FileType == REGULARFILE))  
        {
            bFlag = true;
            break;
        }
        temp = temp -> next;
    }

    return bFlag;
} //End of is File exists.

///////////////////////////////////////////////////////////
//
//  Function Name : CreateFile
//  Discription :   It is used to new regular File
//  Input :         File_Name and Permission
//  Output :        It return the File Discriptor 
//  Author :        Prabhanjan Sanjay Ingle
//  Date :          11/08/2025 
//
///////////////////////////////////////////////////////////

int CreateFile(
                char *name,     //Name of File
                int permission  // Permssion to Create File

          )
{  
    PINODE temp = head;
    int i = 0;

    printf("Current Inodes remaining : %d:\n",superobj.FreeInodes);
    //FILTERS

    //if File_Name is Missing
    if(name == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }

    //if Entered Permission is Invalid
    if(permission < 1 || permission > 3)
    {
        return ERR_INVALID_PARAMETER;
    }

    //Check whether empty Inode is there or not
    if(superobj.FreeInodes == 0)
    {
        return ERR_NO_INODES;

    }

    //Check whether File is already exist or not
    if(IsFileExists(name) == true)
    {
        return ERR_FILE_ALREADY_EXIST;
    }

    //Loop to search free Inode
    while(temp != NULL)
    {
        if(temp -> FileType == 0)
        {
            break;
        }
        temp = temp -> next;
    }

    //If Inode Not found
    if(temp == NULL)
    {
        printf("Inode not Found\n");
        return ERR_NO_INODES;
    }

    //Search First Non NULL value form UFDT
    for(i = 0; i < MAXINODE; i++)
    {
        if(uareaobj.UFDT[i] == NULL)
        {
            break;
        }
    }

    if(i == MAXINODE)
    {
        printf("Unable to Create File as MAX OPENED FILE LIMIT REACHED\n");
        return -1;
    }

    //Allocate Memory for File Table
    uareaobj.UFDT[i] = (PFILETABLE)malloc(sizeof(FILETABLE));

    //Initialised memory for File table elements
    uareaobj.UFDT[i] -> ReadOffset = 0;
    uareaobj.UFDT[i] -> WriteOffset = 0;
    uareaobj.UFDT[i] -> Count = 1;
    uareaobj.UFDT[i] -> Mode = permission;

    //Connect file Table IIT
    uareaobj.UFDT[i] -> ptrinode = temp;


    strcpy(uareaobj.UFDT[i] -> ptrinode -> FileName, name);    
    uareaobj.UFDT[i] -> ptrinode -> FileSize        = MAXFILESIZE;
    uareaobj.UFDT[i] -> ptrinode -> ActualFileSize  = 0;
    uareaobj.UFDT[i] -> ptrinode -> FileType        = REGULARFILE;
    uareaobj.UFDT[i] -> ptrinode -> RefferenceCount = 1 ;
    uareaobj.UFDT[i] -> ptrinode -> LinkCount       = 1 ;
    uareaobj.UFDT[i] -> ptrinode -> Permission      = permission;

    //Allocate memory for Buffer
    uareaobj.UFDT[i] -> ptrinode -> Buffer =(char*)malloc(MAXFILESIZE) ;

    //Decrement the number of free node by 1
    superobj.FreeInodes--;
    return i;

}

///////////////////////////////////////////////////////////
//
//  Function Name : UnlinkFile
//  Discription :   It is used to delte regular File
//  Input :         File_Name 
//  Output :        It return nothing 
//  Author :        Prabhanjan Sanjay Ingle
//  Date :          15/08/2025 
//
///////////////////////////////////////////////////////////

// Marvellous CVFS > unlink Demo.txt
int UnlinkFile(
                char *name          //Name  of the File.
                )
{
    int i = 0;

    if(name == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }
    if(IsFileExists(name) == false)
    {
        return ERR_FILE_NOT_EXIST;
    }

    for(i = 0; i < MAXINODE; i++)
    {
        if(uareaobj.UFDT[i] != NULL)
        {
            if(strcmp(uareaobj.UFDT[i] -> ptrinode -> FileName,name) == 0)
            {
                //Deallocate the memory of buffer
                free(uareaobj.UFDT[i] -> ptrinode -> Buffer);

                //Reset all values of Inode
                uareaobj.UFDT[i] -> ptrinode -> FileSize = 0 ;
                uareaobj.UFDT[i] -> ptrinode -> ActualFileSize = 0;
                uareaobj.UFDT[i] -> ptrinode -> LinkCount = 0;
                uareaobj.UFDT[i] -> ptrinode -> Permission = 0;
                uareaobj.UFDT[i] -> ptrinode -> FileType = 0;
                uareaobj.UFDT[i] -> ptrinode -> RefferenceCount = 0;

                //Deallocate Memory of File Table
                free(uareaobj.UFDT[i]);

                //Set NULL to UFDT member
                uareaobj.UFDT[i] = NULL;

                //Increment the value of FreeInodes of Count
                superobj.FreeInodes++;

                break;
            }//END of if
            
        }//END of if
    } /// END of For

    return EXECUTE_SUCCESS;
}

///////////////////////////////////////////////////////////
//
//  Function Name : ls_file
//  Discription :   It is used to display the information about all files ibt the directory 
//  Input :         NOthing 
//  Output :        Nothing 
//  Author :        Prabhanjan Sanjay Ingle
//  Date :          15/08/2025 
//
///////////////////////////////////////////////////////////

//Marvellous CVFS > ls

void ls_file()
{
    PINODE temp = head;
    while(temp != NULL)
    {
        if(temp -> FileType != 0)
        {
            printf("%s\n",temp -> FileName);
        }
        temp = temp -> next;
    }
}

///////////////////////////////////////////////////////////
//
//  Function Name : stat_file
//  Discription :   It is used to diplsy the information about the given files
//  Input :         File_Name 
//  Output :        Nothing 
//  Author :        Prabhanjan Sanjay Ingle
//  Date :          15/08/2025 
//
///////////////////////////////////////////////////////////


int stat_file(
                char *name          //Name of File
             )
{
    PINODE temp = head;

    if(name == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }

    if(IsFileExists(name) == false)
    {
        return ERR_FILE_NOT_EXIST;
    }

    while(temp != NULL)
    {
        if((strcmp(name,temp -> FileName) == 0) && (temp -> FileType != 0))
        {
            printf("------------------Statistical Information of File---------------\n");
        
        printf("File Name           : %s\n ",temp -> FileName);
        printf("File size on Disk   : %d\n ",temp -> FileSize);
        printf("Actual File Size    : %d\n ",temp -> ActualFileSize);
        printf("Link Count          : %d\n ",temp -> LinkCount);

        printf("File Permissoin : ");

        if(temp -> Permission == READ)
        {
            printf("Read\n");
        }
        else if(temp -> Permission == WRITE)
        {
            printf("Write\n");
        }
        else if(temp -> Permission == READ + WRITE)
        {
            printf("Read + Write\n");
        }

        printf("File Type : ");

        if(temp -> FileType == REGULARFILE)
        {
            printf("Regular File\n");
        }
        else if(temp -> FileType == SPECIALFILE)
        {
            printf("Special File\n");
        }

        printf("--------------------------------------------------------------------\n");

        }
        temp = temp -> next;
    }

    return EXECUTE_SUCCESS;
}

///////////////////////////////////////////////////////////
//
//  Function Name : write_file
//  Discription :   It is used to write the content into the file 
//  Input :         File descriptor
//  Input :         Address of buffer which contains data
//                  Size of data that you want to write
//  Output :        Number of bytes successfully written into file
//  Author :        Prabhanjan Sanjay Ingle
//  Date :          15/08/2025 
//
///////////////////////////////////////////////////////////

//Marvellous CVFS > write 3

int write_file(
                int fd,             //File Descriptor of file
                char *data,         // Data that we wnat to write
                int size           // Size of data that we want to write
               )
{
    unsigned long int offset = 0;

    printf("File Descriptor is : %d\n",fd);
    printf("Data that we want to write : %s\n",data);
    printf("Number of bytes that we want to write : %d\n",size);

    //Invalid value of FD
    if(fd < 0 || fd > MAXOPENEDFILES)
    {
        return ERR_INVALID_PARAMETER;
    }

    //File is not opened or created with or given fd
    if(uareaobj.UFDT[fd] == NULL)
    {
        return ERR_FILE_NOT_EXIST;
    }

    //If there is no permission to write the data into the file
    if(uareaobj.UFDT[fd] -> ptrinode -> Permission <= WRITE)
    {
        return ERR_PERMISSION_DENIED;
    }

    //Unabl to write as there is no sufficient space;
    if((MAXFILESIZE-uareaobj.UFDT[fd] -> WriteOffset) < size)
    {
        return ERR_INSUFFICIENT_SPACE;
    }

    //Calculate the actual offset
    // offset = uareaobj.UFDT[fd] -> ptrinode -> Buffer + uareaobj.UFDT[fd] -> WriteOffset;

    //wirte the actual data
    strncpy(uareaobj.UFDT[fd] -> ptrinode -> Buffer + uareaobj.UFDT[fd] -> WriteOffset,data,size);

    //update the write offset
    uareaobj.UFDT[fd] -> WriteOffset = uareaobj.UFDT[fd] -> WriteOffset + size;

    // Update the actual size of file after the writing the data
    uareaobj.UFDT[fd] -> ptrinode -> ActualFileSize = uareaobj.UFDT[fd] -> ptrinode -> ActualFileSize + size; 

    return 0;
}


///////////////////////////////////////////////////////////
//
//  Function Name : read_file
//  Discription :   It is used to read the content from the file 
//  Input :         File descriptor
//  Input :         Address of EmptyBuffer 
//                  Size of data that you want to read
//  Output :        Number of bytes successfully written into file
//  Author :       Prabhanjan Sanjay Ingle
//  Date :          15/08/2025 
//
///////////////////////////////////////////////////////////

//Marvellous CVFS > read 3 10

int read_file(
                int fd,             //File_Descriptor
                char *data,         //Address of Empty Buffer
                int size            //Number of bytes that we want read
              )
{
    //Invalid value of FD
    if(fd < 0 || fd > MAXOPENEDFILES)
    {
        return ERR_INVALID_PARAMETER;
    }

    if(data == NULL || size <= 0)
    {

    }

    //File is not opened or created with or given fd
    if(uareaobj.UFDT[fd] == NULL)
    {
        return ERR_FILE_NOT_EXIST;
    }

    //If there is no permission to write the data into the file
    if(uareaobj.UFDT[fd] -> ptrinode -> Permission <= READ)
    {
        return ERR_PERMISSION_DENIED;
    }

    //Unable to read as there is no sufficient data
    if((MAXFILESIZE - uareaobj.UFDT[fd] -> ReadOffset) < size)
    {
        return ERR_INSUFFICIENT_DATA;
    }

    strncpy(data, uareaobj.UFDT[fd] ->ptrinode -> Buffer + uareaobj.UFDT[fd] -> ReadOffset,size);

    uareaobj.UFDT[fd] -> ReadOffset = uareaobj.UFDT[fd] -> ReadOffset + size;

    return size;
}

///////////////////////////////////////////////////////////
//
//  Entry point Function of Project(main)
//
///////////////////////////////////////////////////////////

int main()
{
    char str[80] = {'\0'};
    int iCount = 0;
    char Command[5][80];
    int iRet = 0;
    char InputBuffer[MAXFILESIZE] = {'\0'};
    char *EmptyBuffer = NULL; 

    StartAuxilaryDataInitialisation();

    printf("--------------------------------------------------------------------\n");
    printf("------------------Marvellous CVFS Started Successfuly---------------\n");
    printf("--------------------------------------------------------------------\n");

    while(1)
    {
        fflush(stdin);                  

        strcpy(str,"");
        printf("\nMarvellous CVFS > ");
        fgets(str,sizeof(str),stdin);


        iCount = sscanf(str,"%s %s %s %s",Command[0],Command[1],Command[2],Command[3]);


        fflush(stdin);
        if(iCount == 1)
        {
            //Marvellous CVFS > exit

            if(strcmp(Command[0],"exit") == 0)
            {
                printf("Thank you for using Marvellous CVFS\n");
                printf("Deallocating all resources...\n");

                break;
            }

            //Marvellous CVFS > help
            
            else if (strcmp(Command[0],"help") == 0)
            {
                DisplayHelp();
            }

            //Marvellous CVFS > clear
            else if(strcmp(Command[0],"clear") == 0)
            {
                system("clear");
            }

            //Marvellous CVFS > ls
            else if(strcmp(Command[0],"ls") ==  0)
            {
                ls_file();
            }
            else 
            {
            printf("Command not Found...\n");
            printf("Please refer help Optioin or use man Command\n");
            } 

        }   //End of if iCount == 1


        else if( iCount == 2)
        {
            //Marvellous CVFS > man creat

            if(strcmp(Command[0],"man") == 0)
            {
                ManPage(Command[1]);    
            }
            else if(strcmp(Command[0],"unlink") == 0)
            {
                iRet = UnlinkFile(Command[1]);

                if(iRet == EXECUTE_SUCCESS)
                {
                    printf("Unlink operation successfully performed\n");
                }
                else if(iRet == ERR_FILE_NOT_EXIST)
                {
                    printf("Error : Unable to do unlink activity as File is not present\n");
                }
                else if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Invalid Parameters for the function\n");
                    printf("Please check Man page for more details\n"); 
                }
                
            }
            //Marvellous CVFS > write 3
            else if(strcmp(Command[0],"write") == 0)
                {
                    printf("Please Enter the Data you want to write into the file\n");
                    fgets(InputBuffer,MAXFILESIZE,stdin);

                    iRet = write_file(atoi(Command[1]), InputBuffer, strlen(InputBuffer) - 1);
                    
                    if(iRet == ERR_INSUFFICIENT_SPACE)
                    {
                        printf("Error : Insufficient space in the data block for the file\n");
                    }
                    else if(iRet == ERR_PERMISSION_DENIED)
                    {
                        printf("Error : Unble to write as there is not permission to write\n");
                    }
                    else if(iRet == ERR_INVALID_PARAMETER)
                    {
                        printf("Error : Invalid Parameters for the function\n");
                        printf("Please check Man page for more details\n");
                    }
                    else if(iRet == ERR_FILE_NOT_EXIST)
                    {
                        printf("Error : FD is invalid\n");
                    }
                    else
                    {
                        printf("%d bytes gets succcessfully written into the file\n",iRet);
                        printf("Data from file is : %s\n",uareaobj.UFDT[0] -> ptrinode->Buffer);
                    }
                }
            else if(strcmp(Command[0],"stat") == 0)
            {
                iRet = stat_file(Command[1]);
                if(iRet == ERR_FILE_NOT_EXIST)
                {
                    printf("Error : Unable to do Display statics as File is not present\n");
                }
                else if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Invalid Parameters for the function\n");
                    printf("Please check Man page for more details\n"); 
                }
            }
            else 
            {
            printf("Command not Found...\n");
            printf("Please refer help Optioin or use man Command\n");
            } 
        }   //End of if iCount == 2

        else if(iCount == 3)
        {
            //Marvellous CVFS > creat Ganesh.txt 3
            
            if(strcmp(Command[0],"creat") == 0)
            {
                iRet = CreateFile(Command[1], atoi(Command[2]));     //here we converting the string to int for Command[2]
                
                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Invalid Parameters for the function\n");
                    printf("Please check Man page for more details\n");
                }

                else if(iRet == ERR_NO_INODES)
                {

                    printf("Error : Unable to create to file as there is no Inodes\n");

                }
                else if(iRet == ERR_FILE_ALREADY_EXIST)
                {
                    printf("Error : Unable to create file as Already existing\n");
                }
                else
                {
                    printf("File is successfully created with FD : %d\n",iRet );
                }
            }
            //Marvellous CVFS > read  3(fd) 10(size)
            else if(strcmp(Command[0],"read") == 0)
            {
                EmptyBuffer = (char*)malloc(sizeof(atoi(Command[2])));

                iRet = read_file(atoi(Command[1]),EmptyBuffer,atoi(Command[2]));

                if(iRet == ERR_INSUFFICIENT_DATA)
                    {
                        printf("Error : Insufficient data in the data block for the file\n");
                    }
                    else if(iRet == ERR_PERMISSION_DENIED)
                    {
                        printf("Error : Unble to read as there is not permission to read\n");
                    }
                    else if(iRet == ERR_INVALID_PARAMETER)
                    {
                        printf("Error : Invalid Parameters for the function\n");
                        printf("Please check Man page for more details\n");
                    }
                    else if(iRet == ERR_FILE_NOT_EXIST)
                    {
                        printf("Error : FD is invalid\n");
                    }
                    else
                    {
                        printf("Read operation is successfull\n");
                        printf("Data from file is : %s\n",EmptyBuffer);
                        free(EmptyBuffer);
                    }
            }
            else 
            {
            printf("Command not Found...\n");
            printf("Please refer help Optioin or use man Command\n");
            } 
        }   //End of if iCount == 3
        else if(iCount == 4)
        {

        }   //End of if iCount == 4
        else 
        {
            printf("Command not Found...\n");
            printf("Please refer help Optioin or use man Command\n");
        }   //End of Invalid Command part

        
    }//End of While(Custom Shell)
    return 0;
} //End of mian