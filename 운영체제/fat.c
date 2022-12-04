#include <stdio.h>
#include <stdlib.h>
#include "fat.h"
#include "Disk.h"

// Disk.c 에 DevWriteBlock

#define FAT_NUM (BLOCK_SIZE / sizeof(int))

/*
   FAT 테이블을 초기화 하는 함수

   : 블록 크기의 메모리를 할당하고 0으로 채움
   
*/
void FatInit(void)
{
   DevCreateDisk();
   int *pMem = (int *)malloc(BLOCK_SIZE);

   for (int i = 0; i < FAT_NUM; i++)
   {
      *(pMem + i) = 0;
   }

   for (int i = 0; i < NUM_FAT_BLOCK; i++)
   {
      DevWriteBlock(i, (char *)pMem);
   }
}

/* 
   FAT 테이블에 블록을 추가하는 함수
   @param
      lastBlkNum : 마지막 블록 Num
      newBlkNum : 새 블록 Num

   lastBlkNum의 다음 블록을 newBlkNum으로 지정

   @return :
   newBlkNum FAT entry value is not 0 return -1
   lastBlkNum FAT entry value is not -1 return -1
 */


int FatAdd(int lastBlkNum, int newBlkNum)
{
   int tempBlock[FAT_NUM];
   int newFATNum = newBlkNum / FAT_NUM;
   int newFATidx = newBlkNum % FAT_NUM;

   if (lastBlkNum == -1)
   {
      DevReadBlock(newFATNum, (char *)tempBlock);
      if (tempBlock[newFATidx] != 0)
         return -1;

      tempBlock[newFATidx] = -1;
      DevWriteBlock(newFATNum, (char *)tempBlock);
      return 0;
   }
   else
   {
      int readLstBlK = lastBlkNum / FAT_NUM;
      DevReadBlock(readLstBlK, (char *)tempBlock);
      if (tempBlock[lastBlkNum % FAT_NUM] != -1)
         return -1;
      tempBlock[lastBlkNum % FAT_NUM] = newBlkNum;

      DevWriteBlock(readLstBlK, (char *)tempBlock);
      DevReadBlock(newFATNum, (char *)tempBlock);

      if (tempBlock[newFATidx] != 0)
         return -1;

      tempBlock[newFATidx] = -1;
      DevWriteBlock(newFATNum, (char *)tempBlock);

      return 0;
   }
}

/* 
   Logical block number로 physical block number를 획득하는 함수

   @param :
      firstBlkNum : 시작 블록 Num
      logicalBlkNum :  논리적인 block Num

   @return :
   firstBlkNum부터 시작해서 logicalBlkNum에 해당하는 블록 값이 담긴 index를 반환

firstBlkNum FAT entry value  is 0 or logicalBlkNum physical block num -1 or 0 return -1
    */
int FatGetBlockNum(int firstBlkNum, int logicalBlkNum)
{
   int readBlkNum = firstBlkNum / FAT_NUM;
   int readBlkIdx = firstBlkNum % FAT_NUM;
   int rsltIndex = firstBlkNum;
   int tempBlock[FAT_NUM];

   while (logicalBlkNum--)
   {

      DevReadBlock(readBlkNum, (char *)tempBlock);
      if (tempBlock[readBlkIdx] == 0 || tempBlock[readBlkIdx] == -1)
         return -1;
      rsltIndex = tempBlock[readBlkIdx];
      readBlkNum = rsltIndex / FAT_NUM;
      readBlkIdx = rsltIndex % FAT_NUM;
   }

   return rsltIndex;
}

/*
   startBlock 부터 마지막 블록까지 제거하는 함수

   @param 
      firstBlock: 파일 시작을 지정
      startBlock: 삭제를 시작할 block
   
   @return
      삭제된 블록 개수
 firstBlkNum FAT entry value is 0 or start BlkNm Fat entry value is 0 return -1
*/
int FatRemove(int firstBlkNum, int startBlkNum)
{
   int rmCount = 0;
   int readBlkNum = firstBlkNum / FAT_NUM;
   int readBlkIdx = firstBlkNum % FAT_NUM;
   int tempBlock[FAT_NUM];
   DevReadBlock(readBlkNum, (char *)tempBlock);

   int nxtBlk;
   while (tempBlock[readBlkIdx] != startBlkNum)
   {
      if (tempBlock[readBlkIdx] == 0)
         return -1;
      nxtBlk = tempBlock[readBlkIdx];
      readBlkNum = nxtBlk / FAT_NUM;
      readBlkIdx = nxtBlk % FAT_NUM;
      DevReadBlock(readBlkNum, (char *)tempBlock);
   }

   // 같다!! tempBlock[readBlkIndx] == startBlkNum
   nxtBlk = tempBlock[readBlkIdx];
   tempBlock[readBlkIdx] = -1;
   DevWriteBlock(readBlkNum, (char *)tempBlock);

   readBlkNum = nxtBlk / FAT_NUM;
   readBlkIdx = nxtBlk % FAT_NUM;
   DevReadBlock(readBlkNum, (char *)tempBlock);

   while (tempBlock[readBlkIdx] != -1)
   {
      if (tempBlock[readBlkIdx] == 0)
         return -1;
      nxtBlk = tempBlock[readBlkIdx];
      tempBlock[readBlkIdx] = 0;
      DevWriteBlock(readBlkNum, (char *)tempBlock);
      rmCount++;

      readBlkNum = nxtBlk / FAT_NUM;
      readBlkIdx = nxtBlk % FAT_NUM;
      DevReadBlock(readBlkNum, (char *)tempBlock);
   }
   tempBlock[readBlkIdx] = 0;
   DevWriteBlock(readBlkNum, (char *)tempBlock);
   rmCount++;
   return rmCount;
}
