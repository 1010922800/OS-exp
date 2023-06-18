#include "File.h"
#include "Disk.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

dirTable *rootDirTable; //��Ŀ¼
dirTable *currentDirTable;  //��ǰλ��
char path[200]; //���浱ǰ����·��

//��ʼ����Ŀ¼
void initRootDir() {
    //����һ���̿�ռ��rootDirTable
    int startBlock = getBlock(1);
    char parent[] = "..";
    if (startBlock == -1)
        return;
    rootDirTable = (dirTable *) getBlockAddr(startBlock);
    rootDirTable->dirUnitAmount = 0;
    //����Linux�����ø�Ŀ¼�ĸ�Ŀ¼��Ϊ��Ŀ¼
    addDirUnit(rootDirTable, parent, 0, startBlock);
    currentDirTable = rootDirTable;
    //��ʼ����ʼ����·��
    path[0] = '/';
    path[1] = '\0';
}


//��õ�ǰ����·��
char *getPath() {
    return path;
}


//չʾ��ǰĿ¼ ls
void showDir() {
    int unitAmount = currentDirTable->dirUnitAmount;
    printf("total:%d\n", unitAmount);
    printf("name\ttype\tsize\tFCB\tdataStartBlock\n");
    //�������б���
    for (int i = 0; i < unitAmount; i++) {
        //��ȡĿ¼��
        dirUnit unitTemp = currentDirTable->dirs[i];
        printf("%s\t%s\t", unitTemp.fileName, unitTemp.type == 0 ? "dir" : "file");
        //�ñ������ļ������������С����ʼ�̿��
        if (unitTemp.type == 1) {
            int FCBBlock = unitTemp.startBlock;
            FCB *fileFCB = (FCB *) getBlockAddr(FCBBlock);
            printf("%d\t%d\t%d\n", fileFCB->fileSize, FCBBlock, fileFCB->dataStartBlock);
        } else {
            int dirBlock = unitTemp.startBlock;
            dirTable *myTable = (dirTable *) getBlockAddr(dirBlock);
            printf("%d\t%d\t-\n", myTable->dirUnitAmount, unitTemp.startBlock);
        }
    }
}


//TODO �л�Ŀ¼ cd
int changeDir(char dirName[])
{
    //Ŀ¼����Ŀ¼λ��

    //�ļ���������

    //����Ŀ¼����

    //�޸ĵ�ǰĿ¼(currentDirTable)

    //�޸�ȫ�־���·��(path)
}


//TODO �޸��ļ�������Ŀ¼�� rn
int changeName(char oldName[], char newName[])
{
}

//�����ļ� touch
int creatFile(char fileName[], int fileSize) {
    //����ļ����ֳ���
    if (strlen(fileName) >= FILENAME_MAX_LENGTH) {
        printf("file name too long\n");
        return -1;
    }
    //���FCB�Ŀռ�
    int FCBBlock = getBlock(1);
    if (FCBBlock == -1)
        return -1;
    //��ȡ�ļ����ݿռ�
    int FileBlock = getBlock(fileSize);
    if (FileBlock == -1)
        return -1;
    //����FCB
    if (creatFCB(FCBBlock, FileBlock, fileSize) == -1)
        return -1;
    //��ӵ�Ŀ¼��
    if (addDirUnit(currentDirTable, fileName, 1, FCBBlock) == -1)
        return -1;
    return 0;
}


//TODO ����Ŀ¼ mkdir
int creatDir(char dirName[])
{
    //����ļ����ֳ���

    //ΪĿ¼�����ռ�

    //��Ŀ¼��ΪĿ¼����ӵ���ǰĿ¼

    //Ϊ�½���Ŀ¼���һ������Ŀ¼��Ŀ¼��

}


