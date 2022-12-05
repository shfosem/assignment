#include <stdio.h>
#include <stdlib.h>
#include "buf.h"
#include "queue.h"
#include "Disk.h"
#include <string.h>

//BufSyncBlock 구현

//Bufsync 업데이트

//BufWrite LRU Control 추가

static int lst_num=0;

void BufInit(void)
{
    //Initialize
    TAILQ_INIT(&ppStateListHead[BUF_LIST_CLEAN]);
    TAILQ_INIT(&ppStateListHead[BUF_LIST_DIRTY]);
    TAILQ_INIT(&pLruListHead);
    DevCreateDisk();
    DevOpenDisk();
}
/*
Buffer Replacement 구현

buffer list 개수가 초과되었을 시, 가장 오래된(Least Recently Used) 블록 대체
LRU list head는 가장 오랫동안 접근되지 않은 buffer를 가리킴
LRU list Tail은 가장 최근에 접근한 buffer를 가리킴

LRU 관리
현재 접근되는 buffer는 LRU list tail에 넣음

Buffer replacement 시에, LRU buffer인 head를 victim으로 선택하고, 새로운 블록을 위해 재사용

victim 시  해당 buffer block이 dirty인 경우 disk에 저장 Sync()
                               clean인 경우 disk에 저장하지 않고 buffer 재사용 

Read, Write 관계 없이 현재 접근 되는 Buffer는 무조건 LRU list의 Tail로 이동
*/

Buf* BufGet(int blkno) // 주소를 불러옴
    Buf *item = malloc(sizeof(Buf));
    TAILQ_FOREACH(item, &ppBufList[blkno%4], blist)
    {
        if(item->blkno==blkno)
            return item;
    }
    return NULL;
}


 void BufSyncBlock(int blkno)  // block 하나만 Sync 제대로 구현하고, BufSync에서 반복하도록 
 {
     Buf* pBuf = BufGet(blkno);
     
     char temp[BLOCK_SIZE]={0,};
     memccpy(temp, (char*)pBuf->pMem,(int)']',BLOCK_SIZE);
     DevWriteBlock(blkno, temp); 
    pBuf->state=BUF_STATE_CLEAN;
    TAILQ_REMOVE(&ppStateListHead[BUF_LIST_DIRTY],pBuf,slist);
    TAILQ_INSERT_TAIL(&ppStateListHead[BUF_STATE_CLEAN],pBuf,slist);
 }

void BufRead(int blkno, char* pData)
{ //blkno 읽을 block 번호, pData 블록을 담을 메모리 공간
   
    Buf *item ;

    if(BufGet(blkno)!=NULL)// is_buffercache
    {
        item = BufGet(blkno);
        memcpy(pData, (char*)item->pMem, BLOCK_SIZE);

        //LRU Update
        Buf* temp;
            TAILQ_FOREACH(temp,&pLruListHead,llist)
            {
                if(temp->blkno == blkno)
                {
                    TAILQ_REMOVE(&pLruListHead,temp,llist);
                    TAILQ_INSERT_TAIL(&pLruListHead,temp,llist);
                    break;
                }
            }
    }
    else // no cache 
    {
         if(lst_num==MAX_BUF_NUM)
         {
            Buf *victim;
            victim=TAILQ_FIRST(&pLruListHead);
            if(victim->state) // 1 is CLEAN
                {
                    Buf *temp=BufGet(victim->blkno);
                    //Delete_ppBuf
                    TAILQ_REMOVE(&ppBufList[(victim->blkno)%HASH_ENTRY_NUM],temp,blist);
                    //Delete_stateBuf
                    TAILQ_REMOVE(&ppStateListHead[BUF_LIST_CLEAN],temp,slist);
                    TAILQ_REMOVE(&pLruListHead,victim,llist);
                }
             else // 0 is dirty
                {   
                    DevWriteBlock(victim->blkno,pData);
                    Buf *temp = BufGet(victim->blkno);
                   
                    TAILQ_REMOVE(&ppBufList[(victim->blkno)%4],temp,blist);
                    TAILQ_REMOVE(&ppStateListHead[BUF_LIST_CLEAN],temp, slist);
                    TAILQ_REMOVE(&pLruListHead,victim,llist);
            
                }
            lst_num-=1;
         }

        // add item 
        item=malloc(sizeof(Buf));
        item->blkno=blkno;
        item->pMem=malloc(BLOCK_SIZE);
        item->state=BUF_STATE_CLEAN;
        DevReadBlock(blkno,(char*)item->pMem); //(char*)
        TAILQ_INSERT_HEAD(&ppBufList[blkno%HASH_ENTRY_NUM],item,blist);
        TAILQ_INSERT_TAIL(&ppStateListHead[BUF_LIST_CLEAN],item,slist);
        memcpy(pData, item->pMem, BLOCK_SIZE);
        lst_num+=1;

        //LRU update
        TAILQ_INSERT_TAIL(&pLruListHead,item,llist);
        
    }

    // LRU Control
    //

    //buffer list에서 blkno 검색 OK
    //있다면 -> 디스크에서 읽지 않고, 해당 buffer list에서 읽음. OK

    //없다면 -> buf(pMem)를 할당하여, blkno 세팅 -> DevReadBlock 사용하여, 블록을 읽어서 blk 메모리에 저장 OK
    // buffer를 Buffer list head에 추가, Clean list Tail에 추가 OK
    // pMem에 저장된 blk 데이터를 pData로 복사! OK

}