//����FCB
int creatFCB(int fcbdataStartBlock, int filedataStartBlock, int fileSize) {
    //�ҵ�fcb�Ĵ洢λ��
    FCB *currentFCB = (FCB *) getBlockAddr(fcbdataStartBlock);
    currentFCB->dataStartBlock = filedataStartBlock;//�ļ�������ʼλ��
    currentFCB->fileSize = fileSize;//�ļ���С
    currentFCB->link = 1;//�ļ�������
    currentFCB->dataSize = 0;//�ļ���д�����ݳ���
    currentFCB->readPtr = 0;//�ļ���ָ��
    currentFCB->read_sem = sem_open("read_sem", O_CREAT, 0644, READER_MAX_NUM);
    if (currentFCB->read_sem == SEM_FAILED) {
        perror("sem_open error");
        exit(1);
    }
    currentFCB->write_sem = sem_open("write_sem", O_CREAT, 0644, 1);
    if (currentFCB->write_sem == SEM_FAILED) {
        perror("sem_open error");
        exit(1);
    }
    currentFCB->read_sem2 = sem_open("read_sem2", O_CREAT, 0644, 1);
    if (currentFCB->read_sem2 == SEM_FAILED) {
        perror("sem_open error");
        exit(1);
    }
    return 0;
}


//TODO ���Ŀ¼��
int addDirUnit(dirTable* myDirTable, char fileName[], int type, int FCBBlockNum)
{
    //���Ŀ¼��ʾ�Ƿ�����

    //�Ƿ����ͬ���ļ�

    //������Ŀ¼��
}


//TODO ɾ���ļ� rm
int deleteFile(char fileName[])
{
    //����ϵͳ���Զ������ĸ�Ŀ¼

    //�����ļ���Ŀ¼��λ��

    //���ļ�����ΪĿ¼���򷵻ش���

    //�ͷ��ļ��ڴ�

    //��Ŀ¼�����޳�
}


//�ͷ��ļ��ڴ�
int releaseFile(int FCBBlock) {
    FCB *myFCB = (FCB *) getBlockAddr(FCBBlock);
    myFCB->link--;  //��������һ
    //�����ӣ�ɾ���ļ�
    if (myFCB->link == 0) {
        //�ͷ��ļ������ݿռ�
        releaseBlock(myFCB->dataStartBlock, myFCB->fileSize);
    }
    //�ͷ�FCB�Ŀռ�
    sem_close(myFCB->read_sem);
    sem_close(myFCB->write_sem);
    sem_close(myFCB->read_sem2);
    sem_unlink("read_sem");
    sem_unlink("write_sem");
    sem_unlink("read_sem2");
    //�ͷ�FCB�ռ�
    releaseBlock(FCBBlock, 1);
    return 0;
}

//ɾ��Ŀ¼��
int deleteDirUnit(dirTable *myDirTable, int unitIndex) {
    //Ǩ�Ƹ���
    int dirUnitAmount = myDirTable->dirUnitAmount;
    for (int i = unitIndex; i < dirUnitAmount - 1; i++) {
        myDirTable->dirs[i] = myDirTable->dirs[i + 1];
    }
    myDirTable->dirUnitAmount--;
    return 0;
}


//ɾ��Ŀ¼ rmdir
int deleteDir(char dirName[]) {
    //����ϵͳ���Զ������ĸ�Ŀ¼
    if (strcmp(dirName, "..") == 0) {
        printf("can't delete ..\n");
        return -1;
    }
    //�����ļ�
    int unitIndex = findUnitInTable(currentDirTable, dirName);
    if (unitIndex == -1) {
        printf("file not found\n");
        return -1;
    }
    dirUnit myUnit = currentDirTable->dirs[unitIndex];
    //�ж�����
    if (myUnit.type == 0)//Ŀ¼
    {
        deleteFileInTable(currentDirTable, unitIndex);
    } else {
        printf("not a dir\n");
        return -1;
    }
    //��Ŀ¼�����޳�
    deleteDirUnit(currentDirTable, unitIndex);
    return 0;
}