//LRU 업데이트  +replacement
void BufWrite(int blkno, char* pData)
{
    Buf *item= malloc(sizeof(Buf));
    //buffer list에서 blkno 검색
    if(BufGet(blkno)!=NULL) // is buffer cache 
    {   
        item= BufGet(blkno);
        if(item->state) // is clean
        {
            memccpy((char*)item->pMem, pData, (int)']', BLOCK_SIZE);
            item->state =BUF_STATE_DIRTY;
             Buf * titem;
                TAILQ_FOREACH(titem,&ppStateListHead[BUF_LIST_CLEAN],slist) //delete in clean, Insert in Dirty
                {
                    if(titem->blkno==blkno){
                        TAILQ_REMOVE(&ppStateListHead[BUF_LIST_CLEAN],titem,slist);
                        TAILQ_INSERT_TAIL(&ppStateListHead[BUF_LIST_DIRTY], titem,slist);
                        break;
                    }
                }
        }
        else // is dirty
        {
            memccpy((char*)item->pMem, pData, (int)']',BLOCK_SIZE);
        }
        //LRU Control
        TAILQ_REMOVE(&pLruListHead,item,llist);
        TAILQ_INSERT_TAIL(&pLruListHead,item,llist);

    }

    
    //저장할 block 이 Buffer Cache에 없다고 가정.
    // 디스크에도 저장되지 않은 새로운 block!
    else //isn't buffercache
    {
        // 개수 control
        if(lst_num==MAX_BUF_NUM)
         {
            Buf *victim;
            victim=TAILQ_FIRST(&pLruListHead);
            int tempblkno= victim->blkno;
            if(victim->state) // 1 is CLEAN
                {
                    Buf *temp;
                    //Delete_ppBuf
                    temp = BufGet(tempblkno);
                    TAILQ_REMOVE(&ppBufList[(tempblkno)%HASH_ENTRY_NUM],temp,blist);
                    //Delete_stateBuf
                    TAILQ_REMOVE(&ppStateListHead[BUF_LIST_CLEAN],temp,slist);
                    TAILQ_REMOVE(&pLruListHead,victim,llist);
                }
             else // 0 is dirty
                {
                  DevWriteBlock(tempblkno,(char*)victim->pMem); // char*
                    Buf *temp;
                    //Delete_ppuf
                    temp = BufGet(tempblkno);
                    TAILQ_REMOVE(&ppBufList[(tempblkno)%4],temp,blist);
                    TAILQ_REMOVE(&ppStateListHead[BUF_LIST_DIRTY],temp,slist);
                    TAILQ_REMOVE(&pLruListHead,victim,llist);
                }
            lst_num-=1;
         }


         //BLOCK_SIZE
        item->blkno=blkno;
        item->pMem=malloc(BLOCK_SIZE);
        memccpy((char*)item->pMem, pData, (int)']',BLOCK_SIZE);
        *(char*)item->pMem=*pData; 
        item->state=BUF_STATE_DIRTY;
        TAILQ_INSERT_HEAD(&ppBufList[blkno%HASH_ENTRY_NUM],item,blist);
        TAILQ_INSERT_TAIL(&ppStateListHead[BUF_STATE_DIRTY],item,slist);
        TAILQ_INSERT_TAIL(&pLruListHead,item,llist);
        lst_num+=1;
    }

   
    
}

void BufSync(void) // LRU Update
{
    //Dirty list head를 확인 -> Head의 buffer부터 tail로 이동하면서 디스크로 저장( DevWriteBlock 사용)
    //                       -> Diry list의 Head에 있는 buffer 부터 차례대로 clean list tail로 이동
    // 구현 시 블록단위로 디스크에 저장하고 리스트 이동, 디스크에 저장하고 리스트 이동, 저장하고 이동 이런 방식으로 해야됨!
    Buf *item;
    int numBuf=0;
    Buf* ppBufInfo[MAX_BUF_NUM];
    GetBufInfoByListNum(BUF_STATE_DIRTY, ppBufInfo, &numBuf);

    while(numBuf)
    {
        TAILQ_FOREACH(item, &ppStateListHead[BUF_LIST_DIRTY], slist)
        {
            BufSyncBlock(item->blkno);
        }
        GetBufInfoByListNum(BUF_STATE_DIRTY,ppBufInfo, &numBuf);
    }
   
}

/*
 * GetBufInfoByListNum: Get all buffers in a list specified by listnum.
 *                      This function receives a memory pointer to "ppBufInfo" that can contain the buffers.
 */
void GetBufInfoByListNum(StateList listnum, Buf** ppBufInfo, int* pNumBuf)
{
    int i=0;
    *pNumBuf=0;
    Buf* item;
    
    TAILQ_FOREACH(item, &ppStateListHead[listnum], slist)
    {
        ppBufInfo[i]=item;
        *pNumBuf+=1;
        i++;
    }
}

/*
 * GetBufInfoInLruList: Get all buffers in a list specified at the LRU list.
 *                         This function receives a memory pointer to "ppBufInfo" that can contain the buffers.
 */
void GetBufInfoInLruList(Buf** ppBufInfo, int* pNumBuf)
{

   int i=0;
   *pNumBuf=0;
   Buf* item;
   TAILQ_FOREACH(item, &pLruListHead, llist)
   {
       ppBufInfo[i]=item;
       *pNumBuf+=1;
       i++;
   }

}


/*
 * GetBufInfoInBufferList: Get all buffers in the buffer list.
 *                         This function receives a memory pointer to "ppBufInfo" that can contain the buffers.
 */
void GetBufInfoInBufferList(int hashEntNum, Buf** ppBufInfo, int* pNumBuf)
{
    int i=0;
    *pNumBuf=0;
    Buf* item;
    TAILQ_FOREACH(item, &ppBufList[hashEntNum], blist)
    {
        ppBufInfo[i] = item;
        *pNumBuf+=1;
        i++;
    }
}