//ɾ���ļ�/Ŀ¼��
int deleteFileInTable(dirTable *myDirTable, int unitIndex) {
    //�����ļ�
    dirUnit myUnit = myDirTable->dirs[unitIndex];
    //�ж�����
    if (myUnit.type == 0)//Ŀ¼
    {
        //�ҵ�Ŀ¼λ��
        int FCBBlock = myUnit.startBlock;
        dirTable *table = (dirTable *) getBlockAddr(FCBBlock);
        //�ݹ�ɾ��Ŀ¼�µ������ļ�
        printf("cycle delete dir %s\n", myUnit.fileName);
        int unitCount = table->dirUnitAmount;
        for (int i = 1; i < unitCount; i++)//���ԡ�..��
        {
            printf("delete %s\n", table->dirs[i].fileName);
            deleteFileInTable(table, i);
        }
        //�ͷ�Ŀ¼��ռ�
        releaseBlock(FCBBlock, 1);
    } else {//�ļ�
        //�ͷ��ļ��ڴ�
        int FCBBlock = myUnit.startBlock;
        releaseFile(FCBBlock);
    }
    return 0;
}


//���ļ�����ÿ��ƿ�
FCB *open(char fileName[]) {
    int unitIndex = findUnitInTable(currentDirTable, fileName);
    if (unitIndex == -1) {
        printf("file not found\n");
        return NULL;
    }
    //���ƿ�
    int FCBBlock = currentDirTable->dirs[unitIndex].startBlock;
    FCB *myFCB = (FCB *) getBlockAddr(FCBBlock);
    return myFCB;
}


//���ļ� read
int read_file(char fileName[], int length) {
    //���ļ�����ÿ��ƿ�
    FCB *myFCB = open(fileName);
    myFCB->readPtr = 0; //�ļ�ָ������
    //������
    char *data = (char *) getBlockAddr(myFCB->dataStartBlock);
    int val;
    myFCB->read_sem = sem_open("read_sem", 0);
    /* ��ȡ��¼������������ */
    myFCB->read_sem2 = sem_open("read_sem2", 0);
    sem_wait(myFCB->read_sem2);
    if (sem_wait(myFCB->read_sem) == -1)
        perror("sem_wait error");
    sem_getvalue(myFCB->read_sem, &val);
    /* ����ӵ�����Ľ����������ж��Ƿ��ǵ�һ������ */
    /* ����ǵ�һ�����߾͸�������д���� */
    if (val == READER_MAX_NUM - 1) {
        myFCB->write_sem = sem_open("write_sem", 0);
        if (sem_wait(myFCB->write_sem) == -1)
            perror("sem_wait error");
    }
    sem_post(myFCB->read_sem2);
    int dataSize = myFCB->dataSize;
    /* printf("myFCB->dataSize = %d\n", myFCB->dataSize); */
    //�ڲ��������ݳ����£���ȡָ�����ȵ�����
    for (int i = 0; i < length && myFCB->readPtr < dataSize; i++, myFCB->readPtr++) {
        printf("%c", *(data + myFCB->readPtr));
    }
    /* ��������ֻ��Ϊ��ģ��༭���Ĺر�֮ǰ������� */
    /* �������ܿ��ƽ��̲��������ͷ��� */
    printf("\n> Read finished, press any key to continue....");
    getchar();
    /* ��������һ�����߾͸����ͷ�д���� */
    //��������˸��ģ��ɣ�READER_MAX_NUM���£�READER_MAX_NUM-1 By. LeoHao
    sem_wait(myFCB->read_sem2);
    if (val == READER_MAX_NUM - 1) {
        sem_post(myFCB->write_sem);
    }
    sem_post(myFCB->read_sem);
    sem_post(myFCB->read_sem2);
    return 0;
}

//TODO д�ļ�����ĩβд�� write
int write_file(char fileName[], char content[])
{
    //��ÿ��ƿ�

    /* ���д���� */

    //�ڲ������ļ��Ĵ�С�ķ�Χ��д��

    /* ģ��༭��,����д�߲������˳� */
    printf("> Write finished, press any key to continue....");
    getchar();
    /* �ͷ�д���� */

}


//��Ŀ¼�в���Ŀ¼��Ŀ
int findUnitInTable(dirTable *myDirTable, char unitName[]) {
    //���Ŀ¼��
    int dirUnitAmount = myDirTable->dirUnitAmount;
    int unitIndex = -1;
    for (int i = 0; i < dirUnitAmount; i++)//����Ŀ¼��λ��
        if (strcmp(unitName, myDirTable->dirs[i].fileName) == 0)
            unitIndex = i;
    return unitIndex;
}